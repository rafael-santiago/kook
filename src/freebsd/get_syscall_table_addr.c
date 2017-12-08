#include <freebsd/get_syscall_table_addr.h>
#include <sys/sysent.h>

void *get_syscall_table_addr(void) {
    return &sysent[0];
}
