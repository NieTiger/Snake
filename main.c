/*
 * @Author: Tiger Nie
 * @github: http://github.com/tiega
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <ncurses.h>

#include "definitions.h"
#include "types.h"
#include "utilities.h"
#include "bot.h"

#define HELP "Classic Snake Game\nUsage: snake [options]\n\
  -l, --length <number>   Length of the snake to start with \n\
  -s, --score             Show current highscore \n\
"

char* score_file_name = "/.snake";

int get_save_high_score(char* path, int score) {
    int highscore = 0;

    char buffer[100];                           // open file to read
    /*check if file exists and read file*/
    FILE* file_ptr = fopen(path, "r");
    if (file_ptr != NULL) {                     // if file does exist
        fgets(buffer, 100, file_ptr);           // read file to buffer
        sscanf(buffer, "%d", &highscore);       // save previous score
        fclose(file_ptr);                       // close file
    }


    if (score > highscore) {
        /* open file to write */
        file_ptr = fopen(path, "w");            // open file to write
        if (file_ptr != NULL) {                 // if file opens successfully
            sprintf(buffer, "%d", score);       // put score in buffer
            fputs(buffer, file_ptr);            // write buffer to file
            fclose(file_ptr);                   // close file
            highscore = score;
        } else {
            highscore = score;                  // if file fails to open, return current score
        }
    }
    return highscore;
}


int main(int argc, char** argv) {

    /*Get score file path*/
    char* _PATH = getenv("HOME");
    strcat(_PATH, score_file_name);

    int highscore = get_save_high_score(_PATH, 0);

    /*Seed random number generator*/
    time_t t;
    srand((unsigned) time(&t));

    /* Init var */
    int c = 0; // stores user input
    int _start_len = START_LEN;
    int _game_pause = INIT_PAUSE;
    BOT_FLAG bot_flag = NO_BOT;

    /* Input parsing */
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
                printf("snake: option %s: requires a parameter between 1 and %d.\n", argv[i-1], GAME_MAX_DIM/2+1);
                return EXIT_SUCCESS;
            }
            _start_len = atoi(argv[i]);
            if (_start_len > GAME_MAX_DIM/2+1) {
                printf("snake: option %s: parameter must be between 1 and %d.\n", argv[i-1], GAME_MAX_DIM/2+1);
                sleep(1);
                _start_len = GAME_MAX_DIM/2+1;
            }
        /*} else if (strcmp(argv[i], "-b") == 0 ||*/
                   /*strcmp(argv[i], "--bot") == 0) {*/
            /*// Using a bot to run :)*/
            /*bot_flag = BOT;*/
            /*_game_pause = 100;*/
        } else if (strcmp(argv[i], "-s") == 0 ||
                strcmp(argv[i], "--score") == 0) {
            // Print score
            printf("snake: The highscore on this machine is %d. Beat that!\n", highscore);
            return EXIT_SUCCESS;
        } else {
            printf("snake: Unrecognised option '%s'\n", argv[i]);
            return EXIT_FAILURE;
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
    init_game(&game_state, _start_len, _game_pause, highscore);

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

        // If game over
        if (game_state.over) {
            // Save highscore
            get_save_high_score(_PATH, game_state.score);

            // Draw game over screen
            game_over(&game_state, &SCREEN_MAX);

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
            case UP:
                if (game_state.snake.dir != DOWN) game_state.snake.dir = UP;
                break;
            case 's':
            case KEY_DOWN:
            case DOWN:
                if (game_state.snake.dir != UP) game_state.snake.dir = DOWN;
                break;
            case 'a':
            case KEY_LEFT:
            case LEFT:
                if (game_state.snake.dir != RIGHT) game_state.snake.dir = LEFT;
                break;
            case 'd':
            case KEY_RIGHT:
            case RIGHT:
                if (game_state.snake.dir != LEFT) game_state.snake.dir = RIGHT;
                break;
            default:
                break;
        }

        if (c == 'r') {
            // Restart
            /* reset/construct game */
            getmaxyx(stdscr, SCREEN_MAX.y, SCREEN_MAX.x);
            update_border(&game_state, &SCREEN_MAX);
            init_game(&game_state, _start_len, _game_pause, game_state.highscore);
        }
    }

    /* clean up */
    get_save_high_score(_PATH, game_state.score);
    endwin();
    return EXIT_SUCCESS;
}
