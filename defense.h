#define BOARD_SQUARE 15

typedef struct {
    int x; // x座標
    int y; // y座標
} place;

int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p, int turn);
int countWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDiagonallyLowerLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
void defense3ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode, int turn);
void defense4ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode, int turn);
int calculateDistance(int x, int y);