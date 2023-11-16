import pygame

from random import randint
from quadtree import *
from entity import Entity
from pygame.math import Vector2
from pygame import mouse
from physics import *

SIZE = 800             # quadtree size = windows width = windows height
NUM_ENTITIES = 512
ENTITY_RADIUS = 10
NODE_CAPACITY = 2   # >=2

FLAG_ENABLE_QUADTREE = 0
FLAG_SHOW_QUADTREE   = 1
FLAG_MOVE_ENTITIES   = 2
FLAG_SHOW_ENTITIES   = 3
FLAG_SIZE_SHAPE      = 4
FLAG_COLLISION_MODE  = 5
TEST_NONE     = 0
TEST_ENTITIES = 1
TEST_BOX      = 2
TEST_CIRCLE   = 3

COLOR_RED   = (255,  63,  63)
COLOR_WHITE = (255, 255, 255)
COLOR_BLACK = (  0,   0,   0)

def init(entities):
    for i in range(NUM_ENTITIES):
        offset = 32
        x = randint(offset, SIZE-offset)
        y = randint(offset, SIZE-offset)
        entity = Entity(Vector2(x, y), ENTITY_RADIUS, COLOR_WHITE)
        entities.append(entity)

def input(entities, flags):
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return False
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_ESCAPE:
                return False
            if event.key == pygame.K_t:
                flags[FLAG_ENABLE_QUADTREE] = not flags[FLAG_ENABLE_QUADTREE]
            if event.key == pygame.K_m:
                flags[FLAG_MOVE_ENTITIES] = not flags[FLAG_MOVE_ENTITIES]
            if event.key == pygame.K_e:
                flags[FLAG_SHOW_ENTITIES] = not flags[FLAG_SHOW_ENTITIES]
            if event.key == pygame.K_q:
                flags[FLAG_SHOW_QUADTREE] = not flags[FLAG_SHOW_QUADTREE]
            if event.key == pygame.K_0:
                flags[FLAG_COLLISION_MODE] = TEST_NONE
            if event.key == pygame.K_1:
                flags[FLAG_COLLISION_MODE] = TEST_ENTITIES
            if event.key == pygame.K_2:
                flags[FLAG_COLLISION_MODE] = TEST_BOX
            if event.key == pygame.K_3:
                flags[FLAG_COLLISION_MODE] = TEST_CIRCLE
            if event.key == pygame.K_d:
                for e in entities:
                    print(e.circle.pos)
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1: #left
                x, y = pygame.mouse.get_pos()
                entity = Entity(Vector2(x, y), ENTITY_RADIUS, COLOR_WHITE)
                entities.append(entity)
            elif event.button == 2: #middle
                entities.clear()
            elif event.button == 3: #right
                if len(entities)>0:
                    entities.pop()
            elif event.button == 4: #wheel up
                flags[FLAG_SIZE_SHAPE] += 10
            elif event.button == 5: #wheel down
                flags[FLAG_SIZE_SHAPE] -= 10
                if flags[FLAG_SIZE_SHAPE] < 10:
                    flags[FLAG_SIZE_SHAPE] = 10
            
    return True

def update(screen, quadtree, entities, flags):
    # move
    for entity in entities:
        if flags[FLAG_MOVE_ENTITIES]:
            entity.move()
        quadtree.insert(entity)
    # check collisions
    if flags[FLAG_COLLISION_MODE] == TEST_NONE:
        for entity in entities:
            entity.setColor(COLOR_WHITE)
        return
    if flags[FLAG_COLLISION_MODE] == TEST_ENTITIES:
        for entity in entities:
            if flags[FLAG_ENABLE_QUADTREE]:
                candidates = quadtree.queryArea(entity)
            else:
                candidates = entities
            for candidate in candidates:
                if entity != candidate:
                    if entity.intersectEntity(candidate):
                        entity.setColor(COLOR_RED)
                        break
                    else:
                        entity.setColor(COLOR_WHITE)
    else:
        for entity in entities:
            entity.setColor(COLOR_WHITE)
    if flags[FLAG_COLLISION_MODE] == TEST_BOX:
        size = flags[FLAG_SIZE_SHAPE]
        x, y = mouse.get_pos()
        x -= size//2
        y -= size//2
        box = Box(Vector2(x,y), size)
        if flags[FLAG_ENABLE_QUADTREE]:
            candidates = quadtree.queryArea(box)
        else:
            candidates = entities
        for candidate in candidates:
            if candidate.intersectBox(box):
                candidate.setColor(COLOR_RED)
            else:
                candidate.setColor(COLOR_WHITE)
    elif flags[FLAG_COLLISION_MODE] == TEST_CIRCLE:
        r = flags[FLAG_SIZE_SHAPE] // 2
        circle = Circle(Vector2(mouse.get_pos()), r)
        if flags[FLAG_ENABLE_QUADTREE]:
            candidates = quadtree.queryArea(circle)
        else:
            candidates = entities
        for candidate in candidates:
            if candidate.intersectCircle(circle):
                candidate.setColor(COLOR_RED)
            else:
                candidate.setColor(COLOR_WHITE)

def draw(screen, clock, quadtree, entities, flags):
    screen.fill(COLOR_BLACK)
    if flags[FLAG_SHOW_ENTITIES]:
        for entity in entities:
            entity.draw(screen)
    if flags[FLAG_SHOW_QUADTREE]:
        quadtree.draw(screen)
    if flags[FLAG_COLLISION_MODE] == TEST_BOX:
        size = flags[FLAG_SIZE_SHAPE]
        x, y = mouse.get_pos()
        x -= size//2
        y -= size//2
        box = Box(Vector2(x,y), size)
        box.draw(screen, COLOR_RED)
    elif flags[FLAG_COLLISION_MODE] == TEST_CIRCLE:
        r = flags[FLAG_SIZE_SHAPE] // 2
        circle = Circle(Vector2(mouse.get_pos()), r)
        circle.draw(screen, COLOR_RED)
    if flags[FLAG_ENABLE_QUADTREE]:
        enable = 'ON'
    else:
        enable = 'OFF'
    s = 'FPS:' + str(int(clock.get_fps())) + ' - Quadtree: ' + enable
    pygame.display.set_caption(s)
    pygame.display.flip()
    clock.tick(120)

def main():
    pygame.init()
    screen = pygame.display.set_mode((SIZE, SIZE))
    pygame.display.set_caption('Quadtree test')
    clock = pygame.time.Clock()
    
    # Init
    flags = {FLAG_ENABLE_QUADTREE: True,
             FLAG_SHOW_QUADTREE: True,
             FLAG_MOVE_ENTITIES: True,
             FLAG_SHOW_ENTITIES: True,
             FLAG_SIZE_SHAPE: SIZE//4,
             FLAG_COLLISION_MODE: TEST_ENTITIES
    }
    entities = []
    init(entities)
    
    # Game Loop
    while True:
        # Input
        if not input(entities, flags):
            break
        # Logic
        box = Box(Vector2(0, 0), SIZE)
        quadtree = QuadTree(NODE_CAPACITY, box)
        update(screen, quadtree, entities, flags)
        # Draw
        draw(screen, clock, quadtree, entities, flags)

    pygame.quit()

if __name__ == "__main__":
	main()