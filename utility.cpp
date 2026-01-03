#include "utility.h"
int factial(int n) {

    
    return n ? n * factial(n - 1) : 1;
}

int add(int a, int b) {
    int sum;
    sum += a + b;
    return sum;
}

