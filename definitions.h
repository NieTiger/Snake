#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

// macro definitions
#define START_LEN 5
#define INIT_PAUSE 500
#define GAME_MAX_DIM 50

#define SNAKE_COLOUR 11
#define BORDER_COLOUR 12

#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7

#define MAX_LENGTH 100

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} dirs_t;

typedef struct {
    int y, x;
    int flag;
} pt_t;

typedef struct {
    int len;
    dirs_t dir;
    pt_t arr[MAX_LENGTH];
} snake_t;

typedef struct {
    int over;
    int level;
    int pause;
    int score;
} game_t;

typedef struct {
    int x1, x2, y1, y2;
} square_t;

#endif // __DEFINITIONS_H__
