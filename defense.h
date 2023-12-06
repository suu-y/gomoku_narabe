#define BOARD_SQUARE 15

typedef struct place {
    int x; // x座標の位置
    int y; // y座標の位置
} place;

int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE]);
int countWidth(int x, int y);
int countVertical(int x, int y);
int countDiagonallyLowerRight(int x, int y);
int countDiagonallyLowerLeft(int x, int y);
place defense(int board[BOARD_SQUARE][BOARD_SQUARE]);
