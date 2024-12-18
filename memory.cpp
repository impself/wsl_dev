/*
https://mp.weixin.qq.com/s/tIq57be6ib8V88olP1Iasg
面试题：如果两个进程都访问0x10086地址会发生什么？
*/

/*
0x10086这个地址可能并没有被映射到当前进程的地址空间中。也就是说，这个地址可能不属于该进程可以合法访问的内存区域。
操作系统会检测到这种非法访问，并触发段错误（Segmentation Fault），导致程序崩溃。
*/
// #include <iostream>
// #include <unistd.h>
// using namespace std;

// int main()
// {
//     int *ptr = (int *)0x10086; // 定义一个指针，指向0x10086
//     *ptr = 42;                 // 可能导致段错误
//     cout << "Value at 0x10086: " << *ptr << endl;
//     return 0;
// }

/*
使用共享内存以下代码展示如何通过共享内存让两个进程访问同一个物理地址：　父进程和子进程共享内存　
*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    key_t key = 1234; // 共享内存的唯一标识
    int shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget failed");
        return 1;
    }

    int *shared_mem = (int *)shmat(shmid, nullptr, 0);
    if (shared_mem == (void *)-1)
    {
        perror("shmat failed");
        return 1;
    }

    if (fork() == 0)
    {
        // 子进程写入共享内存
        *shared_mem = 42;
        cout << "Child wrote: " << *shared_mem << endl;
    }
    else
    {
        // 父进程读取共享内存
        sleep(1);
        cout << "Parent read: " << *shared_mem << endl;
    }

    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}