#include<unistd.h>
#include<stdio.h>

#include<sys/types.h>
#include<sys/wait.h>


int main(int argc, char* argv[])
{
	int pipes[3][2];
	int ret;
	pid_t pid[7];
	int status;
	
	if (argc < 7)
	{
		printf("arguments error");
		return 1;
	}
	
	ret = pipe(pipes[0]);
	if (ret == -1)
	{
		printf("pipe err");
		return 1;
	}
	ret = pipe(pipes[1]);
	if (ret == -1)
	{
		printf("pipe err");
		return 1;
	}
	
	
	pid[0] = fork();
	
	if (pid[0] == 0)
	{
		close(pipes[0][0]);
		close(pipes[1][1]);
		close(pipes[1][0]);
		dup2(1, pipes[0][1]);
		execlp(argv[1], argv[1], NULL);
		printf("error");
		return 1;
	}
	
	pid[1] = fork();
	
	if (pid[1] == 0)
	{
		close(pipes[0][1]);
		close(pipes[1][0]);
		dup2(0, pipes[0][0]);
		dup2(1, pipes[1][1]);
		execlp(argv[2], argv[2], NULL);
		printf("error");
		return 1;
	}
	
	pid[2] = fork();
	
	if (pid[2] == 0){
		close(pipes[0][1]);
		close(pipes[0][0]);
		close(pipes[1][1]);
		dup2(0, pipes[1][0]);
		execlp(argv[3], argv[3], NULL);
		printf("error");
		return 1;
	}
	
	close(pipes[0][1]);
	close(pipes[0][0]);
	close(pipes[1][1]);
	close(pipes[1][0]);
	
	waitpid(pid[0], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling first program");
		return 1;
	} 
	waitpid(pid[1], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling second program");
		return 1;
	} 
	waitpid(pid[2], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling third program");
		return 1;
	} 
	
	
	pid[3] = fork();
	
	if (pid[3] == 0){
		execlp(argv[4], argv[4], NULL);
		printf("error");
		return 1;
	}
	
	waitpid(pid[3], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling forth program");
		return 1;
	}
	
	ret = pipe(pipes[2]);
	if (ret == -1)
	{
		printf("pipe err");
		return 1;
	}
	pid[4] = fork();
	
	if (pid[4] == 0){
		close(pipes[2][0]);
		dup2(1, pipes[2][1]);
		execlp(argv[5], argv[5], "root", NULL);
		printf("error");
		return 1;
	}
	
	pid[5] = fork();
	
	if (pid[5] == 0){
		close(pipes[2][1]);
		dup2(0, pipes[2][0]);
		execlp(argv[6], argv[6], NULL);
		printf("error");
		return 1;
	}
	close(pipes[2][0]);
	close(pipes[2][1]);
	
	waitpid(pid[4], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling fifth program");
		return 1;
	}
	
	waitpid(pid[5], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error calling sixth program");
		return 1;
	}
	
	
	
	
	return 0;
}	
	
