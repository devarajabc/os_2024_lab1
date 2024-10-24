#include "sender.h"

void send(message_t message, mailbox_t* mailbox_ptr){
    /*  TODO: 
        1. Use flag to determine the communication method
        2. According to the communication method, send the message
    */
   if (mailbox_ptr->flag == 2){// share memory
        //char *string = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mailbox_ptr->fd, 0);// each line
        strncpy(mailbox_ptr->ptr, message.buffer,SIZE);
        printf("Sending message: %s\n", mailbox_ptr->ptr);
        //munmap(string, SIZE);
   }
}

int main(int argc, char *argv[]){
// Read command line
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
    double total_time = 0;
    if (mailbox.flag == 2){
        int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);//creat a shared memory object and return a file descriptor fd.
        if (fd < 0)
           exit(1);
        ftruncate(fd, SIZE);// set the shared memory object size.
        message_t message;
        mailbox.ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);// each line
        struct timespec start, end;
        while (fgets(message.buffer, sizeof(message.buffer), file)) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            send(message,&mailbox);
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time += ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9);
        }
        if (feof(file)) {
            printf("End of input file! exit!\n");
        }
        close(fd);
        shm_unlink(NAME);
    }
    fclose(file); 
    printf("Total time taken in sending msg: %f s\n", total_time);
    return 0;
}