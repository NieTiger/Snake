/*
 * @Author: Tiger Nie
 * @github: http://github.com/haolinnie
 */
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "definitions.h"
#include "types.h"
#include "utilities.h"
#include "bot.h"

#define HELP "Classic Snake Game\nUsage: snake [options]\n\
  -l, --length <number>   Length of the snake to start with :)\n\
"

typedef enum {
    NO_BOT, BOT
} BOT_FLAG;

int main(int argc, char** argv) {

    /* init var */
    int c = 0; // stores user input
    int _start_len = START_LEN;
    int _game_pause = INIT_PAUSE;
    BOT_FLAG bot_flag = NO_BOT;

    /* input parsing */
    for (int i=1; i<argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 ||
            // help
            strcmp(argv[i], "--help") == 0) {
            printf(HELP);
            return EXIT_SUCCESS;

        } else if (strcmp(argv[i], "-l") == 0 ||
                   strcmp(argv[i], "--length") == 0) {
            // modify starting snake length

            if (++i >= argc || atoi(argv[i]) == 0) {
                printf("Invalid input: -l, --length, missing length argument.\n");
                return EXIT_SUCCESS;
            }
            _start_len = atoi(argv[i]);
            if (_start_len == 0) {
                printf("Invalid input: length must be an integer.\n");
                return EXIT_SUCCESS;
            }
            if (_start_len > 20) {
                printf("Invalid input: length cannot exceed 20.\n");
                return EXIT_SUCCESS;
            }
        } else if (strcmp(argv[i], "-b") == 0 ||
                   strcmp(argv[i], "--bot") == 0) {
            // Using a bot to run :)
            bot_flag = BOT;
        }
    }

    /* ncurses init */
    initscr();      // init screen
    start_color();  // enable colours
    noecho();       // don't display keypresses
    refresh();      // refresh the screen to empt
    keypad(stdscr, TRUE); // init keypad (arrow keys and more)

    /* init colour pairs */
    initialise_colour_pairs();

    /* declare game objects */
    game_t game_state;
    pt_t SCREEN_MAX;

    /* reset/construct game */
    getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
    update_border(&game_state, &SCREEN_MAX);
    init_game(&game_state, _start_len, _game_pause);

    /*
     * main loop
     *
     * ESC and q will exit the game
     */
    while(c != 'q' && c != 27) {
        /* overwrite all chars on the screen with blanks */
        erase();

        // set timeout
        timeout(game_state.pause);

        /* store the max y and x coordinates */
        getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
        update_border(&game_state, &SCREEN_MAX);

        // Check for game over conditions
        detect_collision(&game_state);

        // if game is over, draw over screen
        if (game_state.over) {
            game_over(&SCREEN_MAX);
        } else {
            /* draw! */
            draw_snake_logo(&game_state, &SCREEN_MAX);
            draw_border(&game_state);
            draw_ball(&game_state);
            draw_snake(&game_state);
            draw_score(&game_state);
        }
        /*printf("broke after this!\n");*/
        /*return 0;*/


        /* update display */
        refresh();

        /* Get next move */
        c = getch();
        int _control = 'w';

        if (bot_flag == NO_BOT) {
            /* Get used input */
            _control = c;
        } else if (bot_flag == BOT) {
            _control = bot_greedy(&game_state);
        }

        /* Move snake on screen */
        switch (_control) {
            case 'w':
            case KEY_UP:
                if (game_state.snake.dir != DOWN) game_state.snake.dir = UP;
                break;
            case 's':
            case KEY_DOWN:
                if (game_state.snake.dir != UP) game_state.snake.dir = DOWN;
                break;
            case 'a':
            case KEY_LEFT:
                if (game_state.snake.dir != RIGHT) game_state.snake.dir = LEFT;
                break;
            case 'd':
            case KEY_RIGHT:
                if (game_state.snake.dir != LEFT) game_state.snake.dir = RIGHT;
                break;
            case 'r':
                // Restart
                /* reset/construct game */
                getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
                update_border(&game_state, &SCREEN_MAX);
                init_game(&game_state, _start_len, _game_pause);
                break;
            default:
                break;
        }
    }

    /* clean up */
    endwin();
    return EXIT_SUCCESS;
}
