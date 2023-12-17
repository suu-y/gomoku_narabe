#include <stdio.h>
#include <math.h>
#include "./defense.h"
#include "./judge.h"

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
    int directions[8] = {1}; // 8方向のコマ数を配列で格納．
    // y方向
    for(i=0;i<BOARD_SQUARE;i++)
    {
        // x方向
        for(j=0;j<BOARD_SQUARE;j++)
        {
            // 何も置かれていないところを調べる
            if(board[j][i]==0)
            {
                int cnt; // 自分を含めて何個連続してコマがあるか調べる．(削除できそう？)
               // フラグの設定
                flag = 0b00000000; // フラグを初期化
                if(j>0 && board[j-1][i]==2)                                 flag |= (1 << LEFT);        // 左に相手のコマ
                if(j<BOARD_SQUARE-1 && board[j+1][i]==2)                    flag |= (1 << RIGHT);       // 右に相手のコマ
                if(i>0 && board[j][i-1]==2)                                 flag |= (1 << UPPER);       // 上に相手のコマ
                if(i<BOARD_SQUARE-1 && board[j][i+1]==2)                    flag |= (1 << DOWN);        // 下に相手のコマ
                if(j>0 && i>0 && board[j-1][i-1]==2)                        flag |= (1 << LEFT_UPPER);  // 左上に相手のコマ
                if(j<BOARD_SQUARE-1 && i<BOARD_SQUARE-1 && board[j+1][i+1]) flag |= (1 << RIGHT_DOWN);  // 右下に相手のコマ
                if(j<BOARD_SQUARE-1 && i>0 && board[j+1][i-1]==2)           flag |= (1 << RIGHT_UPPER); // 右上に相手のコマ
                if(j>0 && i<BOARD_SQUARE-1 && board[j-1][i+1]==2)           flag |= (1 << LEFT_DOWN);   // 左下に相手のコマ
            
                search8directions(board,j,i,flag,directions);
                /* デバッグ用 */
                int m;
                for(m=0;m<8;m++) printf("direction[%d]:%d\n",m,directions[m]);

// ------------------------------未変更--------------------------------
                // 左方向にコマがないのでそのまま調べていく
                if((flag&0b0001)==0b0000)
                {
                    cnt = countWidth(board, j,i);
                    // 4個の場合(3個の後に発生する可能性あり)
                    if(cnt==4 &&
                        ((j-1>=0 && board[j-1][i]==0) || (j+4<BOARD_SQUARE && board[j+4][i]==0)))
                    {
                        defense4ren(board, j, i, p, 0);
                        if(p->x>=0 && p->y>=0) return 0;
                    }
                    // 3個の場合，守りに徹するので結果を返す
                    else if(cnt==3 && 
                        ((j-1>=0 && board[j-1][i]==0) || (j+3<BOARD_SQUARE && board[j+3][i]==0)))
                    {
                        // 置く場所を決める
                        defense3ren(board, j, i, p, 0);
                        return 0;
                    }
                    // 2個の場合，飛び三の場合を考える
                    else if(cnt == 2)
                    {
                        // j, iの位置から右に並んでいる、すなわちboard[j][i]とboard[j+1][i]に在る状態
                        // したがって、飛び三の場合はboard[j-2][i]の時かboard[j+3][i]の時
                        if(j-2>=0 && board[j-2][i]==2 && board[j-1][i]==0) // ○(空)(i,j->)○○
                        {
                            // 間(board[j-1][i]に置く)
                            p->x=j-1; p->y=i;
                            return 0;
                        } else if(j+3<BOARD_SQUARE && board[j+3][i]==2 && board[j+2][i]==0) { // (i,j->)○○(空)○
                            // 間(board[j+2][i]に置く)
                            p->x=j+2; p->y=i;
                            return 0;
                        }
                    }
                }

                // 上方向にコマが無いので，そのまま調べる
                if((flag&0b0010)==0b0000)
                {
                    cnt = countVertical(board, j,i);
                    // 4個の場合(3個の後に発生する可能性あり)
                    if(cnt==4 &&
                        ((i-1>=0 && board[j][i-1]==0) || (i+4<BOARD_SQUARE && board[j][i+4]==0)))
                    {
                        defense4ren(board, j, i, p, 1);
                        if(p->x>=0 && p->y>=0) return 0;
                    }
                    // 3個の場合，守りに徹するので結果を返す
                    else if(cnt==3 && 
                        ((i-1>=0 && board[j][i-1]==0) || (i+3<BOARD_SQUARE && board[j][i+3]==0)))
                    {
                        // 置く場所を決める
                        defense3ren(board, j, i, p, 1);
                        return 0;
                    }
                    // 2個の場合，飛び三の場合を考える
                    else if(cnt == 2)
                    {
                        // j, iの位置から下に並んでいる、すなわちboard[j][i]とboard[j][i+1]に在る状態
                        // したがって、飛び三の場合はboard[j][i-2]の時かboard[j][i+3]の時
                        if(i-2>=0 && board[j][i-2]==2 && board[j][i-1]==0)
                        {
                            // 間(board[j][i-1]に置く)
                            p->x=j; p->y=i-1;
                            return 0;
                        } else if(i+3<BOARD_SQUARE && board[j][i+3]==2 && board[j][i+2]==0) {
                            // 間(board[j][i+2]に置く)
                            p->x=j; p->y=i+2;
                            return 0;
                        }
                    }
                }

                // 左上方向に相手のコマがないので，そのまま調べる
                if((flag&0b0100)==0b0000)
                {
                    cnt = countDiagonallyLowerRight(board, j,i);
                    // 4個の場合(3個の後に発生する可能性あり)
                    if(cnt==4 &&
                        ((j-1>=0 && i-1>=0 && board[j-1][i-1]==0) ||
                            (j+4<BOARD_SQUARE && i+4<BOARD_SQUARE && board[j+4][i+4]==0)))
                    {
                        defense4ren(board, j, i, p, 2);
                        if(p->x>=0 && p->y>=0) return 0;
                    }
                    // 3個の場合，守りに徹するので結果を返す
                    else if(cnt==3 && 
                        ((j-1>=0 && i-1>=0 && board[j-1][i-1]==0) || 
                            (j+3<BOARD_SQUARE && i+3<BOARD_SQUARE && board[j+3][i+3]==0)))
                    {
                        // 置く場所を決める
                        defense3ren(board, j, i, p, 2);
                        return 0;
                    }
                    // 2個の場合，飛び三の場合を考える
                    else if(cnt == 2)
                    {
                        // j, iの位置から右下に並んでいる、すなわちboard[j][i]とboard[j+1][i+1]に在る状態
                        // したがって、飛び三の場合はboard[j-2][i-2]の時かboard[j+3][i+3]の時
                        if(j-2>=0 && i-2>=0 && board[j-2][i-2]==2 && board[j-1][i-1]==0)
                        {
                            // 間(board[j-1][i-1]に置く)
                            p->x=j-1; p->y=i-1;
                            return 0;
                        } else if(j+3<BOARD_SQUARE && i+3<BOARD_SQUARE && 
                                    board[j+3][i+3]==2 && board[j+2][i+2]==0) {
                            // 間(board[j+2][i+2]に置く)
                            p->x=j+2; p->y=i+2;
                            return 0;
                        }
                    }
                }

                // 右上に相手のコマがないので，そのまま調べる
                if((flag&0b1000)==0b0000)
                {
                    cnt = countDiagonallyLowerLeft(board, j,i);
                    // 4個の場合(3個の後に発生する可能性あり)
                    if(cnt==4 &&
                        ((j+1<BOARD_SQUARE && i-1>=0 && board[j+1][i-1]==0) ||
                            (j-4>=0 && i+4<BOARD_SQUARE && board[j-4][i+4]==0)))
                    {
                        defense4ren(board, j, i, p, 3);
                        if(p->x>=0 && p->y>=0) return 0;
                    }
                    // 3個の場合，守りに徹するので結果を返す
                    else if(cnt==3 &&
                        ((j+1<BOARD_SQUARE && i-1>=0 && board[j+1][i-1]==0) ||
                            (j-3>=0 && i+3<BOARD_SQUARE && board[j-3][i+3]==0)))
                    {
                        // 置く場所を決める
                        defense3ren(board, j, i, p, 3);
                        return 0;
                    }
                    // 2個の場合，飛び三の場合を考える
                    else if(cnt == 2)
                    {
                        // j, iの位置から左下に並んでいる、すなわちboard[j][i]とboard[j-1][i+1]に在る状態
                        // したがって、飛び三の場合はboard[j+2][i-2]の時かboard[j-3][i+3]の時
                        if(j+2<BOARD_SQUARE && i-2>=0 && board[j+2][i-2]==2 && board[j+1][i-1]==0)
                        {
                            // 間(board[j+1][i-1]に置く)
                            p->x=j+1; p->y=i-1;
                            return 0;
                        } else if(j-3>=0 && i+3<BOARD_SQUARE && board[j-3][i+3]==2 && board[j-2][i+2]==0) {
                            // 間(board[j-2][i+2]に置く)
                            p->x=j-2; p->y=i+2;
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

/* 左方向に自分を含めて相手のコマが何個並ぶか確認 */
int countLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(--x>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 右方向に自分を含めて相手のコマが何個並ぶか確認 */
int countRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++x<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 上方向に自分を含めて相手のコマが何個並ぶか確認 */
int countUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(--y>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 下方向に自分を含めて相手のコマが何個並ぶか確認 */
int countDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 左斜め上に自分を含めて相手のコマが何個並ぶか確認 */
int countLeftUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(--x>=0 && --y>=0 && board[x][y]==2) cnt++;
    return cnt;
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

/* 右斜め下に自分を含めて相手のコマが何個並ぶか確認 */
int countRightDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++x<BOARD_SQUARE && ++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 右斜め上に自分を含めて何個並ぶか確認*/
int countRightUpper(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++x<BOARD_SQUARE && --y>=0 && board[x][y]==2) cnt++;
    return cnt;
}

/* 左斜め下に自分を含めて何個並ぶか確認*/
int countLeftDown(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(--x>=0 && ++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 3つ連続で並んでいた時に，それを防ぐための関数
 * どの方向に並んでいたとしても，盤は中央から外側に向かって展開していくことが多いと予測できるため，より中央に近い場所に置くようにする
 * x,y  : 相手の並び方を確認した座標の位置(そこから右or下or右下or左下に3つ並んでいる状態)
 * p    : 座標を格納する構造体へのポインタ
 * mode : 右，下，右下，左下どちらの方向に並んでいるかを制御する．
 *        0:右方向，1:下方向，2:右下方向，3:左下方向とする 
 */
void defense3ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode)
{
    int dist1=1000, dist2=1000;
    switch(mode)
    {
        case 0: // 右方向に3つ
            // 今の場所から1つ左●○(<-x,y)○○か，3つ右(x,y->)○○○●に置くことになる
            if(x-1>=0 && board[x-1][y]==0) dist1=calculateDistance(x-1, y);
            if(x+3<BOARD_SQUARE && board[x+3][y]==0) dist2=calculateDistance(x+3, y);
            if(dist1<=dist2) // 1つ左の方が中心に近い
            {
                p->x=x-1; p->y=y;
            } else {
                p->x=x+3; p->y=y;
            }
            break;

        case 1: // 下方向に3つ
            // 今の場所から1つ上●○(<-x,y)○○か，3つ下(x,y->)○○○●に置くことになる
            if(y-1>=0 && board[x][y-1]==0) dist1=calculateDistance(x, y-1);
            if(y+3<BOARD_SQUARE && board[x][y+3]==0) dist2=calculateDistance(x, y+3);
            if(dist1<=dist2) // 1つ上の方が中心に近い
            {
                p->x=x; p->y=y-1;
            } else {
                p->x=x; p->y=y+3;
            }
            break;

        case 2: // 右下に3つ
            // 今の場所から1つ左上●○(<-x,y)○○か，3つ右下(x,y->)○○○●に置くことになる
            if(x-1>=0 && y-1>=0 && board[x-1][y-1]==0) dist1=calculateDistance(x-1, y-1);
            if(x+3<BOARD_SQUARE && y+3<BOARD_SQUARE && board[x+3][y+3]==0) dist2=calculateDistance(x+3, y+3);
            if(dist1<=dist2) // 1つ左上の方が中心に近い
            {
                p->x=x-1; p->y=y-1;
            } else {
                p->x=x+3; p->y=y+3;
            }
            break;

        case 3: // 左下に3つ
            // 今の場所から1つ右上○○○(<-x,y)●か3つ左下●○○(x,y->)○に置くことになる
            if(x+1<BOARD_SQUARE && y-1>=0 && board[x+1][y-1]==0) dist1=calculateDistance(x+1, y-1);
            if(x-3>=0 && y+3<BOARD_SQUARE && board[x-3][y+3]==0) dist2=calculateDistance(x-3, y+3);
            if(dist1<=dist2) // 1つ右上の方が中心に近い
            {
                p->x=x+1; p->y=y-1;
            } else {
                p->x=x-3; p->y=y+3;
            }
            break;
    }
}

/* 4つ連続で並んでいた時に，それを防ぐための関数
 * 3つ並んだ時点で一端は阻止されているので、逆側の端を埋める
 * x,y  : 相手の並び方を確認した座標の位置(そこから右or下or右下or左下に3つ並んでいる状態)
 * p    : 座標を格納する構造体へのポインタ
 * mode : 右，下，右下，左下どちらの方向に並んでいるかを制御する．
 *        0:右方向，1:下方向，2:右下方向，3:左下方向とする 
 */
void defense4ren(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y, place *p, int mode)
{
    switch(mode)
    {
        case 0: // 右方向に4つ
            // 今の場所から1つ左●○(<-x,y)○○○か，4つ右(x,y->)○○○○●に置くことになる。
            // まだコマがおかれていない方に置く
            if(x-1>=0 && board[x-1][y]==0)
            {
                p->x=x-1; p->y=y;
            } else if(x+4<BOARD_SQUARE && board[x+4][y]==0) {
                p->x=x+4; p->y=y;
            }
            break;

        case 1: // 下方向に4つ
            // 今の場所から1つ上●○(<-x,y)○○○か，4つ下(x,y->)○○○○●に置くことになる
            if(y-1>=0 && board[x][y-1]==0)
            {
                p->x=x; p->y=y-1;
            }
            else if(y+4<BOARD_SQUARE && board[x][y+4]==0)
            {
                p->x=x; p->y=y+4;
            }
            break;

        case 2: // 右下に4つ
            // 今の場所から1つ左上●○(<-x,y)○○○か，4つ右下(x,y->)○○○○●に置くことになる
            if(x-1>=0 && y-1>=0 && board[x-1][y-1]==0)
            {
                p->x=x-1; p->y=y-1;
            }
            else if(x+4<BOARD_SQUARE && y+4<BOARD_SQUARE && board[x+4][y+4]==0)
            {
                p->x=x+4; p->y=y+4;
            } else {
                p->x=-1; p->y=-1;
            }
            break;

        case 3: // 左下に4つ
            // 今の場所から1つ右上○○○○(<-x,y)●か4つ左下●○○○(x,y->)○に置くことになる
            if(x+1<BOARD_SQUARE && y-1>=0 && board[x+1][y-1]==0)
            {
                p->x=x+1; p->y=y-1;
            }
            else if(x-4>=0 && y+4<BOARD_SQUARE && board[x-4][y+4]==0)
            {
                p->x=x-4; p->y=y+4;
            } else {
                p->x=-1; p->y=-1;
            }
            break;
    }
}

/* 在る座標(x,y)から中心の座標(BOARD_SQUARE/2, BOARD_SQUARE/2)までの距離を計算．
 * ユークリッド距離を用いると2乗計算，平方計算等が必要になってしまうため，マンハッタン距離で計算を行う
 */

int calculateDistance(int x, int y)
{
    return abs(x-BOARD_SQUARE/2)+abs(y-BOARD_SQUARE/2);
}