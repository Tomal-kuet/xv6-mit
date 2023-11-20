#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  int p1[2];
  int p2[2]; 
  char buf;

  pipe(p1);
  pipe(p2);
  int processId = fork();
  
  if (processId < 0)
	  fprintf(1, "Fork Error\n");

  else if (processId == 0)
  {
	read(p1[0], &buf, 1);
	int pid = getpid();
	fprintf(1, "%d: received ping\n", pid, buf);
	write(p2[1], "B", 2);
	close(p1[0]);
	close(p1[1]);
	close(p2[0]);
	close(p2[1]);
  }
  else
  {
	write(p1[1], "A", 2);
	wait(0);
	int pid = getpid();
	read(p2[0], &buf, 1);
	fprintf(1, "%d: received pong\n",pid, buf);
	close(p1[0]);
	close(p1[1]);
	close(p2[0]);
	close(p2[1]);
  }
  exit(0);
}
