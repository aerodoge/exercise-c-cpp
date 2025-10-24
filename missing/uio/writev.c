#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main() {
    char* str1="Hello ";
    char* str2="World";
    char* str3="!\n";

    struct iovec iov[3];
    iov[0].iov_base = str1;
    iov[0].iov_len = strlen(str1);
    iov[1].iov_base = str2;
    iov[1].iov_len = strlen(str2);
    iov[2].iov_base = str3;
    iov[2].iov_len = strlen(str3);

    ssize_t nwritten = writev(STDERR_FILENO, iov, 3);
    printf("Wrote %zd bytes\n", nwritten);
    return 0;
}
