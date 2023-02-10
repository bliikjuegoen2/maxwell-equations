#!/usr/bin/python3
import numpy as np
from numpy.typing import *
from numpy.linalg import *

def lerp(a: NDArray, b: NDArray, x: float) -> NDArray:
    return b*x + a*(1-x)

def normalize(x: NDArray) -> NDArray:
    x_norm = norm(x)
    return x/x_norm if x_norm != 0 else x