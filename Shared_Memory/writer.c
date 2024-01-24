#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>
#include<sys/shm.h>

#define SHARED_MEM_SIZE 100 /* the size (in bytes) of shared memory object */
#define FILE_NAME "thong_mmap" /*name of the shared memory object*/

/*
* 1. create fd
* 2. set size
* 3. map/unmap process memory
* 4. delete memory segment
*/

int main() {
    /**
     * int shm_open(const char *name, int oflag, mode_t mode);
     *  @brief 
     * 
     *  @param[in] name File path
     *  @param[in] oflag O_RDONLY --> process open shared memory object with ReadOnly
     *                   O_RDWR   --> process open shared memory object with Read/Write
     *                   O_CREAT  --> create shared memory segment if not existed
     *                   O_EXCL   --> if shared memory object and the file currently given are existed, return EEXIST error
     *                   O_TRUNC  --> if shared memory object existing is operating, truncate to value 0
     * @param[in] mode permission of shared memory object iss created. If oflag doesn't use O_CREAT flag or 
     *                 shared memory object is existing, mode will be ignore
     * 
     * @return when created successfully, be able to see file connecting with shared memory in path, -1 if error
     * 
    */
    int shm_fd = shm_open(FILE_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd < 0) {
        printf("shm_open() is failed, %s.\n", strerror(errno));
        return -1;
    }

    /**
     * int ftruncate(int shm_fd, off_t length);
     * @brief Configure the size of the shared memory object.
     *        After opening, the size of the shared memory object is 0
     * @param[in] shm_fd shm file descriptor
     * @param[in] length the size (in bytes) of shared memory object
     * 
     * @return
    */
    ftruncate(shm_fd, SHARED_MEM_SIZE);

    /**
     * void *mmap(void* adr, size_t length, int prot, int flags, int fd, off_t offset);
     * @brief Configure the size of shared memory object.
     * 
     * @param[in] addr Memory address of calling process mapped, similar to shm 
     * so set value to 0 and kernel will allocate adress appropiately
     * @param[in] length Length of shared memory object mapped (>0).
     * @param[in] prot Memory protection of mapping (cannot conflict with file permission).
     *                 Can have value:
     *                 PROT_EXEC  --> Pages may be executed.
     *                 PROT_READ  --> Pages may be read
     *                 PROT_WRITE --> Pages may be written
     *                 PROT_NONE  --> Pages may not be accessed.
     * @param[in] flags Flag permit processes be able to see the update of processes
     *                  into shared memory
     *                  MAP_SHARED  --> update this process into shared memory is available with other process
     *                  MAP_PRIVATE --> kernel using this copy-on-write to create own memory segment instead of 
     *                                  changing the process. These updates wwill not visible to other process
     *                  MAP_ANONYMOUS --> Create anonymous mapping. All memory on this segment is initialized to 0.
     *                  not care fd.
     *                  MAP_UNINITIALIZED --> Not create anonymous mapping segment
     * 
     * @param[in] fd shm file descriptor
     * @param[in] offset offset of shared memory object will be mapping to virtual memory of process.
     * 
     * @return On success, mmap() returns a pointer to the mapped area.
     *         On error, the value MAP_FAILED (that is, (void *) -1) is returned,
     *         and errno is set to indicate the cause of the error.
     * 
    */
    char *data = (char *)mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    strcpy(data, "MMAP Hello Thong !");
    printf("%s: Write data: %s\n", __FILE__, data);

    /**
     * int munmap(void *addr, size_t length);
     * @brief Unmap shared memory object from calling process
     * 
     * @param[in] addr Memory address of calling process mapped
     * @param[in] length length of shared memory object mapped (>0)
     * 
     * @return On success, munmap() returns 0.
     *         On failure, it returns -1, and errno is set to 
     *         indicate the cause of the error (probably to EINVAL).
    */
    munmap(data, SHARED_MEM_SIZE);

    close(shm_fd);
    /**
     * int shm_unlink(const char *name);
     * @brief Remove shared memory object created before
     *        Shared memory object will be deleted after the last process unmap
     * 
     * @param[in] name File path
     * 
     * @return
    */
    //shm_unlink(FILE_NAME);

    return 0;
}