
#include <stdlib.h>
#include <ncurses.h>
#include "types.h"
#include "utilities.h"
#include "definitions.h"

/*
 * draw something
 */
void draw_border(game_t* gs) {
    attron(COLOR_PAIR(BORDER_COLOUR));
    rectangle(gs->border.y1, gs->border.x1,
              gs->border.y2, gs->border.x2);
    attroff(COLOR_PAIR(BORDER_COLOUR));
}

/*
 * draw the snake
 */
void draw_snake(game_t* gs) {
    attron(COLOR_PAIR(SNAKE_COLOUR));
    for (int i=gs->snake.len-2; i>=0; --i) {
        mvprintw(gs->snake.arr[i].y, gs->snake.arr[i].x, " ");
        gs->snake.arr[i+1].y = gs->snake.arr[i].y;
        gs->snake.arr[i+1].x = gs->snake.arr[i].x;
    }

    switch (gs->snake.dir) {
        case UP:
            --(gs->snake.arr[0].y);
            break;
        case DOWN:
            ++(gs->snake.arr[0].y);
            break;
        case LEFT:
            --(gs->snake.arr[0].x);
            break;
        case RIGHT:
            ++(gs->snake.arr[0].x);
            break;
    }
    attroff(COLOR_PAIR(SNAKE_COLOUR));
    attron(COLOR_PAIR(SNAKE_COLOUR_HEAD));
    mvaddch(gs->snake.arr[0].y, gs->snake.arr[0].x, ACS_BULLET);
    attroff(COLOR_PAIR(SNAKE_COLOUR_HEAD));
}

void draw_ball(game_t* gs) {
    if (gs->ball.flag) { // set flag will calculate a new random position for the ball
        gs->ball.flag = 0;
        gs->ball.x = gs->border.x1 + 1 + rand() % (gs->border.x2 - 1 - gs->border.x1);
        gs->ball.y = gs->border.y1 + 1 + rand() % (gs->border.y2 - 1 - gs->border.y1);
    }
    /*mvprintw(gs->ball.y, gs->ball.x, "O");*/
    mvaddch(gs->ball.y, gs->ball.x, ACS_DIAMOND);
}

void detect_collision(game_t *gs) {
    // Get current snake head
    int* snake_x = &(gs->snake.arr[0].x);
    int* snake_y = &(gs->snake.arr[0].y);
    
    // Check for ball collision -- increment score and speed
    if (gs->ball.x == *snake_x &&
            gs->ball.y == *snake_y) {
        gs->ball.flag = 1; // Generate new ball
        ++(gs->snake.len); // increment snake length
        ++gs->score; // increment score
        gs->pause -= 10*(gs->score / 5);
    // increment
    }

    // Check for border collision -- game over
    if (*snake_x <= gs->border.x1 || *snake_x >= gs->border.x2 ||
            *snake_y <= gs->border.y1 || *snake_y >= gs->border.y2) {
        gs->over = true;
        return;
    }

    // Check for self collision -- game over
    for (int i=1; i<gs->snake.len; ++i) {
        if (gs->snake.arr[i].x == *snake_x && gs->snake.arr[i].y == *snake_y) {
            gs->over = true;
            return;
        }
    }

}

void draw_score(game_t *gs) {
    // Print ball, snake
    mvaddch(gs->border.y2+1, gs->border.x1+1, ACS_DIAMOND);
    move(gs->border.y2+1, gs->border.x1+3);
    printw("SNAKE!");

    // Print score
    move(gs->border.y2+1, gs->border.x2-17);
    printw("Current score: %2d", gs->score);

    // Print instructions
    move(gs->border.y2+2, gs->border.x1+1);
    printw("W: up | S: Down | A: Left | D: Right");
    move(gs->border.y2+3, gs->border.x1+1);
    printw("R: restart");
    
}

void game_over(pt_t *SCREEN_MAX) {
    // print end message
    int x_begin = SCREEN_MAX->x/2 - 53/2 - 1;
    move(1, x_begin);
    printw("  ____    _    __  __ _____    _____     _______ ____");
    move(2, x_begin);
    printw(" / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\");
    move(3, x_begin);
    printw("| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |");
    move(4, x_begin);
    printw("| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <");
    move(5, x_begin);
    printw(" \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\");

    move(7, x_begin);
    printw("Press q to quit. ");
    move(8, x_begin);
    printw("Press r to restart. ");
    move(10, x_begin);
    printw("Better luck next time!");

}

void draw_snake_logo(game_t* gs, pt_t *SCREEN_MAX) {
    int x_ = SCREEN_MAX->x/2-43/2;
    
    move(gs->border.y1-5, x_);
    printw(" ____  _   _    _    _  _______   _   _   _");
    move(gs->border.y1-4, x_);
    printw("/ ___|| \\ | |  / \\  | |/ / ____| | | | | | |");
    move(gs->border.y1-3, x_);
    printw("\\___ \\|  \\| | / _ \\ | ' /|  _|   | | | | | |");
    move(gs->border.y1-2, x_);
    printw(" ___) | |\\  |/ ___ \\| . \\| |___  |_| |_| |_|");
    move(gs->border.y1-1, x_);
    printw("|____/|_| \\_/_/   \\_\\_|\\_\\_____| (_) (_) (_)");

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
