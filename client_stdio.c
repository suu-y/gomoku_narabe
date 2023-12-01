#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "./judge.h"

int main(void) {	
    // アドレス、ポート番号、送信メッセージを入力
    char destination[20];  // アドレス
    int port;             // ポート番号
    char user_name[100];  // ユーザー名
    char message[100];    // 送信メッセージ

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

    // ユーザー名をサーバに送信
    char buffer_name[1024];
    recv(s, buffer_name, 1024, 0);
    printf("%s", buffer_name);
    scanf("%s", user_name);
    send(s, user_name, strlen(user_name), 0);

    /* -----追加部分----- */
    char *token, *str;
    int x, y;
    int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}}; // 何も置かれていない場所を0，自分が置いた位置を1，相手が置いた位置を2
    /* -----ここまで----- */
    
    while (1) {
        memset(&message, '\0', sizeof(message));
        // サーバからデータを受信
        recv(s, message, sizeof(message), 0);
        if (strcmp(message, "start") != 0) {
            printf("相手が置いた位置: ");
            /* -----追加部分----- */
            strcpy(str, message);
            token=strtok(str,",");
            x = atoi(token);
            token=strtok(NULL, ",");
            y = atoi(token);
            if(x && y) board[x-1][y-1] = 2;

            // 相手の手の禁じ手を確認
            judge_chouren(x-1, y-1, board);
            judge_33(x-1, y-1, board);
            /* -----ここまで----- */
        }
        printf("%s\n", message);


        // 五目並べの石を置く座標をユーザーに入力
        printf("どこに置きますか？: ");
        scanf("%s", message);

        /* -----追加部分----- */
        strcpy(str, message);
        token=strtok(str,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        if(x && y) board[x-1][y-1] = 1;

        // 自分の手の禁じ手を確認
        judge_chouren(x-1, y-1, board);
        /* -----ここまで----- */

        // サーバにデータを送信
        send(s, message, strlen(message), 0);


    }

    // Windows でのソケットの終了
    closesocket(s);
    WSACleanup();

    return 0;
}
