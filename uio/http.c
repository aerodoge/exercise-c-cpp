#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 创建测试文件
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 准备数据（模拟 HTTP 响应）
    char *part1 = "HTTP/1.1 200 OK\r\n";
    char *part2 = "Content-Type: application/json\r\n";
    char *part3 = "Content-Length: 27\r\n\r\n";
    char *part4 = "{\"message\":\"Hello World\"}";

    // 构建 iovec 数组
    struct iovec iov[4];

    iov[0].iov_base = part1;
    iov[0].iov_len = strlen(part1);

    iov[1].iov_base = part2;
    iov[1].iov_len = strlen(part2);

    iov[2].iov_base = part3;
    iov[2].iov_len = strlen(part3);

    iov[3].iov_base = part4;
    iov[3].iov_len = strlen(part4);

    // 一次写入所有数据
    ssize_t total = writev(fd, iov, 4);

    if (total == -1) {
        perror("writev");
        close(fd);
        return 1;
    }

    printf("Successfully wrote %zd bytes\n", total);

    close(fd);
    return 0;
}