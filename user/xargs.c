#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
	char buff[MAXARG];
	char *xargv[MAXARG];
	char *p = buff;
	char *q = buff;
	int i;
	int xargc = argc - 1;

	memset(buff, 0, MAXARG);
	memset(xargv, 0, MAXARG);
	

	for( i = 1; i < argc; i++)
	{
		//fprintf(1, "%d argv %s\n", i + 1, argv[i]);
		xargv[i - 1] = argv[i];
		//fprintf(1, "%d xargv %s\n", i + 1, xargv[i]);
	}

	while(read(0, p, 1) > 0)
	{
		if(*p == '\n')
		{
			*p = '\0';
			//fprintf(1, "%s\n", q);
			xargv[xargc] = q;
			q = p + 1;
			xargc++;
		}
		p++;
	}
	
	/*fprintf(1, "xargc %d\n", xargc);
        for (i = 0; i < xargc; i++)
        {
                fprintf(1, "%d argv %s\n",i, xargv[i]);
        }*/
	if (fork() == 0)
	{
		exec(xargv[0], xargv);
	}
	else
	{
		wait(0);
	}

	exit(0);
}
