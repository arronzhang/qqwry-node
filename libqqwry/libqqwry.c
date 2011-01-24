#include "libqqwry.h"
#include <stdio.h>
#include <string.h>

int qqwry_init(char *path, qqwry_data_t *data) {
    FILE *fd;
    void *buf;
    size_t len;
    
    if (!(fd = fopen(path, "rb"))) {
        return 1;
    }
    
    fseek(fd, 0, SEEK_END);
    len = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    
    buf = malloc(len);
    if (!buf) {
        fclose(fd);
        return 2;
    }
    
    if (len != fread(buf, 1, len, fd)) {
        fclose(fd);
        return 3;
    }
    
    fclose(fd);
    
    data->raw = buf;
    data->len = len;
    
    return 0;
}

int qqwry_clean(qqwry_data_t data) {
    free(data.raw);
    data.raw = NULL;
    data.len = 0;
    return 0;
}

#define qqwry_read_uint32(X) ((uint32_t)*(uint8_t *)(X) |\
                              (uint32_t)*((uint8_t *)(X)+1)<<8 |\
                              (uint32_t)*((uint8_t *)(X)+2)<<16 |\
                              (uint32_t)*((uint8_t *)(X)+3)<<24)

#define qqwry_read_uint24(X) ((uint32_t)*(uint8_t *)(X) |\
                              (uint32_t)*((uint8_t *)(X)+1)<<8 |\
                              (uint32_t)*((uint8_t *)(X)+2)<<16)

#define qqwry_read_uint8(X)  (*(uint8_t *)(X))

#define qqwry_setpos(X,Y) ((uint8_t *)(X) + (Y))

int qqwry_query(qqwry_data_t data, uint32_t ip_addr, qqwry_result_t *result) {    
    uint8_t *pos = data.raw;

    uint32_t idx_first = qqwry_read_uint32(pos);
    uint32_t idx_last = qqwry_read_uint32(pos + 4);    
    uint32_t idx_found = idx_last;
    
    unsigned int h = (idx_last - idx_first) / 7;
    unsigned int l = 0;
    unsigned int m;
   
    while (l <= h) {
        m = (l + h) / 2;
        pos = qqwry_setpos(data.raw, idx_first + m * 7);
        if (ip_addr < qqwry_read_uint32(pos)) {
            h = m - 1;
        } else {
            pos = qqwry_setpos(data.raw, qqwry_read_uint24(pos + 4));
            if (ip_addr > qqwry_read_uint32(pos)) {
                l = m + 1;
            } else {
                /* found */
                idx_found = idx_first + m * 7;
                break;
            }
        }
    }
    
    uint32_t record_offset;
    uint32_t country_offset;
    
    pos = qqwry_setpos(data.raw, idx_found + 4);
    record_offset = qqwry_read_uint24(pos);
    pos = qqwry_setpos(data.raw, record_offset + 4);
    uint8_t flag;    
    switch (flag = qqwry_read_uint8(pos)) {
        case 0x01:
            country_offset = qqwry_read_uint24(pos + 1);
            pos = qqwry_setpos(data.raw, country_offset);
            switch (flag = qqwry_read_uint8(pos)) {
                case 0x02:
                    /* Country information redirected again */
                    pos = qqwry_setpos(data.raw, qqwry_read_uint24(pos + 1));
                    result->country = (char *)(pos);
                    pos = qqwry_setpos(data.raw, country_offset + 4);
                    break;
                default:
                    result->country = (char *)(pos);
                    pos += strlen(result->country) + 1;
                    break;
            }            
            break;
        case 0x02:
            pos = qqwry_setpos(data.raw, qqwry_read_uint24(pos + 1));
            result->country = (char *)(pos);
            /* Skip 4 bytes ip and 4 bytes country offset */
            pos = qqwry_setpos(data.raw, record_offset + 8);
            break;
        default:
            result->country = (char *)(pos);
            pos += strlen(result->country) + 1;
            break;
    }
    
    /* Read area information */
    switch (flag = qqwry_read_uint8(pos)) {
        case 0x00:
            result->area = NULL;
            break;
        case 0x01:
        case 0x02:
            pos = qqwry_setpos(data.raw, qqwry_read_uint24(pos + 1));
            result->area = (char *)(pos);
            break;
        default:
            result->area = (char *)(pos);
            break;
    }
    
    return 0;
}
