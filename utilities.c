
#include <stdlib.h>
#include <ncurses.h>
#include "utilities.h"
#include "definitions.h"


/*
 * draw something
 */
void draw_border(square_t *BORDER) {
    attron(COLOR_PAIR(BORDER_COLOUR));
    rectangle(BORDER->y1, BORDER->x1,
              BORDER->y2, BORDER->x2);
    attroff(COLOR_PAIR(BORDER_COLOUR));
}

/*
 * draw the snake
 */
void draw_snake(snake_t* snake) {
    attron(COLOR_PAIR(SNAKE_COLOUR));
    for (int i=snake->len-2; i>=0; --i) {
        mvprintw(snake->arr[i].y, snake->arr[i].x, "#");
        snake->arr[i+1].y = snake->arr[i].y;
        snake->arr[i+1].x = snake->arr[i].x;
    }

    switch (snake->dir) {
        case UP:
            --(snake->arr[0].y);
            break;
        case DOWN:
            ++(snake->arr[0].y);
            break;
        case LEFT:
            --(snake->arr[0].x);
            break;
        case RIGHT:
            ++(snake->arr[0].x);
            break;
    }
    mvprintw(snake->arr[0].y, snake->arr[0].x, "#");
    attroff(COLOR_PAIR(SNAKE_COLOUR));
}

void draw_ball(square_t *BORDER, pt_t *ball) {
    if (ball->flag) { // set flag will calculate a new random position for the ball
        ball->flag = 0;
        ball->x = BORDER->x1 + rand() % (BORDER->x2 - BORDER->x1);
        ball->y = BORDER->y1 + rand() % (BORDER->y2 - BORDER->y1);
    }
    /*mvprintw(ball->y, ball->x, "O");*/
    mvaddch(ball->y, ball->x, ACS_DIAMOND);
}

void detect_collision(game_t *game_state, square_t *BORDER, snake_t *snake, pt_t *ball) {
    // Get current snake head
    int* snake_x = &(snake->arr[0].x);
    int* snake_y = &(snake->arr[0].y);
    
    // Check for ball collision -- increment score
    if (ball->x == *snake_x &&
            ball->y == *snake_y) {
        ball->flag = 1; // Generate new ball
        ++(snake->len); // increment snake length
        ++game_state->score; // increment score
    }

    // Check for border collision -- game over
    if (*snake_x <= BORDER->x1 || *snake_x >= BORDER->x2 ||
            *snake_y <= BORDER->y1 || *snake_y >= BORDER->y2) {
        game_state->over = true;
        return;
    }

    // Check for self collision -- game over
    for (int i=1; i<snake->len; ++i) {
        if (snake->arr[i].x == *snake_x && snake->arr[i].y == *snake_y) {
            game_state->over = true;
            return;
        }
    }

}

void draw_score(game_t *game_state, square_t *BORDER) {
    mvaddch(BORDER->y2+1, 1, ACS_DIAMOND);
    move(BORDER->y2+1, 5);
    printw("SNAKE!");
    move(BORDER->y2+1, BORDER->x2/2);
    printw("Current score: %d", game_state->score);
}

void game_over() {
    // print end message
    printw("Game over!");
}

void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}
