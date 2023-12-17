#define BOARD_SQUARE 15

void offense(int* x, int* y, int board[BOARD_SQUARE][BOARD_SQUARE], int flag_first_second);
int is_5ren_mid(int* x, int* y, int board[BOARD_SQUARE][BOARD_SQUARE]);
int is_5ren_edge(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]);
int is_chouren(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]);
int is_43(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]);