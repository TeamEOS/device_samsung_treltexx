/*
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
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

#include <hardware/memtrack.h>

#include "memtrack_exynos5.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define min(x, y) ((x) < (y) ? (x) : (y))

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000 /* Assume 4k pages on ARM */
#endif

int ion_memtrack_get_memory(pid_t pid,
                            int type,
                            struct memtrack_record *records,
                            size_t *num_records)
{
    struct stat sb;
    const char *ion_path = "/sys/kernel/debug/ion/clients";
    const char *fmt = "%s/%d-0";
    char ion_file[64] = { 0 };
    FILE *fp;
    int cmp;
    int rc;
    char line[1024];
    int result;

    if (*num_records == 0) {
        return -EINVAL;
    }

    rc = lstat(ion_path, &sb);
    if (rc != 0) {
        ion_path = "/sys/kernel/debug/ion/";
        fmt = "%s/%d";
    }

    snprintf(ion_file, sizeof(ion_file), fmt, ion_path, pid);

    fp = fopen(ion_file, "r");
    if (fp == NULL) {
        return -errno;
    }

    *num_records = 1;

    for(;;) {
        unsigned int x;

        if (fgets(line, sizeof(line), fp) == NULL) {
            break;
        }

        /* Format:
         * ion_noncontig_he:         19304448
         */
        cmp = strncmp(line, "ion_noncontig_he", 16);
        if (cmp != 0) {
            continue;
        }

        rc = sscanf(line, "%*s %u", &x);
        if (rc != 1) {
            break;
        }

        records[0].flags = MEMTRACK_FLAG_PRIVATE|MEMTRACK_FLAG_NONSECURE;
        records[0].size_in_bytes = x;
    }

    fclose(fp);

    return 0;
}
