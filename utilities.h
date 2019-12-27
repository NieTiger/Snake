#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "definitions.h"

/* function declarations */
void draw_border(square_t* BORDER);
void draw_snake(snake_t* snake);
void draw_ball(square_t *BORDER, pt_t *ball);
void detect_collision(game_t *game_state, square_t *BORDER, snake_t *snake, pt_t *ball);
void draw_score(game_t *game_state, square_t *BORDER);
void game_over();
void rectangle(int y1, int x1, int y2, int x2);

#endif // __UTILITIES_H__
