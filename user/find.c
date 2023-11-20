#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *name)
{
	char buf[512];
	int fd;
	struct dirent de;
	struct stat st;
	
	char *p;
	
	memset(buf, '\0', 512);

	if ((fd = open(path, O_RDONLY)) < 0 )
	{
		fprintf(2, "find: can not open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0)
	{
		fprintf(2, "find: cannot state %s\n", path);
		close(fd);
		return;
	}
	
	if(st.type != T_DIR)
	{
		fprintf(2, "find: first argument %s must a directory\n", path);
		return;
	}

	memcpy(buf, path, strlen(path));
	buf[strlen(path)] = '/';
	p = buf + strlen(path) + 1;

	while(read(fd, &de, sizeof(de)) == sizeof(de))
	{
		if(de.inum == 0)
			continue;

		if(strcmp(".", de.name) == 0)
			continue;

		if(strcmp("..", de.name) == 0)
			continue;
		struct stat st2;
		int fd_internal;
		
		memcpy(p, de.name, strlen(de.name));
		memset(p + strlen(de.name), '\0', strlen(de.name));
		
		//fprintf(1, "Debug: %s\n", buf);

		if ((fd_internal = open(buf, O_RDONLY)) < 0)
		{
			fprintf(2, "find: can not open %s\n", buf);
			continue;
		}

		if(fstat(fd_internal, &st2) < 0)
		{
			fprintf(2, "find: can not state %s\n", buf);
			close(fd_internal);
			continue;
		}
		switch(st2.type)
		{
			case T_FILE:
				if (strcmp(de.name, name) == 0)
				{
					fprintf(1, "%s\n", buf);
				}
				break;
			case T_DIR:
				find(buf, name);
				break;
		}
		close(fd_internal);

	}
	close(fd);

}
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(2, "Usage: find needs at least three arguments\n");
		exit(0);
	}

	find(argv[1], argv[2]);
	exit(0);
}
