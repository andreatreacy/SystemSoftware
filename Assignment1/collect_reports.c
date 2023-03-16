#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>


void collect_reports(void) {
    //printf("collect reports functionality could go here.. fork/cp will probably be used");
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) { // child process
        //char *args[] = {"sh", "-c", "mv shareDirectory/* dashboardDirectory/", NULL};
        //char *args[] = {"mv", "shareDirectory/*", "dashboardDirectory/", NULL};
        char *args[] = {"find", "shareDirectory", "-mindepth", "1", "-exec", "mv", "-t", "dashboardDirectory", "{}", "+", NULL};
        execvp("find", args);
        perror("execvp");
        exit(1);
    }
    else { // parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("All contents of 'shareDirectory' moved to 'dashboardDirectory'.\n");
        }
        else {
            printf("Failed to move contents of 'shareDirectory' to 'dashboardDirectory'.\n");
        }
    }
}


int main ()
{
    collect_reports();
}
