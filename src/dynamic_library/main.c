#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>

typedef int(*fn)(int, int);

int main(){  
    // 打开一个动态链接库，拿到一个 handler  
    void * handler = dlopen("liblibrary.so",RTLD_LAZY);  
    // 取出动态链接库里的函数 add  
    void * add = dlsym(handler, "add");  
    // 执行  
    printf("%d",((fn)add)(1,1));  
    dlclose(handler);  
    return 0;  
}  