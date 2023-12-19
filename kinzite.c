#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include "./kinzite.h"


int judge_kinzite(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE], char *message){
   
    int flag, flag_direction;
    flag = 0;
    flag_direction = 0;

    if(judge_33(x, y, board)){
        printf("forbidden (三三禁)");
        strcat(message, ",forbidden (三三禁)");
        return 1;
    }
    else if(judge_44(x, y, board, &flag, &flag_direction)){
        printf("forbidden (四四禁)\n");
        strcat(message, ",forbidden (四四禁)");
        return 1;
    }
    else if(judge_chouren(x, y, board)){
        printf("forbidden (長連)\n");
        strcat(message, ",forbidden (長連)");
        return 1;   
    }
    else    return 0;
}

int judge_33(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * 三三禁は -|, -\, -/, |\, |/, \/ の6通りの列の組み合わせ間で成立する
     *
     * Args:
     *  x - 石を置くx座標
     *  y - 石を置くy座標
     *  board - 現在の盤面の状態
     */

    // 三三禁フラグ用の、方向を表すbit
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

    int flag_33 = 0;
    int judge_x_o = board[x][y];    // 今回判定する石の種類
    int cnt_stone = 0;

    int flag = 0b000000000000;  // 12bitでbitごとに上記12つのフラグを管理
    int flag_4ren = 0b00000000;         // 8bitで4連の時は特別にこのフラグを立てる

    // 左側2連の判定
    if((x-3)>=0){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y] != judge_x_o && board[x+i][y] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x-2][y] == judge_x_o && board[x-1][y] == judge_x_o) && (x-2)>=0){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << LEFT;
        if(board[x-4][y] == judge_x_o && (x-4)>=0)   flag_4ren |= 1 << LEFT;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << LEFT;
        flag_4ren |= 1 << LEFT;
    }

    // 右側2連の判定
    cnt_stone = 0;
    if((x+3) < BOARD_SQUARE){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y] != judge_x_o && board[x+i][y] == 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x+2][y] == judge_x_o && board[x+1][y] == judge_x_o) && (x+2)<BOARD_SQUARE){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << RIGHT;
        if(board[x+4][y] == judge_x_o && (x+4)<BOARD_SQUARE)   flag_4ren |= 1 << RIGHT;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << RIGHT;
        flag_4ren |= 1 << RIGHT;
    }
   
    // 下側2連の判定
    cnt_stone = 0;
    if((y+3) < BOARD_SQUARE){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x][y+2] == judge_x_o && board[x][y+1] == judge_x_o) && (y+2)<BOARD_SQUARE){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << LOWER;
        if(board[x][y+4] == judge_x_o && (y+4)<BOARD_SQUARE)   flag_4ren |= 1 << LOWER;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << LOWER;
        flag_4ren |= 1 <<LOWER;
    }

    // 上側2連の判定
    cnt_stone = 0;
    if((y-3) >= 0){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x][y-2] == judge_x_o && board[x][y-1] == judge_x_o) && (y-2)>=0){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << UPPER;
        if(board[x][y-4] == judge_x_o && (y-4)>=0)   flag_4ren |= 1 << UPPER;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << UPPER;
        flag_4ren |= 1 << UPPER;
    }

    // 斜め右下2連の判定
    cnt_stone = 0;
    if(((x+3)<BOARD_SQUARE && (y+3)<BOARD_SQUARE)){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y+i] != judge_x_o && board[x+i][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x+2][y+2] == judge_x_o && board[x+1][y+1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y+2)<BOARD_SQUARE)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << RIGHT_LOWER;
        if(board[x+4][y+4] == judge_x_o && (x+4)<BOARD_SQUARE && (y+4)<BOARD_SQUARE)   flag_4ren |= 1 << RIGHT_LOWER; // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << RIGHT_LOWER;
        flag_4ren |= 1 << RIGHT_LOWER;
    }

    // 斜め左上2連の判定
    cnt_stone = 0;
    if(((x-3)>=0 && (y-3)>=0)){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y+i] != judge_x_o && board[x+i][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x-2][y-2] == judge_x_o && board[x-1][y-1] == judge_x_o)
            && ((x-2)>=0 && (y-2)>=0)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << LEFT_UPPER;
        if(board[x-4][y-4] == judge_x_o && (x-4)>=0 && (y-4)>=0)   flag_4ren |= 1 << LEFT_UPPER;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << LEFT_UPPER;
        flag_4ren |= 1 << LEFT_UPPER;
    }

    // 斜め右上2連の判定
    cnt_stone = 0;
    if(((x+3)<BOARD_SQUARE && (y-3)>=0)){     // 飛び三
        for(int i=1; i<4; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0 ){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x+2][y-2] == judge_x_o && board[x+1][y-1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y-2)>=0)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << RIGHT_UPPER;
        if(board[x+4][y-4] == judge_x_o && (x+4)<BOARD_SQUARE && (y-4)>=0)   flag_4ren |= 1 << RIGHT_UPPER;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << RIGHT_UPPER;
        flag_4ren |= 1 <<RIGHT_UPPER;
    }

    // 斜め左下2連の判定
    cnt_stone = 0;
    if((x-3)>=0 && (y+3)<BOARD_SQUARE){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    else if((board[x-2][y+2] == judge_x_o && board[x-1][y+1] == judge_x_o)
            && ((x-2)>=0 && (y+2)<BOARD_SQUARE)){
        cnt_stone = 2;
    }
    if(cnt_stone == 2){
        flag |= 1 << LEFT_LOWER;
        if(board[x-4][y+4] == judge_x_o && (x-4)>=0 && (y+4)<BOARD_SQUARE)   flag_4ren |= 1 << LEFT_LOWER;  // 飛び四
    }
    else if(cnt_stone == 3){    // 四
        flag |= 1 << LEFT_LOWER;
        flag_4ren |= 1 <<LEFT_LOWER;
    }

    // 水平方向左右の判定
    cnt_stone = 0;
    if((x-2)>=0 && (x+1)<BOARD_SQUARE && board[x+1][y]==judge_x_o){
        for(int i=-2; i<0; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y] != judge_x_o && board[x+i][y] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << HORIZONTAL_MID;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && board[x-1][y]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y] != judge_x_o && board[x+i][y] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << HORIZONTAL_MID;
    cnt_stone = 0;
    if((x-1)>=0 && (x+1)<BOARD_SQUARE){
        for(int i=-1; i<2; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone ++;
        }
    }
    if(cnt_stone == 3)  flag |= 1 << HORIZONTAL_MID;

    // 垂直方向上下の判定
    cnt_stone = 0;
    if((y-2)>=0 && (y+1)<BOARD_SQUARE && board[x][y+1]==judge_x_o){
        for(int i=-2; i<0; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone ++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << VERTICAL_MID;
    cnt_stone = 0;
    if((y-1)>=0 && (y+2)<BOARD_SQUARE && board[x][y-1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone ++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << VERTICAL_MID;
    cnt_stone = 0;
    if((y-1)>=0 && (y+1)<BOARD_SQUARE){
        for(int i=-1; i<2; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone ++;
        }
    }
    if(cnt_stone == 3)  flag |= 1 << VERTICAL_MID;

    // 左斜め方向上下の判定
    cnt_stone = 0;
    if((x-2)>=0 && (x+1)<BOARD_SQUARE && (y-2)>=0 && (y+1)<BOARD_SQUARE
        && board[x+1][y+1]==judge_x_o){
        for(int i=-2; i<0; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y+i] != judge_x_o && board[x+i][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_LEFT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && (y-1)>=0 && (y+2)<BOARD_SQUARE
        && board[x-1][y-1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y+i] != judge_x_o && board[x+i][y+i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_LEFT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE){
        for(int i=-1; i<2; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone ++;
        }
    }
    if(cnt_stone == 3)  flag |= 1 << DIAGONALLY_LEFT;
           
    // 右斜め方向上下の判定
    cnt_stone = 0;
    if((x-2)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+2)<BOARD_SQUARE
        && board[x+1][y-1]==judge_x_o){
        for(int i=-2; i<0; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_RIGHT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && (y-2)>=0 && (y+1)<BOARD_SQUARE
        && board[x-1][y+1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone ++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_stone = 0;
                break;
            }
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_RIGHT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE){
        for(int i=-1; i<2; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone ++;
        }
    }
    if(cnt_stone == 3)  flag |= 1 << DIAGONALLY_RIGHT;

    /*
     * 三三禁になるのは、0-7bit間で2bit立つ または 8-11bit間で2bit立つ
     * または　0-7bit, 8-11bit間で1bitずつ立つ
     * しかし
     * 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　または
     * 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ　または
     * 「0-7bit, 8-11bit間で1bitずつ立つ」時にそのbit同士が同一方向
     * 時は三三禁ではない（四三）
     * さらに、「0-7bit間で2bit立つ」時に、flag_4renも立っているときは四三
     * 
     */

    int flag_tmp = flag;
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
                // フラグ2個で四三の時
                if(flag_4ren == (1 << i) || flag_4ren == (1 << j)){
                    flag_33 = 0;
                    break;
                }
                flag_33 = 1;
                // 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　時を調査
                for(int h = 8; h < 12; ++h){
                    int condition_1 = (1 << h);
                    if(flag_8to11 == condition_1){
                        switch(h){
                            case 8:
                                if(i==LEFT || i==RIGHT || j==LEFT || j==RIGHT){
                                    flag_33 = 0;
                                } break;
                            case 9:
                                if(i==LOWER || i==UPPER || j==LOWER || j==UPPER){
                                    flag_33 = 0;
                                } break;
                            case 10:
                                if(i==RIGHT_LOWER || i==LEFT_UPPER || j==RIGHT_LOWER || j==LEFT_UPPER){
                                    flag_33 = 0;
                                } break;
                            case 11:
                                if(i==RIGHT_UPPER || i==LEFT_LOWER || j==RIGHT_UPPER || j==LEFT_LOWER){
                                    flag_33 = 0;
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
                flag_33 = 1;
                // 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ を調査
                for(int h = 0; h < 8; ++h){
                    int condition_4 = (1 << h);
                    if(flag_0to7 == condition_4){
                        switch(h){
                            case 0:
                            case 1:
                                if(i==HORIZONTAL_MID || j==HORIZONTAL_MID){
                                    flag_33 = 0;
                                } break;
                            case 2:
                            case 3:
                                if(i==VERTICAL_MID || j==VERTICAL_MID){
                                    flag_33 = 0;
                                } break;
                            case 4:
                            case 5:
                                if(i==DIAGONALLY_LEFT || j==DIAGONALLY_LEFT){
                                    flag_33 = 0;
                                } break;
                            case 6:
                            case 7:
                                if(i==DIAGONALLY_RIGHT || j==DIAGONALLY_RIGHT){
                                    flag_33 = 0;
                                } break;
                        }
                        break;
                    }
                }
            }
        }
    }

    // 0-7bit, 8-11bit間で1bitずつ立つ（このとき計2bitしか立っていない）
    // かつ flag_4ren が立っていない
    for(int i = 0; i < 8; ++i) {
        int condition_5 = (1 << i);
        if(flag_0to7 == condition_5 && flag_4ren != condition_5) {
            for(int j = 8; j < 12; ++j) {
                int condition_6 = (1 << j);
                if(flag_8to11 == condition_6){
                    flag_33 = 1;

                    // bit同士が同一方向の場合は三三禁ではない
                    switch (j){
                        case 8:
                            if(i == 0 || i == 1)    flag_33 = 0;
                            break;
                        case 9:
                            if(i == 2 || i == 3)    flag_33 = 0;
                            break;
                        case 10:
                            if(i == 4 || i == 5)    flag_33 = 0;
                            break;
                        case 11:
                            if(i == 6 || i == 7)    flag_33 = 0;
                            break;
                    }
                    break;
                }
            }
        }
    } 

    if(flag_33) return 1;
    else    return 0;
}


int judge_44(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE], int *flag, int *place_flag_send){

    /*
     * 四四禁は - | \ / の4種類の重複組み合わせ10通り間で成立する
     *
     * Args:
     *  x - 石を置くx座標
     *  y - 石を置くy座標
     *  board - 現在の盤面の状態
     */

    // 四四禁フラグ用の、方向を表すbit
    enum {
        HORIZONTAL,             // 0
        VERTICAL,               // 1
        DIAGONALLY_LEFT_2,      // 2
        DIAGONALLY_RIGHT_2      // 3
    };

    // 四四禁の、同一方向内の石の配置フラグ用
    enum {
        ooxoo,    // 0
        oxooo,    // 1
        oooxo,    // 2
        xoooo,    // 3
        oooox     // 4
    };

    int flag_44 = 0;
    int judge_x_o = board[x][y];    // 今回判定する石の種類
    int flag_init = 0b00000;        // アドレス代入用
    *flag = 0b0000;  // 4bitでbitごとに上記4つのフラグを管理
    int *place_flag = &flag_init;  // 5bitでbitごとに「同一方向内の石の配置フラグ」を管理

    // ============= 水平方向の判定 ==================================
    int cnt_horizontal = 0;
    if((x+2)<BOARD_SQUARE && (x-2)>=0){     // ・・＋・・
        for(int i=-2; i<3; i++){
            if(board[x+i][y] == judge_x_o)  cnt_horizontal++;
            else if(board[x+i][y] == 2){
                cnt_horizontal = 0;
                break;
            }
        }
    }
    if(cnt_horizontal == 4)     *place_flag |= 1 << ooxoo;
    cnt_horizontal = 0;
    if((x+3)<BOARD_SQUARE && (x-1)>=0){     // ・＋・・・
        for(int i=-1; i<4; i++){
            if(board[x+i][y] == judge_x_o)  cnt_horizontal++;
            else if(board[x+i][y] == 2){
                cnt_horizontal = 0;
                break;
            }
        }
    }
    if(cnt_horizontal == 4)     *place_flag |= 1 << oxooo;    
    cnt_horizontal = 0;
    if((x+1)<BOARD_SQUARE && (x-3)>=0){     // ・・・＋・
        for(int i=-3; i<2; i++){
            if(board[x+i][y] == judge_x_o)  cnt_horizontal++;
            else if(board[x+i][y] == 2){
                cnt_horizontal = 0;
                break;
            }
        }
    }
    if(cnt_horizontal == 4)     *place_flag |= 1 << oooxo;  
    cnt_horizontal = 0;
    if((x+4)<BOARD_SQUARE){     // ＋・・・・
        for(int i=0; i<5; i++){
            if(board[x+i][y] == judge_x_o)  cnt_horizontal++;
            else if(board[x+i][y] == 2){
                cnt_horizontal = 0;
                break;
            }
        }
    }
    if(cnt_horizontal == 4)     *place_flag |= 1 << xoooo; 
    cnt_horizontal = 0;
    if((x-4)>=0){               // ・・・・＋
        for(int i=-4; i<1; i++){
            if(board[x+i][y] == judge_x_o)  cnt_horizontal++;
            else if(board[x+i][y] == 2){
                cnt_horizontal = 0;
                break;
            }
        }
    }
    if(cnt_horizontal == 4)     *place_flag |= 1 << oooox;

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int condition = (1 << i) | (1 << j);
            if (*place_flag == condition){
                flag_44 = 1;
            }
            // 4連を四四禁とするのを防ぐ
            switch (j){
                case 1:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 2:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 3:
                    if(i == oxooo)    flag_44 = 0;
                    break;
                case 4:
                    if(i == oooxo)    flag_44 = 0;
                    break;
                default:
                    break;
            }            
        }
        if(*place_flag == (1 << i)){
            *flag |= 1 << HORIZONTAL;
        }
    }

    // judge_kinzite関数用にフラグを格納
    if(*place_flag != 0){
        *place_flag_send = *place_flag;
    }

    // ============= 垂直方向の判定 ==================================
    flag_init = 0b00000;
    place_flag = &flag_init;
    int cnt_vertical = 0;
    if((y+2)<BOARD_SQUARE && (y-2)>=0){     // ・・＋・・
        for(int i=-2; i<3; i++){
            if(board[x][y+i] == judge_x_o)  cnt_vertical++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_vertical = 0;
                break;
            }
        }
    }
    if(cnt_vertical == 4)     *place_flag |= 1 << ooxoo;
    cnt_vertical = 0;
    if((y+3)<BOARD_SQUARE && (y-1)>=0){     // ・＋・・・
        for(int i=-1; i<4; i++){
            if(board[x][y+i] == judge_x_o)  cnt_vertical++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_vertical = 0;
                break;
            }
        }
    }
    if(cnt_vertical == 4)     *place_flag |= 1 << oxooo;    

    cnt_vertical = 0;
    if((y+1)<BOARD_SQUARE && (y-3)>=0){     // ・・・＋・
        for(int i=-3; i<2; i++){
            if(board[x][y+i] == judge_x_o)  cnt_vertical++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_vertical = 0;
                break;
            }
        }
    }
    if(cnt_vertical == 4)     *place_flag |= 1 << oooxo;  
    cnt_vertical = 0;
    if((y+4)<BOARD_SQUARE){     // ＋・・・・
        for(int i=0; i<5; i++){
            if(board[x][y+i] == judge_x_o)  cnt_vertical++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_vertical = 0;
                break;
            }
        }
    }
    if(cnt_vertical == 4)     *place_flag |= 1 << xoooo; 
    cnt_vertical = 0;
    if((y-4)>=0){               // ・・・・＋
        for(int i=-4; i<1; i++){
            if(board[x][y+i] == judge_x_o)  cnt_vertical++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_vertical = 0;
                break;
            }
        }
    }
    if(cnt_vertical == 4)     *place_flag |= 1 << oooox;

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int condition = (1 << i) | (1 << j);
            if (*place_flag == condition){
                flag_44 = 1;
            }
            // 4連を四四禁とするのを防ぐ
            switch (j){
                case 1:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 2:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 3:
                    if(i == oxooo)    flag_44 = 0;
                    break;
                case 4:
                    if(i == oooxo)    flag_44 = 0;
                    break;
                default:
                    break;
            }  
        }
        if(*place_flag == (1 << i)){
            *flag |= 1 << VERTICAL;
        }
    }

    // judge_kinzite関数用にフラグを格納
    if(*place_flag != 0){
        *place_flag_send = *place_flag;
    }

    // ============= 左斜め方向の判定 ==================================
    flag_init = 0b00000;
    place_flag = &flag_init;
    int cnt_diagonally_left = 0;    
    if((x-2)>=0 && (y-2)>=0 && (x+2)<BOARD_SQUARE && (y+2)<BOARD_SQUARE){     // ・・＋・・
        for(int i=-2; i<3; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_diagonally_left++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_diagonally_left = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_left == 4)     *place_flag |= 1 << ooxoo;
    cnt_diagonally_left = 0;
    if((x-1)>=0 && (y-1)>=0 && (x+3)<BOARD_SQUARE && (y+3)<BOARD_SQUARE){     // ・＋・・・
        for(int i=-1; i<4; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_diagonally_left++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_diagonally_left = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_left == 4)     *place_flag |= 1 << oxooo;    
    cnt_diagonally_left = 0;
    if((x-3)>=0 && (y-3)>=0 && (x+1)<BOARD_SQUARE && (y+1)<BOARD_SQUARE){     // ・・・＋・
        for(int i=-3; i<2; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_diagonally_left++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_diagonally_left = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_left == 4)     *place_flag |= 1 << oooxo;  
    cnt_diagonally_left = 0;
    if((x+4)<BOARD_SQUARE && (y+4)<BOARD_SQUARE){     // ＋・・・・
        for(int i=0; i<5; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_diagonally_left++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_diagonally_left = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_left == 4)     *place_flag |= 1 << xoooo; 
    cnt_diagonally_left = 0;
    if((x-4)>=0 && (y-4)>=0){               // ・・・・＋
        for(int i=-4; i<1; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_diagonally_left++;
            else if(board[x][y+i] != judge_x_o && board[x][y+i] != 0){
                cnt_diagonally_left = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_left == 4)     *place_flag |= 1 << oooox;

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int condition = (1 << i) | (1 << j);
            if (*place_flag == condition){
                flag_44 = 1;
            }
            // 4連を四四禁とするのを防ぐ
            switch (j){
                case 1:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 2:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 3:
                    if(i == oxooo)    flag_44 = 0;
                    break;
                case 4:
                    if(i == oooxo)    flag_44 = 0;
                    break;
                default:
                    break;
            }  
        }
        if(*place_flag == (1 << i)){
            *flag |= 1 << DIAGONALLY_LEFT_2;
        }
    }

    // judge_kinzite関数用にフラグを格納
    if(*place_flag != 0){
        *place_flag_send = *place_flag;
    }

    // ============= 右斜め方向の判定 ==================================
    flag_init = 0b00000;
    place_flag = &flag_init;
    int cnt_diagonally_right = 0;    
    if((x+2)<BOARD_SQUARE && (y-2)>=0 && (x-2)>=0 && (y+2)<BOARD_SQUARE){     // ・・＋・・
        for(int i=-2; i<3; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_diagonally_right++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_diagonally_right = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_right == 4)     *place_flag |= 1 << ooxoo;
    cnt_diagonally_right = 0;
    if((x+3)<BOARD_SQUARE && (y-3)>=0 && (x-1)>=0 && (y+1)<BOARD_SQUARE){     // ・＋・・・
        for(int i=-1; i<4; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_diagonally_right++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_diagonally_right = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_right == 4)     *place_flag |= 1 << oxooo;    
    cnt_diagonally_right = 0;
    if((x-1)>=0 && (y+1)<BOARD_SQUARE && (x+3)<BOARD_SQUARE && (y-1)>=0){     // ・・・＋・
        for(int i=-3; i<2; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_diagonally_right++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_diagonally_right = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_right == 4)     *place_flag |= 1 << oooxo;  
    cnt_diagonally_right = 0;
    if((x+4)<BOARD_SQUARE && (y-4)>=0){     // ＋・・・・
        for(int i=0; i<5; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_diagonally_right++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_diagonally_right = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_right == 4)     *place_flag |= 1 << xoooo; 
    cnt_diagonally_right = 0;
    if((x-4)>=0 && (y+4)<BOARD_SQUARE){               // ・・・・＋
        for(int i=-4; i<1; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_diagonally_right++;
            else if(board[x+i][y-i] != judge_x_o && board[x+i][y-i] != 0){
                cnt_diagonally_right = 0;
                break;
            }
        }
    }
    if(cnt_diagonally_right == 4)     *place_flag |= 1 << oooox;

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int condition = (1 << i) | (1 << j);
            if (*place_flag == condition){
                flag_44 = 1;
            }
            // 4連を四四禁とするのを防ぐ
            switch (j){
                case 1:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 2:
                    if(i == ooxoo)    flag_44 = 0;
                    break;
                case 3:
                    if(i == oxooo)    flag_44 = 0;
                    break;
                case 4:
                    if(i == oooxo)    flag_44 = 0;
                    break;
                default:
                    break;
            }  
        }
        if(*place_flag == (1 << i)){
            *flag |= 1 << DIAGONALLY_RIGHT_2;
        }
    }
    
    // judge_kinzite関数用にフラグを格納
    if(*place_flag != 0){
        *place_flag_send = *place_flag;
    }

    // - | / \ の二つが組み合わさると四四禁
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            int condition = (1 << i) | (1 << j);
            if(*flag == condition){
                flag_44 = 1;
            }
        }
    }

    if(flag_44) return 1; 
    else    return 0;

}

int judge_chouren(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * 長連は「4連-1連」　または　「2連-3連」　の時に間に石を置くと成立する
     *
     * Args:
     *  x - 石を置くx座標
     *  y - 石を置くy座標
     *  board - 現在の盤面の状態
     */

    int chouren_flag = 0;
    int judge_x_o = board[x][y];    // 今回判定する石の種類

    // =============== 垂直方向 判定 ============================================
    int vertical_upper_exists = 0;        // 垂直方向上向きの長連を探す
    if(board[x][y-4] == judge_x_o && (y-4) >= 0){    // 「4連-1連」の長連を判定
        for(int i=3; i>0; i--){
            if(board[x][y-i] == judge_x_o){
                vertical_upper_exists = 1;
            }else{
                vertical_upper_exists = 0;
                break;
            }
        }
        if((board[x][y+1] == judge_x_o && (y+1) < BOARD_SQUARE)
                && (vertical_upper_exists == 1)){
            chouren_flag = 1;
        }
    }
    else if(board[x][y-3] == judge_x_o && (y-3) >= 0){   // 「2連-3連」の長連を判定
        for(int i=2; i>0; i--){
            if(board[x][y-i] == judge_x_o){
                vertical_upper_exists = 1;
            }else{
                vertical_upper_exists = 0;
                break;
            }
        }
        if((board[x][y+1] == judge_x_o && (board[x][y+2] == judge_x_o))
                && (y+2) < BOARD_SQUARE
                && (vertical_upper_exists == 1)){
            chouren_flag = 1;
        }
    }

    int vertical_lower_exists = 0;        // 垂直方向下向きの長連を探す
    if(board[x][y+4] == judge_x_o && (y+4) < BOARD_SQUARE){     // 「4連-1連」の長連を判定
        for(int i = 3; i > 0; i--){
            if(board[x][y+i] == judge_x_o){
                vertical_lower_exists = 1;
            }else{
                vertical_lower_exists = 0;
                break;
            }
        }
        if((board[x][y-1] == judge_x_o && (y-1) >= 0)
                && (vertical_lower_exists == 1)){
            chouren_flag = 1;
        }
    }
    else if(board[x][y+3] == judge_x_o && (y+3) < BOARD_SQUARE){   // 「2連-3連」の長連を判定
        for(int i=2; i>0; i--){
            if(board[x][y+i] == judge_x_o){
                vertical_lower_exists = 1;
            }else{
                vertical_lower_exists = 0;
                break;
            }
        }
        if((board[x][y-1] == judge_x_o && (board[x][y-2] == judge_x_o))
                && (y-2) >= 0
                && (vertical_lower_exists == 1)){
            chouren_flag = 1;
        }
    }

    // =============== 水平方向 判定 ============================================
    int horizontal_right_exists = 0;        // 水平方向右向きの長連を探す
    if(board[x+4][y] == judge_x_o && (x+4) < BOARD_SQUARE){    // 「4連-1連」の長連を判定
        for(int i=3; i>0; i--){
            if(board[x+i][y] == judge_x_o){
                horizontal_right_exists = 1;
            }else{
                horizontal_right_exists = 0;
                break;
            }
        }
        if((board[x-1][y] == judge_x_o && (x-1) >= 0)
                && (horizontal_right_exists == 1)){
            chouren_flag = 1;
        }
    }
    else if(board[x+3][y] == judge_x_o && (x+3) < BOARD_SQUARE){   // 「2連-3連」の長連を判定
        for(int i=2; i>0; i--){
            if(board[x+i][y] == judge_x_o){
                horizontal_right_exists = 1;
            }else{
                horizontal_right_exists = 0;
                break;
            }
        }
        if((board[x-1][y] == judge_x_o && (board[x-2][y] == judge_x_o))
                && (x-2) >= 0
                && (horizontal_right_exists == 1)){
            chouren_flag = 1;
        }
    }

    int horizontal_left_exists = 0;        // 水平方向左向きの長連を探す
    if(board[x-4][y] == judge_x_o && (x-4) >= 0){     // 「4連-1連」の長連を判定
        for(int i = 3; i > 0; i--){
            if(board[x-i][y] == judge_x_o){
                horizontal_left_exists = 1;
            }else{
                horizontal_left_exists = 0;
                break;
            }
        }
        if((board[x+1][y] == judge_x_o && (x+1) < BOARD_SQUARE)
                && (horizontal_left_exists == 1)){
            chouren_flag = 1;
        }
    }
    else if(board[x-3][y] == judge_x_o && (x-3) >= 0){   // 「2連-3連」の長連を判定
        for(int i=2; i>0; i--){
            if(board[x-i][y] == judge_x_o){
                horizontal_left_exists = 1;
            }else{
                horizontal_left_exists = 0;
                break;
            }
        }
        if((board[x+1][y] == judge_x_o && (board[x+2][y] == judge_x_o))
                && (x+2) < BOARD_SQUARE
                && (horizontal_left_exists == 1)){
            chouren_flag = 1;
        }
    }

    // =============== 斜め方向 判定 ============================================

    // 初めに / 方向を判定する
    int diagonal_right_upper_exists = 0;    // 斜め右上方向に石が4連or3連
    int diagonal_left_lower_exists = 0;     // 斜め左下方向に石が4連or3連

    // 両隣に石があるのは必須
    if(board[x+1][y-1] == judge_x_o && board[x-1][y+1] == judge_x_o
            && ((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE)){
        // 斜め右上方向を調査
        if(board[x+4][y-4] == judge_x_o 
                && ((x+4)<BOARD_SQUARE && (y-4)>=0)){    // 「4連-1連」の長連を判定
            for(int i=3; i>1; i--){
                if(board[x+i][y-i] == judge_x_o){
                    diagonal_right_upper_exists = 1;
                }else{
                    diagonal_right_upper_exists = 0;
                    break;
                }
            }

        }
        else if(board[x+3][y-3] == judge_x_o
                && ((x+3)<BOARD_SQUARE && (y-3)>=0)){   // 「2連-3連」の長連を判定
            if((board[x+2][y-2] == judge_x_o && board[x-2][y+2] == judge_x_o)
                    && ((x-2)>=0 && (y+2)<BOARD_SQUARE)){
                diagonal_right_upper_exists = 1;
            }
        }

        // 斜め左下方向を調査
        if(board[x-4][y+4] == judge_x_o 
                && ((x-4)>=0 && (y+4)<BOARD_SQUARE)){    // 「4連-1連」の長連を判定
            for(int i=3; i>1; i--){
                if(board[x-i][y+i] == judge_x_o){
                    diagonal_left_lower_exists = 1;
                }else{
                    diagonal_left_lower_exists = 0;
                    break;
                }
            }
        }
        else if(board[x-3][y+3] == judge_x_o
                && ((x-3)>=0 && (y+3)<BOARD_SQUARE)){   // 「2連-3連」の長連を判定
            if((board[x+2][y-2] == judge_x_o && board[x-2][y+2] == judge_x_o)
                    && ((x+2)<BOARD_SQUARE && (y-2)>=0)){
                diagonal_left_lower_exists = 1;
            }
        } 
    }
    if(diagonal_right_upper_exists == 1 || diagonal_left_lower_exists == 1){
        chouren_flag = 1;
    }

    // 次に \ 方向を判定する
    int diagonal_left_upper_exists = 0;    // 斜め左上方向に石が4連or3連
    int diagonal_right_lower_exists = 0;     // 斜め右下方向に石が4連or3連

    if(board[x-1][y-1] == judge_x_o && board[x+1][y+1] == judge_x_o
            && ((x-1)>=0 && (x+1)<BOARD_SQUARE && (y-1)>=0 && (y+1)<BOARD_SQUARE)){
        // 斜め左上方向を調査
        if(board[x-4][y-4] == judge_x_o 
                && ((x-4)>=0 && (y-4)>=0)){    // 「4連-1連」の長連を判定
            for(int i=3; i>1; i--){
                if(board[x-i][y-i] == judge_x_o){
                    diagonal_left_upper_exists = 1;
                }else{
                    diagonal_left_upper_exists = 0;
                    break;
                }
            }

        }
        else if(board[x-3][y-3] == judge_x_o
                && ((x-3)>=0 && (y-3)>=0)){   // 「2連-3連」の長連を判定
            if((board[x-2][y-2] == judge_x_o && board[x+2][y+2] == judge_x_o)
                    && ((x+2)<BOARD_SQUARE && (y+2)<BOARD_SQUARE)){
                diagonal_left_upper_exists = 1;
            }
        }

        // 斜め右下方向を調査
        if(board[x+4][y+4] == judge_x_o 
                && ((x+4)<BOARD_SQUARE && (y+4)<BOARD_SQUARE)){    // 「4連-1連」の長連を判定
            for(int i=3; i>1; i--){
                if(board[x+i][y+i] == judge_x_o){
                    diagonal_right_lower_exists = 1;
                }else{
                    diagonal_right_lower_exists = 0;
                    break;
                }
            }
        }
        else if(board[x+3][y+3] == judge_x_o
                && ((x+3)<BOARD_SQUARE && (y+3)<BOARD_SQUARE)){   // 「2連-3連」の長連を判定
            if((board[x+2][y+2] == judge_x_o && board[x-2][y-2] == judge_x_o)
                    && ((x-2)>=0 && (y-2)>=0)){
                diagonal_right_lower_exists = 1;
            }
        } 
    }
    if(diagonal_left_upper_exists == 1 || diagonal_right_lower_exists == 1){
        chouren_flag = 1;
    }

    if(chouren_flag)    return 1;
    else    return 0;

}