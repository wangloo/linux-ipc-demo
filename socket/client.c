#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>


#define SOCKET_PATH "/tmp/uds_socket"

int main() {
  int client_id;
  struct sockaddr_un server_addr;
  char buf[128];
  const char *reply_message = "Hello from client!";

  // 创建socket
  if ((client_id = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  
  // 配置套接字地址结构
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, SOCKET_PATH);

  // 连接到服务器
  if (connect(client_id, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  // 接收消息
  if (read(client_id, buf, sizeof(buf)) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  printf("Client recv message: %s\n", buf);
  
  // 发送消息回复给服务器
  if (write(client_id, reply_message, strlen(reply_message)+1) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  printf("Client send reply message: %s\n", reply_message);

  // 关闭套接字
  close(client_id);
  return 0;
}
