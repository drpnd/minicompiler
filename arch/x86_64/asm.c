/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "reg.h"

typedef struct {
    char *name;
    off_t pos;
} symbol_t;

typedef struct {
    struct {
        uint8_t *bin;
        off_t len;
        size_t size;
    } text;
    void *symbols;
} code_t;


static int
try_append(code_t *code, int *arr, size_t sz)
{
    ssize_t i;

    if ( code->text.len + sz > code->text.size ) {
        return -1;
    }
    for ( i = 0; i < (ssize_t)sz; i++ ) {
        code->text.bin[code->text.len++] = arr[i];
    }

    return 0;
}

#define D(...)  (int[]){__VA_ARGS__}, NARGS(__VA_ARGS__)
#define NARGS(...) ((int)(sizeof((int[]){ __VA_ARGS__ })/sizeof(int)))
#define TRY_APPEND(code, ...)                           \
    do {                                                \
        if ( try_append((code), (int[]){__VA_ARGS__},   \
                        NARGS(__VA_ARGS__)) < 0 ) {     \
            return -1;                                  \
        }                                               \
    } while ( 0 )

int
pushq(code_t *code)
{
    TRY_APPEND(code, 0x55);

    return 0;
}

int
popq(code_t *code)
{
    TRY_APPEND(code, 0x5d);

    return 0;
}

int
retq(code_t *code)
{
    TRY_APPEND(code, 0xc3);

    return 0;
}

int
mov(code_t *code)
{
    TRY_APPEND(code, 0x55);

    return 0;
}


int
main(void)
{
    code_t code;

    printf("%d\n", NARGS(1ULL, 3));

    code.text.bin = malloc(sizeof(1024));
    code.text.len = 0;
    code.text.size = 1024;

    retq(&code);

    return 0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
