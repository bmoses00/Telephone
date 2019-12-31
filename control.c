# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/shm.h>
# include <fcntl.h>
# include <string.h>

# define KEY 42
# define SIZE 100

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
  int shmd = shmget(KEY, SIZE, IPC_CREAT | 0600);
  shmctl(shmd, IPC_RMID, 0);
  printf("Removed file\nRemoved shared memory segment\n");
}

void create_content() {
  int shmd = shmget(KEY, SIZE, IPC_CREAT | 0600);
  open("story.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
  printf("File created\nShared memory segment created\n");
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
