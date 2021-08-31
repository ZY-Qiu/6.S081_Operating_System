struct vma {
    int valid;
    void * addr;        // start address in address space
    uint64 length;      // length of the vma
    int prot;           // R, W, X or NONE
    int flags;          // shared or private
    int fd;             // file descriptor
    struct file* file;  // file the vma refer to
};