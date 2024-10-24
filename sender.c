#include "sender.h"

void send(message_t message, mailbox_t* mailbox_ptr){
    /*  TODO: 
        1. Use flag to determine the communication method
        2. According to the communication method, send the message
    */
   if (mailbox_ptr->flag == 2){// share memory
       printf("Share Memory\n");
       int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);//creat a shared memory object and return a file descriptor fd.
       if (fd < 0)
           exit(1);
        ftruncate(fd, mailbox_ptr->size);// set the shared memory object size. 
        char *string = (char *)mmap(0, mailbox_ptr->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);// each line
        strncpy(string, message.buffer,mailbox_ptr->size);
        printf("Sending message: %s\n", string);
        munmap(string, mailbox_ptr->size);
        close(fd);
        shm_unlink(NAME);
   }
}

int main(int argc, char *argv[]){
    /*  TODO: 
        1) Call send(message, &mailbox) according to the flow in slide 4
        2) Measure the total sending time
        3) Get the mechanism and the input file from command line arguments
            • e.g. ./sender 1 input.txt
                    (1 for Message Passing, 2 for Shared Memory)
        4) Get the messages to be sent from the input file
        5) Print information on the console according to the output format
        6) If the message form the input file is EOF, send an exit message to the receiver.c
        7) Print the total sending time and terminate the sender.c
    */
    if (argc < 3) {
        printf("Usage: %s <integer> <filename>\n", argv[0]);
        return 1;
    }
    mailbox_t mailbox;
    mailbox.flag = atoi(argv[1]);
      const char *filename = argv[2];
    printf("Opening file: %s\n", filename);

    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    message_t message;
    struct timespec start, end;
    double time_taken;
    double total_time = 0;
    while (fgets(message.buffer, sizeof(message.buffer), file)) {
        mailbox.size = strlen(message.buffer)+10;
        clock_gettime(CLOCK_MONOTONIC, &start);
        send(message,&mailbox);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total_time += ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9);
    }
    if (feof(file)) {
        printf("End of input file! exit!\n");
        //If the message form the input file is EOF, send an exit message to the receiver.c

    }
    fclose(file); 
    printf("Total time taken in sending msg: %f s\n", total_time);
    return 0;
}