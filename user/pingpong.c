#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	char c;
	int n;
	
	// a pair of pipes, one for each direction
	int p_2_c[2];
	int c_2_p[2];
	
	pipe(p_2_c);
	pipe(c_2_p);
	
	if(fork() == 0)
	{
		close(p_2_c[1]);
		close(c_2_p[0]);
		
		n = read(p_2_c[0], &c, 1);
		if (n != 1)
		{
			fprintf(2, "child read error\n");
			exit(1);
		}
		printf("%d: received ping\n", getpid());
		write(c_2_p[1], &c, 1);

		close(p_2_c[0]);
		close(c_2_p[1]);
		exit(0);
	}	
	else
	{
		close(p_2_c[0]);
		close(c_2_p[1]);

		write(p_2_c[1], &c, 1);
		n = read(c_2_p[0], &c, 1);
		if (n != 1)
		{
			fprintf(2, "parent read error\n");
			exit(1);
		}
		printf("%d: received pong\n", getpid());
		
		close(c_2_p[0]);
		close(p_2_c[1]);
		wait(0);
		exit(0);

	}
}
