typedef struct {
    int fd;
    struct stat sb;
    char *file;
} MappedFile;

MappedFile map_file_rd(char *path);
void unmap_file(MappedFile mf);
