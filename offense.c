#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./offense.h"

// 攻め・守り判定関数の返り値を受けて、攻め関数offense()が呼び出される
void offense(int* arg_x, int* arg_y, int board[BOARD_SQUARE][BOARD_SQUARE], int flag_first_second){

    // TODO: 序盤とか、まだ石の数が足らない等で四三を作るに至らない時の石を置く基準を検討する

    /*
     * Args:
     *  *x - 次に置く位置となるx座標
     *  *y - 次に置く位置となるy座標
     *  board - 現在の盤面
     *  flag_first_second - この関数を呼び出したのかは先手か(0)後手か(1)
     * 
     * 【方針】
     * - 四三を作りに行く
     * 
     * **優先順位**
     * 1. 5連（勝ち）
     * 2. 長連（勝ち、後攻の時のみ）
     * 3. 四三
     * 
     * 四三を作るに至らない時は、できるだけ連続で石が並ぶように置く
     */

    int flag_5ren = 0;
    int flag_chouren = 0;
    int flag_43 = 0;

    // 四三を作るに至らない時のために座標を格納しておく
    int sub_x = -1;
    int sub_y = -1;

    // 盤面を走査して、自分の石の並び方を調査する
    for(int y=0; y<BOARD_SQUARE; y++){          // 縦方向
        for(int x=0; x<BOARD_SQUARE; x++){      // 横方向
            
            if(x >= (BOARD_SQUARE*2/3) && y >= (BOARD_SQUARE*2/3))    break;
            
            //printf("(%d, %d)\n", x+1, y+1);
            int stone_x_o = board[x][y];
            if(stone_x_o == 0){

                // 判定のために(x, y)に自分の石を置く
                board[x][y] = 1;

                flag_5ren = is_5ren_edge(x, y, board);
                if(flag_first_second == 1){     // 後手の時のみ長連を打てる
                    flag_chouren = is_chouren(x, y, board);
                }
                flag_43 = is_43(x, y, board);

                // 判定を行う           
                if(flag_5ren){
                    printf("\n5連を作れます: (%d, %d)\n", x+1, y+1);
                    // 盤面を元の状態に戻す
                    board[x][y] = stone_x_o;
                    // この座標をポインタで返す
                    *arg_x = x;
                    *arg_y = y;                
                    break;
                }
                else if(flag_chouren){
                    printf("\n長連を作れます: (%d, %d)\n", x+1, y+1);
                    // 盤面を元の状態に戻す
                    board[x][y] = stone_x_o;
                    // この座標をポインタで返す
                    *arg_x = x;
                    *arg_y = y;   
                    break;
                }
                else if(flag_43){
                    printf("\n四三を作れます: (%d, %d)\n", x+1, y+1);
                    // 盤面を元の状態に戻す
                    board[x][y] = stone_x_o;
                    // この座標をポインタで返す
                    *arg_x = x;
                    *arg_y = y;
                    break;
                }
                else{
                    // 盤面を元の状態に戻す
                    // この時、盤面の探索は続けたいのでbreakしない
                    board[x][y] = stone_x_o; 
                }
            }
            else if(stone_x_o == 1){ 
                sub_x = x;
                sub_y = y;
                flag_5ren = is_5ren_mid(&x, &y, board);
                if(flag_5ren){
                    printf("\n5連を作れます(is_5ren_mid): (%d, %d)\n", x+1, y+1);
                }
            }
            // この座標をポインタで返す
            *arg_x = x;
            *arg_y = y;  
            if(flag_5ren || flag_chouren || flag_43)   break;
        }
        if(flag_5ren || flag_chouren || flag_43)   break;
    }

    // 四三を作るに至らない時
    // 現在自分の石が置かれている周囲に置きに行く
    if(!flag_5ren && !flag_chouren && !flag_43){

        int put_flag = 0;   // 石を置けたかどうかのフラグ

        while(1){

            // 周囲で空いている箇所を探す
            // 飛び三・飛び四を作るために、変数iで隣接だけでなく1つ空きも埋めるようにする
            srand((unsigned int) time(NULL));
            int rand_num = rand() % 8;
            int i = rand() % 2 + 1;

            switch(rand_num){
                case 0:
                    if(board[sub_x + i][sub_y + i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x+i+1, sub_y+i+1);
                        *arg_x = sub_x + i;
                        *arg_y = sub_y + i;
                        put_flag = 1;
                    }
                    break;
                case 1:
                    if(board[sub_x - i][sub_y + i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x-i+1, sub_y+i+1);
                        *arg_x = sub_x - i;
                        *arg_y = sub_y + i;
                        put_flag = 1;
                    }
                    break;
                case 2:
                    if(board[sub_x + i][sub_y] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x+i+1, sub_y+1);
                        *arg_x = sub_x + i;
                        *arg_y = sub_y;
                        put_flag = 1;
                    }
                    break;
                case 3:
                    if(board[sub_x + i][sub_y - i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x+i+1, sub_y-i+1);
                        *arg_x = sub_x + i;
                        *arg_y = sub_y - i;
                        put_flag = 1;
                    }
                    break;
                case 4:
                    if(board[sub_x - i][sub_y - i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x-i+1, sub_y-i+1);
                        *arg_x = sub_x - i;
                        *arg_y = sub_y - i;
                        put_flag = 1;
                    }
                    break;
                case 5:
                    if(board[sub_x - i][sub_y] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x-i+1, sub_y+1);
                        *arg_x = sub_x - i;
                        *arg_y = sub_y;
                        put_flag = 1;
                    }
                    break;
                case 6:
                    if(board[sub_x][sub_y + i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x+1, sub_y+i+1);
                        *arg_x = sub_x;
                        *arg_y = sub_y + i;
                        put_flag = 1;
                    }
                    break;
                case 7:
                    if(board[sub_x][sub_y - i] == 0){
                        printf("\nここに置いてください: (%d, %d)\n", sub_x+1, sub_y-i+1);
                        *arg_x = sub_x;
                        *arg_y = sub_y - i;
                        put_flag = 1;
                    }
                    break;
                default:
                    break;
            }
            if(put_flag)    break;
            
        }
    }
}

int is_5ren_mid(int* x, int* y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * この関数は offence 関数で呼び出され、盤面上の全ての(x, y)に対して
     * 5連があるかを判定する
     * ただし、5連の中の石がまだ置かれていない場合のみしか判定できない
     * 
     * Args:
     *  x - 探索の起点となするx座標
     *  y - 探索の起点となするy座標
     *  board - 現在の盤面の状態
     * 
     * Rtn:
     *  0 - 入力(x, y)で5連は成立しない
     *  1 - 入力(x, y)で5連が成立する
     */

    /*
     * 方針: 盤面を9つのブロックに分けて、その位置から5連があり得る方向を探索する
     * 識別のため、テンキーの数字と同じ順でブロックに番号を付けている↓
     * 7 8 9
     * 4 5 6
     * 1 2 3
     */  
    
    int is_5ren = 0;
    int is_empty = 0;   // 5連となる箇所が空の時は真、埋まっていると偽
    int cnt_stone = 0;
    int stone_x_o = board[*x][*y];

    int store_x = -1;
    int store_y = -1;

    switch(*x/5){
        case 0:
            switch(*y/5){
                case 0:
                    // ７番ブロックの時、あり得るのは - | \\ の3種類
                case 1:
                    // 4番ブロックの時、あり得るのは - | \\ の3種類
                    // -方向
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y] == 0){
                            store_x = *x + k;
                            store_y = *y;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[*x][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x][*y+k] == 0){
                            store_x = *x;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    // \\方向
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y+k] == 0){
                            store_x = *x + k;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    break;
                case 2:
                    // 1番ブロックの時、あり得るのは -
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y] == 0){
                            store_x = *x + k;
                            store_y = *y;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    break;
                default:
                    break;
            }
            cnt_stone = 0;
            is_empty = 0;
            break;
        case 1:
            switch(*y/5){
                case 0:
                    // 8番ブロックの時、あり得るのは - | \\ / の3種類
                case 1:
                    // 5番ブロックの時、あり得るのは - | \\ / の3種類
                    // -方向
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y] == 0){
                            store_x = *x + k;
                            store_y = *y;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[*x][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x][*y+k] == 0){
                            store_x = *x;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    // \\方向
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y+k] == 0){
                            store_x = *x + k;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    // /方向
                    for(int k=0; k<5; k++){
                        if(board[*x-k][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x-k][*y+k] == 0){
                            store_x = *x - k;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    break;
                case 2:
                    // 2番ブロックの時、あり得るのは -
                    for(int k=0; k<5; k++){
                        if(board[*x+k][*y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x+k][*y] == 0){
                            store_x = *x + k;
                            store_y = *y;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    break;                  
                default:
                    break;
            }
            cnt_stone = 0;
            is_empty = 0;
            break;
        case 2:
            switch(*y/5){
                case 0:
                    // 9番ブロックの時、あり得るのは | / の3種類
                case 1:
                    // 6番ブロックの時、あり得るのは | / の3種類
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[*x][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x][*y+k] == 0){
                            store_x = *x;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    is_empty = 0;
                    break;
                    // / 方向
                    for(int k=0; k<5; k++){
                        if(board[*x-k][*y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else if(board[*x-k][*y+k] == 0){
                            store_x = *x - k;
                            store_y = *y + k;
                            is_empty = 1;
                        }
                    }
                    if(cnt_stone == 4 && is_empty){
                        is_5ren = 1;
                        break;
                    }
                default:
                    break;
            }
            cnt_stone = 0;
            is_empty = 0;
            break;
        default:
            break;
    }

    *x = store_x;
    *y = store_y;

    if(is_5ren)    return 1;
    else    return 0;

}

int is_5ren_edge(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * この関数は offence 関数で呼び出され、盤面上の全ての(x, y)に対して
     * 5連があるかを判定する
     * ただし、5連の端点がまだ置かれていない場合のみしか判定できない
     * 
     * Args:
     *  x - 探索の起点となするx座標
     *  y - 探索の起点となするy座標
     *  board - 現在の盤面の状態
     * 
     * Rtn:
     *  0 - 入力(x, y)で5連は成立しない
     *  1 - 入力(x, y)で5連が成立する
     */

    /*
     * 方針: 盤面を9つのブロックに分けて、その位置から5連があり得る方向を探索する
     * 識別のため、テンキーの数字と同じ順でブロックに番号を付けている　下
     * 7 8 9
     * 4 5 6
     * 1 2 3
     */  
    
    int is_5ren = 0;
    int cnt_stone = 0;
    int stone_x_o = board[x][y];

    switch(x/5){
        case 0:
            switch(y/5){
                case 0:
                    // ７番ブロックの時、あり得るのは - | \\ の3種類
                case 1:
                    // 4番ブロックの時、あり得るのは - | \\ の3種類
                    // -方向
                    for(int k=0; k<5; k++){
                        if(board[x+k][y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else break;
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[x][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else break;
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    // \\方向
                    for(int k=0; k<5; k++){
                        if(board[x+k][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                        else break;
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    break;
                case 2:
                    // 1番ブロックの時、あり得るのは -
                    for(int k=0; k<5; k++){
                        if(board[x+k][y] == stone_x_o){
                            cnt_stone ++;
                        }
                        else break;
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    break;
                default:
                    break;
            }
            cnt_stone = 0;
            break;
        case 1:
            switch(y/5){
                case 0:
                    // 8番ブロックの時、あり得るのは - | \\ / の3種類
                case 1:
                    // 5番ブロックの時、あり得るのは - | \\ / の3種類
                    // -方向
                    for(int k=0; k<5; k++){
                        if(board[x+k][y] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[x][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    // \\方向
                    for(int k=0; k<5; k++){
                        if(board[x+k][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    // /方向
                    for(int k=0; k<5; k++){
                        if(board[x-k][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    break;
                case 2:
                    // 2番ブロックの時、あり得るのは -
                    for(int k=0; k<5; k++){
                        if(board[x+k][y] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    break;                  
                default:
                    break;
            }
            cnt_stone = 0;
            break;
        case 2:
            switch(y/5){
                case 0:
                    // 9番ブロックの時、あり得るのは | / の3種類
                case 1:
                    // 6番ブロックの時、あり得るのは | / の3種類
                    // |方向
                    for(int k=0; k<5; k++){
                        if(board[x][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                    cnt_stone = 0;
                    break;
                    // / 方向
                    for(int k=0; k<5; k++){
                        if(board[x-k][y+k] == stone_x_o){
                            cnt_stone ++;
                        }
                    }
                    if(cnt_stone == 5){
                        is_5ren = 1;
                        break;
                    }
                default:
                    break;
            }
            cnt_stone = 0;
            break;
        default:
            break;
    }

    if(is_5ren)    return 1;
    else    return 0;

}

int is_chouren(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * この関数は offence 関数で呼び出され、盤面上の全ての(x, y)に対して
     * 長連があるかを判定する
     * 
     * Args:
     *  x - 探索の起点となするx座標
     *  y - 探索の起点となするy座標
     *  board - 現在の盤面の状態
     * 
     * Rtn:
     *  0 - 入力(x, y)で長連は成立しない
     *  1 - 入力(x, y)で長連が成立する
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

int is_43(int x, int y, int board[BOARD_SQUARE][BOARD_SQUARE]){

    /*
     * この関数は offence 関数で呼び出され、盤面上の全ての(x, y)に対して
     * 四三があるかを判定する
     *
     * Args:
     *  x - 探索の起点となるx座標
     *  y - 探索の起点となるy座標
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
    int flag_4ren = 0b00000000;         // 8bitで4連の時は特別にこのフラグを立てる

    // 左側2連の判定
    if((x-3)>=0){     // 飛び三
        for(int i=-3; i<0; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone++;
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
        }
    }
    if(cnt_stone == 1)  flag |= 1 << HORIZONTAL_MID;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && board[x-1][y]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y] == judge_x_o)  cnt_stone ++;
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
        }
    }
    if(cnt_stone == 1)  flag |= 1 << VERTICAL_MID;
    cnt_stone = 0;
    if((y-1)>=0 && (y+2)<BOARD_SQUARE && board[x][y-1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x][y+i] == judge_x_o)  cnt_stone ++;
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
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_LEFT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && (y-1)>=0 && (y+2)<BOARD_SQUARE
        && board[x-1][y-1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y+i] == judge_x_o)  cnt_stone ++;
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
        }
    }
    if(cnt_stone == 1)  flag |= 1 << DIAGONALLY_RIGHT;
    cnt_stone = 0;
    if((x-1)>=0 && (x+2)<BOARD_SQUARE && (y-2)>=0 && (y+1)<BOARD_SQUARE
        && board[x-1][y+1]==judge_x_o){
        for(int i=1; i<3; i++){
            if(board[x+i][y-i] == judge_x_o)  cnt_stone ++;
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
     * 　四三が成立するには、0-11bit間で少なくとも3bit分は立つ必要がある
     *  さらに、
     * 「0-7bit間で2bit立つ」時に8-11bit間で同一方向のbitが1つ立つ　または
     * 「8-11bit間で2bit立つ」時に0-7bit間で同一方向のbitが1つ立つ　または
     * 　時が**四三**となる
     */

    //printf("43-フラグ: %x\n", flag);
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
                // フラグ2個で四三の時
                if(flag_4ren == (1 << i) || flag_4ren == (1 << j)){
                    flag_43 = 1;
                    break;
                }
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

    // 0-7bit, 8-11bit間で1bitずつ立つ（このとき計2bitしか立っていない）
    // かつ flag_4ren が立っている
    for(int i = 0; i < 8; ++i) {
        int condition_5 = (1 << i);
        if(flag_0to7 == condition_5 && flag_4ren == condition_5) {
            for(int j = 8; j < 12; ++j) {
                int condition_6 = (1 << j);
                if(flag_8to11 == condition_6){
                    flag_43 = 1;

                    // bit同士が同一方向の場合は四三ではない
                    switch (j){
                        case 8:
                            if(i == 0 || i == 1)    flag_43 = 0;
                            break;
                        case 9:
                            if(i == 2 || i == 3)    flag_43 = 0;
                            break;
                        case 10:
                            if(i == 4 || i == 5)    flag_43 = 0;
                            break;
                        case 11:
                            if(i == 6 || i == 7)    flag_43 = 0;
                            break;
                    }
                    break;
                }
            }
        }
    } 

    if(flag_43) return 1;
    else    return 0;
}