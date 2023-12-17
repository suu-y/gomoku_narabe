#define BOARD_SQUARE 15

typedef struct {
    int x; // x座�?
    int y; // y座�?
} place;

typedef struct kinjite {
    place p;        // 禁じ手であった座�?
    struct kinjite *next;  // リスト構�?のためのポインタ
} kinjite;

int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p);
int countLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countLeftUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countRightDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countRightUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
int countLeftDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y);
void search8directions(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, int flag, int directions[8]);

void defense3ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode);
void defense4ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode);
int calculateDistance(int x, int y);