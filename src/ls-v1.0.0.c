/*
* Programming Assignment 02: lsv1.0.0
* This is the source file of version * Read the write-up of the assignment to add the features to this base version
* Usage:
*       $ lsv1.0.0 
*       % lsv1.0.0  /home
*       $ lsv1.0.0  /home/kali/   /etc/
*/
/*
* Programming Assignment 02: lsv1.1.0
* This is the source file of version 1.1.0
* Adds -l long listing format to the base ls-v1.0.0
* Usage:
*       $ lsv1.1.0 
*       $ lsv1.1.0 /home
*       $ lsv1.1.0 -l /home/kali/ /etc/
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

extern int errno;
int long_listing = 0;   // flag for -l option

void do_ls(const char *dir);

int main(int argc, char const *argv[])
{
	 if (argc == 1) {
        do_ls(".");
    } else {
        for (int i = 1; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i]);
            puts("");
        }
    }
    return 0;
}
    
void do_ls(const char *dir)
{
   struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    /* Gather filenames */
    char *names[1024];
    int count = 0, maxlen = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        names[count] = strdup(entry->d_name);
        int len = strlen(entry->d_name);
        if (len > maxlen) maxlen = len;
        count++;
    }
    closedir(dp);

    /* Compute columns */
    int termwidth = 80;
    int cols = termwidth / (maxlen + 2);
    if (cols < 1) cols = 1;
    int rows = (count + cols - 1) / cols;

    /* Print in columns */
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int i = c * rows + r;
            if (i < count)
                printf("%-*s", maxlen + 2, names[i]);
        }
        printf("\n");
    }

    for (int i = 0; i < count; i++)
        free(names[i]);
}
