#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

static inline uint64_t f(uint64_t x) {
    x ^= (x >> 33) | (x << 31);
    x *= 0xff51afd7ed558ccdL;
    x ^= (x >> 33) | (x << 31);
    x *= 0xc4ceb9fe1a85ec53L;
    x ^= (x >> 33) | (x << 31);
    return x;
}

void solve(uint64_t t, uint64_t a, uint64_t b) {
    int c = 65536;
    int i1 = 0;
    int i2 = 0;

    uint64_t *t1 = malloc(c * sizeof(uint64_t));
    uint64_t *t2 = malloc(c * sizeof(uint64_t));

    while (a != t) {
        t1[i1] = a;
        i1++;
        if (i1 >= c) {
            c <<= 1;
            t1 = realloc(t1, c * sizeof(uint64_t));
        }
        a = f(a);
    }

    c = 65536;
    while (b != t) {
        t2[i2] = b;
        i2++;
        if (i2 >= c) {
            c <<= 1;
            t2 = realloc(t2, c * sizeof(uint64_t));
        }   
        b = f(b);
    }

    i1--;
    i2--;
    while (1) {
        if (t1[i1] == t2[i2]) {
            i1--;
            i2--;
        } else {
            printf("%llu and %llu hash to %llu\n", t1[i1], t2[i2], f(t1[i1]));

            assert(f(t1[i1]) == f(t2[i2]));
            return;
        }
    }
}

int main() {
    uint64_t x = 1;

    int c = 65536;
    int i = 0;
    uint64_t *d = malloc(c * sizeof(uint64_t));

    while (1) {
        x = f(x);

        if ((x >> 48) == 0) {
            for (int j = 0; j < i; j++) {
                if (x == d[j]) {
                    solve(x, d[j - 1], d[i - 1]);
                    return 0;
                }
            }

            d[i] = x;
            i++;
            if (i >= c) {
                c <<= 1;
                d = realloc(d, c * sizeof(uint64_t));
            }
        }
    }
}
