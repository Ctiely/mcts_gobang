from distutils.core import setup, Extension
from Cython.Build import cythonize
#import numpy as np

COMPILE_ARGS = ['-std=c++11']
ext = Extension("cBoard",
                sources=["cBoard.pyx", "Board.cpp"],
                language="c++",
                extra_compile_args = COMPILE_ARGS)
#include_dirs=['.', np.get_include()])

setup(name="cBoard",
      ext_modules=cythonize(ext))
