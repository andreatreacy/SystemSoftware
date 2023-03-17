
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>

void unlock_directories() {
    //printf("unlock directory functionality should go here. fork/chmod will be used here to change permissions");
    
    char* directory_path = "shareDirectory";
    int result = chmod(directory_path, S_IRWXU | S_IRWXG | S_IRWXO);
    if(result != 0) {
        printf("Error: Failed to unlock directory.\n");
    }
    else
    {
        printf("Unlocked directory.\n");
    }
}


        