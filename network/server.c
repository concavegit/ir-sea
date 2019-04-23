/**
   @file server.c
   @see server.h for function documentation
*/
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t topic_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int uid = 0;
int cli_count = 0;
char topic[BUFFER_SZ / 2];

void queue_add(Client *client) {
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (!clients[i]) {
      clients[i] = client;
      break;
    }

  pthread_mutex_unlock(&clients_mutex);
}

void queue_delete(unsigned int uid) {
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (clients[i])
      if (clients[i]->uid == uid) {
        clients[i] = NULL;
        break;
      }

  pthread_mutex_unlock(&clients_mutex);
}

/* Send message to all clients but the sender */
void send_message(char *s, unsigned int uid) {
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (clients[i])
      if (clients[i]->uid != uid)
        if (write(clients[i]->connfd, s, strlen(s)) < 0) {
          perror("Failed to send message");
          break;
        }

  pthread_mutex_unlock(&clients_mutex);
}

void send_message_all(char *s) {
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (clients[i])
      if (write(clients[i]->connfd, s, strlen(s)) < 0) {
        perror("Failed to send message");
        break;
      }

  pthread_mutex_unlock(&clients_mutex);
}

void send_message_self(char *s, int connfd) {
  if (write(connfd, s, strlen(s)) < 0) {
    perror("Failed to send message");
    exit(-1);
  }
}

void send_message_client(char *s, unsigned int uid) {
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (clients[i])
      if (clients[i]->uid == uid)
        if (write(clients[i]->connfd, s, strlen(s)) < 0) {
          perror("Failed to send message");
          break;
        }

  pthread_mutex_unlock(&clients_mutex);
}

void send_active_clients(int connfd) {
  char s[64];

  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; ++i)
    if (clients[i]) {
      sprintf(s, "<< [%d] %s\r\n", clients[i]->uid, clients[i]->name);
      send_message_self(s, connfd);
    }

  pthread_mutex_unlock(&clients_mutex);
}

void strip_newline(char *s) {
  while (*s != 0) {
    if (*s == '\r' || *s == '\n')
      *s = 0;

    s++;
  }
}

void print_client_addr(struct sockaddr_in6 *addr) {
  char ip6[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &(addr->sin6_addr), ip6, INET6_ADDRSTRLEN);
  printf("%s", ip6);
}

void *handle_client(void *arg) {
  char buff_out[BUFFER_SZ];
  char buff_in[BUFFER_SZ / 2];
  int num_bytes;

  cli_count++;
  Client *cli = (Client *)arg;

  printf("<< accept ");
  print_client_addr(cli->addr);
  printf(" referenced by %d\n", cli->uid);

  sprintf(buff_out, "<< %s has joined\r\n", cli->name);
  send_message_all(buff_out);

  pthread_mutex_lock(&topic_mutex);
  if (strlen(topic)) {
    buff_out[0] = 0;
    sprintf(buff_out, "<< topic: %s\r\n", topic);
    send_message_self(buff_out, cli->connfd);
  }
  pthread_mutex_unlock(&topic_mutex);

  send_message_self("<< see /help for assistance\r\n", cli->connfd);

  /* Receive input from client */
  while ((num_bytes = read(cli->connfd, buff_in, sizeof(buff_in) - 1)) > 0) {
    buff_in[num_bytes] = 0;
    buff_out[0] = 0;
    strip_newline(buff_in);

    /* Ignore empty buffer */
    if (!strlen(buff_in))
      continue;

    /* Special options */
    if (buff_in[0] == '/') {
      char *command, *param;
      command = strtok(buff_in, " ");

      if (!strcmp(command, "/quit"))
        break;

      else if (!strcmp(command, "/ping"))
        send_message_self("<< pong\r\n", cli->connfd);

      else if (!strcmp(command, "/topic")) {
        param = strtok(NULL, " ");

        if (param) {
          pthread_mutex_lock(&topic_mutex);
          topic[0] = 0;

          while (param != NULL) {
            strcat(topic, param);
            strcat(topic, " ");
            param = strtok(NULL, " ");
          }

          pthread_mutex_unlock(&topic_mutex);
          sprintf(buff_out, "<< topic changed to: %s \r\n", topic);
          send_message_all(buff_out);
        } else
          send_message_self("<< message cannot be null\r\n", cli->connfd);
      }

      else if (!strcmp(command, "/nick")) {
        param = strtok(NULL, " ");

        if (param) {
          char *old_name = strdup(cli->name);

          strcpy(cli->name, param);
          sprintf(buff_out, "<< %s is now known as %s\r\n", old_name,
                  cli->name);
          free(old_name);
          send_message_all(buff_out);
        } else
          send_message_self("<< name cannot be null\r\n", cli->connfd);
      }

      else if (!strcmp(command, "/msg")) {
        param = strtok(NULL, " ");

        if (param) {
          unsigned int uid = atoi(param);
          param = strtok(NULL, " ");

          if (param) {
            sprintf(buff_out, "[PM][%s]", cli->name);

            while (param != NULL) {
              strcat(buff_out, " ");
              strcat(buff_out, param);
              param = strtok(NULL, " ");
            }

            strcat(buff_out, "\r\n");
            send_message_client(buff_out, uid);
          } else
            send_message_self("<< message cannot be null\r\n", cli->connfd);

        } else
          send_message_self("<< reference cannot be null\r\n", cli->connfd);
      }

      else if (!strcmp(command, "/list")) {
        sprintf(buff_out, "<< clients %d\r\n", cli_count);
        send_message_self(buff_out, cli->connfd);
        send_active_clients(cli->connfd);
      }

      else if (!strcmp(command, "/help")) {
        strcat(buff_out, "<< /quit     Quit chatroom\r\n");
        strcat(buff_out, "<< /ping     Server test\r\n");
        strcat(buff_out, "<< /topic    <message> Set chat topic\r\n");
        strcat(buff_out, "<< /nick     <name> Change nickname\r\n");
        strcat(buff_out,
               "<< /msg      <reference> <message> Send private message\r\n");
        strcat(buff_out, "<< /list     Show active clients\r\n");
        strcat(buff_out, "<< /help     Show help\r\n");
        send_message_self(buff_out, cli->connfd);
      } else
        send_message_self("<< unknown command\r\n", cli->connfd);
    }

    else {
      snprintf(buff_out, sizeof(buff_out), "[%s] %s\r\n", cli->name, buff_in);
      send_message(buff_out, cli->uid);
    }
  }

  sprintf(buff_out, "<< %s has left\r\n", cli->name);
  send_message_all(buff_out);
  close(cli->connfd);

  queue_delete(cli->uid);
  printf("<< quit ");
  print_client_addr(cli->addr);
  printf(" referenced by %d\r\n", cli->uid);
  free(cli);
  cli_count--;
  pthread_detach(pthread_self());

  return NULL;
}

int main() {
  int listenfd = 0, connfd = 0;
  struct sockaddr_in6 serv_addr;
  struct sockaddr_in6 cli_addr;
  pthread_t tid;

  listenfd = socket(AF_INET6, SOCK_STREAM, 0);
  serv_addr.sin6_family = AF_INET6;
  serv_addr.sin6_addr = in6addr_any;
  serv_addr.sin6_port = htons(PORT);

  signal(SIGPIPE, SIG_IGN);

  if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Socket binding failed");
    return EXIT_FAILURE;
  }

  if (listen(listenfd, 10) < 0) {
    perror("Socket listening failed");
    return EXIT_FAILURE;
  }

  puts("<< server started");

  while (1) {
    socklen_t client_len = sizeof(cli_addr);
    connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &client_len);

    if ((cli_count + 1) == MAX_CLIENTS) {
      puts("<< max clients reached");
      printf("<< reject ");
      print_client_addr(&cli_addr);
      puts("");
      close(connfd);
      continue;
    }

    Client *cli = (Client *)malloc(sizeof(Client));
    cli->addr = &cli_addr;
    cli->connfd = connfd;
    cli->uid = uid++;
    sprintf(cli->name, "%d", cli->uid);

    queue_add(cli);
    pthread_create(&tid, NULL, &handle_client, (void *)cli);

    sleep(1);
  }

  return EXIT_SUCCESS;
}
