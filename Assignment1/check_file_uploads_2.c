#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void check_file_uploads(void) {
   //printf("you can potentially use this place holder to determine if a file has been uploaded");

    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *sales_file = NULL;
    char *warehouse_file = NULL;
    char *manufacturing_file = NULL;
    char *distribution_file = NULL;
    char *files[4];
    char *missing_files[4] = { "Sales", "Warehouse", "Manufacturing", "Distribution" };
    int i, j;

    // Open the directory
    dir = opendir("dashboardDirectory");
    if (dir == NULL) {
        perror("Unable to open directory");
        return 1;
    }

    // Read all the filenames in the directory
    while ((ent = readdir(dir)) != NULL) {
        // Check if the filename starts with 2023-03-03
        if (strncmp(ent->d_name, "2023-03-03", 10) == 0) {
            // Add the filename to the files array
            files[count] = ent->d_name;
            count++;

            // Check if the filename contains sales, warehouse, Manufacturing, or Distribution
            if (strstr(ent->d_name, "Sales") != NULL) {
                sales_file = ent->d_name;
                missing_files[0] = NULL; // Mark category as found
            }
            else if (strstr(ent->d_name, "Warehouse") != NULL) {
                warehouse_file = ent->d_name;
                missing_files[1] = NULL; // Mark category as found
            }
            else if (strstr(ent->d_name, "Manufacturing") != NULL) {
                manufacturing_file = ent->d_name;
                missing_files[2] = NULL; // Mark category as found
            }
            else if (strstr(ent->d_name, "Distribution") != NULL) {
                distribution_file = ent->d_name;
                missing_files[3] = NULL; // Mark category as found
            }
        }
    }

    // Close the directory
    closedir(dir);

    // Check if there are exactly four files that match the criteria
    if (count == 4 && sales_file != NULL && warehouse_file != NULL &&
        manufacturing_file != NULL && distribution_file != NULL) {
        printf("All four files are present.\n");
    }
    else {
        printf("Error: The following files are missing:\n");
        for (i = 0; i < 4; i++) {
            if (missing_files[i] != NULL) {
                printf("- %s\n", missing_files[i]);
            }
        }
    }

}

int main()
{
    check_file_uploads();
}