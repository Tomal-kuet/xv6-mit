#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void spwn_child(int read_fd)
{
	int num = 0;

	if (read(read_fd, &num, 4))
		fprintf(1, "prime %d\n", num);
	else
		return;
	
	int p[2];
	int recevied_val = 0;
	pipe(p);

	if(fork() == 0)
	{
		close(p[1]);
		close(read_fd);
		spwn_child(p[0]);
		close(p[0]);
		exit(0);
	}
	else
	{
		close(p[0]);
		while(read(read_fd, &recevied_val, 4))
		{
			if(recevied_val % num != 0)
				write(p[1], &recevied_val, 4);
		}
		
		close(read_fd);
		close(p[1]);
		wait(0);
	}

}

int main(int argc, char *argv[])
{

  int p1[2];

  pipe(p1);
  
  int processId = fork();

  if (processId < 0)
          fprintf(1, "Fork Error\n");

  else if (processId == 0)
  {
	close(p1[1]); 
        spwn_child(p1[0]);
	close(p1[0]);
	exit(0);
  }
  else
  {
       close(p1[0]);
       for (int i = 2; i <= 35; i++)
       {
	       write(p1[1], &i, 4);
       }
       close(p1[1]);
       wait(0);
  }
  exit(0);
}
