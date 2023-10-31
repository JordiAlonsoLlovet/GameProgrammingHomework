import pygame
from pygame.locals import *
from random import randrange
import math

WIDTH = 1024
HEIGHT = 768 
MAX_STARS = 500
STAR_SPEED = 1
STAR_DEPTH = 255
DIST = 50
MODE_STARFIELD = 1
MODE_WORM = 2
MODE_WAVE = 3
MODE_COOL = 4
# TODO: create a new star mode and update needed functions (input key: '4', init, update, draw)

def init_star(star, mode):
	if mode == MODE_STARFIELD:
		w = WIDTH/2
		h = HEIGHT/2
		x = randrange(-w, w)
		y = randrange(-h, h)
	elif mode == MODE_WORM:
		r = 200
		alpha = randrange(0,360)
		x = r * math.sin(alpha)
		y = r * math.cos(alpha)
	elif mode == MODE_WAVE:
		w = WIDTH/2 
		h = HEIGHT/2
		x = randrange(-w, w)
		y = -1 #unnecessary
	elif mode == MODE_COOL:
		w = WIDTH
		h = 0
		x = randrange(0,1)*w/3
		y = 0
		
	z = STAR_DEPTH
	star[0] = x
	star[1] = y
	star[2] = z
	
def init(stars, mode):
	for i in range(MAX_STARS):
		star = [0, 0, 0]
		init_star(star, mode)
		star[2] = randrange(5, STAR_DEPTH)
		stars.append(star)
 
def update(stars, mode):
	for star in stars:
		if mode == MODE_STARFIELD:
			speed = STAR_SPEED
		elif mode == MODE_WORM:
			speed = STAR_SPEED * 3
		elif mode == MODE_WAVE:
			speed = STAR_SPEED/2
		elif mode == MODE_COOL:
			speed = STAR_SPEED
		star[2] -= speed
		if star[2] < 1:
			init_star(star, mode)

def draw(screen, stars, mode):
	w = WIDTH/2
	h = HEIGHT/2
	for star in stars:
		x = w + ( DIST * float(star[0])/float(star[2]) )
		if mode == MODE_WAVE:
			alpha = (star[2] / float(STAR_DEPTH)) * 8 * math.pi
			star[1] = (h/2) * math.sin(alpha)
		y = h + ( DIST * float(star[1])/float(star[2]) )			
		
		#color = white..color <--> z = [DEPTH..0]
		if mode == MODE_STARFIELD:
			color = (star[2], star[2], 255)
		elif mode == MODE_WORM:
			color = (255, star[2], star[2])
		elif mode == MODE_WAVE:
			color = (star[2]/2, 192, star[2]/2)
		elif mode == MODE_COOL:
			color = (star[2], star[2], star[2])
			
		#size = [10..1] <--> z = [0..DEPTH]
		size = 10 - ( (star[2]/float(STAR_DEPTH)) * 9)
		r = (int(x), int(y), size, size)
		pygame.draw.rect(screen, color, r)
 
def main():
	pygame.init()
	screen = pygame.display.set_mode((WIDTH, HEIGHT))
	pygame.display.set_caption("Starfield Simulation - Keys '1': Starfield, '2': Worm, '3': Wave")
	clock = pygame.time.Clock()
 
	mode = 1
	stars = []
	init(stars, mode)
 
	# Game Loop
	while True:
		# Input
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				return
			elif event.type == KEYDOWN:
				if event.key == K_ESCAPE:
					pygame.quit()
					return
				elif event.key == K_1:
					mode = MODE_STARFIELD
				elif event.key == K_2:
					mode = MODE_WORM
				elif event.key == K_3:
					mode = MODE_WAVE
				elif event.key == K_4:
					mode = MODE_COOL

		# Logic
		update(stars, mode)
		# Render
		screen.fill((255, 255, 255))
		draw(screen, stars, mode)
		pygame.display.flip()
		
		clock.tick(50)
 
if __name__ == "__main__":
	main()
