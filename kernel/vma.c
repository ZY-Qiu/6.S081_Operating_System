#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "vma.h"
#define vmatablesize 16

struct vmaTable {
    struct spinlock lock;
    struct vma vma[vmatablesize];
}vmatable;

void vmainit()
{
    initlock(&vmatable.lock, "vmatbl");
}

struct vma* vmaalloc()
{
    struct vma* v;
    acquire(&vmatable.lock);
    for(v = vmatable.vma; v < vmatable.vma + vmatablesize; v++)
    {
        if(v->valid == 0)
        {
            release(&vmatable.lock);
            return v;
        }
    }
    release(&vmatable.lock);
    return 0;
}


void vmafree(struct vma* v)
{
    acquire(&vmatable.lock);
    v->valid = 0;
    release(&vmatable.lock);
}

