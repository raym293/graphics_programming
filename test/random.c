#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    while(1) {
        long x = random();
        printf("%lld\n", x);
        sleep(1);
    }
    return 0;
}
