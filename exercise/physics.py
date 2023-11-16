import pygame
from pygame import Vector2
from pygame import Rect

class Box:
    def __init__(self, pos, size):
        self.pos = pos
        self.size = size
		#Rect(Vector2(left, top), Vector2(width, height))
        self.rect = Rect(pos, Vector2(size, size))
        self.color = (255, 255, 255)

    def intersectBox(self, box):
        return self.rect.colliderect(box.rect)

    def draw(self, screen, color=None):
        if color == None:
            r, g, b = self.color
        else:
            r, g, b = color
        pygame.draw.rect(screen, (r,g,b), self.rect, 1)
        
class Circle:
    def __init__(self, pos, r, color = (255, 255, 255)):
        self.pos = Vector2(pos)
        self.radius = r
        self.r2 = self.radius * self.radius
        self.color = color

    def move(self, v):
        self.pos = self.pos + v

    def intersectCircle(self, circle):
        return self.pos.distance_to(circle.pos) < self.radius + circle.radius

    def intersectBox(self, box):
        cx, cy = self.pos
        rx, ry = box.pos
        rw = rh = box.size

        # temporary variables to set edges for testing
        testX = cx
        testY = cy

        # which edge is closest?
        if cx < rx:
            testX = rx  # test left edge
        elif cx > rx + rw:
            testX = rx + rw  # right edge
        if cy < ry:
            testY = ry  # top edge
        elif cy > ry + rh:
            testY = ry + rh  # bottom edge

        # get distance from closest edges
        distX = cx - testX
        distY = cy - testY
        d2 = (distX * distX) + (distY * distY)

        # if the distance is less than the radius, collision
        return d2 <= self.r2
    
    def draw(self, screen, color=None):
        if color == None:
            r, g, b = self.color
        else:
            r, g, b = color
        x = int(self.pos.x)
        y = int(self.pos.y)
        pygame.draw.circle(screen, (r,g,b), (x,y), self.radius, 1)