#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "./judge.h"
#include "./kachimake.h"

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
        scanf("%s", message);

        /* -----追加部分----- */
        strcpy(str, message);
        token=strtok(str,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        if(x>0 && y>0) board[x-1][y-1] = 1;

        // 自分の手の禁じ手を確認
        judge_kinzite(x-1, y-1, board,message);
        win(board, x, y, message);
        /* -----ここまで----- */

        // サーバにデータを送信
        send(s, message, strlen(message), 0);


    }

    // Windows でのソケットの終了
    closesocket(s);
    WSACleanup();

    return 0;
}