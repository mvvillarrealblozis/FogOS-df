#include "user/user.h"
#include "user/stat.h"
#include "kernel/types.h"


int main(int argc, char *argv[]) {
    struct statvfs stats;
    int human_readable = 0;
    int inode_usage = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            human_readable = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            inode_usage = 1;
        }
    }

    if (statvfs(&stats) < 0) {
        printf("Error: Failed to retrieve filesystem stats\n");
        exit(1);
    }

    if (inode_usage) {
        printf("Inodes: Total: %d, Free: %d\n", stats.total_inodes, stats.free_inodes);
    } else {
        if (human_readable) {
            printf("Disk: Total: %dKB, Used: %dKB, Free: %dKB\n",
                   stats.total_blocks / 2, stats.used_blocks / 2, stats.free_blocks / 2);
        } else {
            printf("Disk: Total: %d blocks, Used: %d blocks, Free: %d blocks\n",
                   stats.total_blocks, stats.used_blocks, stats.free_blocks);
        }
    }

    return 0;
}
