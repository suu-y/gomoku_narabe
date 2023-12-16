#define BOARD_SQUARE 15

void win(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, char *message);
int judgeWin(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 勝ちな�?1?��ま�?勝てな�?な�?0を返す
int judgeVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 縦方向に5つ並ぶかを判�?
int judgeWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 横方向に5つ並ぶかを判�?
int judgeDiagonal(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 斜め方向に5つ並ぶかを判�?