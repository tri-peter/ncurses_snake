/**
	@file
	@brief Main file for world
	@author Tri-Peter Shrive, 2017
	@TODO food should not seed inside snake
	@TODO wall tiles
**/

#include "world.h"

int main(
	int argc,
	char *argv[]
	)
{

	WINDOW * world = NULL;
	int offsetx;
	int offsety;
	int ch;
	
	// Get character-at-a-time input without echoing
	initscr();
	cbreak();
	noecho();
	// additional initialisation
	nonl();
	timeout(TICKET_RATE);
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	curs_set(0);
	// create window
	offsetx = (COLS - WORLD_X) / 2;
	offsety = (LINES - WORLD_Y) / 2;
	world = newwin(WORLD_Y, WORLD_X, offsety, offsetx);
	assert(world);

	int * current_x_direction = NULL;
	int * current_y_direction = NULL;
	current_x_direction = calloc(1, sizeof(int));
	current_y_direction = calloc(1, sizeof(int));
	assert(current_x_direction && current_y_direction);

	int * bit_snake = NULL;
	int * sparse_x_snake = NULL;
	int * sparse_y_snake = NULL;
	int * snake_length = NULL;
	bit_snake = calloc(WORLD_X * WORLD_Y, sizeof(int));
	sparse_x_snake = calloc(WORLD_X * WORLD_Y, sizeof(int));
	sparse_y_snake = calloc(WORLD_X * WORLD_Y, sizeof(int));
	snake_length = calloc(1, sizeof(int));
	assert(bit_snake && sparse_x_snake && sparse_y_snake && snake_length);
	
	int * bit_wall = NULL;
	int * sparse_x_wall = NULL;
	int * sparse_y_wall = NULL;
	int * wall_length = NULL;
	bit_wall = calloc(WORLD_X * WORLD_Y, sizeof(int));
	sparse_x_wall = calloc(WORLD_X * WORLD_Y, sizeof(int));
	sparse_y_wall = calloc(WORLD_X * WORLD_Y, sizeof(int));
	wall_length = calloc(1, sizeof(int));
	assert(bit_wall && sparse_x_wall && sparse_y_wall && wall_length);

	int * bit_food = NULL;
	int * sparse_x_food = NULL;
	int * sparse_y_food = NULL;
	bit_food = calloc(WORLD_X * WORLD_Y, sizeof(int));
	sparse_x_food = calloc(1, sizeof(int));
	sparse_y_food = calloc(1, sizeof(int));
	assert(bit_food && sparse_x_food && sparse_y_food);

	// initialise snake
	for(int i = 0; i < SNAKE_LENGTH; i++)
	{
		bit_snake[(offsety-1) * WORLD_X + offsetx + i] = 1;
		sparse_x_snake[i] = offsetx + i;
		sparse_y_snake[i] = offsety;
		*snake_length = *snake_length + 1;
	}
	assert(0 == test_rep(bit_snake, sparse_x_snake, sparse_y_snake, snake_length));

	seed_food(
		bit_food,
		sparse_x_food,
		sparse_y_food,
		bit_wall, 
		bit_snake 
		);

	int end = 0;
	for(;;)
	{
		ch = getch();
		end = run(
			world, 
			bit_wall, 
			sparse_x_wall, 
			sparse_y_wall, 
			wall_length, 
			bit_snake, 
			sparse_x_snake, 
			sparse_y_snake, 
			snake_length, 
			bit_food,
			sparse_x_food,
			sparse_y_food,
			current_x_direction,
			current_y_direction,
			ch
			);

		if(end)
			for(;;){}
	}

	free(current_x_direction);
	free(current_y_direction);
	free(bit_food);
	free(sparse_x_food);
	free(sparse_y_food);
	free(bit_snake);
	free(sparse_x_snake);
	free(sparse_y_snake);
	free(snake_length);
	free(bit_wall);
	free(sparse_x_wall);
	free(sparse_y_wall);
	free(wall_length);

	delwin(world);
	endwin();

	return 0;
}

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
	)
{
	int end = 0;

	wclear(world);

	get_direction(
		current_x_direction,
		current_y_direction,
		ch
		);

	// we only move if we have a direction
	if(0 != *current_x_direction  || 0 != *current_y_direction)
	{
		end = move_snake(
			bit_wall, 
			sparse_x_wall, 
			sparse_y_wall, 
			wall_length, 
			bit_snake, 
			sparse_x_snake, 
			sparse_y_snake, 
			snake_length,
			bit_food,
			sparse_x_food,
			sparse_y_food,
			current_x_direction,
			current_y_direction
			);
	}

	draw_food(
		world,
		sparse_x_food,
		sparse_y_food
		);

	draw_snake(
		world,
		sparse_x_snake, 
		sparse_y_snake, 
		snake_length
		);

	box(world, 0, 0);
	wrefresh(world);

	return end;
}

int get_direction(
	int * current_x_direction,
	int * current_y_direction,
	int ch	
	)
{
	// check for change of direction
	// P = (0, 0) is the top left hand corner
	switch(ch)
	{
		// direction left - ASCII h: 104
		// direction left - ASCII a: 97
		case 97:
			if(0 == *current_x_direction)
			{
				*current_x_direction = -1;
				*current_y_direction = 0;
			}
			break;
		// direction down - ASCII j: 106
		// direction down - ASCII s: 115
		case 115:
			if(0 == *current_y_direction)
			{
				*current_x_direction = 0;
				*current_y_direction = 1;
			}
			break;
		// direction up - ASCII k: 107
		// direction up - ASCII w: 119
		case 119:
			if(0 == *current_y_direction)
			{
				*current_x_direction = 0;
				*current_y_direction = -1;
			}
			break;
		// direction right - ASCII l: 108
		// direction right - ASCII d: 100
		case 100:
			if(0 == *current_x_direction)
			{
				*current_x_direction = 1;
				*current_y_direction = 0;
			}
			break;
		default:
			break;
	}

	return 0;
}


int draw_snake(
	WINDOW * world,
	int * sparse_x_snake, 
	int * sparse_y_snake, 
	int * snake_length
	)
{
	int x;
	int y;
	for(int i = 0; i < *snake_length; i++)
	{
		x = sparse_x_snake[i];
		y = sparse_y_snake[i];
		mvwaddch(world, y, x, 'X');
	}
	return 0;
}

int draw_food(
	WINDOW * world,
	int * sparse_x_food, 
	int * sparse_y_food
	)
{
	int x;
	int y;
	x = *sparse_x_food;
	y = *sparse_y_food;
	mvwaddch(world, y, x, 'O');

	return 0;
}

int seed_food(
	int * bit_food,
	int * sparse_x_food,
	int * sparse_y_food,
	int * bit_wall,
	int * bit_snake
	)
{
	int x;
	int y;
	int i;

	for(;;)
	{
		x = rand() % (WORLD_X - 3);
		y = rand() % (WORLD_Y - 3);
		x++;
		y++;

		if(0 == bit_snake[(y-1) * WORLD_X + x]) break;
	}

	*sparse_x_food = x;
	*sparse_y_food = y;

	i = (y - 1) * WORLD_X + x;
	bit_food[i] = 1;

	return 0;
}

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
	)
{
	int x;
	int y;
	int tail_x;
	int tail_y;
	int end = 0;

	assert(0 == test_rep(bit_snake, sparse_x_snake, sparse_y_snake, snake_length));

	// get tail end of snake
	tail_x = sparse_x_snake[*snake_length - 1];
	tail_y = sparse_y_snake[*snake_length - 1];

	// move sparse representation along
	assert( (WORLD_X * WORLD_Y) > *snake_length);
	assert( 0 < *snake_length);
	for(int i = *snake_length; 0 != i; i--)
	{
		sparse_x_snake[i] = sparse_x_snake[i-1];
		sparse_y_snake[i] = sparse_y_snake[i-1];
	}

	// get head of snake
	// at the moment the head has two identical sparse representations
	// let's change that
	x = sparse_x_snake[0];
	y = sparse_y_snake[0];

	// calculate movement
	x = x + *current_x_direction;
	y = y + *current_y_direction;

	// wrap arround world
	if(WORLD_X - 1 == x)
		x = 1;
	else if(0 == x)
		x = WORLD_X - 2;
	
	if(WORLD_Y - 1 == y)
		y = 1;
	else if(0 == y)
		y = WORLD_Y - 2;

	// check for food
	if(1 == bit_food[(y-1) * WORLD_X + x])
	{
		log_msg("INFO: FOOD FOUND AT (%d, %d)\n", x, y);
		bit_food[(y-1) * WORLD_X + x] = 0;
		*snake_length = *snake_length + 1;
		
		assert(WORLD_X * WORLD_Y > *snake_length);

		seed_food(
			bit_food,
			sparse_x_food,
			sparse_y_food,
			bit_wall, 
			bit_snake 
			);
	}
	else
	{
		// remove old bit representation
		bit_snake[(tail_y-1) * WORLD_X + tail_x] = 0;
	}

	// collision detection
	if(1 == bit_snake[(y-1) * WORLD_X + x])
	{
		end = 1;
		log_msg("INFO: COLLISION AT (%d, %d)\n", x, y);
		log_msg("INFO: CURRENT LENGTH = %d\n", *snake_length);
	}
	
	// place new bit representation
	bit_snake[(y-1) * WORLD_X + x] = 1;

	// update sparse representation
	sparse_x_snake[0] = x;
	sparse_y_snake[0] = y;

	if(!end) assert(0 == test_rep(bit_snake, sparse_x_snake, sparse_y_snake, snake_length));

	return end;
}
/**
	tests the validity a given representation
	@returns the number of errors in the representation
**/
int test_rep(
	int * bit,
	int * sparse_x,
	int * sparse_y,
	int * length
	)
{
	int err = 0;
	int x;
	int y;
	int b = 0;

	// check that length is equal to number of bits in bitmap
	for(int i = 0; i < WORLD_X * WORLD_Y; i++)
		if(1 == bit[i])
		{
			b++;
		}

	if( *length != b )
	{
		log_msg("DEBUG: sparse_length = %d\n", *length);
		log_msg("DEBUG: bit_length = %d\n", b);
		err = 1;
	}

	// check that each sparse entry has corresponding bit in bitmap	
	for(int i = 0; i < *length; i++)
	{
		x = sparse_x[i];
		y = sparse_y[i];
		if(1 != bit[(y-1) * WORLD_X + x])
		{
			err = 1;
			log_msg("DEBUG: bit[%d] = %d != 1\n", (y-1) * WORLD_X + x, bit[(y-1) * WORLD_X + x]);
		}
	}
	
	if(err)
	{
		for(int i = 0; i < *length; i++)
			log_msg("DEBUG: sparse_snake (%d, %d)\n", sparse_x[i], sparse_y[i]);

		for(int i = (WORLD_X * WORLD_Y - 1); i != -1; i--)
		{
			log_msg("%d ", bit[i]);
			if( i % WORLD_X == 0 ) log_msg("\n");
		}
	}

	return err;
}

int log_msg(const char * format, ...)
{
	va_list ap;
	FILE *fp = NULL;

	fp = fopen("log.txt", "a+");
	assert(fp);
	va_start(ap, format);
	vfprintf(fp, format, ap);
	va_end(ap);
	fclose(fp);

	return 0;
}

