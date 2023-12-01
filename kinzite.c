#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include "./judge.h"

int judge_33(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * 三三禁は -|, -\, -/, |\, |/, \/ の6通りの列の組み合わせ間で成立する
     *
     * Args:
     *  x - 石を置くx座標
     *  y - 石を置くy座標
     *  board - 現在の盤面の状態
     */

    int judge_x_o = board[x][y];    // 今回判定する石の種類

    // フラグをビット演算する
    enum {
        LEFT,           // 0
        RIGHT,          // 1
        LOWER,          // 2
        UPPER,          // 3
        RIGHT_LOWER,    // 4
        LEFT_UPPER,     // 5
        RIGHT_UPPER,    // 6
        LEFT_LOWER      // 7
    };

    int flag = 0;  // 8bitでbitごとに上記8つのフラグを管理

    // 左側2連の判定
    if((board[x-2][y] == judge_x_o && board[x-1][y] == judge_x_o) && (x-2)>=0){
        flag |= 1 << LEFT;
    }
    // 右側2連の判定
    if((board[x+2][y] == judge_x_o && board[x+1][y] == judge_x_o) && (x+2)<BOARD_SQUARE){
        flag |= 1 << RIGHT;
    }    
    // 下側2連の判定
    if((board[x][y+2] == judge_x_o && board[x][y+1] == judge_x_o) && (y+2)<BOARD_SQUARE){
        flag |= 1 << LOWER;
    }
    // 上側2連の判定
    if((board[x][y-2] == judge_x_o && board[x][y-1] == judge_x_o) && (y-2)>=0){
        flag |= 1 << UPPER;
    }
    // 斜め右下2連の判定
    if((board[x+2][y+2] == judge_x_o && board[x+1][y+1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y+2)<BOARD_SQUARE)){
        flag |= 1 << RIGHT_LOWER;
    }
    // 斜め左上2連の判定
    if((board[x-2][y-2] == judge_x_o && board[x-1][y-1] == judge_x_o)
            && ((x-2)>=0 && (y-2)>=0)){
        flag |= 1 << LEFT_UPPER;
    }
    // 斜め右上2連の判定
    if((board[x+2][y-2] == judge_x_o && board[x+1][y-1] == judge_x_o)
            && ((x+2)<BOARD_SQUARE && (y-2)>=0)){
        flag |= 1 << RIGHT_UPPER;
    }
    // 斜め左下2連の判定
    if((board[x-2][y+2] == judge_x_o && board[x-1][y+1] == judge_x_o)
            && ((x-2)>=0 && (y+2)<BOARD_SQUARE)){
        flag |= 1 << LEFT_LOWER;
    }

    // 三三禁となるのは、8bit中2bit立っている∧それが同一直線状ではない
    for (int i = 0; i < 8; ++i) {
        for (int j = i + 1; j < 8; ++j) {
            int condition = (1 << i) | (1 << j);

            if ((flag == condition)
                    && !(i==LEFT && j==RIGHT) 
                    && !(i==LOWER && j==UPPER)
                    && !(i==RIGHT_LOWER && j==LEFT_UPPER)
                    && !(i==RIGHT_UPPER && j==LEFT_LOWER)) {
                printf("三三禁です、ゲーム終了");
            }
        }
    }
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
            printf("長連です, ゲーム終了(上)");
            return 1;
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
            printf("長連です, ゲーム終了(上)");
            return 1;
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
            printf("長連です, ゲーム終了(下)");
            return 1;
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
            printf("長連です, ゲーム終了(下)");
            return 1;
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
            printf("長連です, ゲーム終了(右)");
            return 1;
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
            printf("長連です, ゲーム終了(右)");
            return 1;
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
            printf("長連です, ゲーム終了(左)");
            return 1;
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
            printf("長連です, ゲーム終了(左)");
            return 1;
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
        printf("長連です, ゲーム終了(斜め/)");
        return 1;
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
        printf("長連です, ゲーム終了(斜め\\)");
        return 1;
    }    

}