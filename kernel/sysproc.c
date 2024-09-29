#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "file.h"
#include "statvfs.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/**
 * Fills the given statvfs structure with filesystem statistics.
 *
 * @param stats Pointer to a statvfs structure that will be populated with:
 *              - f_blocks: Total number of blocks.
 *              - f_bfree: Free blocks.
 *              - f_bavail: Free blocks available to non-superusers.
 *              - f_files: Total number of inodes.
 *              - f_ffree: Free inodes.
 *              - f_frsize: Block size (set to 512 bytes).
 *
 * Helper functions bmap_free_blocks() and free_inodes() are used to fetch free blocks and inodes.
 */
void get_fs_stats(struct statvfs *stats) {
    struct superblock *sb = &sb;

    stats->f_blocks = sb->nblocks;
    stats->f_bfree = bmap_free_blocks();
    stats->f_bavail = stats->f_bfree;
    
    stats->f_files = sb->ninodes; 
    stats->f_ffree = free_inodes();  

    stats->f_frsize = 512;
}

uint64 sys_statvfs(void) {
	struct statvfs stats;
	get_fs_stats(&stats);
	return (uint64)&stats;
}
