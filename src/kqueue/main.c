#include <sys/event.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{   
    // 用于注册事件到 kqueue
    struct	kevent event;
    // 用于接收从 kqueue 返回到事件，表示哪个 fd 触发了哪些事件
    struct	kevent emit_event;
    int kqueue_fd, file_fd, result;
    // 打开需要监控的文件，拿到一个 fd
    file_fd = open(argv[1], O_RDONLY);
    if (file_fd	== -1) {
      printf("Fail to open %s", argv[1]);
      return 1;
    }
    // 创建 kqueue 实例
    kqueue_fd = kqueue();
    // 设置需要监听的事件，文件被写入时触发
    EV_SET(&event,file_fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_RENAME, 0,	NULL);
    // 注册到操作系统
    result = kevent(kqueue_fd, &event, 1, NULL, 0, NULL);
    // 不断阻塞等待，直到文件被写入
    while(1) {
        // result 返回触发事件的 fd 个数，这里是一个
        result = kevent(kqueue_fd, NULL, 0, &emit_event,	1, NULL);
        if (result > 0) {
            printf("%s have been renamed\n", argv[1]);
        }
    }
}