import os
import platform
import subprocess
import sys
import shutil

def run(cmd, shell=False):
    print(f"> {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    result = subprocess.run(cmd, shell=shell)
    if result.returncode != 0:
        sys.exit(result.returncode)

def get_generator():
    if shutil.which("g++"):
        return ("MinGW Makefiles", [])
    elif shutil.which("cl.exe"):
        # Force 64-bit Visual Studio build if available
        return ("Visual Studio 17 2022", ["-A", "x64"])
    else:
        return None

def main():
    # Git submodules
    # run(["git", "submodule", "update", "--init", "--recursive"])

    # Build dir
    if not os.path.exists("build"):
        os.mkdir("build")
    os.chdir("build")

    system = platform.system()

    if system == "Windows":
        generator_info = get_generator()
        if not generator_info:
            print("No compiler was found! Install MinGW or Visual Studio.")
            return
        
        generator, extra_args = generator_info

        if not os.path.exists("CMakeCache.txt"):
            run(["cmake", "..", "-G", generator] + extra_args)

        run(["cmake", "--build", "."])
        os.chdir("build")

    # Linux
    else:
        if not os.path.exists("CMakeCache.txt"):
            run(["cmake", ".."])

        run(["cmake", "--build", "."])
        os.chdir("build") 

if __name__ == "__main__":
    main()
