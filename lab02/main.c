#include <stdio.h>
#include "collatz.h"

#ifdef USE_DYNAMIC
#include <dlfcn.h>
int main() {
    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);

    void* handle = dlopen("./libcollatz_shared.so", RTLD_LAZY);
    if (!handle) {
        printf("Error");
        return 1;
    }

    collatz_conjecture = dlsym(handle, "collatz_conjecture");
    test_collatz_convergence = dlsym(handle, "test_collatz_convergence");

    printf("%d\n", collatz_conjecture(10));
    printf("%d\n", test_collatz_convergence(27, 1000));

    dlclose(handle);
}
#else
int main() {
    printf("%d\n", collatz_conjecture(10));
    printf("%d\n", test_collatz_convergence(27, 1000));
}
#endif