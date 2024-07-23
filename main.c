#define DEBUG 0

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <bsd/stdlib.h>

#if DEBUG
    #define debug printf
#else
    #define debug(...)
#endif

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

    debug("Solving two trails: %016llx %016llx -> %016llx\n", a, b, t);

    debug("Generating trail for %016llx\n", a);
    while (a != t) {
        t1[i1] = a;
        i1++;
        if (i1 >= c) {
            c <<= 1;
            debug("Reallocating to %llu bytes\n", c * sizeof(uint64_t));
            t1 = realloc(t1, c * sizeof(uint64_t));
        }
        a = f(a);
    }
    debug("Finished trail with length of %llu\n", i1);

    c = 65536;
    debug("Generating trail for %016llx\n", b);
    while (b != t) {
        t2[i2] = b;
        i2++;
        if (i2 >= c) {
            c <<= 1;
            debug("Reallocating to %llu bytes\n", c * sizeof(uint64_t));
            t2 = realloc(t2, c * sizeof(uint64_t));
        }   
        b = f(b);
    }
    debug("Finished trail with length of %llu\n", i2);

    i1--;
    i2--;
    debug("Finding divergence in trails\n");
    while (1) {
        if (t1[i1] == t2[i2]) {
            i1--;
            i2--;
        } else {
            printf("%016llx and %016llx hash to %016llx\n", t1[i1], t2[i2], f(t1[i1]));

            assert(f(t1[i1]) == f(t2[i2]));

            free(t1);
            free(t2);
            return;
        }
    }
}

void sieve(uint64_t x) {
    int c = 65536;
    int i = 0;
    int s = 0;
    uint64_t *d = malloc(c * sizeof(uint64_t));

    debug("Sieving for %016llx\n", x);
    while (1) {
        x = f(x);
        s++;

        if ((x >> 48) == 0) {
            for (int j = 0; j < i; j++) {
                if (x == d[j]) {
                    uint64_t a = d[j - 1];
                    uint64_t b = d[i - 1];
                    free(d);
                    debug("Found hit after %llu steps and %llu points\n", s, i);
                    solve(x, a, b);
                    return;
                }
            }

            d[i] = x;
            i++;
            if (i >= c) {
                c <<= 1;
                debug("Reallocating to %llu bytes\n", c * sizeof(uint64_t));
                d = realloc(d, c * sizeof(uint64_t));
            }
        }
    }
}

int main() {
    uint64_t x;
    arc4random_buf(&x, sizeof(uint64_t));

    sieve(x);
}
