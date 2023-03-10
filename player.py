#!/usr/bin/python3
from typing import Tuple
import numpy as np
from numpy.typing import *
from mathUtits import *


class Player:
    def __init__(self, speed: float, acceleration_factor: float) -> None:
        self.position = np.zeros(2)
        self.velocity = np.zeros(2)
        self.speed = speed
        self.acceleration_factor = acceleration_factor
    
    @property
    def x(self) -> float:
        return self.position[0]
    
    @property
    def y(self) -> float:
        return self.position[1]
    
    @x.setter
    def set_x(self, value: float):
        self.position[0] = value
    
    @y.setter
    def set_y(self, value: float):
        self.position[1] = value

    def pos(self) -> Tuple[int,int]:
        return tuple(self.position)
    
    def move(self, direction: NDArray, delta: float):

        desired_velocity = self.speed*delta*direction

        self.velocity = lerp(
            self.velocity
            , desired_velocity
            , self.acceleration_factor
        )

        self.position += self.velocity
