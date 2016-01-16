#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "mapfile.h"

MappedFile map_file_rd(char *path)
{
    MappedFile mf;

    /* Open the file */
    mf.fd = open(path, O_RDONLY);
    assert(mf.fd != 0);

    /* Get info about the file, including size */
    int err = fstat(mf.fd, &mf.sb);
    assert(err == 0);

    /* Map the file */
    mf.file = mmap(NULL, mf.sb.st_size, PROT_READ, MAP_SHARED, mf.fd, 0);
    assert(mf.file != MAP_FAILED);

    return mf;
}

void unmap_file(MappedFile mf)
{
    /* Clean up */
    munmap(mf.file, mf.sb.st_size);
    close(mf.fd);
}
