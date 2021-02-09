#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dirName, char *fileName)
{
	int fd;
	struct stat st;
	struct dirent de;
	char buf[512], *p;

	if((fd = open(dirName, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", dirName);
		close(fd);
		return;
	}
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", dirName);
		close(fd);
		return;
	}
    
	if (st.type != T_DIR)
	{
		fprintf(2, "find: %s is not a directory\n", dirName);
		close(fd);
		return;
	}
	
	if (strlen(dirName) + 1 + DIRSIZ + 1 > sizeof buf)
	{
		fprintf(2, "find: directory too long\n");
		close(fd);
		return;
	}	

	strcpy(buf, dirName);
	p = buf + strlen(buf);
	*p = '/';
	p ++;
	
	while(read(fd, &de, sizeof(de)) == sizeof(de))
	{
		if (de.inum == 0)
			continue;
		if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
			continue;
		
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if (stat(buf, &st) < 0)
		{
			fprintf(2, "find: cannnot stat %s\n", buf);
			continue;
		}
		
		if (st.type == T_DIR)
		{
			find(buf, fileName);
		}
		else if (st.type == T_FILE && !strcmp(de.name, fileName))
		{
			printf("%s\n", buf);
		}
	}

}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(2, "usage: find dirName fileName\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
