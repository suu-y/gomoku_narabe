#define BOARD_SQUARE 15

typedef struct {
    int x; // x���W�̈ʒu
    int y; // y���W�̈ʒu
} place;

int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p);
int countWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
void defense3ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode);
int calculateDistance(int x, int y);