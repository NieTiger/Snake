#ifndef __TYPES_H__
#define __TYPES_H__

#include "definitions.h"

// Type definitions
typedef enum {
    NO_BOT, BOT
} BOT_FLAG;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} dirs_t;

// pt_t stores a single 2D point on the game display
typedef struct {
    int y, x;
    int flag;
} pt_t;

// square_t holds the 4 corners of a square
typedef struct {
    int x1, x2, y1, y2;
} square_t;

// snake_t holds states of the snake
typedef struct {
    int len;
    dirs_t dir;
    pt_t arr[MAX_LENGTH];
} snake_t;

// game_t holds the global game states
typedef struct {
    int over;
    int level;
    int pause;
    int score;
    int highscore;
    snake_t* snake;
    pt_t ball;
    square_t border;
} game_t;

game_t* make_game();
void destroy_game(game_t* game_ptr);

void init_game(game_t *game_state, int start_len, int game_pause, int highscore);
void update_border(game_t *game_state, pt_t *SCREEN_MAX);
void initialise_colour_pairs();

#endif // __TYPES_H__
