#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <errno.h>

void lock_directories() {
    //printf("lock directory functionality should go here. fork/chmod will be used here to change permissions"); 
    
    pid_t pid;
    int status;
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return;
    }
    fprintf(fptr, "__________________lock_directories____________________\n");

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) { // child process
        char* directory_path = "shareDirectory";
        int result = chmod(directory_path, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if(result != 0) 
        {
            printf("Error: Failed to lock shareDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. shareDirectory locked'.\n");
        }

        char* directory_path2 = "dashboardDirectory";
        int result2 = chmod(directory_path2, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if(result2 != 0) 
        {
            printf("Error: Failed to lock dashboardDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. dashboardDirectory locked'.\n");
        }

        char* directory_path3 = "backupDirectory";
        int result3 = chmod(directory_path3, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if(result3 != 0) 
        {
            printf("Error: Failed to lock backupDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. backupDirectory locked'.\n");
        }
        exit(1);
    }
    else { // parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Directories are locked.'.\n");
            fprintf(fptr, "Directories are locked.'.\n");
        }
        else {
            printf("ERROR. Failed to lock directories'.\n");
            fprintf(fptr, "ERROR. Failed to lock directories'.\n");
        }
    }
    fprintf(fptr, "___________________________________________\n");
    fclose(fptr);
}

