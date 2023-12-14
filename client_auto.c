/* 自動で手が打てるクライアントプログラム */
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "./judge.h"
#include "./kachimake.h"
#include "./defense.h"

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
    
    /* -----追加部分----- */
    int turn;
    if(port == 12345) turn = 0; // 先攻
    else if(port == 12346) turn = 1; // 後攻
    /* -----ここまで----- */

    // ユーザー名をサーバに送信
    char buffer_name[1024];
    recv(s, buffer_name, 1024, 0);
    printf("%s", buffer_name);
    scanf("%s", user_name);
    send(s, user_name, strlen(user_name), 0);

    /* -----追加部分----- */
    char *token;
    int x, y;
    int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}}; // 何も置かれていない場所を0，自分が置いた位置を1，相手が置いた位置を2
    /* -----ここまで----- */
    
    int isKinzite = 0;
    while (1) {
        memset(&message, '\0', sizeof(message));
        // サーバからデータを受信
        recv(s, message, sizeof(message), 0);
        if(strcmp(message, "end")==0) break;
        else if (strcmp(message, "start") != 0) {
            printf("相手が置いた位置: ");
            /* -----追加部分----- */
            strcpy(str, message);
            token=strtok(str,",");
            x = atoi(token);
            token=strtok(NULL, ",");
            y = atoi(token);
            if(x>0 && y>0) board[x-1][y-1] = 2;

            // 相手の手の禁じ手を確認
            judge_kinzite(x-1, y-1, board,message);
            /* -----ここまで----- */
        }
        printf("%s\n", message);

        // 五目並べの石を置く座標をユーザーに入力
        memset(&str, '\0', sizeof(str));
        printf("どこに置きますか？: ");

        place p;
        do
        {
            if(judgeDefense(board, &p, turn))
            {
                // 攻め : この場合は自分の手がどこにあるかを走査する必要があるため，専用の関数を呼び出す想定
                /* テスト用---左上から走査してとにかく空いてるところに置く-- */
                int i, j,finish=0;
                for(i=0;i<BOARD_SQUARE;i++)
                {
                    for(j=0;j<BOARD_SQUARE;j++)
                    {
                        if(board[j][i]==0)
                        {
                            p.x=j;p.y=i;
                            finish=1;
                            break;
                        }
                    }
                    if(finish) break;
                }
            }
            board[p.x][p.y] = 1;
            // 自分の手の禁じ手を確認 : 禁じ手じゃなければwhile文を抜ける
            if(turn) isKinzite = judge_kinzite(x-1, y-1, board, message);
            if(isKinzite) board[x][y] = 0;
            // 一旦ここで呼び出す形にしているけど、もしかしてこれ各関数の中で呼び出す形にしやんかったら何回でも禁じ手に置くのでは…？
            // 自分が置いたら禁じ手になるところがこの後のターン経過で変わることは無いと思うので、いっそ3とか入れてしまう…？
        } while(isKinzite);
        sprintf(message, "%d,%d", p.x+1, p.y+1);
        printf("%d,%d\n", p.x+1, p.y+1);
        win(board, p.x, p.y, message);
        /* -----ここまで----- */

        // サーバにデータを送信
        send(s, message, strlen(message), 0);


    }

    // Windows でのソケットの終了
    closesocket(s);
    WSACleanup();

    return 0;
}