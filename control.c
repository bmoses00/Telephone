# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/shm.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/sem.h>

# define SHMKEY 42
# define SEMKEY 500
# define SIZE 100

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};

void display_story() {
  int fd = open("story.txt", O_RDONLY);
  char buffer[1000];
  int i;
  for (i = 0; i < 100; i++) {
    buffer[i] = 0;
  }
  read(fd, buffer, 1000);
  printf("The story so far:\n%s\n", buffer);
}

void remove_content() {
  if (!fork())
    execlp("rm", "rm", "story.txt", NULL);
  int w;
  wait(&w);
  int shmd = shmget(SHMKEY, SIZE, IPC_CREAT | 0600);
  shmctl(shmd, IPC_RMID, 0);

  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_op = -1;
  int semd = semget(SEMKEY, 1, 0600);
  semop(semd, &semaphore, 1);
  semctl(semd, IPC_RMID, 0);
  printf("Removed file\nRemoved shared memory segment\nRemoved semaphore\n");
}

void create_content() {
  shmget(SHMKEY, SIZE, IPC_CREAT | 0600);
  open("story.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
  int semd = semget(SEMKEY, 1, 0600);
  union semun us;
  us.val = 1;
  semctl(semd, 0, SETVAL, us);
  printf("File created\nShared memory segment created\nSemaphore created\n");
}

int main(int argc, char * argv[]) {
  if (strcmp(argv[1], "-c") == 0)
    create_content();

  if (strcmp(argv[1], "-r") == 0) {
    display_story();
    remove_content();
  }

  if (strcmp(argv[1], "-v") == 0)
    display_story();
}
