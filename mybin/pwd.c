/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   文件名称：pwd.c
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
int main()
{
    char path[128]={0};
    getcwd(path,127);
    printf("%s\n",path);
    exit(0);
}
