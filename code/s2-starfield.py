import pygame
from pygame.locals import *
from random import randrange

WIDTH = 1024
HEIGHT = 768 
MAX_STARS = 500
STAR_SPEED = 1
STAR_DEPTH = 255
DIST = 50

def project(x, y, z, win_width, win_height, fov, viewer_distance):
	factor = fov / (viewer_distance + z)
	x2d = win_width/2  + (x * factor)
	y2d = win_height/2 + (y * factor)
	return x2d, y2d

def process_input(fov, viewer_distance):
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			return True, fov, viewer_distance
		elif event.type == KEYDOWN:
			if event.key == K_ESCAPE:
				pygame.quit()
				return True, fov, viewer_distance
	pressed = pygame.key.get_pressed()
	if pressed[K_q]:
		fov = fov + 1
		update_caption(fov, viewer_distance)
	elif pressed[K_a]:
		if fov > 0:
			fov = fov - 1
			update_caption(fov, viewer_distance)
	elif pressed[K_w]:
		viewer_distance = viewer_distance + 1
		update_caption(fov, viewer_distance)
	elif pressed[K_s]:
		if viewer_distance > 0:
			viewer_distance = viewer_distance - 1
			update_caption(fov, viewer_distance)
	return False, fov, viewer_distance

def update_caption(fov, viewer_distance):
	caption = 'Starfield Simulation, fov: '+ str(fov) + ' (q/a), viewer distance: ' + str(viewer_distance) + ' (w/s)'
	pygame.display.set_caption(caption)
	
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

def draw(screen, stars, fov, viewer_distance):
	color = (0, 0, 255)
	for star in stars:
		x, y = project(star[0], star[1], star[2], WIDTH, HEIGHT, fov, viewer_distance)
		size = 5
		r = (int(x), int(y), size, size)
		pygame.draw.rect(screen, color, r)
 
def main():
	pygame.init()
	screen = pygame.display.set_mode((WIDTH, HEIGHT))
	fov = 45
	viewer_distance = 0
	update_caption(fov, viewer_distance)
	clock = pygame.time.Clock()
	
	stars = []
	init(stars)
 
	# Game Loop
	while True:
		# Input
		exit, fov, viewer_distance = process_input(fov, viewer_distance)
		if exit:
			return
		# Logic
		update(stars)
		# Render
		screen.fill((255, 255, 255))
		draw(screen, stars, fov, viewer_distance)
		pygame.display.flip()
		
		clock.tick(50)
 
if __name__ == "__main__":
	main()
