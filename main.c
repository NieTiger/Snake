/*
 * @Author: Tiger Nie
 * @github: http://github.com/haolinnie
 */
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "definitions.h"
#include "utilities.h"


int main(int argc, char** argv) {

    /* init var */
    int c; // stores user input
    
    /* input parsing */
    for (int i=1; i<argc; ++i) {
        // help
        if (strcmp(argv[i], "-h") == 0 ||
                strcmp(argv[i], "--help") == 0) {
            printf("Hello!\n");
            return EXIT_SUCCESS;
        }
    }

    /* ncurses init */
    initscr();      // init screen
    start_color();  // enable colours
    noecho();       // don't display keypresses
    refresh();      // refresh the screen to empt
    keypad(stdscr, TRUE); // init keypad (arrow keys and more)

    /* init screen dimensions */
    pt_t SCREEN_MAX;
    square_t BORDER;
    BORDER.x1 = 0;
    BORDER.y1 = 5;

    /* init game state */
    game_t game_state;
    game_state.over = false;
    game_state.level = 1;
    game_state.pause = INIT_PAUSE;
    game_state.score = 0;

    /* set getch timeout */
    timeout(game_state.pause);

    /* set colour pairs to number defined above */
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);

    init_pair(SNAKE_COLOUR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(BORDER_COLOUR, COLOR_WHITE, COLOR_WHITE);
    init_pair(SNAKE_COLOUR_HEAD, COLOR_BLACK, COLOR_GREEN);

    /* store the max y and x coordinates */
    getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
    
    // init ball
    pt_t ball;
    ball.x = -1; ball.y = -1;
    ball.flag = 1;

    BORDER.x2 = (SCREEN_MAX.x - 1) < GAME_MAX_DIM ? (SCREEN_MAX.x-1) : GAME_MAX_DIM;
    BORDER.y2 = (SCREEN_MAX.y - 5) < GAME_MAX_DIM ? (SCREEN_MAX.y-5) : GAME_MAX_DIM;

    // init snake
    snake_t snake;
    snake.dir = RIGHT;
    snake.len = START_LEN;
    for (int i=0; i<snake.len; ++i) {
        snake.arr[i].x = BORDER.x2/2 - i;
        snake.arr[i].y = BORDER.y2/2;
    }

    /*
     * main loop
     */
    while(true) {
        /* overwrite all chars on the screen with blanks */
        erase();
        
        // reset timeout
        timeout(game_state.pause);

        /* store the max y and x coordinates */
        getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
        BORDER.x2 = (SCREEN_MAX.x - 1) < GAME_MAX_DIM ? (SCREEN_MAX.x-1) : GAME_MAX_DIM;
        BORDER.y2 = (SCREEN_MAX.y - 5) < GAME_MAX_DIM ? (SCREEN_MAX.y-5) : GAME_MAX_DIM;

        // Check for game over conditions
        detect_collision(&game_state, &BORDER, &snake, &ball);

        // if game is over, draw over screen
        if (game_state.over) {
            game_over();
            timeout(-1);
        } else {
            /* draw! */
            draw_border(&BORDER);
            draw_ball(&BORDER, &ball);
            draw_snake(&snake);
            draw_score(&game_state, &BORDER);
        }
        /*printf("broke after this!\n");*/
        /*return 0;*/


        /* update display */
        refresh();

        /* Get used input */
        c = getch();
        if (!game_state.over) {
            switch (c) {
                case 'w':
                case KEY_UP:
                    if (snake.dir != DOWN) snake.dir = UP;
                    break;
                case 's':
                case KEY_DOWN:
                    if (snake.dir != UP) snake.dir = DOWN;
                    break;
                case 'a':
                case KEY_LEFT:
                    if (snake.dir != RIGHT) snake.dir = LEFT;
                    break;
                case 'd':
                case KEY_RIGHT:
                    if (snake.dir != LEFT) snake.dir = RIGHT;
                    break;
            }
        }

        /*
         * ESC and q will exit the game
         */
        if (c == 'q' || c == 27) {
            break;
        }
    }

    /* clean up */
    endwin();
    return EXIT_SUCCESS;
}


