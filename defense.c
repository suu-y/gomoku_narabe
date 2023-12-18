#include <stdio.h>
#include <math.h>
#include "./defense.h"
#include "./judge.h"
#define _5REN  5
#define _4REN  4
#define _43    3
#define _TOBI4 2
#define _3REN  1

/* 優先順位(評価値)
 * 1.5連(5)
 * 2.4連(4)
 * 3.四三(3)
 * 4.飛び四(2)
 * 5.三連(1)
*/

typedef struct cand
{
    place p; // 候補地の座標を保持
    int eval; // 評価値
    cand *next; // 次の候補座標へのポインタ
} cand;

// 各方向がどうなるかを
enum {
    LEFT,               // 0(左)
    RIGHT,              // 1(右)
    UPPER,              // 2(上)
    DOWN,               // 3(下)
    LEFT_UPPER,         // 4(左上)
    RIGHT_DOWN,         // 5(右下)
    RIGHT_UPPER,        // 6(右上)
    LEFT_DOWN           // 7(左下)
};

/* 守りに徹するべきか，攻めに転じるべきかを判定
 * 守りに徹する場合，二度走査するのは面倒なので，ここで打つ手を確定してしまう．
 * board : 盤の状況を把握
 * 返り値0 : 守り
 * 返り値1 : 攻め
 */
int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p)
{
    int flag = 0b00000000; // 8方向にコマがあるかを確認する．
    int i, j;
    int directions[8] = {0}; // 8方向のコマ数を配列で格納．

    /* リスト構造の作成 */
    cand *initc = NULL;
    cand *prevc = NULL;

    // y方向
    for(i=0;i<BOARD_SQUARE;i++)
    {
        // x方向
        for(j=0;j<BOARD_SQUARE;j++)
        {
            // 何も置かれていないところを調べる
            if(board[j][i]==0)
            {
               // フラグの設定
               setFlag(board, j, i, &flag);
            
                search8directions(board,j,i,flag,directions);

                //横，縦，左斜め，右斜めに自分を入れて5個並ぶなら，そこに評価値5を与え，リストに組み込む．(長連あがりもここで封じる)
                if((directions[LEFT]+directions[RIGHT]>=4) || (directions[UPPER]+directions[DOWN]>=4) || 
                    (directions[LEFT_UPPER]+directions[RIGHT_DOWN]>=4) || 
                    (directions[RIGHT_UPPER]+directions[LEFT_DOWN]>=4))
                {
                    addList(initc, prevc, j, i, _5REN);
                    // 5連があった場合はこの時点で評価値最大なので次のコマへ
                    continue;
                }

                //横，縦，左斜め，右斜めに自分を入れて4個並ぶなら，そこに評価値4を与え，リストに組み込む．
                if((directions[LEFT]+directions[RIGHT]==3) || (directions[UPPER]+directions[DOWN]==3) || 
                    (directions[LEFT_UPPER]+directions[RIGHT_DOWN]==3) || 
                    (directions[RIGHT_UPPER]+directions[LEFT_DOWN]==3))
                {
                    addList(initc, prevc, j, i, _4REN);
                    // ここに来てる時点で5連なしで4連．評価値最大なので次のコマへ.
                    continue;
                }
                //横，縦，左斜め，右斜めに自分を入れて3個並ぶ
                if((directions[LEFT]+directions[RIGHT]==2) || (directions[UPPER]+directions[DOWN]==2) || 
                    (directions[LEFT_UPPER]+directions[RIGHT_DOWN]==2) || 
                    (directions[RIGHT_UPPER]+directions[LEFT_DOWN]==2))
                {
                    // 四三，飛び四，三連のどれか
                    if(is43)
                    {
                        addList(initc, prevc, j, i, _43);
                        continue;
                    } else if(isTobi4(board,j,i,directions)) addList(initc, prevc,j,i,_TOBI4);
                    else addList(initc,prevc,j,i,_3REN); 
                }
            }
        }
    }
    return 1;
}

/* 左方向に自分を含めて相手のコマが何個並ぶか確認 */
int countLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(--x>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 右方向に自分を含めて相手のコマが何個並ぶか確認 */
int countRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(++x<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 上方向に自分を含めて相手のコマが何個並ぶか確認 */
int countUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(--y>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 下方向に自分を含めて相手のコマが何個並ぶか確認 */
int countDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 左斜め上に自分を含めて相手のコマが何個並ぶか確認 */
int countLeftUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(--x>=0 && --y>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 右斜め下に自分を含めて相手のコマが何個並ぶか確認 */
int countRightDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(++x<BOARD_SQUARE && ++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 右斜め上に自分を含めて何個並ぶか確認*/
int countRightUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(++x<BOARD_SQUARE && --y>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 左斜め下に自分を含めて何個並ぶか確認*/
int countLeftDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 0;
    while(--x>=0 && ++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* フラグをセットする */
void setFlag(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, int *flag)
{
    *flag = 0b00000000; // フラグを初期化
    if(x>0 && board[x-1][y]==2)                                 *flag |= (1 << LEFT);        // 左に相手のコマ
    if(x<BOARD_SQUARE-1 && board[x+1][y]==2)                    *flag |= (1 << RIGHT);       // 右に相手のコマ
    if(y>0 && board[x][y-1]==2)                                 *flag |= (1 << UPPER);       // 上に相手のコマ
    if(y<BOARD_SQUARE-1 && board[x][y+1]==2)                    *flag |= (1 << DOWN);        // 下に相手のコマ
    if(x>0 && y>0 && board[x-1][y-1]==2)                        *flag |= (1 << LEFT_UPPER);  // 左上に相手のコマ
    if(x<BOARD_SQUARE-1 && y<BOARD_SQUARE-1 && board[x+1][y+1]) *flag |= (1 << RIGHT_DOWN);  // 右下に相手のコマ
    if(x<BOARD_SQUARE-1 && y>0 && board[x+1][y-1]==2)           *flag |= (1 << RIGHT_UPPER); // 右上に相手のコマ
    if(x>0 && y<BOARD_SQUARE-1 && board[x-1][y+1]==2)           *flag |= (1 << LEFT_DOWN);   // 左下に相手のコマ
}

/* 8方向にそれぞれコマが何個並ぶかをカウントして，配列に格納する． */
void search8directions(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, int flag, int directions[8])
{
    if(flag & (1 << LEFT) == (1 << LEFT)) // 左方向にコマあり
        directions[LEFT] = countLeft(board, x, y);
    if(flag & (1 << RIGHT) == (1 << RIGHT)) // 右方向にコマあり
        directions[RIGHT] = countLeft(board, x, y);
    if(flag & (1 << UPPER) == (1 << UPPER)) // 上方向にコマあり
        directions[UPPER] = countLeft(board, x, y);
    if(flag & (1 << DOWN) == (1 << DOWN)) // 下方向にコマあり
        directions[DOWN] = countLeft(board, x, y);
    if(flag & (1 << LEFT_UPPER) == (1 << LEFT_UPPER)) // 左上方向にコマあり
        directions[LEFT_UPPER] = countLeft(board, x, y);
    if(flag & (1 << RIGHT_DOWN) == (1 << RIGHT_DOWN)) // 右下方向にコマあり
        directions[RIGHT_DOWN] = countLeft(board, x, y);
    if(flag & (1 << RIGHT_UPPER) == (1 << RIGHT_UPPER)) // 右上方向にコマあり
        directions[RIGHT_UPPER] = countLeft(board, x, y);
    if(flag & (1 << LEFT_DOWN) == (1 << LEFT_DOWN)) // 左下方向にコマあり
        directions[LEFT_DOWN] = countLeft(board, x, y);
}

/* 与えられた座標，評価値の値を元にリストに組み込むための関数 */
void addList(cand *initc, cand *prevc, int x, int y, int eval)
{
    cand *tmp=malloc(sizeof(cand));
    (tmp->p).x = x;
    (tmp->p).y = y;
    tmp->eval = eval;
    tmp->next = NULL;
    if(initc==NULL) initc=tmp;
    else prevc->next = tmp;

    prevc=tmp;
}

// 3個並ぶ時に，四三になるか調べる
int is43(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, int directions[8])
{
    int flag2;
    int directions2[8] = {0};
    /* 3個の並び方は以下の通り(●がx,yの位置)
     * ●○○, ○●○, ○○●
     * 四三で四側は四連で防げるはずなので，ここでは3側に組み込まれていると想定
     * 全パターンについて,4つ並ぶ箇所があるかを確認する．
     */
    if(directions[LEFT]+directions[RIGHT]==2) // 横並び
    {
        if(directions[LEFT]&&directions[RIGHT]) // ○●○
        {
            // 左のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x-1, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x-1,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;

            // 右のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x+1, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x+1,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;
        } else if(directions[LEFT]) { // ○○●
            // 1つ左のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x-1, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x-1,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;

            // 2つ左のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x-2, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x-2,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;
        } else { // ●○○
            // 1つ右のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x+1, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x+1,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;

            // 2つ右のコマについて，四連が成立する箇所があるか調べる
            setFlag(board, x+2, y, &flag2);
            memset(directions2,0,sizeof(directions2));
            search8directions(board,x+2,y,flag2,directions2);
            if((directions2[UPPER]+directions2[DOWN]==3) || 
                (directions2[LEFT_UPPER]+directions2[RIGHT_DOWN]==3) ||
                (directions2[RIGHT_UPPER]+directions2[LEFT_DOWN]==3)) return 1;
        }
    }
}

// 3個並ぶ時に，飛び四になるか調べる
int isTobi4_3(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, int directions[8])
{
    /* 考え得る状況としては，(●がx,yの位置)
     * ●○○(空)○，○(空)●○○，○●○(空)○，○(空)○●○，○○●(空)○，○(空)○○●のどれか
     */
    if(directions[LEFT]+directions[RIGHT]==2) // 横並びの場合
    {
        if(directions[LEFT]&&directions[RIGHT]) // ○●○(空)○,○(空)○●○
        {
            if((x+3<BOARD_SQUARE && board[x+3][y]==1) || 
                (x-3>=0 && board[x-3][y]==1)) return 1;
        } else if(directions[LEFT]) { // ○○●(空)○,○(空)○○●
            if((x+2<BOARD_SQUARE && board[x+2][y]==1) || 
                (x-4>=0 && board[x-4][y]==1)) return 1;
        } else { // ●○○(空)○,○(空)●○○
            if((x+4<BOARD_SQUARE && board[x+4][y]==1) ||
                (x-2>=0 && board[x-2][y]==1)) return 1;
        }
    } else if(directions[UPPER]+directions[DOWN]==2) { // 縦並びの場合
        if(directions[UPPER]&&directions[DOWN]) // ○●○(空)○,○(空)○●○
        {
            if((y+3<BOARD_SQUARE && board[x][y+3]==1) || 
                (y-3>=0 && board[x][y-3]==1)) return 1;
        } else if(directions[UPPER]) { // ○○●(空)○,○(空)○○●
            if((y+2<BOARD_SQUARE && board[x][y+2]==1) || 
                (y-4>=0 && board[x][y-4]==1)) return 1;
        } else { // ●○○(空)○,○(空)●○○
            if((y+4<BOARD_SQUARE && board[x][y+4]==1) ||
                (y-2>=0 && board[x][y-2]==1)) return 1;
        }
    } else if(directions[LEFT_UPPER]+directions[RIGHT_DOWN]==2) { // 左斜め並びの場合
        if(directions[LEFT_UPPER]&&directions[RIGHT_DOWN]) // ○●○(空)○,○(空)○●○
        {
            if((x+3<BOARD_SQUARE && y+3<BOARD_SQUARE && board[x+3][y+3]==1) || 
                (x-3>=0 && y-3>=0 && board[x-3][y-3]==1)) return 1;
        } else if(directions[LEFT_UPPER]) { // ○○●(空)○,○(空)○○●
            if((x+2<BOARD_SQUARE && y+2<BOARD_SQUARE && board[x+2][y+2]==1) || 
                (x-4>=0 && y-4>=0 && board[x-4][y-4]==1)) return 1;
        } else { // ●○○(空)○,○(空)●○○
            if((x+4<BOARD_SQUARE && y+4<BOARD_SQUARE && board[x+4][y+4]==1) ||
                (x-2>=0 && y-2>=0 && board[x-2][y-2]==1)) return 1;
        }
    } else { //右斜め並びの場合
        if(directions[RIGHT_UPPER]&&directions[LEFT_DOWN]) // ○●○(空)○,○(空)○●○
        {
            if((x+3<BOARD_SQUARE && y-3>=0 && board[x+3][y-3]==1) || 
                (x-3>=0 && y+3<BOARD_SQUARE && board[x-3][y+3]==1)) return 1;
        } else if(directions[LEFT_DOWN]) { // ○○●(空)○,○(空)○○●
            if((x+2<BOARD_SQUARE && y-2>=0 && board[x+2][y-2]==1) || 
                (x-4>=0 && y+4<BOARD_SQUARE && board[x-4][y+4]==1)) return 1;
        } else { // ●○○(空)○,○(空)●○○
            if((x+4<BOARD_SQUARE && y-4>=0 && board[x+4][y-4]==1) ||
                (x-2>=0 && y+2<BOARD_SQUARE && board[x-2][y+2]==1)) return 1;
        }
    }
    return 0;
}