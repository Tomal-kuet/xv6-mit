#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
	char *buff[MAXARG];
	int i, mask = 0;

	memset(buff, 0, MAXARG);

	if(argc > 1)
		mask = atoi(argv[1]);

	if (trace(mask) != 1)
	{
		fprintf(2, "Trace failed \n");
		exit(1);

	}
	for( i = 2; i < argc; i++)
	{
		buff[i - 2] = argv[i];
	}

	exec(buff[0], buff);

	exit(0);
}
