#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include"get_command.c"
#define SHOULD_RUN 1

int main(void){
    printf("                                                   \n\
        ****     ***            ***               \n\
        *****    ***     ****                      \n\
        *** ***  ***  **** **** *** ******        \n\
        ***  ******* ***    *** *** ***           \n\
        ***    *****  ***   *** *** ***           \n\
        ***      ***   ******   *** ***           \n\
                                                  \n\
        \033[1;31mWELCOME TO THE SHELL OF LIGHT\033[m\n");
    init_history();
    while(SHOULD_RUN){
        get_Prompt();
        get_command();
    }
    return 0;
}



