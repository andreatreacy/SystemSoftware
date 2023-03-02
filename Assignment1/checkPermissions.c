#include <stdio.h>
#include <sys/stat.h>

int main() {

    // variable to store the file status
    struct stat st;

    /* stat() function is used to retrieve the file status of "shareDirectory" 
    this is then stored in st. The result of the function call is stored in result */
    int result = stat("shareDirectory", &st);

    // If stat() successfully retrieves the file status of "shareDirectory" it returns 0
    // if result is 0, the permissions are printed
    if (result == 0) 
    {
        /*To print the permissions, bitwise AND operations are used to check if 
        each permission bit is set in the st_mode field of st. 
        If a bit is set, the corresponding character is printed r w or x. If a bit 
        is not set, a '-' character is printed.*/
        printf("Permissions for 'shareDirectory':\n");
        printf("Owner: %c%c%c\n", st.st_mode & S_IRUSR ? 'r' : '-', 
                                   st.st_mode & S_IWUSR ? 'w' : '-', 
                                   st.st_mode & S_IXUSR ? 'x' : '-');
        printf("Group: %c%c%c\n", st.st_mode & S_IRGRP ? 'r' : '-', 
                                   st.st_mode & S_IWGRP ? 'w' : '-', 
                                   st.st_mode & S_IXGRP ? 'x' : '-');
        printf("Others: %c%c%c\n", st.st_mode & S_IROTH ? 'r' : '-', 
                                    st.st_mode & S_IWOTH ? 'w' : '-', 
                                    st.st_mode & S_IXOTH ? 'x' : '-');
    } 
    else 
    {
        printf("Unable to get permissions for 'shareDirectory'.\n");
    }
    return 0;
}