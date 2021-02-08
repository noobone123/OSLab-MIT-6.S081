#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


/*eg: filename=a/b/haha.txt => fmt_filename=haha.txt*/
char *fmt_name(char *filename){
	static char buf[DIRSIZ+1];
	char *p;
	
	for(p=filename+strlen(filename); p >= filename && *p != '/'; p--);
	p++;
	memmove(buf, p, strlen(p)+1);
	return buf;
}

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
	
	switch(st.type){
		case T_FILE:
			if (strcmp(fmt_name(dirName), fileName) == 0){
				printf("%s\n",fileName);	
			}
		case T_DIR:
			// dirname + \ + filename + '\0'
			if (strlen(dirName) + 1 + DIRSIZ + 1 > sizeof(buf)) {
				printf("find: path too long\n");
				break;	
			}
			strcpy(buf, dirName);
			p = buf + strlen(buf);
			*p = '/';
			p ++;
			while(read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
					continue;
				memmove(p, de.name, strlen(de.name));
				p[strlen(de.name)] = 0;
				find(buf, fileName);
			}
			break;
	}
	close(fd);
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
