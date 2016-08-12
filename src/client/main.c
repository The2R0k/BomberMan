#include "network.h"


int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("Incorrect input ! \nExample: %s 192.168.0.2 4444\n", argv[0]);
    exit(-1);
  }

  // /*msg send to server*/
  // struct ClientToServer *sendMsg;
  // sendMsg = (struct ClientToServer *)malloc(CLIENT_MSG_SIZE);
  // /*msg get from server*/
  // struct ClientToServer *getMsg;
  // getMsg = (struct ClientToServer *)malloc(SERVER_MSG_SIZE);
  // // getMsg->field
  //
  // initUDP(argv);
  // /*send message*/
  // registration(sendMsg);
  // /*recv message*/
  // initTCP();
  // getMsg = recvMsg2();
  //
  // printf("id = %d\n", getMsg->id);



  return 0;
}
