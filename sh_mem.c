#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>


#define SHMOBJ_PATH         "/shmjeshu"

/* message structure for messages in the shared segment */

struct shared_data {
    int var1;
    int var2;
};

void signal_callback_handler(int signum)
{
	int status;
        /**
         * Semaphore unlink: Remove a named semaphore  from the system.
         */
        status =  shm_unlink(SHMOBJ_PATH);


        if ( status < 0 )
        {
                perror("shm_unlink");
        }

   // Terminate program
   exit(signum);
}


int main(int argc, char *argv[]) {
    int shmfd;
    int vol, cur;
    int shared_seg_size = (1 * sizeof(struct shared_data));   /* want shared segment capable of storing 1 message */
    struct shared_data *shared_msg;      /* the shared segment, and head of the messages list */

    signal(SIGINT, signal_callback_handler);

    /* creating the shared memory object    --  shm_open()  */
    shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (shmfd < 0)
    {
        perror("In shm_open()");
        exit(1);
    }

    fprintf(stderr, "Created shared memory object %s\n", SHMOBJ_PATH);

    /* adjusting mapped file size (make room for the whole segment to map)      --  ftruncate() */
    ftruncate(shmfd, shared_seg_size);

    /* requesting the shared segment    --  mmap() */
    shared_msg = (struct shared_data *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shared_msg == NULL)
    {
        perror("In mmap()");
        exit(1);
    }
    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes).\n", shared_seg_size);

    vol = 10;
    cur = 0;
    while(1)
    {
    shared_msg->var1 = vol;
    shared_msg->var2 = cur;
    sleep(3);
    vol++;
    cur++;
    }

signal_callback_handler(SIGINT);

    if (shm_unlink(SHMOBJ_PATH) != 0) {
        perror("In shm_unlink()");
        exit(1);
    }

    return 0;
}
