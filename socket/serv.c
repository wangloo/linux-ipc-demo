#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/uds_socket"

int main() {
  int server_fd, client_fd;
  struct sockaddr_un server_addr;
  char buf[128];
  const char *message = "Hello from server";
  
  // 创建套接字
  if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 配置套接字地址结构
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, SOCKET_PATH);

  // 绑定套接字
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // 监听套接字
  if (listen(server_fd, 5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on %s\n", SOCKET_PATH);


  // 创建进程运行client
  if (fork() == 0) {
    close(server_fd);
    char *args[] = {"", NULL};
    execv("./client", args);

    // 如果execv返回，表示出错
    perror("execv failed");
    exit(EXIT_FAILURE);
  } else {
    // 接受客户端连接，阻塞
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
      perror("accept"); exit(EXIT_FAILURE);
    }

    // 发送消息给client
    if (write(client_fd, message, strlen(message)+1) == -1) {
      perror("write"); exit(EXIT_FAILURE);
    }
    printf("Server: sent message: %s\n", message);

    // 从client接受回复
    if (read(client_fd, buf, sizeof(buf)) == -1) {
      perror("read"); exit(EXIT_FAILURE);
    }
    printf("Serve received: %s\n", buf);

    // 关闭套接字
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH); // 删除套接字文件
  }
  return 0;
}
