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
    // dirs_t curr_dir = gs->snake.dir;
    pt_t snake_pos = gs->snake.arr[0];
    pt_t ball_pos = gs->ball;

    int dx = ball_pos.x - snake_pos.x;
    int dy = ball_pos.y - snake_pos.y;

    int predicted_dir = 0;
    int pred_x = snake_pos.x;
    int pred_y = snake_pos.y;

    int dx_is_positive = dx >= 0;
    int dy_is_positive = dy >= 0;

    if (abs(dx) > abs(dy)) {
        // move in x dir
        if (dx_is_positive) {
            // right
            pred_x++;
            /*if (curr_dir == LEFT) {*/
                /*predicted_dir = dy_is_positive ? DOWN : UP;*/
            /*} else {*/
                predicted_dir = RIGHT;
            /*}*/
        } else {
            // left
            pred_x--;
            /*if (curr_dir == RIGHT) {*/
                /*predicted_dir = dy_is_positive ? DOWN : UP;*/
            /*} else {*/
                predicted_dir = LEFT;
            /*}*/
        }
    } else {
        // move in y dir
        if (dy_is_positive) {
            // down
            pred_y++;
            /*if (curr_dir == UP) {*/
                /*predicted_dir = dx_is_positive ? RIGHT : LEFT;*/
            /*} else {*/
                predicted_dir = DOWN;
            /*}*/
        } else {
            // up
            pred_y--;
            /*if (curr_dir == DOWN) {*/
                /*predicted_dir = dx_is_positive ? RIGHT : LEFT;*/
            /*} else {*/
                predicted_dir = UP;
            /*}*/
        }
    }

    int opposite_dir = _reverse(predicted_dir);
    if (detect_self_collision( &(gs->snake), pred_x, pred_y)) {
        // naive prediction causes a self collision
        // go in the opposite direction
        return opposite_dir;
    }

    // TODO: rn if the prediction is exactly opposite of the curr dir, snake needs to make a turn

    return predicted_dir;
}
