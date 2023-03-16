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

    // Declare variables
    DIR *source_dir, *dest_dir; // DIR is a data structure defined in dirent.h that represents a directory stream
    struct dirent *entry; // dirent is another data structure that represents a directory entry
    char source_path[100] = "dashboardDirectory"; // Path to the source directory
    char dest_path[100] = "backupDirectory"; // Path to the destination directory
    char source_file[200], dest_file[200]; // Buffers for file paths
    FILE *source, *destination; // File pointers for reading and writing files
    char buffer[1024]; // Buffer for reading and writing file contents
    size_t bytes_read; // Number of bytes read from a file

    // Open the source directory
    source_dir = opendir(source_path);
    if (source_dir == NULL) {
        printf("Error opening source directory.\n");
        exit(1);
    }

    // Open the destination directory
    dest_dir = opendir(dest_path);
    if (dest_dir == NULL) {
        printf("Error opening destination directory.\n");
        exit(1);
    }

    // Loop through each entry in the source directory
    while ((entry = readdir(source_dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Only process regular files, not directories or special files
            // Construct the full path to the source and destination files
            sprintf(source_file, "%s/%s", source_path, entry->d_name);
            sprintf(dest_file, "%s/%s", dest_path, entry->d_name);

            // Open the source file for reading
            source = fopen(source_file, "rb");
            if (source == NULL) {
                printf("Error opening source file: %s\n", source_file);
                exit(1);
            }

            // Open the destination file for writing
            destination = fopen(dest_file, "wb");
            if (destination == NULL) {
                printf("Error opening destination file: %s\n", dest_file);
                exit(1);
            }

            // Read from the source file and write to the destination file
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
                fwrite(buffer, 1, bytes_read, destination);
            }

            // Close the files
            fclose(source);
            fclose(destination);
        }
    }

    // Close the directories
    closedir(source_dir);
    closedir(dest_dir);

    printf("Directory copied successfully.\n");
}

int main()
{
    backup_dashboard();
}