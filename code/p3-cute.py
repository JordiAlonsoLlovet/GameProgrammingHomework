import pygame
from pygame.locals import *
from random import randrange

WIDTH = 1024
HEIGHT = 768 
MAX_STARS = 500
STAR_SPEED = 1
STAR_DEPTH = 255
DIST = 50

def init_star(star):
	w = WIDTH/2
	h = HEIGHT/2
	x = randrange(-w, w)
	y = randrange(-h, h)
	z = STAR_DEPTH
	star[0] = x
	star[1] = y
	star[2] = z
	
def init(stars):
	for i in range(MAX_STARS):
		star = [0, 0, 0]
		init_star(star)
		star[2] = randrange(5, STAR_DEPTH)
		stars.append(star)
 
def update(stars):
	for star in stars:
		star[2] -= STAR_SPEED
		if star[2] < 1:
			init_star(star)

def draw(screen,stars):
	w = WIDTH/2
	h = HEIGHT/2
	for star in stars:
		x = w + ( DIST * float(star[0])/float(star[2]) )
		y = h + ( DIST * float(star[1])/float(star[2]) )
		# TODO: have fun with other color and size values on purpose
		#color = white..blue <--> z = [DEPTH..0]
		color = (star[2], 255, star[2])
		#size = [10..1] <--> z = [0..DEPTH]
		size = 10 - ( (star[2]/float(STAR_DEPTH)) * 9)
		r = (int(x), int(y), size, size)
		pygame.draw.rect(screen, color, r)
 
def main():
	pygame.init()
	screen = pygame.display.set_mode((WIDTH, HEIGHT))
	pygame.display.set_caption("Starfield Simulation")
	clock = pygame.time.Clock()
 
	stars = []
	init(stars)
 
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
		# Logic
		update(stars)
		# Render
		screen.fill((255, 255, 255))
		draw(screen, stars)
		pygame.display.flip()
		
		clock.tick(50)
 
if __name__ == "__main__":
	main()
