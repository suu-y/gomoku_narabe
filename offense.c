#include "./offense.h"

// 攻め・守り判定関数の返り値を受けて、攻め関数offense()が呼び出される
int offense(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE], int flag_isBatFirst){

    /*
     * 【方針】
     * - 四三を作りに行く
     * 
     * **優先順位**
     * 1. 5連（勝ち）
     * 2. 長連（勝ち、後攻の時のみ）
     * 3. 四三
     * TODO: 序盤とか、まだ石の数が足らない等で四三を作るに至らない時の石を置く基準を検討する
     */

    // 盤面を全走査して、自分の石の並び方を調査する
    for(int i=0; i<BOARD_SQUARE; i++){          // 縦方向
        for(int j=0; j<BOARD_SQUARE; j++){      // 横方向
            
            int flag_43 = is_43(x, y, board);

    }

    return 0;
}

int is_43(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * この関数は offence 関数で呼び出され、盤面上の全ての(x, y)に対して
     * 四三があるかを判定する
     *
     * Args:
     *  x - 探索の起点となするx座標
     *  y - 探索の起点となするy座標
     *  board - 現在の盤面の状態
     * 
     * Rtn:
     *  0 - 入力(x, y)で四三は成立しない
     *  1 - 入力(x, y)で四三が成立する
     */

    // 起点に対して、自分の石が並ぶ方向を表すbit
    enum {
        LEFT,               // 0
        RIGHT,              // 1
        LOWER,              // 2
        UPPER,              // 3
        RIGHT_LOWER,        // 4
        LEFT_UPPER,         // 5
        RIGHT_UPPER,        // 6
        LEFT_LOWER,         // 7
        HORIZONTAL_MID,     // 8
        VERTICAL_MID,       // 9
        DIAGONALLY_LEFT,    // 10
        DIAGONALLY_RIGHT    // 11
    };

    int flag_43 = 0;
    int judge_x_o = board[x][y];    // 今回判定する石の種類
    int cnt_stone = 0;

    int flag = 0b000000000000;  // 12bitでbitごとに上記12つのフラグを管理

    // 左側2連の判定
    if((x-3)>=0){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x-2][y] == judge_x_o && board[x-1][y] == judge_x_o) && (x-2)>=0){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << LEFT;

    // 右側2連の判定
    cnt_stone = 0;
    if((x+3) < BOARD_SQUARE){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x+2][y] == judge_x_o && board[x+1][y] == judge_x_o) && (x+2)<BOARD_SQUARE){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << RIGHT;
   
    // 下側2連の判定
    cnt_stone = 0;
    if((y+3) < BOARD_SQUARE){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x][y+2] == judge_x_o && board[x][y+1] == judge_x_o) && (y+2)<BOARD_SQUARE){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << LOWER;

    // 上側2連の判定
    cnt_stone = 0;
    if((y-3) >= 0){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x][y-2] == judge_x_o && board[x][y-1] == judge_x_o) && (y-2)>=0){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << UPPER;

    // 斜め右下2連の判定
    cnt_stone = 0;
    if(((x+3)<BOARD_SQUARE && (y+3)<BOARD_SQUARE)){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x+2][y+2] == judge_x_o && board[x+1][y+1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y+2)<BOARD_SQUARE)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << RIGHT_LOWER;

    // 斜め左上2連の判定
    cnt_stone = 0;
    if(((x-3)>=0 && (y-3)>=0)){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x-2][y-2] == judge_x_o && board[x-1][y-1] == judge_x_o)
            && ((x-2)>=0 && (y-2)>=0)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << LEFT_UPPER;

    // 斜め右上2連の判定
    cnt_stone = 0;
    if(((x+3)<BOARD_SQUARE && (y-3)>=0)){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x+2][y-2] == judge_x_o && board[x+1][y-1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y-2)>=0)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << RIGHT_UPPER;

    // 斜め左下2連の判定
    cnt_stone = 0;
    if((x-3)>=0 && (y+3)<BOARD_SQUARE){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone++;
        }
    }
    else if((board[x-2][y+2] == judge_x_o && board[x-1][y+1] == judge_x_o)
            && ((x-2)>=0 && (y+2)<BOARD_SQUARE)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2)     flag |= 1 << LEFT_LOWER;

    // 水平方向左右の判定
    if((board[x-1][y] == judge_x_o && board[x+1][y] == judge_x_o)
            && ((x-1)>=0 && (x+1)<BOARD_SQUARE)){
        flag |= 1 << HORIZONTAL_MID;
    }
    // 垂直方向上下の判定
    if((board[x][y-1] == judge_x_o && board[x][y+1] == judge_x_o)
            && ((y-1)>=0 && (y+1)<BOARD_SQUARE)){
        flag |= 1 << VERTICAL_MID;
    }
    // 左斜め方向上下の判定
    if((board[x-1][y-1] == judge_x_o && board[x+1][y+1] == judge_x_o)
            && ((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE)){
        flag |= 1 << DIAGONALLY_LEFT;
    }            
    // 右斜め方向上下の判定
    if((board[x+1][y-1] == judge_x_o && board[x-1][y+1] == judge_x_o)
            && ((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE)){
        flag |= 1 << DIAGONALLY_RIGHT;
    }  

    /*
     * 　四三が成立するには、0-11bit間で少なくとも3bit分は立つ必要がある
     *  さらに、
     * 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　または
     * 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ　または
     * 　時が**四三**となる
     */

    int flag_tmp = flag;
    //int flag_return = flag;   // フラグを返り値にする場合の変数宣言
    int flag_0to7 = (flag &= 0b000011111111);
    int flag_8to11 = (flag_tmp &= 0b111100000000);
    for(int i = 0; i < 8; ++i) {
        for(int j = i + 1; j < 8; ++j) {
            int condition_2 = (1 << i) | (1 << j);
            if((flag_0to7 == condition_2)
                    && !(i==LEFT && j==RIGHT) 
                    && !(i==LOWER && j==UPPER)
                    && !(i==RIGHT_LOWER && j==LEFT_UPPER)
                    && !(i==RIGHT_UPPER && j==LEFT_LOWER)) {
                // 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　時を調査
                for(int h = 8; h < 12; ++h){
                    int condition_1 = (1 << h);
                    if(flag_8to11 == condition_1){
                        switch(h){
                            case 8:
                                if(i==LEFT || i==RIGHT || j==LEFT || j==RIGHT){
                                    flag_43 = 1;
                                } break;
                            case 9:
                                if(i==LOWER || i==UPPER || j==LOWER || j==UPPER){
                                    flag_43 = 1;
                                } break;
                            case 10:
                                if(i==RIGHT_LOWER || i==LEFT_UPPER || j==RIGHT_LOWER || j==LEFT_UPPER){
                                    flag_43 = 1;
                                } break;
                            case 11:
                                if(i==RIGHT_UPPER || i==LEFT_LOWER || j==RIGHT_UPPER || j==LEFT_LOWER){
                                    flag_43 = 1;
                                } break;
                        }
                        break;
                    }
                }
            }
        }
    }

    for(int i = 8; i < 12; ++i) {
        for(int j = i + 1; j < 12; ++j) {
            int condition_3 = (1 << i) | (1 << j);
            if(flag_8to11 == condition_3) {
                // 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ を調査
                for(int h = 0; h < 8; ++h){
                    int condition_4 = (1 << h);
                    if(flag_0to7 == condition_4){
                        switch(h){
                            case 0:
                            case 1:
                                if(i==HORIZONTAL_MID || j==HORIZONTAL_MID){
                                    flag_43 = 1;
                                } break;
                            case 2:
                            case 3:
                                if(i==VERTICAL_MID || j==VERTICAL_MID){
                                    flag_43 = 1;
                                } break;
                            case 4:
                            case 5:
                                if(i==DIAGONALLY_LEFT || j==DIAGONALLY_LEFT){
                                    flag_43 = 1;
                                } break;
                            case 6:
                            case 7:
                                if(i==DIAGONALLY_RIGHT || j==DIAGONALLY_RIGHT){
                                    flag_43 = 1;
                                } break;
                        }
                        break;
                    }
                }
            }
        }
    }

    if(flag_43) return 1;
    else    return 0;
}