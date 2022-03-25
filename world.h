/**
	@file
	@brief Definitions for world
	@author Tri-Peter Shrive, 2017
**/

#ifndef WORLD_H
#define WORLD_H

#include <assert.h>
#include <ncurses.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TICKET_RATE 100 

#define WORLD_X 50
#define WORLD_Y 20

#define SNAKE_LENGTH 3

int run(
	WINDOW * world,
	int * bit_wall, 
	int * sparse_x_wall, 
	int * sparse_y_wall, 
	int * wall_length, 
	int * bit_snake, 
	int * sparse_x_snake, 
	int * sparse_y_snake, 
	int * snake_length,
	int * bit_food,
	int * sparse_x_food,
	int * sparse_y_food,
	int * current_x_direction,
	int * current_y_direction,
	int ch
	);

int get_direction(
	int * current_x_direction,
	int * current_y_direction,
	int ch
	);

int draw_snake(
	WINDOW * world,
	int * sparse_x_snake, 
	int * sparse_y_snake, 
	int * snake_length
	);

int draw_food(
	WINDOW * world,
	int * sparse_x_food, 
	int * sparse_y_food
	);

int seed_food(
	int * bit_food,
	int * sparse_x_food,
	int * sparse_y_food,
	int * bit_wall,
	int * bit_snake
	);

int move_snake(
	int * bit_wall, 
	int * sparse_x_wall, 
	int * sparse_y_wall, 
	int * wall_length, 
	int * bit_snake, 
	int * sparse_x_snake, 
	int * sparse_y_snake, 
	int * snake_length,
	int * bit_food,
	int * sparse_x_food,
	int * sparse_y_food,
	int * current_x_direction,
	int * current_y_direction
	);

int test_rep(
	int * bit, 
	int * sparse_x, 
	int * sparse_y, 
	int * length
	);

int log_msg(const char * format, ...);

#endif
