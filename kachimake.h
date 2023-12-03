#define BOARD_SQUARE 15

void win(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, char *message);
int judgeWin(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 勝ちなら1，まだ勝てないなら0を返す
int judgeVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 縦方向に5つ並ぶかを判定
int judgeWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 横方向に5つ並ぶかを判定
int judgeDiagonal(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y); // 斜め方向に5つ並ぶかを判定