import pygame
from pygame.math import Vector2
from physics import *

class QuadTree:
    def __init__(self, capacity, box):
        self.capacity = capacity
        self.box = box
		# TODO 1: create the attribute list "entities" and initialise to the empty list
        self.entities = []
        
        self.color = (255, 255, 255)

        self.nodeNW = None
        self.nodeNE = None
        self.nodeSW = None
        self.nodeSE = None

    def isLeaf(self):
        return self.nodeNW == None

    def subdivide(self):
        parent = self.box
        size = parent.size/2

        box_nw = Box(Vector2(parent.pos.x,        parent.pos.y       ), size)
        box_ne = Box(Vector2(parent.pos.x + size, parent.pos.y       ), size)
        box_sw = Box(Vector2(parent.pos.x,        parent.pos.y + size), size)
        box_se = Box(Vector2(parent.pos.x + size, parent.pos.y + size), size)

		# TODO 5: create the four children nodes
        self.nodeNW = QuadTree(self.capacity, box_nw)
        self.nodeNE = QuadTree(self.capacity, box_ne)
        self.nodeSW = QuadTree(self.capacity, box_sw)
        self.nodeSE = QuadTree(self.capacity, box_se)

		# TODO 6: insert entities in the children nodes
        for i in range(len(self.entities)):
            self.nodeNW.insert(self.entities[i])
            self.nodeSW.insert(self.entities[i])
            self.nodeNE.insert(self.entities[i])
            self.nodeSE.insert(self.entities[i])            
    
    def insert(self, entity):
		# TODO 2: return false if the given entity does not intersect the box
        if not entity.intersectBox(self.box):
            return False    

        if self.isLeaf():
            if self.box.size <= 4:
                self.entities.append(entity)
                return True
            
			# TODO 3: if there is enough capacity, we append the given entity and return True
            if self.capacity > len(self.entities):
                self.entities.append(entity)
                return True
            
			# TODO 4: there is not enough space, so we call subdivide function
            self.subdivide()
            self.entities.clear()
                        
        inserted = False
        if self.nodeNW.insert(entity):
            inserted = True
        if self.nodeNE.insert(entity):
            inserted = True
        if self.nodeSW.insert(entity):
            inserted = True
        if self.nodeSE.insert(entity):
            inserted = True
        return inserted

    def queryArea(self, shape):
        entitiesInArea = []
        if not shape.intersectBox(self.box):
            return []
        if self.isLeaf():
            return self.entities
        else:
            entitiesInArea.extend(self.nodeNW.queryArea(shape))
            entitiesInArea.extend(self.nodeNE.queryArea(shape))
            entitiesInArea.extend(self.nodeSW.queryArea(shape))
            entitiesInArea.extend(self.nodeSE.queryArea(shape))
        return entitiesInArea

    def draw(self, screen):
        self.box.draw(screen)
		# TODO 7: if the node is not a leaf, draw children recursively
        if not self.isLeaf():
            self.nodeNW.draw(screen)
            self.nodeNE.draw(screen)
            self.nodeSW.draw(screen)
            self.nodeSE.draw(screen)

		
