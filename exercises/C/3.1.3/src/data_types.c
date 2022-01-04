#include <stdint.h>
#include <data_types.h>

//Declare a WORD in the following function
//and give it the value 123, then return it
uint16_t question_a() {
    unsigned short w = 123;
    return w;
}

//Declare a DWORD in the following function
//and give it the value that cannot fit in
//a WORD, then return it
uint32_t question_b() {
    unsigned long d = 65537;
    return d;
}

//Declare a QUADWORD in the following function
//and give it the value that cannot fit in
//a DWORD, then return it
uint64_t question_c() {
    unsigned long long q = 4294967297;
    return q;
}

//Declare a short in the following function
//and give it the value 200, then return it
short question_d() {
    short s = 200;
    return s;
}

//Declare an integer in the following function
//and give it the value 234, then return it
int question_e() {
    int e = 234;
    return e;
}

//Declare a float in the following function
//and give it the value 3.45, then return it
float question_f() {
    float f = 3.45;
    return f;
}

//Declare a character in the following function
//and give it the value 'A', then return it
char question_g() {
    char c = 'A';
    return c;
}

//Declare a double in the following function
//and give it the value 4.56, then return it
double question_h() {
    double d = 4.56;
    return d;
}

//Declare a long in the following function
//and give it the value 311244, then return it
long question_i() {
    long l = 311244;
    return l;
}
