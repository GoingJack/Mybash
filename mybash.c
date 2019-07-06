/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   文件名称：mybash.c
*   创 建 者：gaojie
*   创建日期：2019年07月06日
*   描    述：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include <pwd.h>
#include <sys/types.h>

#include <sys/utsname.h>
#define MAXOP 128

char OLDPWD[128];
void PrintTag()
{
    struct passwd *pw = getpwuid(getuid());
    assert(pw != NULL);
    char flag = pw->pw_uid==0?'#':'$';
    
    struct utsname hostname;
    uname(&hostname);

    char strnode[128] = {0};
    strcpy(strnode,hostname.nodename);
    char *name = strtok(strnode,".");
    
    char path[128] = {0};
    getcwd(path,127);//get current work real directory
    char *p = path + strlen(path);
    while(*p != '/')
    {
        p--;
    }
    if(strlen(path)!=1)//root directory is special.
        p++;

    if(strcmp(pw->pw_dir,path)==0)
        p = "~";
    
    printf("[%s@%s %s]%c ",pw->pw_name,name,p,flag);
    fflush(stdout);
}
void DealString(char *cmd,char **argv)
{
    assert(cmd != NULL);
    int count = 0;
    char *p = strtok(cmd," ");
//    argv[count++]=p;
    while(p!=NULL)
    {
        argv[count++]=p;
        p = strtok(NULL," ");
    }
}
void MyCd(const char *directory)
{
    if(directory == NULL)
    {
        return;
    }
    if(strcmp(directory,"~")==0)
    {
        struct passwd *pw = getpwuid(getuid());
        directory = pw->pw_dir;
    }
    if(strcmp(directory,"-")==0)
    {
        if(strlen(OLDPWD)==0)
        {
            printf("mybash :cd OLDPWD not set\n");
            return;
        }
        directory = OLDPWD;
    }
    char oldpwd[128]={0};
    getcwd(oldpwd,128-1);
    if(-1 ==chdir(directory) )
    {
        char err[128]="mybash : cd ";
        strcat(err,directory);
        perror(err);
        return;
    }
    strcpy(OLDPWD,oldpwd);
}
//deal zombie process
void Zombie(int sign)
{
    wait(NULL);
}
int main()
{
    signal(SIGCHLD,Zombie);
    while(1)
    {
        PrintTag();
        char cmd[128]={0};
        fgets(cmd,127,stdin);
        cmd[strlen(cmd)-1]=0;

        //is bg process
        int bg = 0;
        if(strstr(cmd,"&")!=NULL)
        {
            bg=1;
        }


        char *argv[MAXOP]={0};
        DealString(cmd,argv);
        if(strncmp(argv[0],"exit",4)==0)
        {
          exit(0);
        }
        if(strncmp(argv[0],"cd",2)==0)
        {
            MyCd(argv[1]);
            continue;
        }

        pid_t n = fork();
        assert(n!=-1);
        if(0==n)
        {
            char path[]="/home/goingjack/workspace/APUE/mybash/mybin/";
            if(strstr(argv[0],"/")!=NULL)
            {
                strcpy(path,argv[0]);
            }
            else
            {
                strcat(path,argv[0]);
            }
            execv(path,argv);//change process here
            printf("%s:Command Not Found\n",argv[0]);
            exit(0);//avoid child process is never stop
        }
        else
        {
            if(!bg)
                pause();
        }

    }
}
