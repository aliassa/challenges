#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

#define FILE_NAME "image.bin"

void mmap_read_img()
{
    int fd = open(FILE_NAME, O_RDONLY, S_IRUSR | S_IWUSR);
    struct stat sb;
    if(fstat(fd, &sb) == -1)
        perror("Couldn't get file size.\n");
    printf("file size %ld\n", sb.st_size);

    char *mem = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    for (int i = 0; i < sb.st_size; i++)
    {
        printf("%02x",mem[i]);
    }
    printf("\n");
    munmap(mem, sb.st_size);
    close(fd);
}