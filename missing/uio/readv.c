#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf1[0];
    char buf2[0];
    char buf3[0];

    struct iovec iov[3];
    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1);

    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2);

    iov[2].iov_base = buf3;
    iov[2].iov_len = sizeof(buf3);

    int fd = open("test.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 一次读取，数据分散到三个缓冲区
    ssize_t nread = readv(fd, iov, 3);

    printf("Read %zd bytes\n", nread);
    printf("buf1: %.*s\n", (int)iov[0].iov_len, buf1);
    printf("buf2: %.*s\n", (int)iov[1].iov_len, buf2);
    printf("buf3: %.*s\n", (int)iov[2].iov_len, buf3);

    close(fd);

    return 0;
}