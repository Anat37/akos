#include<unistd.h>
#include<poll.h>
#include<stdio.h>

int main()
{
  int poll_ret;
  int flag = 1;
  struct pollfd polls[2];
  size_t size;
  int type;
  int msg_ret = 0;
  void* buf = NULL;
  polls[0].fd = STDIN_FILENO;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
  while (flag)
  { 
    printf("poll\n");
    poll_ret = poll(polls, 1, -1);
    if (polls[0].revents & POLLNVAL){
      printf("nval\n");
    }
    polls[0].revents = 0;
  }
  return 0;
}