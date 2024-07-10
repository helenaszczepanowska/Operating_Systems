#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
    DIR *d;
    struct dirent *dir;
    struct stat buf;
    long long totalSize = 0;

    d = opendir(".");

    while ((dir = readdir(d)) != NULL) {
        if (stat(dir->d_name, &buf) == 0) {
            if (!S_ISDIR(buf.st_mode)) {
                printf("%s: %lld bytes\n", dir->d_name, (long long) buf.st_size);
                totalSize += buf.st_size;
            }
        }
    }
    closedir(d);

    printf("Total size of files: %lld bytes\n", totalSize);

    return 0;
}
