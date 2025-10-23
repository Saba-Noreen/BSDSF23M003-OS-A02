#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_FILES 1024

// Enum for display modes
enum DisplayMode {
    DEFAULT,
    LONG,
    HORIZONTAL
};

void do_ls(const char *dir, enum DisplayMode mode);
void print_long_listing(const char *dir);
void print_vertical_display(char **files, int count);
void print_horizontal_display(char **files, int count);

int main(int argc, char *argv[]) {
    enum DisplayMode mode = DEFAULT;
    int opt;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "lx")) != -1) {
        switch (opt) {
            case 'l':
                mode = LONG;
                break;
            case 'x':
                mode = HORIZONTAL;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l|-x] [directory...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // If no directory given, list current directory
    if (optind == argc) {
        do_ls(".", mode);
    } else {
        for (int i = optind; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], mode);
            printf("\n");
        }
    }
    return 0;
}

void do_ls(const char *dir, enum DisplayMode mode) {
    DIR *dp;
    struct dirent *entry;
    char *files[MAX_FILES];
    int count = 0;

    dp = opendir(dir);
    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    // Read all file names
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue; // skip hidden files
        files[count] = strdup(entry->d_name);
        count++;
    }
    closedir(dp);

    // Choose display mode
    switch (mode) {
        case LONG:
            print_long_listing(dir);
            break;
        case HORIZONTAL:
            print_horizontal_display(files, count);
            break;
        default:
            print_vertical_display(files, count);
            break;
    }

    // Free memory
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
}

void print_long_listing(const char *dir) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char path[512];

    dp = opendir(dir);
    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if (stat(path, &statbuf) == -1) continue;
        printf("%10ld %s\n", statbuf.st_size, entry->d_name);
    }
    closedir(dp);
}

// Default vertical (down then across)
void print_vertical_display(char **files, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s\n", files[i]);
    }
}

// Horizontal (across then down)
void print_horizontal_display(char **files, int count) {
    int max_len = 0;
    struct winsize w;

    // Get terminal width
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        w.ws_col = 80; // fallback
    }

    // Find longest filename
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > max_len) max_len = len;
    }

    int col_width = max_len + 2;
    int cols = w.ws_col / col_width;
    if (cols == 0) cols = 1;

    // Compute number of rows
    int rows = (count + cols - 1) / cols;

    // 🔹 Print row-major (across → down)
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int i = r * cols + c;
            if (i < count)
                printf("%-*s", col_width, files[i]);
        }
        printf("\n");
    }
}


