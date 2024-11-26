#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void receive_and_reply(DBusConnection *conn);

int main() {
  DBusConnection *conn;
  DBusError err;

  dbus_error_init(&err);

  // 连接到会话总线守护线程
  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Connection Error: %s\n", err.message);
    dbus_error_free(&err);
  }
  if (conn == NULL)
      exit(EXIT_FAILURE);
  
  // 向总线注册了一个服务，名称 com.example.DemoService
  int ret = dbus_bus_request_name(conn, "com.example.DemoService", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Name error: %s\n", err.message);
    dbus_error_free(&err);
  }
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    exit(EXIT_FAILURE);
  }

  // 处理传入的数据
  receive_and_reply(conn);
  return 0;
}

void receive_and_reply(DBusConnection *conn) {
  DBusMessage *msg;
  DBusMessage *reply;
  DBusError err;
  char *received_msg;

  dbus_error_init(&err);

  // 循环等待消息
  while (1) {
    // 阻塞等待总线上的信号，当产生读取或写入请求时阻塞会结束
    dbus_connection_read_write(conn, 0);
    // 返回传入消息队列中接收的第一条信息，并将其从队列中删除
    msg = dbus_connection_pop_message(conn);
    if (!msg)
      continue;

    // 确保消息是一个方法调用
    if (dbus_message_is_method_call(msg, "com.example.DemoService", "SayHello")) {
      if (!dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &received_msg, DBUS_TYPE_INVALID)) {
        fprintf(stderr, "Error getting message: %s\n", err.message);
        dbus_error_free(&err);
        continue;
      }

      printf("Server received: %s\n", received_msg);

      // 创建回复消息
      reply = dbus_message_new_method_return(msg);
      const char *reply_msg = "Hello from server!";
      dbus_message_append_args(reply, DBUS_TYPE_STRING, &reply_msg, DBUS_TYPE_INVALID);

      // 发送回复
      if (!dbus_connection_send(conn, reply, NULL)) {
        fprintf(stderr, "Out of Memory!\n");
        exit(EXIT_FAILURE);
      }

      dbus_connection_flush(conn);
      dbus_message_unref(reply);
    }

    dbus_message_unref(msg);
  }
}

    

