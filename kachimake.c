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

/* ã²ã¼ã?ã®åã¡è²?ããå¤å®ãã?
 * int board[BOARD_SQUARE][BOARD_SQUARE] : æãåºãåã®ç¤ã®ç¶æ? 
 * int x : åºããæã?®xåº§æ¨?
 * int y : åºããæã?®yåº§æ¨? (ä»å?ºããæã?¯board[x-1][y-1]ã®ä½ç½®ã«ãªãã£ã½ã?) 
 * ãã?®æã§åã¦ããªã?1ãï¼åã¦ãªã?ãªã?0ãè¿ã
 */
int judgeWin(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    // ç¸¦ã«ä¸¦ã¶ãå¤å®ãã?
    if(judgeVertical(board, x, y)) return 1;
    // æ¨ªã«ä¸¦ã¶ãå¤å®ãã?
    if(judgeWidth(board, x, y)) return 1;
    // æãã«ä¸¦ã¶ãå¤å®ãã?
    if(judgeDiagonal(board, x, y)) return 1;
    return 0;
}

/* ç¸¦ã«5ã¤ä¸¦ã¶ãå¤å®ãã?
 * board : ç¤ã®ç¶æ³?
 * x : åºããæã?®xåº§æ¨?
 * y : åºããæã?®yåº§æ¨?
 * ç¸¦ã«5ã¤ä¸¦ãã§ã?ãã1ãï¼ä¸¦ãã§ã?ãªãã£ãã0ãè¿ã
 */
int judgeVertical(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // ä¸¦ãã§ã?ãæ°ãã«ã¦ã³ããã?
    int i, x2 = x-1, y2 = y-2;
    // x-1, y-1ããä¸æ¹åï¼ä¸æ¹åã?®é ?çªã«ä½åä¸¦ã¶ãç¢ºèªãã¦ã?ã?
    // ä¸æ¹åã«ä½åä¸¦ã¶ãç¢ºèªãæå¤§4å?([x-1][y-1]é¨å?ãé¤ãã?®ã§)
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    for(i=0; i<4;i++)
    {
        if(board[x2][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // ä¸æ¹åã«ä½åä¸¦ã¶ãç¢ºèªãæå¤§4å?([x-1][y-1]é¨å?ãé¤ãã?®ã§)
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* æ¨ªã«5ã¤ä¸¦ã¶ãå¤å®ãã?
 * board : ç¤ã®ç¶æ³?
 * x : åºããæã?®xåº§æ¨?
 * y : åºããæã?®yåº§æ¨?
 * æ¨ªã«5ã¤ä¸¦ãã§ã?ãã1ãï¼ä¸¦ãã§ã?ãªãã£ãã0ãè¿ã
 */
int judgeWidth(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // ä¸¦ãã§ã?ãæ°ãã«ã¦ã³ããã?
    int i, x2 = x-2, y2 = y-1;
    // x-1, y-1ããå·¦æ¹åï¼å³æ¹åã?®é ?çªã«ä½åä¸¦ã¶ãç¢ºèªãã¦ã?ã?
    // å·¦æ¹åã«ä½åä¸¦ã¶ãç¢ºèªãæå¤§4å?([x-1][y-1]é¨å?ãé¤ãã?®ã§)
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // å³æ¹åã«ä½åä¸¦ã¶ãç¢ºèªãæå¤§4å?([x-1][y-1]é¨å?ãé¤ãã?®ã§)
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    x2 = x;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    return 0;
}

/* æãã«5ã¤ä¸¦ã¶ãå¤å®ãã?
 * board : ç¤ã®ç¶æ³?
 * x : åºããæã?®xåº§æ¨?
 * y : åºããæã?®yåº§æ¨?
 * æãã«5ã¤ä¸¦ãã§ã?ãã1ãï¼ä¸¦ãã§ã?ãªãã£ãã0ãè¿ã
 */
int judgeDiagonal(int board[BOARD_SQUARE][BOARD_SQUARE], int x, int y)
{
    int cnt = 1; // ä¸¦ãã§ã?ãæ°ãã«ã¦ã³ããã?
    // x-1, y-1ããå³æã?¼å·¦æãã®é ?çªã«ä½åä¸¦ã¶ãç¢ºèªãã¦ã?ã?
    // å·¦ä¸ã«ä½åä¸¦ã¶ãç¢ºèªãæå¤§4å?([x-1][y-1]é¨å?ãé¤ãã?®ã§)
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    int i, x2 = x-2, y2 = y;
    for(i=0; i<4;i++)
    {
        if(board[x2--][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    // å³ä¸ã«ä½åä¸¦ã¶ãç¢ºèªã?
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    x2 = x; y2 = y-2;
    for(i=0; i<4;i++)
    {
        if(board[x2++][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    // å³ä¸ã«ä½åä¸¦ã¶ãç¢ºèªã?
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    cnt = 1; x2 = x; y2 = y;
    for(i=0;i<4;i++)
    {
        if(board[x2++][y2++]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;
    
    // å·¦ä¸ã«ä½åä¸¦ã¶ãç¢ºèªã?
    // ä»®ã«boardå?ã«ä½ããªã?ã¨ããã¯0?¼è?ªå?ãç½®ã?ãã¨ããã¯1?¼ç¸æãç½®ã?ãã¨ããã¯2ã§æ ¼ç´ããã¦ã?ãã¨ãã
    x2 = x-2; y2 = y-2;
    for(i=0;i<4;i++)
    {
        if(board[x2--][y2--]==1) cnt++;
        else break;
    }
    if(cnt==5) return 1;

    return 0;
}