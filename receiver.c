#include "sender.h"


void receive(message_t* message_ptr, mailbox_t* mailbox_ptr){
    /*  TODO: 
        1. Use flag to determine the communication method
        2. According to the communication method, receive the message
    */
    int fd = shm_open(NAME, O_RDONLY, 0666);
    if (fd < 0) {
        perror("shm_open()");
        exit(1);
    }
    char* string = (char *)mmap(0, mailbox_ptr->size, PROT_READ, MAP_SHARED, fd, 0);
    strncpy(message_ptr->buffer, string, mailbox_ptr->size);
    munmap(string, mailbox_ptr->size);
    close(fd);
    shm_unlink(NAME);
}

int main(int argc, char *argv[]){
    /*  TODO: 
        1) Call receive(&message, &mailbox) according to the flow in slide 4
        2) Measure the total receiving time
        3) Get the mechanism from command line arguments
            • e.g. ./receiver 1
        4) Print information on the console according to the output format
        5) If the exit message is received, print the total receiving time and terminate the receiver.c
    */
    if (argc < 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return 1;
    }
    mailbox_t mailbox;
    mailbox.flag = atoi(argv[1]);
    message_t message;
    struct timespec start, end;
    double time_taken;
    double total_time = 0;
    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        receive(&message,&mailbox);
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("Receiving message: %s \n", message.buffer);
        total_time += ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9);
    }
    printf("Total time taken in receiving msg: %f s\n", total_time);
    return 0;
}