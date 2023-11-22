#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "kachimake.h"

int main(void) {	
    // �A�h���X�A�|�[�g�ԍ��A���M���b�Z�[�W�����
    char destination[20];  // �A�h���X
    int port;             // �|�[�g�ԍ�
    char user_name[100];  // ���[�U�[��
    char message[100];    // ���M���b�Z�[�W

    printf("�A�h���X����͂��Ă�������: ");
    scanf("%s", destination);

    printf("�|�[�g�ԍ�����͂��Ă�������: ");
    scanf("%d", &port);

    // �ڑ�����T�[�o�̏��̍\���̂�p��
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_port = htons(port);  // �|�[�g�ԍ�
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(destination);

    // �\�P�b�g�ʐM�̏����E����
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    // �T�[�o�ւ̐ڑ�
    if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {
        printf("%s�ɐڑ��ł��܂���ł���\n", destination);
        return -1;
    }

    printf("%s�ɐڑ����܂���\n", destination);

    // ���[�U�[�����T�[�o�ɑ��M
    char buffer_name[1024];
    recv(s, buffer_name, 1024, 0);
    printf("%s", buffer_name);
    scanf("%s", user_name);
    send(s, user_name, strlen(user_name), 0);

    /* -----�ǉ�����----- */
    char *token;
    int x, y;
    int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}}; // �����u����Ă��Ȃ��ꏊ��0�C�������u�����ʒu��1�C���肪�u�����ʒu��2
    /* -----�����܂�----- */
    
    while (1) {
        memset(&message, '\0', sizeof(message));
        // �T�[�o����f�[�^����M
        recv(s, message, sizeof(message), 0);
        if (strcmp(message, "start") != 0) {
            printf("���肪�u�����ʒu: ");
        }
        printf("%s\n", message);

        /* -----�ǉ�����----- */
        token=strtok(message,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        board[x-1][y-1] = 2;
        /* -----�����܂�----- */

        // �ܖڕ��ׂ̐΂�u�����W�����[�U�[�ɓ���
        printf("�ǂ��ɒu���܂����H: ");
        scanf("%s", message);

        /* -----�ǉ�����----- */
        token=strtok(message,",");
        x = atoi(token);
        token=strtok(NULL, ",");
        y = atoi(token);
        board[x-1][y-1] = 1;

        win(board, x, y, message);
        /* -----�����܂�----- */

        // �T�[�o�Ƀf�[�^�𑗐M
        send(s, message, strlen(message), 0);


    }

    // Windows �ł̃\�P�b�g�̏I��
    closesocket(s);
    WSACleanup();

    return 0;
}