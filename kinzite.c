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

    // ------------- 判定 -------------------------------------

    int vertical_upper_exists = 0;        // 垂直方向上向きの長連を探す
    if(board[x][y-4] == judge_x_o && (y-4) > 0){
        for(int i = 3; i > 0; i--){
            if(board[x][y-i] == judge_x_o){
                vertical_upper_exists = 1;
            }else{
                vertical_upper_exists = 0;
                break;
            }
        }
        if((board[x][y+1] == judge_x_o && (y+1) < BOARD_SQUARE + 1)
                && (vertical_upper_exists == 1)){
            chouren_flag = 1;
            printf("長連です, ゲーム終了(上)");
            return 1;
        }
    }

    int vertical_lower_exists = 0;        // 垂直方向下向きの長連を探す
    if(board[x][y+4] == judge_x_o && (y+4) < BOARD_SQUARE){
        for(int i = 3; i > 0; i--){
            if(board[x][y+i] == judge_x_o){
                vertical_lower_exists = 1;
            }else{
                vertical_lower_exists = 0;
                break;
            }
        }
        if((board[x][y-1] == judge_x_o && (y-1) > 0)
                && (vertical_lower_exists == 1)){
            chouren_flag = 1;
            printf("長連です, ゲーム終了(下)");
            return 1;
        }
    }

}