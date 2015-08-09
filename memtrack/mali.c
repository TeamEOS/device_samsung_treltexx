/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2015 Andreas Schneider <asn@cryptomilk.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/mman.h>

#include <hardware/memtrack.h>

#include "memtrack_exynos5.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define min(x, y) ((x) < (y) ? (x) : (y))

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000 /* Assume 4k pages on ARM */
#endif

struct memtrack_record record_templates[] = {
    {
        .flags = MEMTRACK_FLAG_SMAPS_ACCOUNTED |
                 MEMTRACK_FLAG_PRIVATE |
                 MEMTRACK_FLAG_NONSECURE,
    },
    {
        .flags = MEMTRACK_FLAG_SMAPS_UNACCOUNTED |
                 MEMTRACK_FLAG_PRIVATE |
                 MEMTRACK_FLAG_NONSECURE,
    },
};

int mali_memtrack_get_memory(pid_t pid,
                             int type,
                             struct memtrack_record *records,
                             size_t *num_records)
{
    size_t allocated_records = min(*num_records, ARRAY_SIZE(record_templates));
    struct dirent *de;
    DIR *dp;
    FILE *fp;
    char pid_str[8];
    char mali_file[32];
    char line[1024];
    int len;
    int total = 0;
    int native_buffer = 0;

    *num_records = ARRAY_SIZE(record_templates);

    /* fastpath to return the necessary number of records */
    if (allocated_records == 0) {
        return 0;
    }

    dp = opendir("/d/mali/mem/");
    if (dp == NULL) {
        return -errno;
    }

    len = snprintf(pid_str, sizeof(pid_str), "%d", pid);
    if (len <= 0) {
        return -EINVAL;
    }

    for (de = readdir(dp); de != NULL; de = readdir(dp)) {
        int cmp;

        cmp = strncmp(de->d_name, pid_str, len);
        if (cmp != 0) {
            continue;
        }

        snprintf(mali_file, sizeof(mali_file), "/d/mali/mem/%s", pid_str);
        break;
    }
    closedir(dp);

    fp = fopen(mali_file, "r");
    if (fp == NULL) {
        closedir(dp);
        return -errno;
    }

    memcpy(records,
           record_templates,
           sizeof(struct memtrack_record) * allocated_records);

    for (;;) {
        int rc;
        char label1[16] = { 0 };
        char label2[16] = { 0 };
        unsigned int size = 0;

        if (fgets(line, sizeof(line), fp) == NULL) {
            break;
        }

        rc = sscanf(line, "%s %*s %*s %u \n", label1, &size);
        if (rc == 2) {
            int cmp = strcmp(label1, "Total");
            if (cmp == 0) {
                /* TODO Check integer wrap */
                total += size;
            }
            continue;
        }

        rc = sscanf(line, "%*s %s %s %*s %*s %u \n", label1, label2, &size);
        if (rc == 3) {
            int cmp1 = strcmp(label1, "Native");
            int cmp2 = strcmp(label2, "Buffer");

            if (cmp1 == 0 && cmp2 == 0) {
                /* TODO Check integer wrap */
                native_buffer += size;
            }
        }
    }

    records[0].size_in_bytes = native_buffer;
    /* TODO Check integer wrap */
    if (allocated_records == 2) {
        records[1].size_in_bytes = total - native_buffer;
    }

    fclose(fp);

    return 0;
}
