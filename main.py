import subprocess
import os
import re
import shutil
from multiprocessing import Pool

def update_config(numbers):
    """
    仅在原地更新./include中的config.h文件。
    """
    config_file_path = "./include/config.h"  # 定义config.h的路径
    if len(numbers) != 32:
        print("Error: Expected 32 numbers.")
        return
    
    with open(config_file_path, "r") as file:
        content = file.read()
    
    pattern = r"(intBits<|fracBits<)(\d+)>"
    current_index = 0
    
    def replace_with_number(match):
        nonlocal current_index
        replacement_number = numbers[current_index]
        current_index += 1
        return f"{match.group(1)}{replacement_number}>"
    
    updated_content = re.sub(pattern, replace_with_number, content)
    
    if current_index != len(numbers):
        print("Warning: Not all numbers were replaced.")
        
    with open(config_file_path, "w") as file:
        file.write(updated_content)

def build_cmake_project(build_dir):
    """
    在指定的构建目录中重新编译项目。
    """
    try:
        os.makedirs(build_dir, exist_ok=True)
        subprocess.check_call(["cmake", "-S.", "-B", build_dir, "-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++", "-DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang"])
        subprocess.check_call(["cmake", "--build", build_dir])
        print(f"CMake project built successfully in {build_dir}.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to build CMake project in {build_dir}: {e}")

def run_cpp_program(executable_path, iter, SNRdB):
    """
    运行C++程序并获取输出结果。
    """
    iter_str, SNRdB_str = str(iter), str(SNRdB)
    result = subprocess.run([executable_path, iter_str, SNRdB_str], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"Error running the C++ program in {executable_path}: {result.stderr}")
        return None
    
    return int(result.stdout.strip())

def process_config_and_build(numbers, build_dir_index):
    """
    更新config.h文件，并为每组数字构建项目，输出到一个独立的build文件夹。
    """
    build_dir = f"./build/build_{build_dir_index}"  # 为每次构建创建独立的构建目录
    update_config(numbers)  # 更新config.h
    build_cmake_project(build_dir)  # 构建项目
    return build_dir

def parallel_execution(build_directories, iter, SNRdB):
    """
    并行运行所有构建好的C++程序。
    """
    executable_paths = [os.path.join(dir, "QuMMSE") for dir in build_directories]  # 替换your_executable_name
    with Pool(processes=len(executable_paths)) as pool:
        tasks = [(path, iter, SNRdB) for path in executable_paths]
        results = pool.starmap(run_cpp_program, tasks)
    return results

if __name__ == "__main__":
    number_sets = [[2*i+5] * 32 for i in range(3)]  
    build_directories = []

    for index, numbers in enumerate(number_sets):
        build_dir = process_config_and_build(numbers, index)
        build_directories.append(build_dir)
    
    # 替换iter和SNRdB为你需要的值
    iter_value = 1000
    SNRdB_value = 10
    results = parallel_execution(build_directories, iter_value, SNRdB_value)
    
    for result in results:
        print(f"Error: {result}")