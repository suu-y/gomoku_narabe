#include <stdio.h>
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
    int rslt = 1;
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
                cnt += countWidth(j,i);
                // 3個の場合，守りに徹するので結果を返す
                if(cnt==3)
                {
                    // 置く場所を決める
                    /* !!!!!未実装!!!!! */
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
                cnt += countVertical(j,i);
                // 3個の場合，守りに徹するので結果を返す
                if(cnt==3)
                {
                    // 置く場所を決める
                    /* !!!!!未実装!!!!! */
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

            if((flag&0b0100)==0b0000) countDiagonallyLowerRight(j,i);
            if((flag&0b1000)==0b0000) countDiagonallyLowerLeft(j,i);

            }
        }
    }
}

/* 横に自分を含めて何個並ぶか確認 */
int countWidth(int x, int y)
{

}

/* 縦に自分を含めて何個並ぶか確認 */
int countVertical(int x, int y)
{

}

/* 右斜め下に自分を含めて何個並ぶか確認 */
int countDiagonallyLowerRight(int x, int y)
{

}

/* 左斜め下に自分を含めて何個並ぶか確認*/
int countDiagonallyLowerLeft(int x, int y)
{
    
}