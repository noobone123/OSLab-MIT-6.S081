#include "kernel/types.h"
#include "user/user.h"

void func(int *input, int num)
{
	if ( num == 1) {
		printf("prime %d\n", *input);
		return;
	}
	int p[2], i;
	int prime = *input;
	int temp;
	printf("prime %d\n", prime);
	pipe(p);
	if(fork() == 0){
		for(i = 0; i < num; i++){
			temp = *(input + i);
			write(p[1], &temp, 4);
		}
		exit(0);
	}
	close(p[1]);
	if(fork() == 0){
		int temp2;
		int cnt = 0;
		while(read(p[0], &temp2, 4) != 0){
			if (temp2 % prime != 0) {
				cnt ++;
				*input = temp2;
				input += 1;
			}
		}
		func(input - cnt, cnt);
		exit(0);
	}
	wait(0);
	wait(0);
}


int main()
{
	int input[34];
	int i = 0;
	for (; i < 34; i++) {
		input[i] = i + 2;
	}
	func(input, 34);
	exit(0);
}
