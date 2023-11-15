//http://blog.livedoor.jp/akf0/archives/51585502.html

#include <stdio.h>

#include <winsock2.h>



int main(void) {	

	//接続するサーバの情報の構造体を用意

	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(dest));

	//サーバの情報を入力

	char destination[] = "172.28.15.10";  //アドレス

	dest.sin_port = htons(11000);  //ポート番号

	dest.sin_family = AF_INET;

	dest.sin_addr.s_addr = inet_addr(destination);



	//ソケット通信の準備・生成

	WSADATA data;

	WSAStartup(MAKEWORD(2, 0), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



	//サーバへの接続

	if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%sに接続できませんでした\n", destination);

		return -1;

	}

	printf("%sに接続しました\n", destination);


	char msg[] = "Hello Server!\n";
	char buffer[1024];



	//サーバにデータを送信

	send(s, msg, strlen(msg), 0);



	//サーバからデータを受信

	recv(s, buffer, 1024, 0);

	printf("→ %s\n\n", buffer);



	// Windows でのソケットの終了

	closesocket(s);

	WSACleanup();



	return 0;

}
