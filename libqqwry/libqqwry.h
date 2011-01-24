#ifndef LIB_QQWRY_H
#define LIB_QQWRY_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void *raw;
    unsigned int len;
} qqwry_data_t;

typedef struct {
    char *country;
    char *area;
} qqwry_result_t;

int qqwry_init(char *path, qqwry_data_t *data);
int qqwry_clean(qqwry_data_t data);
int qqwry_query(qqwry_data_t data, uint32_t ip, qqwry_result_t *result);

#endif
