/*
* Programming Assignment 02: lsv1.0.0
* This is the source file of version 1.0.0
* Read the write-up of the assignment to add the features to this base version
* Usage:
*       $ lsv1.0.0 
*       % lsv1.0.0  /home
*       $ lsv1.0.0  /home/kali/   /etc/
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

void do_ls(const char *dir,int long_listing);
void print_file_info(const char *path,const char *filename);
void print_permissions(mode_t mode);

int main(int argc, char const *argv[])
{
    int long_listing = 0;
    int arg_start =1;
    if (arg >1 && strcmp(argv[1],"-l")==0)
{
	long_listing =1;
	arg_start=1;
}

 if (argc == arg_start)
    {
        do_ls(".", long_listing);
    }   if (argc == 1)
    {
        do_ls(".");
    }
    else
    {
        for (int i = arg_start; i < argc; i++)
        {
            printf("Directory listing of %s : \n", argv[i]);
            do_ls(argv[i],long_listing);
	    puts("");
        }
    }
    return 0;
}

void do_ls(const char *dir, int long_listing)
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
            continue; // skip hidden files

        if (long_listing)
        {
            print_file_info(dir, entry->d_name);
        }
        else
        {
            printf("%s\n", entry->d_name);
        }
    }

    if (errno != 0)
    {
        perror("readdir failed");
    }

   
    closedir(dp);
}
void print_file_info(const char *path, const char *filename)
{
    char fullpath[1024];
    struct stat info;
    struct passwd *pw;
    struct group *gr;
    char timebuf[80];

    // Build full path (dir + "/" + filename)
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);

    if (lstat(fullpath, &info) == -1)
    {
        perror("lstat");
        return;
    }

    // File type + permissions
    print_permissions(info.st_mode);

    // Number of links
    printf(" %2ld", (long)info.st_nlink);

    // Owner and group
    pw = getpwuid(info.st_uid);
    gr = getgrgid(info.st_gid);
    printf(" %-8s %-8s", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

    // File size
    printf(" %8ld", (long)info.st_size);

    // Modification time
    struct tm *tm_info = localtime(&info.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
    printf(" %s", timebuf);

    // File name
    printf(" %s\n", filename);
}

/*------------------------------------------------------*/

void print_permissions(mode_t mode)
{
    // File type
    if (S_ISREG(mode)) printf("-");
    else if (S_ISDIR(mode)) printf("d");
    else if (S_ISLNK(mode)) printf("l");
    else if (S_ISCHR(mode)) printf("c");
    else if (S_ISBLK(mode)) printf("b");
    else if (S_ISFIFO(mode)) printf("p");
    else if (S_ISSOCK(mode)) printf("s");
    else printf("?");

    // Owner permissions
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');

    // Group permissions
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');

    // Others permissions
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}
