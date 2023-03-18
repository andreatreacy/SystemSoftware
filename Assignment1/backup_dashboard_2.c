#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


void backup_dashboard(void) {
    //printf("backup code should go here..");

    pid_t pid;
    int status;
    FILE *fptr;

    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) { // child process
        char *args[] = {"find", "dashboardDirectory", "-mindepth", "1", "-exec", "cp", "-t", "backupDirectory", "{}", "+", NULL};
        execvp("find", args);
        perror("execvp");
        exit(1);
    }
    else { // parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("All contents of 'dashboardDirectory' copied to 'backupDirectory'.\n");
            fprintf(fptr, "All contents of 'dashboardDirectory' copied to 'backupDirectory'.\n");
        }
        else {
            printf("Failed to copy contents of 'dashboardDirectory' to 'backupDirectory'.\n");
            fprintf(fptr, "Failed to copy contents of 'dashboardDirectory' to 'backupDirectory'.\n");
        }

        fclose(fptr);
    }
}


int main()
{
    backup_dashboard();
}