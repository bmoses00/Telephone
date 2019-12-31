# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/shm.h>

# define KEY 42
# define SIZE 100

int main() {
  int shmd = shmget(KEY, SIZE, 0);
  int * length = shmat(shmd, 0, 0);

  int fd = open("story.txt", O_RDWR);

  printf("Last entry:\n");
  lseek(fd, -*length, SEEK_END);
  char buffer[100];
  int i;
  for (i = 0; i < 100; i++) {
    buffer[i] = 0;
  }
  // could potenetially replace above line with nulling out byte after (length)
  // also: use semaphore!
  read(fd, buffer, *length);
  printf("%s\n", buffer);
  printf("Your addition: \n");
  fgets(buffer, 100, stdin);
  *length = strchr(buffer, '\n') + 1 - buffer;

  lseek(fd, 0, SEEK_END);
  write(fd, buffer, *length);
}
