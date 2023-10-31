import pygame
from pygame.locals import *
from random import randrange

WIDTH = 1024
HEIGHT = 768 
MAX_STARS = 500
STAR_SPEED = 2
 
def init(stars):
	for i in range(MAX_STARS):
		star = [randrange(0, WIDTH),
				randrange(0, HEIGHT)]
		stars.append(star)
 
def update(stars):
	for star in stars:
		star[1] += STAR_SPEED
		if star[1] >= HEIGHT:
			star[1] = 0
			star[0] = randrange(0, WIDTH)

def draw(screen,stars):
	color = (0, 0, 255)
	for star in stars:
		r = (star[0], star[1], 5, 5)
		pygame.draw.rect(screen, color, r)
 
def main():
	pygame.init()
	screen = pygame.display.set_mode((WIDTH, HEIGHT))
	pygame.display.set_caption("Starfield Simulation")
	clock = pygame.time.Clock()
 
	# Init
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
