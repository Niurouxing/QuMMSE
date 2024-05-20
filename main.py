import subprocess
import os
import re
import shutil
from multiprocessing import Pool
import numpy as np
from pymoo.core.problem import Problem
from pymoo.optimize import minimize
from pymoo.core.callback import Callback
from pymoo.algorithms.moo.nsga2 import NSGA2
from pymoo.operators.crossover.sbx import SBX
from pymoo.operators.repair.rounding import RoundingRepair
from pymoo.operators.mutation.pm import PM
# tqdm
from tqdm import tqdm









def create_cmake_projects_copies(num_copies):
    # delete the existing subCMake directory if it exists
    shutil.rmtree('./subCMake', ignore_errors=True)

    source_folders = ['./include', './QuBLAS', './main.cpp', './CMakeLists.txt']
    base_target_folder = './subCMake'

    for i in range(num_copies):
        target_folder = f'{base_target_folder}/subCMake_{i}'
        
        # Create directory structure
        os.makedirs(target_folder, exist_ok=True)
        for folder in source_folders:
            dest = os.path.join(target_folder, os.path.basename(folder))
            if os.path.isdir(folder):
                shutil.copytree(folder, dest, dirs_exist_ok=True)
            else:  # For files
                shutil.copy(folder, dest)
                
    print(f"Created {num_copies} CMake project copies in '{base_target_folder}'.")



def update_config(numbers, build_dir_index):
    """
    仅在原地更新./include中的config.h文件。
    """
    config_file_path = f"./subCMake/subCMake_{build_dir_index}/include/config.h"

    if len(numbers) != 52:
        print("Error: Expected 52 numbers.")
        return
    

    # make array shape (26,2)
    helpArray = np.array(numbers).reshape(26, 2)

    # sum the two columns
    helpArray  = np.sum(helpArray , axis=1)

    # if any number is less than 0, set all numbers to integer 1
    if np.any(helpArray  < 0):
        numbers = np.ones(52, dtype=int)
        

    with open(config_file_path, "r") as file:
        content = file.read()

    pattern = r"(intBits<|fracBits<)(-?\d+)>"
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

def build_cmake_project(build_dir_index, silence_output=True, use_clang=True):
    build_dir = f"./build/build_{build_dir_index}"
    source_dir = f"./subCMake/subCMake_{build_dir_index}"
    os.makedirs(build_dir, exist_ok=True)
    
    # 定义 CMake 命令的一般部分
    cmake_configure_command = [
        "cmake", "-S", source_dir, "-B", build_dir
    ]
    cmake_build_command = [
        "cmake", "--build", build_dir
    ]
    
    # 如果指定使用 clang 编译器
    if use_clang:
        clang_options = [
            "-DCMAKE_C_COMPILER=/usr/bin/clang",
            "-DCMAKE_CXX_COMPILER=/usr/bin/clang++"
        ]
        cmake_configure_command.extend(clang_options)

    # 控制输出
    output_options = {}
    if silence_output:
        output_options = {'stdout': subprocess.DEVNULL, 'stderr': subprocess.DEVNULL}
    
    try:
        subprocess.check_call(cmake_configure_command, **output_options)
        subprocess.check_call(cmake_build_command, **output_options)
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
    update_config(numbers, build_dir_index)  # 更新config.h文件
    build_cmake_project(build_dir_index)  # 构建项目
    return build_dir




class myProblem(Problem):
    def __init__(self):
        super().__init__(
            n_var= 52,
            n_obj=1,
            n_constr=1,
            xl=-8, 
            xu=12,
            vtype=int,
        )

        self.pool = Pool(processes=os.cpu_count())
    def _evaluate(self, x, out, *args, **kwargs):
        pop_size = x.shape[0]
        f = np.zeros((pop_size, 1))
        g = np.zeros((pop_size, 1))


        # clear build directories
        build_directories = []
        
        # compile for each row of x
        # for i in tqdm(range(pop_size), desc="Building projects"):
        #     build_dir = process_config_and_build(x[i], i)
        #     build_directories.append(build_dir)

        # parallel version
        tasks = [(x[i], i) for i in range(pop_size)]
        build_directories = self.pool.starmap(process_config_and_build, tasks)


 

        # run the compiled programs in parallel
        executable_paths = [os.path.join(dir, "QuMMSE") for dir in build_directories]
        tasks = [(path, 30000, 10) for path in executable_paths]
        results = self.pool.starmap(run_cpp_program, tasks)

    
        for i in range(pop_size):
 
            if results[i] is None:
                g[i] = 1
            else:
                g[i] = results[i]-36000

            f[i][0] = np.sum(x[i])
            # f[i][1] = results[i]
        
        out["F"] = f
        out["G"] = g

    def __del__(self):
        # 在对象被销毁时关闭进程池
        self.pool.close()
        self.pool.join()



class MyCallback(Callback):
    def __init__(self) -> None:
        super().__init__()
        self.log=0

    def notify(self, algorithm):
        self.log+=1
        if self.log%5==0:
            currentArg = algorithm.pop.get("X")
            currentPer = algorithm.pop.get("F")

 
 
            print("minArg:", currentArg[0,:])



if __name__ == "__main__":
    myPro = myProblem()
    print("The problem has been initialized!")

    pop_size = 32
    n_gen = 1000

# 6  9  2  5  6 10  5  6  7  6  3  9  3  8  3  6  3  7  4  7  6  6  3 10
#   4  5  4  5  4  5  5  4  6  4  6  2  3  7  6  8  5  5  6  6  6  5  6  4
#   7  4  6  9
 
    good_init =  np.array([6, 9, 2, 5, 6, 10, 5, 6, 7, 6, 3, 9, 3, 8, 3, 6, 3, 7, 4, 7, 6, 6, 3, 10,
                            4, 5, 4, 5, 4, 5, 5, 4, 6, 4, 6, 2, 3, 7, 6, 8, 5, 5, 6, 6, 6, 5, 6, 4,
                            7, 4, 6, 9])



    create_cmake_projects_copies(pop_size)

    algorithm = NSGA2(
        pop_size=pop_size,
        sampling=good_init,
        # sampling=IntegerRandomSampling(),
        crossover=SBX(prob=1.0, eta=3.0, vtype=float, repair=RoundingRepair()),
        mutation=PM(prob=1.0, eta=3.0, vtype=float, repair=RoundingRepair()),
        eliminate_duplicates=True,
    )
    callback = MyCallback()
    res = minimize(
        myPro, algorithm, callback=callback, seed=1, verbose=True
    )