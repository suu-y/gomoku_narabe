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

  SOCKET s, s1;         //繧ｽ繧ｱ繝�繝�
  int result;          //謌ｻ繧雁､

  //謗･邯壹ｒ險ｱ蜿ｯ縺吶ｋ繧ｯ繝ｩ繧､繧｢繝ｳ繝育ｫｯ譛ｫ縺ｮ諠�蝣ｱ

  struct sockaddr_in source;
  char name1[1024], name2[1024];

  memset(name1, '\0', sizeof(name1));

  //騾∽ｿ｡蜈�縺ｮ遶ｯ譛ｫ諠�蝣ｱ繧堤匳骭ｲ縺吶ｋ

  memset(&source, 0, sizeof(source));
  source.sin_family = AF_INET;

  //繝昴�ｼ繝育分蜿ｷ縺ｯ繧ｯ繝ｩ繧､繧｢繝ｳ繝医�励Ο繧ｰ繝ｩ繝�縺ｨ蜈ｱ騾�

  source.sin_port = htons(12345);
  source.sin_addr.s_addr = htonl(INADDR_ANY);

  //繧ｽ繧ｱ繝�繝磯壻ｿ｡縺ｮ髢句ｧ区ｺ門ｙ
  WSADATA data;
  result = WSAStartup(MAKEWORD(2, 0), &data);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("繧ｽ繧ｱ繝�繝磯壻ｿ｡貅門ｙ繧ｨ繝ｩ繝ｼ\n");
  }



  //繧ｽ繧ｱ繝�繝医�ｮ逕滓��
  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s < 0){
    printf("%d\n", GetLastError());
    printf("繧ｽ繧ｱ繝�繝育函謌舌お繝ｩ繝ｼ\n");
  }

  //繧ｽ繧ｱ繝�繝医�ｮ繝舌う繝ｳ繝�
  result = bind(s, (struct sockaddr *)&source, sizeof(source));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("繝舌う繝ｳ繝峨お繝ｩ繝ｼ\n");
  }

  //謗･邯壹�ｮ險ｱ蜿ｯ
  result = listen(s, 1);
  if (result < 0){
    printf("謗･邯夊ｨｱ蜿ｯ繧ｨ繝ｩ繝ｼ\n");
  }


  printf("wait for the first client ... (port 12345)\n");



  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医°繧蛾壻ｿ｡縺後≠繧九∪縺ｧ蠕�讖�
  s1 = accept(s, NULL, NULL);
  if (s1 < 0){
    printf("蠕�讖溘お繝ｩ繝ｼ\n");
  }

  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医∈繝�繝ｼ繧ｿ繧帝∽ｿ｡縺吶ｋ
  result = send(s1, "player1 name = ", 20, 0);

  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医°繧蛾∽ｿ｡縺輔ｌ縺溘ョ繝ｼ繧ｿ縺ｮ蜿嶺ｿ｡
  result = recv(s1, name1, 20, 0);
  if (result < 0){
    printf("蜿嶺ｿ｡繧ｨ繝ｩ繝ｼ\n");
  }

  printf("%s繧貞女菫｡縺励∪縺励◆\n", name1);



  SOCKET s2, s3;         //繧ｽ繧ｱ繝�繝�

  //謗･邯壹ｒ險ｱ蜿ｯ縺吶ｋ繧ｯ繝ｩ繧､繧｢繝ｳ繝育ｫｯ譛ｫ縺ｮ諠�蝣ｱ

  struct sockaddr_in source2;

  memset(name2, '\0', sizeof(name2));

  //騾∽ｿ｡蜈�縺ｮ遶ｯ譛ｫ諠�蝣ｱ繧堤匳骭ｲ縺吶ｋ

  memset(&source2, 0, sizeof(source2));
  source2.sin_family = AF_INET;

  //繝昴�ｼ繝育分蜿ｷ縺ｯ繧ｯ繝ｩ繧､繧｢繝ｳ繝医�励Ο繧ｰ繝ｩ繝�縺ｨ蜈ｱ騾�

  source2.sin_port = htons(12346);
  source2.sin_addr.s_addr = htonl(INADDR_ANY);

  //繧ｽ繧ｱ繝�繝磯壻ｿ｡縺ｮ髢句ｧ区ｺ門ｙ
  WSADATA data2;
  result = WSAStartup(MAKEWORD(2, 0), &data2);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("繧ｽ繧ｱ繝�繝磯壻ｿ｡貅門ｙ繧ｨ繝ｩ繝ｼ\n");
  }

  //繧ｽ繧ｱ繝�繝医�ｮ逕滓��
  s2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s2 < 0){
    printf("%d\n", GetLastError());
    printf("繧ｽ繧ｱ繝�繝育函謌舌お繝ｩ繝ｼ\n");
  }

  //繧ｽ繧ｱ繝�繝医�ｮ繝舌う繝ｳ繝�
  result = bind(s2, (struct sockaddr *)&source2, sizeof(source2));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("繝舌う繝ｳ繝峨お繝ｩ繝ｼ\n");
  }

  //謗･邯壹�ｮ險ｱ蜿ｯ
  result = listen(s2, 1);
  if (result < 0){
    printf("謗･邯夊ｨｱ蜿ｯ繧ｨ繝ｩ繝ｼ\n");
  }


  printf("wait for the second client ... (port 12346)\n");



  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医°繧蛾壻ｿ｡縺後≠繧九∪縺ｧ蠕�讖�
  s3 = accept(s2, NULL, NULL);
  if (s3 < 0){
    printf("蠕�讖溘お繝ｩ繝ｼ\n");
  }

  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医∈繝�繝ｼ繧ｿ繧帝∽ｿ｡縺吶ｋ
  result = send(s3, "player2 name= ", 20, 0);

  //繧ｯ繝ｩ繧､繧｢繝ｳ繝医°繧蛾∽ｿ｡縺輔ｌ縺溘ョ繝ｼ繧ｿ縺ｮ蜿嶺ｿ｡
  result = recv(s3, name2, 20, 0);
  if (result < 0){
    printf("蜿嶺ｿ｡繧ｨ繝ｩ繝ｼ\n");
  }

  printf("%s繧貞女菫｡縺励∪縺励◆\n", name2);


  char buffer[1024] = "start";  //蜿嶺ｿ｡繝�繝ｼ繧ｿ縺ｮ繝舌ャ繝輔ぃ鬆伜沺

  while(1){

    result = send(s1, buffer, strlen(buffer), 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));

    result = recv(s1, buffer, sizeof(buffer), 0);

    if (result < 0){
      printf("蜿嶺ｿ｡繧ｨ繝ｩ繝ｼ\n");
      break;
    }

    printf("%s繧貞女菫｡縺励∪縺励◆(%s)\n", buffer, name1);


    char buffer1[1024];
    strcpy(buffer1,buffer);
    if(!displayBoard(buffer1,1)){
      printf("繧ｲ繝ｼ繝�繧ｻ繝�繝�\n");
      break;
    }

    result = send(s3, buffer, strlen(buffer), 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));
    result = recv(s3, buffer, sizeof(buffer), 0);

    if (result < 0){
      printf("蜿嶺ｿ｡繧ｨ繝ｩ繝ｼ\n");
      break;
    }
    

    printf("%s繧貞女菫｡縺励∪縺励◆(%s)\n", buffer, name2);

    char buffer2[256];
    strcpy(buffer2,buffer);

    if(!displayBoard(buffer2,2)){
      printf("繧ｲ繝ｼ繝�繧ｻ繝�繝�\n");
      break;
    }

  }

  result = send(s1, "end", 10, 0);
  result = send(s3, "end", 10, 0);

  printf("謗･邯夂ｵゆｺ�\n");
  closesocket(s1);
  closesocket(s3);


  //繧ｽ繧ｱ繝�繝磯壻ｿ｡縺ｮ邨ゆｺ�
  WSACleanup();

  printf("END\n");


  return 0;

}