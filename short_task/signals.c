#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo fdsi;
    ssize_t s;
    int i = 0;

    sigfillset(&mask);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    {
        perror("sigpromask");
        return 1;
    }

    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1)
    {
        perror("signalfd");
        return 1;
    }

    for (i; i < 10; ++i)
    {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo))
        {
            perror("read");
            return 2;
        }

        printf("%d\n", fdsi.ssi_pid);
    }
    return 0;
}

