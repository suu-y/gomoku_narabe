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

    int flag_33 = 0;
    int judge_x_o = board[x][y];    // 今回判定する石の種類

    // フラグをビット演算する
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

    int flag = 0b000000000000;  // 12bitでbitごとに上記12つのフラグを管理

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
     * 三三禁になるのは、0-7bit間で2bit立つ ∧ 8-11bit間で2bit立つ
     * しかし
     * 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　または
     * 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ
     * 時は三三禁ではない（四三）
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
                flag_33 = 1;
                // 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　時を調査
                for(int h = 8; h < 12; ++h){
                    int condition_1 = (1 << h);
                    if(flag_8to11 == condition_1){
                        switch(h){
                            case 8:
                                if(i==LEFT || i==RIGHT){
                                    flag_33 = 0;
                                } break;
                            case 9:
                                if(i==LOWER || i==UPPER){
                                    flag_33 = 0;
                                } break;
                            case 10:
                                if(i==RIGHT_LOWER || i==LEFT_UPPER){
                                    flag_33 = 0;
                                } break;
                            case 11:
                                if(i==RIGHT_UPPER || i==LEFT_LOWER){
                                    flag_33 = 0;
                                } break;
                        }
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
                                if(i==HORIZONTAL_MID){
                                    flag_33 = 0;
                                } break;
                            case 2:
                            case 3:
                                if(i==VERTICAL_MID){
                                    flag_33 = 0;
                                } break;
                            case 4:
                            case 5:
                                if(i==DIAGONALLY_LEFT){
                                    flag_33 = 0;
                                } break;
                            case 6:
                            case 7:
                                if(i==DIAGONALLY_RIGHT){
                                    flag_33 = 0;
                                } break;
                        }
                    }
                }
            }
        }
    }   
    if(flag_33){
        printf("三三禁です、ゲーム終了");
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