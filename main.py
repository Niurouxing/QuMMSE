import subprocess
import os
import re

def update_config(numbers):
    # 确保提供的数值列表长度正确
    if len(numbers) != 32:
        print("Error: Expected 32 numbers.")
        return

    # 读取原始文件内容
    with open("./include/config.h", "r") as file:
        content = file.read()

    # 定义一个正则表达式，同时匹配intBits和fracBits内的数字
    pattern = r"(intBits<|fracBits<)(\d+)>"

    # 用于记录当前替换的位置
    current_index = 0

    # 定义一个替换函数，每次匹配到数字时被调用
    def replace_with_number(match):
        nonlocal current_index
        # 提取当前的数字对应的替换值
        replacement_number = numbers[current_index]
        current_index += 1
        # 返回替换文本
        return f"{match.group(1)}{replacement_number}>"

    # 使用正则表达式和替换函数更新文件内容
    updated_content = re.sub(pattern, replace_with_number, content)

    # 确保我们替换了所有的32个数字
    if current_index != len(numbers):
        print("Warning: Not all numbers were replaced.")

    # 写回修改后的内容
    with open("./include/config.h", "w") as file:
        file.write(updated_content)

def build_cmake_project(build_dir="./build"):
    """
    使用CMake编译C++项目。
    假设CMakeLists.txt位于项目的根目录。
    参数:
    - build_dir: 构建目录的路径
    """
    try:
        # 如果构建目录不存在，创建它
        os.makedirs(build_dir, exist_ok=True)
        # 切换到构建目录
        os.chdir(build_dir)
        # 调用CMake和make来编译项目
        subprocess.check_call(["cmake", ".."])
        subprocess.check_call(["cmake", "--build", "."])
        print("CMake project built successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to build CMake project: {e}")
    finally:
        # 切换回原始目录
        os.chdir("..")

def run_cpp_program(executable_path, iter, SNRdB):
    """
    执行C++程序并获取输出结果。
    参数:
    - executable_path: 可执行文件的路径
    - iter: 迭代次数
    - SNRdB: SNRdB值
    """
    iter_str = str(iter)
    SNRdB_str = str(SNRdB)
    
    result = subprocess.run([executable_path, iter_str, SNRdB_str], capture_output=True, text=True)
    
    if result.returncode != 0:
        print("Error running the C++ program:", result.stderr)
        return None
    
    error = int(result.stdout.strip())
    return error

numbers_to_replace = [11] * 32  # 假设你想把所有数字都替换为12，示例使用
update_config(numbers_to_replace)

# 重新编译CMake项目
build_cmake_project()

# 运行C++程序
executable_path = "./build/QuMMSE"  # 请根据实际情况修改路径
iter = 100
SNRdB = 10
error = run_cpp_program(executable_path, iter, SNRdB)
print(f"Error: {error}")