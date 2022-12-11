run:
	gcc -fPIC -shared  library.c -o liblibrary.so && gcc main.c -o main && ./main
run-mutil-thread:
	gcc -fPIC -shared  library.c -o liblibrary.so && g++ mutil-thread-main.cc --std=c++11 -o mutil-thread-main && ./mutil-thread-main
run-mutil-thread-2:
	gcc -fPIC -shared  library.c -o liblibrary.so && g++ mutil-thread-main-2.cc --std=c++11 -o mutil-thread-main-2 && ./mutil-thread-main-2