import pygame
from math import sqrt
from pygame.math import Vector2
from random import uniform
from physics import *

class Entity:
    def __init__(self, pos = Vector2(0, 0), radius=2, color=(255, 255, 255)):
        self.circle = Circle(pos, radius, color)

    def setColor(self, color):
        self.circle.color = color
        
    def getPosition(self):
        return self.circle.pos

    def move(self):
        self.circle.move( (uniform(-4, 4), uniform(-4, 4)) )

    def intersectEntity(self, entity):
        return self.circle.intersectCircle(entity.circle)
    
    def intersectCircle(self, circle):
        return self.circle.intersectCircle(circle)
        
    def intersectBox(self, box):
        return self.circle.intersectBox(box)

    def draw(self, screen, color=None):
        if color == None:
            r, g, b = self.circle.color
        else:
            r, g, b = color
        self.circle.draw(screen, (r,g,b))