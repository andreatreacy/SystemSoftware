#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>

#define SHARE_DIR "shareDirectory"
#define DASHBOARD_DIR "dashboardDirectory"

void collect_reports(void) {
    //printf("collect reports functionality could go here.. fork/cp will probably be used");
    DIR *dir;
    struct dirent *entry;
    char src_path[PATH_MAX], dest_path[PATH_MAX];
    struct stat statbuf;
    
    // Open source directory
    if ((dir = opendir(SHARE_DIR)) == NULL) {
        perror("opendir");
        return -1;
    }
    
    // Loop through source directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Ignore special entries . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Construct source and destination paths
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);
        
        // Check if entry is a directory
        if (lstat(src_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // Entry is a directory, recursively move its contents
            if (move_files(src_path, dest_path) != 0) {
                closedir(dir);
                return -1;
            }
            rmdir(src_path);
        } else {
            // Entry is a file, move it to destination directory
            if (rename(src_path, dest_path) != 0) {
                perror("rename");
                closedir(dir);
                return -1;
            }
        }
    }
    
    // Close source directory
    closedir(dir);
    
    return 0;
}
