#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<string.h>

void sig_hand_parent(int sig)
{
    printf("For parent\n");
}
void sig_hand_child(int sig)
{
    printf("For Child\n");
}

int main()
{
	int pfd[2];
    pipe(pfd);
    int pid=fork();
    if(pid==0)
    {
        signal(SIGUSR2,sig_hand_child);
        close(pfd[1]);
        char buffer[1000];
        while(1)
        {
            read(pfd[0],buffer,1000);
            printf("%s\n",buffer);
            kill(getppid(),SIGUSR1);
            sleep(10);
            if(strcmp(buffer,"10")==0)
                exit(0);
        }
        
    }
    else
    {
        signal(SIGUSR1,sig_hand_parent);
        char temp[10][100]={"1","2","3","4","5","6","7","8","9","10"};
        close(pfd[0]);
        int i=0;
        for(i=0;i<10;i++)
            {
                printf("%d being written\n",i+1);
                write(pfd[1],temp[i],strlen(temp[i])+1);
                kill(pid,SIGUSR2);
                sleep(10);
            }
        close(pfd[1]);
    }
    exit(0);
}
