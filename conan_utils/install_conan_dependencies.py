#!/usr/bin/env python

import subprocess
import os

def _find_conan_executable():
    # Default is that the conan command is on the PATH.
    executable = "conan"
    user_conan = os.path.expanduser("~/.local/bin/conan")
    if os.path.isfile(user_conan):
        executable = user_conan
    return executable


def _find_conanfiles():
    result = []
    for base, dirs, files in os.walk("."):
        for file in files:
            if file == "conanfile.txt" or file == "conanfile.py":
                full_path = os.path.join(base, file)
                result.append(full_path)
    return result


if __name__ == "__main__":
    executable = _find_conan_executable()
    print("Conan: {}".format(executable))
    conanfiles = _find_conanfiles()
    for conanfile in conanfiles:
        print("Processing {}".format(conanfile))
        prev_cwd = os.getcwd()
        os.chdir(os.path.dirname(conanfile))
        subprocess.run([executable, "install", os.path.basename(conanfile)])
        os.chdir(prev_cwd)
