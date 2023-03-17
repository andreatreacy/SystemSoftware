
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>

void lock_directories() {
    //printf("lock directory functionality should go here. fork/chmod will be used here to change permissions");

    char* directory_path = "shareDirectory";
    int result = chmod(directory_path, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if(result != 0) 
    {
        printf("Error: Failed to lock directory.\n");
    }  
}

