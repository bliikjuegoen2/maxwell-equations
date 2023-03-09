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
ROW_SIZE = np.floor(SCREEN_WIDTH/TILE_SIZE)
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

# UI stuff

# Visual Modes
MODE_NORMAL = 0
MODE_ELECTRIC_FIELD = 1
MODE_MAGNETIC_FIELD = 2
MODE_INVENTORY = 3
# MODE_CURRENT = 2
# MODE_CHARGE = 3

mode_to_vec_field = {
    MODE_ELECTRIC_FIELD: fields.get_node_electric_field
    , MODE_MAGNETIC_FIELD: fields.get_node_magnetic_field
    # , MODE_CURRENT: fields.get_current_field
}

mode_to_scalar_field = {
    # MODE_CHARGE: fields.get_node_charge_field
}
    
ITEMBAR_KEYS = [
    pg.K_1
    , pg.K_2
    , pg.K_3
    , pg.K_4
    , pg.K_5
    , pg.K_6
    , pg.K_7
    , pg.K_8
    , pg.K_9
    , pg.K_0
    ]

KEYS_TO_INDEX = {key:i for i,key in enumerate(ITEMBAR_KEYS)}

TILETYPES = [
    fields.TILETYPE_POSITIVE_CHARGE
    , fields.TILETYPE_NEGATIVE_CHARGE
    , fields.TILETYPE_INSULATOR
    , fields.TILETYPE_UP_CURRENT
    , fields.TILETYPE_DOWN_CURRENT
    , fields.TILETYPE_LEFT_CURRENT
    , fields.TILETYPE_RIGHT_CURRENT
    , fields.TILETYPE_FORWARD_CURRENT
    , fields.TILETYPE_BACKWARD_CURRENT
    , fields.TILETYPE_WIRE
    ]

ITEMBAR = TILETYPES[:10]

# Scientific Constants
epsilon0 = 1


class Game:

    def __init__(self) -> None:

        self.screeen = display.set_mode(SCREEN_DIMENSIONS)
        display.set_caption("Maxwell's Equations")

        self.clock = pg.time.Clock()

        self.physical_set = pg.image.load("tilesets/physical-set.png")
        self.vector_set = pg.image.load("tilesets/vector-set.png")
        self.scalar_set = pg.image.load("tilesets/scalar-set.png")

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

        if self.mode == MODE_INVENTORY:
            self.process_inventory()
            return

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
        
        for key in ITEMBAR_KEYS:
            if key_pressed[key]:
                self.current_tile_type = ITEMBAR[KEYS_TO_INDEX[key]]
        
        if key_pressed[pg.K_ESCAPE]:
            self.mode = MODE_NORMAL
        if key_pressed[pg.K_e]:
            self.mode = MODE_ELECTRIC_FIELD
        if key_pressed[pg.K_b]:
            self.mode = MODE_MAGNETIC_FIELD
        if key_pressed[pg.K_e]:
            self.mode = MODE_INVENTORY
        # if key_pressed[pg.K_i]:
        #     self.mode = MODE_CURRENT
        # if key_pressed[pg.K_q]:
        #     self.mode = MODE_CHARGE

        
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
                if self.mode in mode_to_vec_field.keys():
                    field = self.as_list(mode_to_vec_field[self.mode](i,self.current_layer,j))
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
                if self.mode in mode_to_scalar_field.keys():
                    scalar = fields.get_scalar(mode_to_scalar_field[self.mode](i,self.current_layer,j))
                    tile = 4
                    if scalar <= -0.875:
                        tile = 0
                    elif scalar <= -0.625:
                        tile = 1
                    elif scalar <= -0.375:
                        tile = 2
                    elif scalar <= -0.125:
                        tile = 3
                    elif scalar <= 0.125:
                        tile = 4
                    elif scalar <= 0.375:
                        tile = 5
                    elif scalar <= 0.625:
                        tile = 6
                    elif scalar <= 0.875:
                        tile = 7
                    else:
                        tile = 8
                    self.draw_tile(self.scalar_set, np.array([i*TILE_SIZE,j*TILE_SIZE]), tile, 9)
                    continue
    
    def process_inventory(self):
        key_pressed = pg.key.get_pressed()

        if key_pressed[pg.K_ESCAPE]:
            self.mode = MODE_NORMAL
        
        for key in ITEMBAR_KEYS:
            if key_pressed[key]:
                ITEMBAR[KEYS_TO_INDEX[key]] = self.current_tile_type
        
        mouse_pressed = pg.mouse.get_pressed()
        (mouse_pos_x, mouse_pos_y) = pg.mouse.get_pos()

        if mouse_pressed[0]:
            def block():
                column = np.floor(mouse_pos_x/TILE_SIZE)
                row = np.floor(mouse_pos_y/TILE_SIZE)

                if row >= ROW_SIZE:
                    return
                
                tiletype_index = int(row * 64 + column)

                if tiletype_index >= len(TILETYPES):
                    return

                tiletype = TILETYPES[tiletype_index]
                
                self.current_tile_type = tiletype
            
            block()
            
        
        # need to save the player position
        # drawing the inventory messes it up
        player_position = self.player.position
        self.player.position = np.zeros(2)

        for i,tiletype in enumerate(TILETYPES):
            row = np.floor(i/ROW_SIZE)
            column = i % ROW_SIZE
            self.draw_physical_tile(np.array([column*TILE_SIZE,row*TILE_SIZE]), tiletype)
        self.player.position = player_position
    
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

