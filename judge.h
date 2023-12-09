#define BOARD_SQUARE 15

int judge_kinzite(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE], char *message);
int judge_33(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]);
int judge_44(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE], int *flag, int *place_flag_send);
int judge_chouren(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]);