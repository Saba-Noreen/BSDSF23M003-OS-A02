#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

enum DisplayMode { DEFAULT, LONG, HORIZONTAL };

int cmp_strings(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}

int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 80;
    return w.ws_col;
}

void print_vertical_display(char **files, int count) {
    int maxlen = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > maxlen)
            maxlen = len;
    }

    int term_width = get_terminal_width();
    int cols = term_width / (maxlen + 2);
    if (cols < 1)
        cols = 1;
    int rows = (count + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int index = c * rows + r;
            if (index < count)
                printf("%-*s  ", maxlen, files[index]);
        }
        printf("\n");
    }
}

void print_horizontal_display(char **files, int count) {
    int maxlen = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]);
        if (len > maxlen)
            maxlen = len;
    }

    int term_width = get_terminal_width();
    int cols = term_width / (maxlen + 2);
    if (cols < 1)
        cols = 1;

    int current_col = 0;
    for (int i = 0; i < count; i++) {
        printf("%-*s  ", maxlen, files[i]);
        current_col++;
        if (current_col >= cols) {
            printf("\n");
            current_col = 0;
        }
    }
    if (current_col != 0)
        printf("\n");
}

void print_long_listing(char **files, int count, const char *dir) {
    struct stat sb;
    char path[512];

    for (int i = 0; i < count; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir, files[i]);
        if (stat(path, &sb) == -1) {
            perror("stat");
            continue;
        }
        printf("%10ld %s\n", sb.st_size, files[i]);
    }
}

void do_ls(const char *dir, enum DisplayMode mode) {
    DIR *dp;
    struct dirent *entry;
    char *files[1024];
    int count = 0;

    dp = opendir(dir);
    if (!dp) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        files[count++] = strdup(entry->d_name);
    }
    closedir(dp);

    // Sort alphabetically
    qsort(files, count, sizeof(char *), cmp_strings);

    switch (mode) {
        case LONG:
            print_long_listing(files, count, dir);
            break;
        case HORIZONTAL:
            print_horizontal_display(files, count);
            break;
        default:
            print_vertical_display(files, count);
            break;
    }

    for (int i = 0; i < count; i++)
        free(files[i]);
}

int main(int argc, char *argv[]) {
    int opt;
    enum DisplayMode mode = DEFAULT;
    const char *dir = ".";

    while ((opt = getopt(argc, argv, "lx")) != -1) {
        switch (opt) {
            case 'l':
                mode = LONG;
                break;
            case 'x':
                mode = HORIZONTAL;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l | -x] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
        dir = argv[optind];

    do_ls(dir, mode);
    return 0;
}
