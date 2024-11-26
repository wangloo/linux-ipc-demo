#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>

void send_message();


/**
 * client 发送消息，并等待serv回复
 */
int main() {
  send_message();
  return 0;
}

void send_message(void) {
  DBusConnection *conn;
  DBusMessage *msg;
  DBusMessage *reply;
  DBusError err;
  char *response;

  dbus_error_init(&err);

  // 连接到会话总线
  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)){
    fprintf(stderr, "Connection Error: %s\n", err.message);
    dbus_error_free(&err);
  }
  if (!conn) 
    exit(EXIT_FAILURE);

  // 创建一个新的方法调用消息
  msg = dbus_message_new_method_call("com.example.DemoService", // 目标服务名称
                                      "/com/example/DemoService", // 对象路径
                                      "com.example.DemoService", // 接口名称
                                      "SayHello");  // 方法名
  if (!msg) {
    fprintf(stderr, "Message Null\n");
    exit(EXIT_FAILURE);
  }

  // 添加参数到消息
  const char *msg_content = "Hello from client!";
  dbus_message_append_args(msg, DBUS_TYPE_STRING, &msg_content, DBUS_TYPE_INVALID);

  // 发送消息并等待回复
  reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  dbus_message_unref(msg);

  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Reply Error: %s\n", err.message);
    dbus_error_free(&err);
    exit(EXIT_FAILURE);
  }

  // 从回复消息中读取数据
  if (!dbus_message_get_args(reply, &err, DBUS_TYPE_STRING, &response, DBUS_TYPE_INVALID)) {
    fprintf(stderr, "Error getting reply message: %s\n", err.message);
    dbus_error_free(&err);
  } else {
    printf("Client recived: %s\n", response);
  }

  dbus_message_unref(reply);
}
