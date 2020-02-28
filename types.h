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

typedef struct {
    int y, x;
    int flag;
} pt_t;

typedef struct {
    int x1, x2, y1, y2;
} square_t;

typedef struct {
    int len;
    dirs_t dir;
    pt_t arr[MAX_LENGTH];
} snake_t;

typedef struct {
    int over;
    int level;
    int pause;
    int score;
    snake_t snake;
    pt_t ball;
    square_t border;
} game_t;

void init_game(game_t *game_state, int start_len, int game_pause);
void update_border(game_t *game_state, pt_t *SCREEN_MAX);
void initialise_colour_pairs();

#endif // __TYPES_H__
