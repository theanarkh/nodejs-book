#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>
#include <thread>

typedef int(*fn)(int, int);
/**
 * 主线程先加载动态链接库，然后执行子线程，子线程执行完主线程再继续执行。
 * 每次调 dlopen 动态链接库的引用数会加一，引用数大于 0 时再调用 dlopen 
 * 不会再执行动态链接库的初始化函数，并且拿到的 handler 地址是一样的。
 * 每次 dlclose 会减一，如果引用数为 0 再调用 dlopen 则会重新执行动态链接库
 * 的初始化函数，并且 handler 对应的地址不一样。
 */
int main(){  
    // 打开一个动态链接库，拿到一个 handler  
    void * handler = dlopen("liblibrary.so",RTLD_LAZY); 
    // 取出动态链接库里的函数 add  
    void * add = dlsym(handler, "add");  
    // 执行  
    printf("%d %u\n",((fn)add)(1,1), handler);  
    dlclose(handler);
    std::thread thread = std::thread([] () {
        void * handler = dlopen("liblibrary.so",RTLD_LAZY); 
        // 取出动态链接库里的函数 add  
        void * add = dlsym(handler, "add");  
        // 执行  
        printf("%d %u\n",((fn)add)(1,1), handler);  
        dlclose(handler);  
    });
    // 先等子线程执行完
    thread.join();
    return 0;  
}  