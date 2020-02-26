#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "definitions.h"
#include "types.h"

/* function declarations */
void draw_snake_logo(game_t* gs, pt_t *SCREEN_MAX);
void draw_border(game_t*);
void draw_snake(game_t*);
void draw_ball(game_t*);

int detect_self_collision(snake_t* snake, int x, int y);

void detect_collision(game_t *game_state);
void reset_game(game_t* game_state, pt_t* SCREEN_MAX, int start_len);

void draw_score(game_t *game_state);
void game_over(pt_t *SCREEN_MAX);
void rectangle(int y1, int x1, int y2, int x2);

#endif // __UTILITIES_H__
