# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/shm.h>
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

int main() {
  int shmd = shmget(SHMKEY, SIZE, 0);
  int * length = shmat(shmd, 0, 0);

  int semd = semget(SEMKEY, 1, 0600);
  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_flg = SEM_UNDO;
  semaphore.sem_op = -1;
  semop(semd, &semaphore, 1);

  int fd = open("story.txt", O_RDWR);

  printf("Last entry:\n");
  lseek(fd, -*length, SEEK_END);
  char buffer[100];
  int i;
  for (i = 0; i < 100; i++) {
    buffer[i] = 0;
  }
  read(fd, buffer, *length);
  printf("%s\n", buffer);
  printf("Your addition: \n");
  fgets(buffer, 100, stdin);
  *length = strchr(buffer, '\n') + 1 - buffer;

  lseek(fd, 0, SEEK_END);
  write(fd, buffer, *length);
}
