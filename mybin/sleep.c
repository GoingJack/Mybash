/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   文件名称：sleep.c
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

int main(int argc,char *argv[])
{
    int time = 0;
    sscanf(argv[1],"%d",&time);
    sleep(time);
    exit(0);
}
