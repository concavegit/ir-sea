#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  struct sockaddr_in serv;
  int fd, conn;
  char message[100] = "";

  serv.sin_family = AF_INET;
  serv.sin_port = htons(8096);
  serv.sin_addr.s_addr = INADDR_ANY;
  fd = socket(AF_INET, SOCK_STREAM, 0);
  bind(fd, (struct sockaddr *)&serv, sizeof serv);

  listen(fd, 5);
  while ((conn = accept(fd, (struct sockaddr *)NULL, NULL))) {
    int pid;
    if ((pid = fork()) == 0) {
      while (recv(conn, message, 100, 0) > 0) {
        printf("Message Received: %s\n", message);
        message[0] = 0;
      }
      exit(0);
    }
  }
}
