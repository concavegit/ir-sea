/**
   IRC-like server implementation in C.

   The key struct is the client struct. The state of the server is
   handled in handle_client and main.

   All clients are held in a queue of pointers, and a deletion is
   setting a pointer to NULL.
 */

#pragma once
#include <pthread.h>

#define MAX_CLIENTS 512
#define BUFFER_SZ 2048
#define MAX_NAME_SZ 32
#define PORT 5000

/**
   The structure used to describe a client.
   It comprises of a socket address, a file descriptor, a unique
   identifier, and a name.
 */
typedef struct {
  struct sockaddr_in6 *addr;
  int connfd;
  unsigned int uid;
  char name[MAX_NAME_SZ];
} Client;

/** Keep track of the amount of clients */
extern int cli_count;

/** Keep track of the latest uid */
extern unsigned int uid;

/** Keep track of the topic in use */
extern char topic[];

/** A buffer containing the clients */
Client *clients[MAX_CLIENTS];

/** Mutex used for concurrency with clients */
extern pthread_mutex_t clients_mutex;

/** Lock the topic mutex when changing topics */
extern pthread_mutex_t topic_mutex;

/** Add a client to the queue */
void queue_add(Client *client);

/** Delete a client from the queue */
void queue_delete(unsigned int uid);

/**
   Send a message to all other clients

   @param s the message to send
   @param uid the sender's unique identifier
*/
void send_message(char *s, unsigned int uid);

/**
   Send a message to all clients

   @param s the message to send
 */
void send_message_all(char *s);

/**
   Send a message to the sender.
   This is used for diagnostics, help, and the interface.

   @param s the message to send
   @param connfd the file descriptor of the sender
 */
void send_message_self(char *s, int connfd);

/**
   Send a message to a client.
   This is useful for PMs.

   @param s the message to send
   @param uid the uid of the receiving client
 */
void send_message_client(char *s, unsigned int uid);

/**
   Send a list of all active clients.
   @param connfd the file descriptor to send the list to.
 */
void send_active_clients(int connfd);

/**
   Strip a newline from messages.
   This avoids double newlines which would otherwise result from hitting enter.
 */
void strip_newline(char *s);

/**
   Print the client address.
*/
void print_client_addr(struct sockaddr_in6 *addr);

/**
   Handle inputs to the clients as well as disconnections.
   Type /help for help.
 */
void *handle_client(void *arg);
