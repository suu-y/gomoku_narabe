/* 自動で手が打てるクライアントプログラム */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "./kinzite.h"
#include "./kachimake.h"
#include "./defense.h"
#include "./offense.h"

// 禁じ手呼出を各関数内で行うため，その仕様に合わせてクライアントプログラムを変更
// 1手目，2手目はwhile文の外で打つ

int main(void) {	
    // アドレス、ポート番号、送信メッセージを入力
    char destination[20];  // アドレス
    int port;             // ポート番号
    char user_name[100];  // ユーザー名
    char message[100];    // 送信メッセージ
    char str[100];

    printf("アドレスを入力してください: ");
    scanf("%s", destination);

    printf("ポート番号を入力してください: ");
    scanf("%d", &port);

    // 接続するサーバの情報の構造体を用意
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_port = htons(port);  // ポート番号
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(destination);

    // ソケット通信の準備・生成
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    // サーバへの接続
    if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {
        printf("%sに接続できませんでした\n", destination);
        return -1;
    }

    printf("%sに接続しました\n", destination);
    
    int turn;
    if(port == 12345) turn = 0; // 先攻
    else if(port == 12346) turn = 1; // 後攻

    // ユーザー名をサーバに送信
    char buffer_name[1024];
    recv(s, buffer_name, 1024, 0);
    printf("%s", buffer_name);
    scanf("%s", user_name);
    send(s, user_name, strlen(user_name), 0);

    char *token;
    int x, y;
    int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}}; // 何も置かれていない場所を0，自分が置いた位置を1，相手が置いた位置を2

    kinjite *initk; // 禁じ手の最初の座標を格納
    kinjite *prevk;

    // 1手目，2手目を自動で打つ．真ん中は取れるなら取りたいと思うので，もし相手が取って来なかったら後手でもそっちに打つ
    memset(&message, '\0', sizeof(message));
    recv(s, message, sizeof(message), 0); // 先攻ならstartを，後攻なら相手の手を受信
    if(!turn) // 先攻の場合
    {
        // 真ん中に置く．
        sprintf(message, "8,8");
        printf("どこに置きますか？: 8,8\n");
        board[7][7]=1;
        // サーバにデータを送信
        send(s, message, strlen(message), 0);
    } else { // 後攻の場合
        // 相手の手を受け取る
        printf("相手が置いた位置: ");
        strcpy(str, message);
        token=strtok(str,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        if(x>0 && y>0) board[x-1][y-1] = 2;

        // 相手の手の禁じ手を確認
        judge_kinzite(x-1, y-1, board,message);
        printf("%s\n", message);

        // 初手は中央が空いていればそこへ，空いていなければその回り8個のうちどこかに置く
        if(board[7][7]==0)
        {
            // 真ん中に置く．
            sprintf(message, "8,8");
            printf("どこに置きますか？: 8,8\n");
            board[7][7]=1;
            // サーバにデータを送信
            send(s, message, strlen(message), 0);            
        } else { // 8個のうちランダムにどこかに置く
            x = 7 + ((rand()%3)-1); // rand()%3:0,1,2の乱数→rand()%3-1:-1,0,1の乱数→+7:6,7,8の乱数
            y = 7 + ((rand()%3)-1);
            if(x==7&&y==7) x++; // とりあえずxに1足す

            sprintf(message, "%d,%d", x+1, y+1);
            printf("どこに置きますか？: %d,%d\n", x+1, y+1);
            board[x][y]=1;
            // サーバにデータを送信
            send(s, message, strlen(message), 0);
        }
    }
  
    while (1) {
        memset(&message, '\0', sizeof(message));
        // サーバからデータを受信
        recv(s, message, sizeof(message), 0);
        if(strcmp(message, "end")==0) break;
        printf("相手が置いた位置: ");
        strcpy(str, message);
        token=strtok(str,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        if(x>0 && y>0) board[x-1][y-1] = 2;
        printf("%s\n", message);
        // 相手の手の禁じ手を確認
        if(turn && judge_kinzite(x-1, y-1, board,message)) send(s,message,strlen(message),0);

        // 五目並べの石を置く座標をユーザーに入力
        memset(&str, '\0', sizeof(str));
        printf("どこに置きますか？: ");

        place p;
        if(!turn) // 自分が先攻の時のみ禁じ手判定を行う必要がある
        {
            initk=NULL;
            prevk=NULL;
        }

        do
        {
            memset(&message, '\0', sizeof(message));
            if(judgeDefense(board, &p))
            {
                // 攻め : この場合は自分の手がどこにあるかを走査する必要があるため，専用の関数を呼び出す
                int put_x = -1;
                int put_y = -1;
                offense(&put_x, &put_y, board, turn);

                // 構造体に石を置く座標を格納
                p.x = put_x;
                p.y = put_y;
            }
            board[p.x][p.y] = 1;
            // 自分の手の禁じ手を確認 : 禁じ手じゃなければwhile文を抜ける
            if(!turn && judge_kinzite(p.x,p.y,board,message)) // 先攻かつ禁じ手だった場合
            {
                board[p.x][p.y] = 3;
                // 禁じ手のリストを作成
                kinjite *tmp=malloc(sizeof(kinjite));
                (tmp->p).x = p.x; (tmp->p).y = p.y;
                tmp->next = NULL;
                if(initk==NULL)
                {
                    initk=tmp;
                } else {
                    prevk->next = tmp;
                }
                prevk=tmp;
            } else { // 禁じ手じゃなかったらwhile文を抜ける
                break;
            }
        } while(!turn);  // 先攻の時のみ禁じ手判定を行うためループ

        // 先攻の場合，禁じ手リストに格納された盤面を0に戻す
        if(!turn)
        {
            prevk=initk;
            while(prevk!=NULL)
            {
                board[prevk->p.x][prevk->p.y]=0;
                prevk=prevk->next;
            }
        }

        sprintf(message, "%d,%d", p.x+1, p.y+1);
        printf("%d,%d\n", p.x+1, p.y+1);
        win(board, p.x+1, p.y+1, message);

        // サーバにデータを送信
        send(s, message, strlen(message), 0);

    }

    // Windows でのソケットの終了
    closesocket(s);
    WSACleanup();

    return 0;
}
