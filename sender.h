#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>
#include <sys/mman.h>
#define NAME "/shemem-1"
#define SIZE 4096
typedef struct {
    int flag;      // 1 for message passing, 2 for shared memory
    union{
        int msqid; //for system V api. You can replace it with struecture for POSIX api
        char* shm_addr;
    }storage;
    int fd;
    char* ptr;//pointer to share memory object
} mailbox_t;


typedef struct {
    /*  TODO: 
        Message structure for wrapper
    */
    char buffer[1024];
} message_t;

void send(message_t message, mailbox_t* mailbox_ptr);