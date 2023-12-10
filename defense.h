#define BOARD_SQUARE 15

typedef struct place {
    int x; // x���W�̈ʒu
    int y; // y���W�̈ʒu
} place;

int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p);
int countWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
