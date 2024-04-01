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

def update_config(numbers):
    """
    仅在原地更新./include中的config.h文件。
    """
    config_file_path = "./include/config.h"  # 定义config.h的路径
    if len(numbers) != 30:
        print("Error: Expected 30 numbers.")
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

def build_cmake_project(build_dir, silence_output=True):
    """
    在指定的构建目录中重新编译项目。
    """
    try:
        os.makedirs(build_dir, exist_ok=True)
        # 将输出重定向到subprocess.DEVNULL，以避免在命令行中显示
        if silence_output:
            subprocess.check_call(["cmake", "-S.", "-B", build_dir], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            subprocess.check_call(["cmake", "--build", build_dir], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        else:
            subprocess.check_call(["cmake", "-S.", "-B", build_dir])
        subprocess.check_call(["cmake", "--build", build_dir], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        # print(f"CMake project built successfully in {build_dir}.")
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




class myProblem(Problem):
    def __init__(self):
        super().__init__(
            n_var= 30,
            n_obj=1,
            n_constr=1,
            xl=3, 
            xu=12,
            vtype=int,
        )

        self.pool = Pool(processes=os.cpu_count())
    def _evaluate(self, x, out, *args, **kwargs):
        pop_size = x.shape[0]
        f = np.zeros((pop_size, 1))
        g = np.zeros((pop_size, 1))

        # x is np.array(pop_size, 30)

        # clear build directories
        build_directories = []
        
        # compile for each row of x
        for i in tqdm(range(pop_size), desc="Building projects"):
            build_dir = process_config_and_build(x[i], i)
            build_directories.append(build_dir)

        # clear the last line of console, which is the progress bar
        print("\033[F\033[K", end="")
  

        # run the compiled programs in parallel
        executable_paths = [os.path.join(dir, "QuMMSE") for dir in build_directories]
        tasks = [(path, 10000, 10) for path in executable_paths]
        results = self.pool.starmap(run_cpp_program, tasks)

    
        for i in range(pop_size):
            g[i]= results[i]-13000
            f[i] = np.sum(x[i])
        
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
        if self.log%10==0:
            currentArg = algorithm.pop.get("X")
            currentPer = algorithm.pop.get("F")

            # find smallest errorBits in current population
            minIndex = np.argmin(currentPer)
            minErrorBits = currentPer[minIndex]
            minArg = currentArg[minIndex]
            print("Best :", minErrorBits)
            print("minArg:", minArg)



if __name__ == "__main__":
    myPro = myProblem()
    print("The problem has been initialized!")

    pop_size = 10
    n_gen = 1000

    good_init = np.random.randint(9, 12, (pop_size, 30))

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