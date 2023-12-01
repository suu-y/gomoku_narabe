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

/* ゲー�?の勝ち�?けを判定す�?
 * int board[BOARD_SQUARE][BOARD_SQUARE] : 手を出す前の盤の状�? 
 * int x : 出した手�?�x座�?
 * int y : 出した手�?�y座�? (今�?�した手�?�board[x-1][y-1]の位置になるっぽ�?) 
 * こ�?�手で勝てるな�?1を，勝てな�?な�?0を返す
 */
int judgeWin(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    // 縦に並ぶか判定す�?
    if(judgeVertical(board, x, y)) return 1;
    // 横に並ぶか判定す�?
    if(judgeWidth(board, x, y)) return 1;
    // 斜めに並ぶか判定す�?
    if(judgeDiagonal(board, x, y)) return 1;
    return 0;
}

/* 縦に5つ並ぶか判定す�?
 * board : 盤の状�?
 * x : 出した手�?�x座�?
 * y : 出した手�?�y座�?
 * 縦に5つ並んで�?たら1を，並んで�?なかったら0を返す
 */
int judgeVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んで�?る数をカウントす�?
    int i, x2 = x-1, y2 = y-2;
    // x-1, y-1から上方向，下方向�?��?番に何個並ぶか確認して�?�?
    // 上方向に何個並ぶか確認。最大4�?([x-1][y-1]部�?を除く�?�で)
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    for(i=0; i<4;i++)
    {
        if(board[x2][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 下方向に何個並ぶか確認。最大4�?([x-1][y-1]部�?を除く�?�で)
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* 横に5つ並ぶか判定す�?
 * board : 盤の状�?
 * x : 出した手�?�x座�?
 * y : 出した手�?�y座�?
 * 横に5つ並んで�?たら1を，並んで�?なかったら0を返す
 */
int judgeWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んで�?る数をカウントす�?
    int i, x2 = x-2, y2 = y-1;
    // x-1, y-1から左方向，右方向�?��?番に何個並ぶか確認して�?�?
    // 左方向に何個並ぶか確認。最大4�?([x-1][y-1]部�?を除く�?�で)
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 右方向に何個並ぶか確認。最大4�?([x-1][y-1]部�?を除く�?�で)
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    x2 = x;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* 斜めに5つ並ぶか判定す�?
 * board : 盤の状�?
 * x : 出した手�?�x座�?
 * y : 出した手�?�y座�?
 * 斜めに5つ並んで�?たら1を，並んで�?なかったら0を返す
 */
int judgeDiagonal(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // 並んで�?る数をカウントす�?
    // x-1, y-1から右斜め?��左斜めの�?番に何個並ぶか確認して�?�?
    // 左下に何個並ぶか確認。最大4�?([x-1][y-1]部�?を除く�?�で)
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    int i, x2 = x-2, y2 = y;
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // 右上に何個並ぶか確認�?
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    x2 = x; y2 = y-2;
    for(i=0; i<4;i++)
    {
        if(board[x2++][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    // 右下に何個並ぶか確認�?
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    cnt = 1; x2 = x; y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    
    // 左上に何個並ぶか確認�?
    // 仮にboard�?に何もな�?ところは0?���?��?が置�?たところは1?��相手が置�?たところは2で格納されて�?るとする
    x2 = x-2; y2 = y-2;
    for(i=0;i<4;i++)
    {
        if(board[x2--][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    return 0;
}