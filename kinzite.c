#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include "./judge.h"


int judge_chouren(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * 長連は「4連-1連」　または　「2連-3連」　の時に間に石を置くと成立する
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

}