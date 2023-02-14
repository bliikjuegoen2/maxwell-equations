#!/usr/bin/python3
from types import NoneType
from typing import List, Tuple
import pygame as pg
from pygame import display
import numpy as np
from numpy.typing import *
from numpy import dot,cross
from player import Player
from mathUtits import *
import fields

# global variables

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080
SCREEN_DIMENSIONS = (SCREEN_WIDTH, SCREEN_HEIGHT)
TILE_SIZE = 64
TILE_DIMENSIONS = (TILE_SIZE, TILE_SIZE)
WORLD_DIMENSIONS = (fields.WORLD_WIDTH, fields.WORLD_HEIGHT, fields.WORLD_LENGTH)
FPS = 60
FORWARD     = np.array([0,     -1])
BACKWARD    = np.array([0,     1])
LEFT        = np.array([-1,    0])
RIGHT       = np.array([1,     0])

# Vector tiles
VEC_UP = 1
VEC_DOWN = 0
VEC_FORWARD = 2
VEC_BACKWARD = 3
VEC_LEFT = 4
VEC_RIGHT = 5

# Visual Modes
MODE_NORMAL = 0
MODE_ELECTRIC_FIELD = 1

# Scientific Constants
epsilon0 = 1


class Game:

    def __init__(self) -> None:

        self.screeen = display.set_mode(SCREEN_DIMENSIONS)
        display.set_caption("Maxwell's Equations")

        self.clock = pg.time.Clock()

        self.physical_set = pg.image.load("tilesets/physical-set.png")
        self.vector_set = pg.image.load("tilesets/vector-set.png")

        self.current_tile_type = fields.TILETYPE_WIRE

        self.current_layer = int(fields.WORLD_HEIGHT/2)

        self.player = Player(64/FPS, 0.65)

        self.player.position = np.array([
            fields.WORLD_WIDTH*TILE_SIZE/2
            , fields.WORLD_LENGTH*TILE_SIZE/2
        ])

        self.mode = MODE_NORMAL

        self.delta = FPS

        fields.init_fields()

    def handle_event(self, event: pg.event.Event) -> None:
        match event.type:
            case pg.QUIT:
                fields.quit()
            case pg.KEYDOWN:
                match event.key:
                    case pg.K_SPACE:
                        self.current_layer -= 1
                    case pg.K_LSHIFT:
                        self.current_layer += 1
    
    def process(self) -> None:
        key_pressed = pg.key.get_pressed()

        player_direction = np.zeros(2)

        if key_pressed[pg.K_w]:
            player_direction += FORWARD
        if key_pressed[pg.K_s]:
            player_direction += BACKWARD
        if key_pressed[pg.K_a]:
            player_direction += LEFT
        if key_pressed[pg.K_d]:
            player_direction += RIGHT
        if key_pressed[pg.K_0]:
            self.current_tile_type = fields.TILETYPE_WIRE
        if key_pressed[pg.K_1]:
            self.current_tile_type = fields.TILETYPE_POSITIVE_CHARGE
        if key_pressed[pg.K_2]:
            self.current_tile_type = fields.TILETYPE_NEGATIVE_CHARGE
        if key_pressed[pg.K_3]:
            self.current_tile_type = fields.TILETYPE_INSULATOR
        if key_pressed[pg.K_ESCAPE]:
            self.mode = MODE_NORMAL
        if key_pressed[pg.K_e]:
            self.mode = MODE_ELECTRIC_FIELD

        
        mouse_pressed = pg.mouse.get_pressed()

        if mouse_pressed[0]:
            mouse_x, mouse_y = pg.mouse.get_pos()
            i = self.world_to_map(self.player.x + mouse_x)
            j = self.world_to_map(self.player.y + mouse_y)
            fields.set_tile_physical_map(i,self.current_layer,j,self.current_tile_type)
            print(f"pos: ${(i,self.current_layer,j)}")
        
        player_direction = normalize(player_direction)

        self.player.move(player_direction, self.delta)

        self.screeen.fill(pg.color.Color(0xFF, 0xFF, 0xFF))

        for i in range(
            self.world_to_map(self.player.x) - 1
            , self.world_to_map(self.player.x + SCREEN_WIDTH) + 1
        ):
            for j in range(
                self.world_to_map(self.player.y) - 1
                , self.world_to_map(self.player.y + SCREEN_HEIGHT) + 1
            ):
                if self.mode == MODE_NORMAL:
                    self.draw_physical_tile(
                        np.array([i*TILE_SIZE,j*TILE_SIZE])
                        , fields.get_tile_physical_map(i,self.current_layer,j)
                    )
                    continue
                if self.mode == MODE_ELECTRIC_FIELD:
                    field = self.as_list(fields.get_node_electric_field(i,self.current_layer,j))
                    direction = max(enumerate(np.abs(field)), key=lambda x: x[1])[0]
                    size = np.linalg.norm(field)
                    vec_direction = 0
                    vec_mag = 0
                    match direction:
                        case 0:
                            if field[direction] < 0:
                                vec_direction = VEC_LEFT
                            else:
                                vec_direction = VEC_RIGHT
                        case 1:
                            if field[direction] < 0:
                                vec_direction = VEC_UP
                            else:
                                vec_direction = VEC_DOWN
                        case 2:
                            if field[direction] < 0:
                                vec_direction = VEC_FORWARD
                            else:
                                vec_direction = VEC_BACKWARD
                    
                    if size > 1/50:
                        vec_mag = 2
                    elif size > 1/2001:
                        vec_mag = 1

                    self.draw_vec_tile(np.array([i*TILE_SIZE,j*TILE_SIZE]), vec_direction + vec_mag*6)
                    continue
    
    def draw_physical_tile(self, position: NDArray, tile_type: int):
        self.draw_tile(self.physical_set, position, tile_type, 64)
    
    def draw_vec_tile(self, position: NDArray, tile_type: int):
        self.draw_tile(self.vector_set, position, tile_type, 6)
    
    def draw_tile(self, tile_set, position: NDArray, tile_type: int, set_width: int):
        self.screeen.blit(
            tile_set
            , position - self.player.position
            , pg.Rect((tile_type % set_width)*TILE_SIZE,int(tile_type / set_width)*TILE_SIZE,TILE_SIZE,TILE_SIZE))
    
    def world_to_map(self, position: float) -> int:
        return int(position/TILE_SIZE)
    
    def field_index(self, i: Tuple[int,int,int]) -> Tuple[int,int,int]:
        x,y,z = i
        return x*2+1,y*2+1,z*2+1
    
    def add_index(self, i: Tuple[int,int,int],j: Tuple[int,int,int]) -> Tuple[int,int,int]:
        xi,yi,zi = i
        xj,yj,zj = j

        return (xi+xj, yi+yj, zi+zj)
    
    def as_list(self, field) -> List:
        return [fields.get_x(field), fields.get_y(field), fields.get_z(field)]

    def game_loop(self):
        while fields.is_running():
            for event in pg.event.get():
                self.handle_event(event)
            
            self.process()

            display.update()
            self.delta = self.clock.tick(FPS)
    
    def __del__(self):
        fields.destr_fields()

pg.init()

game = Game()

game.game_loop()

