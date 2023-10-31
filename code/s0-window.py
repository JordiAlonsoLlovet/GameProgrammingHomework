import pygame
from pygame.locals import *
from random import randrange
 
def main():
	pygame.init()
	screen = pygame.display.set_mode((1024, 768))
	pygame.display.set_caption("Starfield Simulation")
	clock = pygame.time.Clock()
 
	# Init
 
	# Game Loop
	while True:
		# Input
		for event in pygame.event.get():
			if event.type == QUIT:
				return
			elif event.type == KEYDOWN:
				if event.key == K_ESCAPE:
					pygame.quit()
					return
		# Logic
		
		# Render
		r = randrange(0,255)
		g = randrange(0,255)
		b = randrange(0,255)
		screen.fill((r,g,b))
		pygame.display.flip()
		
		clock.tick(1)
 
if __name__ == "__main__":
	main()
