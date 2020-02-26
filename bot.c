#include <stdlib.h>
#include "bot.h"
#include "utilities.h"

int _reverse(int dir) {
    switch (dir) {
        case 'w':
            return 's';
        case 's':
            return 'w';
        case 'a':
            return 'd';
        case 'd':
            return 'a';
    }
    return 0;
}

int bot_greedy(game_t* gs) {
    // dirs_t dir = gs->snake.dir;
    pt_t snake_pos = gs->snake.arr[0];
    pt_t ball_pos = gs->ball;

    int dx = ball_pos.x - snake_pos.x;
    int dy = ball_pos.y - snake_pos.y;

    int predicted_dir = 0;
    int pred_x = snake_pos.x;
    int pred_y = snake_pos.y;

    if (abs(dx) > abs(dy)) {
        // move in x dir
        if (dx >= 0) {
            // right
            pred_x++;
            predicted_dir = 'd';
        } else {
            // left
            pred_x--;
            predicted_dir = 'a';
        }
    } else {
        // move in y dir
        if (dy > 0) {
            // down
            pred_y++;
            predicted_dir = 's';
        } else {
            // up
            pred_y--;
            predicted_dir = 'w';
        }
    }

    if (detect_self_collision( &(gs->snake), pred_x, pred_y)) {
        // naive prediction causes a self collision
        // go in the opposite direction
        predicted_dir = _reverse(predicted_dir);
    }
    return predicted_dir;
}
