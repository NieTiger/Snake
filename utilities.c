
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
    for (int i=gs->snake->len-2; i>=0; --i) {
        mvprintw(gs->snake->arr[i].y, gs->snake->arr[i].x, " ");
        mvprintw(gs->snake->arr[i].y, gs->snake->arr[i].x+1, " ");
        gs->snake->arr[i+1].y = gs->snake->arr[i].y;
        gs->snake->arr[i+1].x = gs->snake->arr[i].x;
    }

    switch (gs->snake->dir) {
        case UP:
            --(gs->snake->arr[0].y);
            break;
        case DOWN:
            ++(gs->snake->arr[0].y);
            break;
        case LEFT:
            gs->snake->arr[0].x -= 2;
            break;
        case RIGHT:
            gs->snake->arr[0].x += 2;
            break;
    }
    attroff(COLOR_PAIR(SNAKE_COLOUR));

    // Draw snake head
    attron(COLOR_PAIR(SNAKE_COLOUR_HEAD));
    mvaddch(gs->snake->arr[0].y, gs->snake->arr[0].x, ACS_BULLET);
    mvaddch(gs->snake->arr[0].y, gs->snake->arr[0].x+1, ACS_BULLET);
    attroff(COLOR_PAIR(SNAKE_COLOUR_HEAD));
}

void draw_ball(game_t* gs) {
    if (gs->ball.flag) { // set flag will calculate a new random position for the ball
        gs->ball.flag = 0;

        do {
            gs->ball.x = gs->border.x1 + 2 + rand() % ((gs->border.x2 - gs->border.x1)/2 - 1) * 2;
            gs->ball.y = gs->border.y1 + 1 + rand() % (gs->border.y2 - 1 - gs->border.y1);
        } while (detect_self_collision(gs->snake, gs->ball.x, gs->ball.y));
    }

    mvaddch(gs->ball.y, gs->ball.x, ACS_CKBOARD);
    mvaddch(gs->ball.y, gs->ball.x+1, ACS_CKBOARD);

}

int detect_self_collision(snake_t* snake, int x, int y) {
    for (int i=1; i<snake->len; ++i) {
        if (snake->arr[i].x == x && snake->arr[i].y == y) {
            return true;
        }
    }
    return false;
}

void detect_collision(game_t *gs) {
    // Get current snake head
    int* snake_x = &(gs->snake->arr[0].x);
    int* snake_y = &(gs->snake->arr[0].y);

    // Check for ball collision -- increment score
    // If scored, also update speed
    if (gs->ball.x == *snake_x &&
            gs->ball.y == *snake_y) {
        gs->ball.flag = 1; // Generate new ball
        ++(gs->snake->len); // increment snake length
        ++gs->score; // increment score

        // Update highscore
        if (gs->score > gs->highscore) {
            gs->highscore = gs->score;
        }

        // Update speed
        if (gs->score % 5 == 0) {
            gs->pause -= 10;
        }
    }

    // Check for border collision -- game over
    if (*snake_x <= gs->border.x1 || *snake_x >= gs->border.x2 ||
            *snake_y <= gs->border.y1 || *snake_y >= gs->border.y2) {
        gs->over = true;
        return;
    }

    // Check for self collision -- game over
    if (detect_self_collision(gs->snake, *snake_x, *snake_y)) {
        gs->over = true;
    }

}

void draw_score(game_t *gs) {

    // Print score
    move(gs->border.y2+1, gs->border.x1);
    printw("Score: %1d", gs->score);

    // Print highscore
    move(gs->border.y2+2, gs->border.x1);
    printw("High Score: %1d", gs->highscore);

    // Print length
    move(gs->border.y2+1, gs->border.x1+10);
    printw("Length: %1d", gs->snake->len);

    // Print speed
    move(gs->border.y2+1, gs->border.x1+21);
    printw("Speed: %1d ms", gs->pause);

    // Print instructions
    /*move(gs->border.y2+2, gs->border.x1);*/
    /*printw("W: up | S: Down | A: Left | D: Right");*/

    move(gs->border.y2+3, gs->border.x1);
    printw("Press r to restart");

}

void game_over(game_t *gs, pt_t *SCREEN_MAX) {
    // print end message
    int x_begin = SCREEN_MAX->x/2 - 53/2 - 1;
    int total_lines = 10;
    int y_begin = gs->border.y1 - total_lines/2;

    move(y_begin+1, x_begin);
    printw("  ____    _    __  __ _____    _____     _______ ____");
    move(y_begin+2, x_begin);
    printw(" / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\");
    move(y_begin+3, x_begin);
    printw("| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |");
    move(y_begin+4, x_begin);
    printw("| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <");
    move(y_begin+5, x_begin);
    printw(" \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\");

    move(y_begin+7, x_begin);
    printw("Better luck next time!");

    move(y_begin+9, x_begin);
    printw("Press q to quit. ");
    move(y_begin+10, x_begin);
    printw("Press r to restart. ");
}

void draw_snake_logo(game_t* gs, pt_t *SCREEN_MAX) {
    int x_ = SCREEN_MAX->x/2-30/2;

    move(gs->border.y1-6, x_);
    printw(" ____  _   _    _    _  _______   ");
    move(gs->border.y1-5, x_);
    printw("/ ___|| \\ | |  / \\  | |/ / ____|");
    move(gs->border.y1-4, x_);
    printw("\\___ \\|  \\| | / _ \\ | ' /|  ___|");
    move(gs->border.y1-3, x_);
    printw(" ___) | |\\  |/ ___ \\| . \\| |___ ");
    move(gs->border.y1-2, x_);
    printw("|____/|_| \\_/_/   \\_\\_|\\_\\_____|");

}


void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);

    mvvline(y1, x1, 0, y2-y1+1);
    mvvline(y1, x1+1, 0, y2-y1+1);

    mvvline(y1, x2, 0, y2-y1+1);
    mvvline(y1, x2+1, 0, y2-y1+1);


    /*mvaddch(y1, x1, ACS_ULCORNER);*/
    /*mvaddch(y2, x1, ACS_LLCORNER);*/
    /*mvaddch(y1, x2, ACS_URCORNER);*/
    /*mvaddch(y2, x2, ACS_LRCORNER);*/
}
