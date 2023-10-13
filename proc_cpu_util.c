#include <sys/param.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libutil.h>
#include <math.h>
#include <sys/sysctl.h>

#define CPU_MAX_PER_CORE 2048

static int get_hw_ncpu(void) {
    int mib[2];
    size_t len;
    int ncpu;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    len = sizeof(ncpu);
    if (sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1) {
        perror("sysctl hw.ncpu:");
        exit(1);
    }
    return ncpu;
}

int main(int argc, char **argv) {
    const char *target_name = argv[1];
    struct kinfo_proc *procs;
    int nprocs, ncpu, i;
    double cpu_usage = 0;

    if (argc != 2) {
        printf("Wrong params! Accept only process name: %s <prog>\n", argv[0]);
        return 1;
    }

    procs = kinfo_getallproc(&nprocs);
    if (nprocs <= 0) {
        perror("kinfo_getallproc");
        return 1;
    }
    //printf("Number of processes: %d\n", nprocs);

    ncpu = get_hw_ncpu();
    for (i = 0; i < nprocs; i++) {
        if (strcmp(procs[i].ki_comm, target_name) == 0 && procs[i].ki_pctcpu > 0) {
            cpu_usage += (double) procs[i].ki_pctcpu * 100 / (CPU_MAX_PER_CORE * ncpu);
        }
    }
    printf("%.2f\n", cpu_usage);

    free(procs);

    return 0;
}
