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
void do_ls_long(const char *dir);

int main(int argc, char const *argv[])
{
    int opt;
    while ((opt = getopt(argc, (char * const *)argv, "l")) != -1)
    {
        switch (opt)
        {
            case 'l':
                long_listing = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind == argc)  // no directories specified
    {
        if(long_listing)
            do_ls_long(".");
        else
            do_ls(".");
    }
    else  // directories provided
    {
        for (int i = optind; i < argc; i++)
        {
            printf("Directory listing of %s : \n", argv[i]);
            if(long_listing)
                do_ls_long(argv[i]);
            else
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
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory : %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        printf("%s\n", entry->d_name);
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}

void do_ls_long(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        struct stat st;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if(lstat(path, &st) == -1)
        {
            perror("lstat");
            continue;
        }

        // File type
        char type = '-';
        if (S_ISDIR(st.st_mode)) type = 'd';
        else if (S_ISLNK(st.st_mode)) type = 'l';

        // Permissions
        char perms[10] = "---------";
        if (st.st_mode & S_IRUSR) perms[0] = 'r';
        if (st.st_mode & S_IWUSR) perms[1] = 'w';
        if (st.st_mode & S_IXUSR) perms[2] = 'x';
        if (st.st_mode & S_IRGRP) perms[3] = 'r';
        if (st.st_mode & S_IWGRP) perms[4] = 'w';
        if (st.st_mode & S_IXGRP) perms[5] = 'x';
        if (st.st_mode & S_IROTH) perms[6] = 'r';
        if (st.st_mode & S_IWOTH) perms[7] = 'w';
        if (st.st_mode & S_IXOTH) perms[8] = 'x';

        // Owner & group
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);

        // Modification time
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));

        printf("%c%s %ld %s %s %ld %s %s\n",
            type, perms,
            st.st_nlink,
            pw ? pw->pw_name : "unknown",
            gr ? gr->gr_name : "unknown",
            st.st_size,
            timebuf,
            entry->d_name
        );
    }

    if(errno != 0)
        perror("readdir failed");

    closedir(dp);
}


