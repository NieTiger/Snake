#include <stdlib.h>
#include "bot.h"

int bot_greedy(game_t* gs) {
    // dirs_t dir = gs->snake.dir;
    pt_t snake_pos = gs->snake.arr[0];
    pt_t ball_pos = gs->ball;

    int dx = ball_pos.x - snake_pos.x;
    int dy = ball_pos.y - snake_pos.y;

    if (dx >= 0) {
        // x positive
        if (dx > abs(dy)) {
            // right
            return 'd';
        } else {
            if (dy > 0) {
                // down
                return 's';
            } else {
                // up
                return 'w';
            }
        }
    } else {
        // x negative
        if (abs(dx) > abs(dy)) {
            // left
            return 'a';
        } else {
            if (dy > 0) {
                // down
                return 's';
            } else {
                // up
                return 'w';
            }
        }
    }
}
