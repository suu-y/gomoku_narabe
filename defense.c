#include <stdio.h>
#include <math.h>
#include "./defense.h"

enum {
    LEFT,               // 0
    UPPER,              // 1
    LEFT_UPPER,         // 2
    RIGHT_UPPER,        // 3
};

/* 守りに徹するべきか，攻めに転じるべきかを判定
 * 守りに徹する場合，二度走査するのは面倒なので，ここで打つ手を確定してしまう．
 * board : 盤の状況を把握
 * 返り値0 : 守り
 * 返り値1 : 攻め
 */
int judgeDefense(int board[BOARD_SQUARE][BOARD_SQUARE], place *p)
{
    int flag = 0b0000; // 左、上、左上、右上にコマが存在するかを管理する．
    // 左上から順に走査していき、相手の碁が何個並ぶかを判定
    // 仮に3個並んでる or 飛び三の場合には守るべきだと仮定
    // 例えば，在る座標(x,y)で横並びをn個発見した場合は，次に調べるところは(x+n, y)で良いので，これにより計算量を小さくする
    // これを上記のビットを使って考えると，以下のようになる
    /* 0b0001の時は左にあるので，横方向の判定は不要．
     * 0b0010の時は上にあるので，縦方向の判定は不要．
     * 0b0100の時は左上にあるので，右斜め下方向の判定は不要
     * 0b1000の時は右上にあるので，左斜め下方向の判定は不要
    */
    int i, j;
    // y方向
    for(i=0;i<BOARD_SQUARE;i++)
    {
        // x方向
        for(j=0;j<BOARD_SQUARE;j++)
        {
            // 相手のコマが置かれていた場合のみ調べる
            if(board[j][i]==2)
            {
                int cnt; // 自分を含めて何個連続してコマがあるか調べる．
               // フラグの設定
                int flag = 0b0000; // フラグを初期化
                if(j>0 && board[j-1][i]==2) flag |= 1 << LEFT; // 左に相手のコマ
                if(i>0 && board[j][i-1]==2) flag |= 1 << UPPER; // 上に相手のコマ
                if(j>0 && i>0 && board[j-1][i-1]==2) flag |= 1 << LEFT_UPPER; // 左上に相手のコマ
                if(j<BOARD_SQUARE-1 && i>0 && board[j+1][i-1]==2) flag |= 1 << RIGHT_UPPER; // 右上に相手のコマ
            
                // 左方向にコマがないのでそのまま調べていく
                if((flag&0b0001)==0b0000)
                {
                    cnt = countWidth(board, j,i);
                    // 3個の場合，守りに徹するので結果を返す
                    if(cnt==3)
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
                        if((j-2>=0 && board[j-2][i]==2) || (j+3<BOARD_SQUARE && board[j+3][i]==2))
                        {
                            // 置く場所を決める
                            /* !!!!!未実装!!!!! */
                            return 0;
                        }
                    }
                }

                // 上方向にコマが無いので，そのまま調べる
                if((flag&0b0010)==0b0000)
                {
                    cnt = countVertical(board, j,i);
                    // 3個の場合，守りに徹するので結果を返す
                    if(cnt==3)
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
                        if((i-2>=0 && board[j][i-2]==2) || (i+3<BOARD_SQUARE && board[j][i+3]==2))
                        {
                            // 置く場所を決める
                            /* !!!!!未実装!!!!! */
                            return 0;
                        }
                    }
                }

                // 左上方向に相手のコマがないので，そのまま調べる
                if((flag&0b0100)==0b0000)
                {
                    cnt = countDiagonallyLowerRight(board, j,i);
                    // 3個の場合，守りに徹するので結果を返す
                    if(cnt==3)
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
                        if((j-2>=0 && i-2>=0 && board[j-2][i-2]==2) || (
                            j+3<BOARD_SQUARE && i+3<BOARD_SQUARE && board[j+3][i+3]==2))
                        {
                            // 置く場所を決める
                            /* !!!!!未実装!!!!! */
                            return 0;
                        }
                    }
                }

                // 右上に相手のコマがないので，そのまま調べる
                if((flag&0b1000)==0b0000)
                {
                    cnt = countDiagonallyLowerLeft(board, j,i);
                    // 3個の場合，守りに徹するので結果を返す
                    if(cnt==3)
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
                        if((j+2<BOARD_SQUARE && i-2>=0 && board[j+2][i-2]==2) || (
                            j-3>=0 && i+3<BOARD_SQUARE && board[j-3][i+3]==2))
                        {
                            // 置く場所を決める
                            /* !!!!!未実装!!!!! */
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

/* (右方向)横に自分を含めて相手のコマが何個並ぶか確認 */
int countWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++x<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* (下方向)縦に自分を含めて相手のコマが何個並ぶか確認 */
int countVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 右斜め下に自分を含めて相手のコマが何個並ぶか確認 */
int countDiagonallyLowerRight(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1;
    while(++x<BOARD_SQUARE && ++y<BOARD_SQUARE && board[x][y]==2) cnt++;
    return cnt;
}

/* 左斜め下に自分を含めて何個並ぶか確認*/
int countDiagonallyLowerLeft(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
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
    int dist1=-1, dist2=-1;
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

/* 在る座標(x,y)から中心の座標(BOARD_SQUARE/2, BOARD_SQUARE/2)までの距離を計算．
 * ユークリッド距離を用いると2乗計算，平方計算等が必要になってしまうため，マンハッタン距離で計算を行う
 */
int calculateDistance(int x, int y)
{
    return abs(x-BOARD_SQUARE/2)+abs(y-BOARD_SQUARE/2);
}