#ifdef MAIN
#define BASALT_STDLIB_IMPLEMENTATION
#endif

#include "basalt_stdlib.h"

#if defined(DEBUG) || defined(MAIN)

void test_memcpy() {
    const char src[100];
    char dest[100];

    memcpy(dest,src, 100);
    for (size_t i = 0; i < 100; i++) {
        if (dest[i] != src[i]) {
            panic("Bytes are not the same!");
        }
    }
}

void test_memset() {
    char data[64];
    memset(data,69,30);
    memset(&data[30],42,34);
    for (size_t i = 0; i < 30; i++){
        if (data[i] != 69){
            panic("Bytes are not set correctly!");
        }
    }
    for (size_t i = 30; i < 64; i++){
        if (data[i] != 42){
            panic("Bytes are not set correctly!");
        }
    }
}

void test_size_t() {
    if( sizeof(int*) == 4 ){ // if 32-bit
        assert(sizeof(size_t)==4);
    }else{
        assert(sizeof(size_t)==8);
    }
}

void TestStdLib() {
    test_size_t();
    test_memcpy();
    test_memset();
}

#endif

#ifdef MAIN
int main(int argc, int** argv) {
    TestStdLib();
    return 0;
}
#endif

#if defined(WINDOWS) && defined(MAIN)
#include <windows.h>
void __stdcall mainCRTStartup()
{
    int result = main(0,NULL); // TODO: command line args
    ExitProcess(result);
}
#endif
