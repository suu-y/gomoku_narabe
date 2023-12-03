#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>



// display 

#define BOARD_SQUARE 15
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};  

int displayBoard(char *str, int turn){
    

    char *token;
    token=strtok(str,",");

    if(token==NULL){
        printf("error msg=%s\n",str);
        return 0;
    }

    int x = atoi(token);
    
    if( x<1 || x>BOARD_SQUARE) {
        printf("error x=%s\n",x);
        return 0;
    }

    token=strtok(NULL,",");
 
    if(token==NULL){
        printf("error msg=%s\n",str);
        return 0;
    }

   int y = atoi(token);

    if( y<1 || y>BOARD_SQUARE) {
        printf("error y=%s\n",y);
        return 0;
    }

    if(board[x-1][y-1]!=0){
        printf("already put\n");
        return 0;
    }
    board[x-1][y-1] = turn;
    
    int i=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    int j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            
	    if(board[j-1][i-1]==0) printf(" -");
	    if(board[j-1][i-1]==1) printf(" o");
	    if(board[j-1][i-1]==2) printf(" x");

        }
        puts("");
    }
    puts("");

    token=strtok(NULL,",");

    if(token!=NULL){
        printf("recieved judge = %s\n",token);
        return 0;
    }

    Sleep(2000);
    return 1;
    
}


int main(int argc, char* argv[]){

  SOCKET s, s1;         //ソケット
  int result;          //戻り値

  //接続を許可するクライアント端末の情報

  struct sockaddr_in source;
  char name1[1024], name2[1024];

  memset(name1, '\0', sizeof(name1));

  //送信元の端末情報を登録する

  memset(&source, 0, sizeof(source));
  source.sin_family = AF_INET;

  //ポート番号はクライアントプログラムと共通

  source.sin_port = htons(12345);
  source.sin_addr.s_addr = htonl(INADDR_ANY);

  //ソケット通信の開始準備
  WSADATA data;
  result = WSAStartup(MAKEWORD(2, 0), &data);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("ソケット通信準備エラー\n");
  }



  //ソケットの生成
  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s < 0){
    printf("%d\n", GetLastError());
    printf("ソケット生成エラー\n");
  }

  //ソケットのバインド
  result = bind(s, (struct sockaddr *)&source, sizeof(source));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("バインドエラー\n");
  }

  //接続の許可
  result = listen(s, 1);
  if (result < 0){
    printf("接続許可エラー\n");
  }


  printf("wait for the first client ... (port 12345)\n");



  //クライアントから通信があるまで待機
  s1 = accept(s, NULL, NULL);
  if (s1 < 0){
    printf("待機エラー\n");
  }

  //クライアントへデータを送信する
  result = send(s1, "player1 name = ", 20, 0);

  //クライアントから送信されたデータの受信
  result = recv(s1, name1, 20, 0);
  if (result < 0){
    printf("受信エラー\n");
  }

  printf("%sを受信しました\n", name1);



  SOCKET s2, s3;         //ソケット

  //接続を許可するクライアント端末の情報

  struct sockaddr_in source2;

  memset(name2, '\0', sizeof(name2));

  //送信元の端末情報を登録する

  memset(&source2, 0, sizeof(source2));
  source2.sin_family = AF_INET;

  //ポート番号はクライアントプログラムと共通

  source2.sin_port = htons(12346);
  source2.sin_addr.s_addr = htonl(INADDR_ANY);

  //ソケット通信の開始準備
  WSADATA data2;
  result = WSAStartup(MAKEWORD(2, 0), &data2);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("ソケット通信準備エラー\n");
  }

  //ソケットの生成
  s2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s2 < 0){
    printf("%d\n", GetLastError());
    printf("ソケット生成エラー\n");
  }

  //ソケットのバインド
  result = bind(s2, (struct sockaddr *)&source2, sizeof(source2));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("バインドエラー\n");
  }

  //接続の許可
  result = listen(s2, 1);
  if (result < 0){
    printf("接続許可エラー\n");
  }


  printf("wait for the second client ... (port 12346)\n");



  //クライアントから通信があるまで待機
  s3 = accept(s2, NULL, NULL);
  if (s3 < 0){
    printf("待機エラー\n");
  }

  //クライアントへデータを送信する
  result = send(s3, "player2 name= ", 20, 0);

  //クライアントから送信されたデータの受信
  result = recv(s3, name2, 20, 0);
  if (result < 0){
    printf("受信エラー\n");
  }

  printf("%sを受信しました\n", name2);


  char buffer[1024] = "start";  //受信データのバッファ領域

  while(1){

    result = send(s1, buffer, strlen(buffer), 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));

    result = recv(s1, buffer, sizeof(buffer), 0);

    if (result < 0){
      printf("受信エラー\n");
      break;
    }

    printf("%sを受信しました(%s)\n", buffer, name1);


    char buffer1[1024];
    strcpy(buffer1,buffer);
    if(!displayBoard(buffer1,1)){
      printf("ゲームセット\n");
      break;
    }

    result = send(s3, buffer, strlen(buffer), 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));
    result = recv(s3, buffer, sizeof(buffer), 0);

    if (result < 0){
      printf("受信エラー\n");
      break;
    }
    

    printf("%sを受信しました(%s)\n", buffer, name2);

    char buffer2[256];
    strcpy(buffer2,buffer);

    if(!displayBoard(buffer2,2)){
      printf("ゲームセット\n");
      break;
    }

  }

  result = send(s1, "end", 10, 0);
  result = send(s3, "end", 10, 0);

  printf("接続終了\n");
  closesocket(s1);
  closesocket(s3);


  //ソケット通信の終了
  WSACleanup();

  printf("END\n");


  return 0;

}