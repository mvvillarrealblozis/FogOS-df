#ifndef STAT_H
#define STAT_H

struct statvfs {
    unsigned int total_blocks;
    unsigned int free_blocks;
    unsigned int used_blocks;
    unsigned int total_inodes;
    unsigned int free_inodes;
};

#endif
