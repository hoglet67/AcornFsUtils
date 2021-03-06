#include "acorn-fs.h"

int main(int argc, char *argv[])
{
    if (--argc) {
        int status;
        while(argc--) {
            const char *fsname = *++argv;
            acorn_fs *fs = acorn_fs_open(fsname, false);
            if (fs) {
                int astat = fs->check(fs, fsname, stderr);
                if (astat != AFS_OK)
                    status++;
            }
        }
        acorn_fs_close_all();
        return status;
    }
    else {
        fputs("Usage: afschk <acorn-fs-image>\n", stderr);
        return 1;
    }
}
