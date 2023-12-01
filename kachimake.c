#include <stdio.h>
#include <string.h>
#include "kachimake.h"

void win(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, char *message)
{
    if(judgeWin(board, x, y))
    {
        printf(",win\n");
        strcat(message, ",win");
    }
}

/* ゲームの勝ち負けを判定する
 * int board[BOARD_SQUARE][BOARD_SQUARE] : 手を出す前の盤の状態 
 * int x : 出した手のx座標
 * int y : 出した手のy座標 (今出した手はboard[x-1][y-1]の位置になるっぽい) 
 * この手で勝てるなら1を，勝てないなら0を返す
 */
int judgeWin(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    // 縦に並ぶか判定する
    if(judgeVertical(board, x, y)) return 1;
    // 横に並ぶか判定する
    if(judgeWidth(board, x, y)) return 1;
    // 斜めに並ぶか判定する
    if(judgeDiagonal(board, x, y)) return 1;
    return 0;
}

/* 縦に5つ並ぶか判定する
 * board : 盤の状態
 * x : 出した手のx座標
 * y : 出した手のy座標
 * 縦に5つ並んでいたら1を，並んでいなかったら0を返す
 */
int judgeVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んでいる数をカウントする
    int i, x2 = x-1, y2 = y-2;
    // x-1, y-1から上方向，下方向の順番に何個並ぶか確認していく
    // 上方向に何個並ぶか確認。最大4個([x-1][y-1]を除くので)
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    for(i=0; i<4;i++)
    {
        if(board[x2][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 下方向に何個並ぶか確認。最大4個([x-1][y-1]を除くので)
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* 横に5つ並ぶか判定する
 * board : 盤の状態
 * x : 出した手のx座標
 * y : 出した手のy座標
 * 横に5つ並んでいたら1を，並んでいなかったら0を返す
 */
int judgeWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んでる数をカウントする
    int i, x2 = x-2, y2 = y-1;
    // x-1, y-1から左方向，右方向の順番に何個並ぶか確認していく
    // 左方向に何個並ぶか確認。最大4個([x-1][y-1]を除くので)
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 右方向に何個並ぶか確認。最大4個([x-1][y-1]を除くので)
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    x2 = x;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* 斜めに5つ並ぶか判定する
 * board : 盤の状態
 * x : 出した手のx座標
 * y : 出した手のy座標
 * 斜めに5つ並んでいたら1を，並んでいなかったら0を返す
 */
int judgeDiagonal(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んでる数をカウントする
    // x-1, y-1から右斜め、左斜めの順番に何個並ぶか確認していくいく
    // 左下に何個並ぶか確認。最大4個([x-1][y-1]を除くので)
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    int i, x2 = x-2, y2 = y;
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 右上に何個並ぶか確認する
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    x2 = x; y2 = y-2;
    for(i=0; i<4;i++)
    {
        if(board[x2++][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    // 右下に何個並ぶか確認する
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    cnt = 1; x2 = x; y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    
    // 左上に何個並ぶか確認する
    // 仮にboardに何もないところは0、自分が置いたところは1、相手が置いたところは2で格納されているとする
    x2 = x-2; y2 = y-2;
    for(i=0;i<4;i++)
    {
        if(board[x2--][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    return 0;
}