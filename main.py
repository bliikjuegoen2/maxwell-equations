from types import NoneType
import pygame as pg
from pygame import display
import numpy as np
from numpy.typing import *
from player import Player
from mathUtits import *

# global variables

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCREEN_DIMENSIONS = (SCREEN_WIDTH, SCREEN_HEIGHT)
TILE_SIZE = 64
TILE_DIMENSIONS = (TILE_SIZE, TILE_SIZE)
WORLD_WIDTH = 100
WORLD_HEIGHT = 100
WORLD_LENGTH = 100
WORLD_DIMENSIONS = (WORLD_WIDTH, WORLD_HEIGHT, WORLD_LENGTH)
FPS = 60
FORWARD     = np.array([0,     -1])
BACKWARD    = np.array([0,     1])
LEFT        = np.array([-1,    0])
RIGHT       = np.array([1,     0])

# Tile Types
TILETYPE_WIRE = 0
TILETYPE_POSITIVE_CHARGE = 1
TILETYPE_NEGATIVE_CHARGE = 2
TILETYPE_INSULATOR = 3

class Game:

    def __init__(self) -> None:
        self.is_running = True

        self.screeen = display.set_mode(SCREEN_DIMENSIONS)
        display.set_caption("Maxwell's Equations")

        self.clock = pg.time.Clock()

        self.physical_set = pg.image.load("tilesets/physical-set.png")
        self.vector_set = pg.image.load("tilesets/vector-set.png")

        self.current_layer = int(WORLD_HEIGHT/2)

        self.player = Player(64*7/FPS, 0.65)

        self.player.position = np.array([
            WORLD_WIDTH*TILE_SIZE/2
            , WORLD_LENGTH*TILE_SIZE/2
        ])

        self.physical_map = np.zeros(WORLD_DIMENSIONS)

        for i,j,k in np.ndindex(WORLD_DIMENSIONS):
            self.physical_map[i,j,k] = TILETYPE_INSULATOR



    def handle_event(self, event: pg.event.Event) -> None:
        match event.type:
            case pg.QUIT:
                self.is_running = False
    
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
        
        mouse_pressed = pg.mouse.get_pressed()

        if mouse_pressed[0]:
            mouse_x, mouse_y = pg.mouse.get_pos()
            i = self.world_to_map(self.player.x + mouse_x)
            j = self.world_to_map(self.player.y + mouse_y)
            self.physical_map[i,self.current_layer,j] = TILETYPE_WIRE
        
        player_direction = normalize(player_direction)

        self.player.move(player_direction)

        self.screeen.fill(pg.color.Color(0xFF, 0xFF, 0xFF))

        for i in range(
            self.world_to_map(self.player.x) - 1
            , self.world_to_map(self.player.x + SCREEN_WIDTH) + 1
        ):
            for j in range(
                self.world_to_map(self.player.y) - 1
                , self.world_to_map(self.player.y + SCREEN_HEIGHT) + 1
            ):
                self.draw_physical_tile(
                    np.array([i*TILE_SIZE,j*TILE_SIZE])
                    , self.physical_map[i,self.current_layer,j]
                )
    
    def draw_physical_tile(self, position: NDArray, tile_type: int):
        self.screeen.blit(
            self.physical_set
            , position - self.player.position
            , pg.Rect((tile_type % 64)*TILE_SIZE,int(tile_type / 64)*TILE_SIZE,TILE_SIZE,TILE_SIZE))
    
    def world_to_map(self, position: float) -> int:
        return int(position/TILE_SIZE)

    def game_loop(self) -> None:
        while self.is_running:
            for event in pg.event.get():
                self.handle_event(event)
            
            self.process()

            display.update()
            self.clock.tick(FPS)

pg.init()

game = Game()

game.game_loop()