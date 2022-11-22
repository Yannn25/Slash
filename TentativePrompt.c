#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_CHAR_PROMPT 120





int main (int argc, char **argv) {

    int val_ret = 0;
    char *pwd = malloc (MAX_CHAR_PROMPT);

    if ( getcwd( pwd, MAX_CHAR_PROMPT ) == NULL ) {
        printf( "Cannot get current working directory path\n" );
        if ( errno == ERANGE ) {
            //A COMPLETER 
            printf("trop long...\n");
        }
        exit( EXIT_FAILURE );
    }


    printf("\033[32m[%d]\033[00m %s\033[36m$ \n",  val_ret, pwd);
    return 0;
}
