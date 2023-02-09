from types import NoneType
import pygame as pg
from pygame import display
import numpy as np

# global variables

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCREEN_DIMENSIONS = (SCREEN_WIDTH, SCREEN_HEIGHT)
FPS = 60

class Game:

    def __init__(self) -> None:
        self.is_running = True

        self.screeen = display.set_mode(SCREEN_DIMENSIONS)
        display.set_caption("Maxwell's Equations")

        self.clock = pg.time.Clock()

    def handle_event(self, event: pg.event.Event) -> None:
        if event.type == pg.QUIT:
            self.is_running = False
    
    def process(self) -> None:
        pass

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