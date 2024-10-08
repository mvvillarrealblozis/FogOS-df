#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "file.h"
#include "user/stat.h"

extern struct superblock sb;

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
 * sys_statvfs - system call to gather filesystem statistics
 * @returns 0 on success, -1 on failure
 */
uint64 sys_statvfs(void) {
    struct statvfs stats;

    // Fill the stats structure with filesystem information
    stats.total_blocks = sb.nblocks;
    stats.free_blocks = bmap_free_blocks();
    stats.used_blocks = stats.total_blocks - stats.free_blocks;
    stats.total_inodes = sb.ninodes;
    stats.free_inodes = free_inodes();

	uint64 addr;
	
    argaddr(0,&addr);

    // Copy the stats structure to the user space
    if (copyout(myproc()->pagetable, addr, (char *)&stats, sizeof(stats)) < 0) {
        return -1;
    }

    // Return success
    return 0;
}
