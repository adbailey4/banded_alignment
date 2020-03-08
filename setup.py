#!/usr/bin/env python3
"""Create setup script for installation of banded_alignment"""
########################################################################
# File: setup.py
#  executable: setup.py
#
# Author: Andrew Bailey (https://www.benjack.io/2018/02/02/python-cpp-revisited.html)
# History: 3/05/20 Created
########################################################################

import os
import re
import sys
import platform
import subprocess
from shutil import copyfile, copymode
from timeit import default_timer as timer

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install

from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


def copy_test_file(src_file):
    """
    Copy ``src_file`` to `tests/bin` directory, ensuring parent directory
    exists. Messages like `creating directory /path/to/package` and
    `copying directory /src/path/to/package -> path/to/package` are
    displayed on standard output. Adapted from scikit-build.
    """
    # Create directory if needed
    dest_dir = os.path.join(os.path.dirname(
        os.path.abspath(__file__)), 'tests', 'bin')
    if dest_dir != "" and not os.path.exists(dest_dir):
        print("creating directory {}".format(dest_dir))
        os.makedirs(dest_dir)

    # Copy file
    dest_file = os.path.join(dest_dir, os.path.basename(src_file))
    print("copying {} -> {}".format(src_file, dest_file))
    copyfile(src_file, dest_file)
    copymode(src_file, dest_file)


class CMakeBuild(build_ext):

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2 ** 32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j8']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        # Copy *_test file to tests directory
        for executable in TEST_EXECUTABLES + MAIN_EXECUTABLES:
            copy_test_file(os.path.join(self.build_temp, executable))
        print()  # Add empty line for nicer output


class PostInstallCommand(install):
    """Post-installation for installation mode."""
    def run(self):
        install.do_egg_install(self)
        build_temp = self.build_lib.replace("lib", "temp")
        for executable in MAIN_EXECUTABLES:
            source = os.path.join(os.path.dirname(os.path.abspath(__file__)), build_temp, executable)
            target = os.path.join(self.install_scripts, executable)
            if os.path.isfile(target):
                os.remove(target)
            self.copy_file(source, target)


TEST_EXECUTABLES = ["tests/CppTests"]
MAIN_EXECUTABLES = ["bandedAlignment"]


def main():
    """Main docstring"""
    start = timer()

    setup(
        name="banded_alignment",
        version='0.0.1',
        description='Wrap libgaba with python',
        url="https://github.com/adbailey4/banded_alignment",
        author='Andrew Bailey',
        license='MIT',
        packages=find_packages('src'),
        package_dir={'': 'src'},
        ext_modules=[CMakeExtension('banded_alignment.bindings')],
        cmdclass=dict(build_ext=CMakeBuild, install=PostInstallCommand),
        author_email='bailey.andrew4@gmail.com',
        install_requires=['py3helpers[seq_tools]>=0.4.1'],
        zip_safe=False
    )

    stop = timer()
    print("Running Time = {} seconds".format(stop - start), file=sys.stderr)


if __name__ == "__main__":
    main()
    raise SystemExit
