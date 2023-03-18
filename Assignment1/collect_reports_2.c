#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdlib.h>


void collect_reports(void) {
    //printf("collect reports functionality could go here.. fork/cp will probably be used");
    pid_t pid;
    int status;
    FILE *fptr;

    // Open the file in "append" mode
    fptr = fopen("logs.txt", "a");

    if (fptr == NULL) {
        printf("Error opening log file.\n");
        return 1;
    }


    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) { // child process
        char *args[] = {"find", "shareDirectory", "-mindepth", "1", "-exec", "mv", "-t", "dashboardDirectory", "{}", "+", NULL};
        execvp("find", args);
        perror("execvp");
        exit(1);
    }
    else { // parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("All contents of 'shareDirectory' moved to 'dashboardDirectory'.\n");
            // Write the text to the file using fprintf()
            fprintf(fptr, "This is some text to add to the file.\n");

            // Close the file
            fclose(fptr);
        }
        else {
            printf("Failed to move contents of 'shareDirectory' to 'dashboardDirectory'.\n");
        }
    }
}

int main()
{
    collect_reports();
}
