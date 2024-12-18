/*
写时复制（COW）是一种延迟内存复制的优化技术，只有在需要修改数据时才会进行复制。它主要用于操作系统的进程创建（如 fork() 系统调用）和内存映射文件等场景，可以有效减少内存消耗和提高性能。通过共享内存和延迟复制，COW能够优化内存管理，避免了不必要的复制操作，从而提高了系统的整体效率。　
*/
#include <unistd.h>
#include <iostream>

int main()
{
    int x = 42;
    pid_t pid = fork();

    if (pid == 0)
    {
        // 子进程
        x = 100; // 修改 x 的值
        std::cout << "Child process x: " << x << std::endl;
    }
    else
    {
        // 父进程
        std::cout << "Parent process x: " << x << std::endl;
    }

    return 0;
}