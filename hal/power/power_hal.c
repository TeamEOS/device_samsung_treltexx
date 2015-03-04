/*
 * Copyright (c) 2014 The Android Open Source Project
 * Copyright (c) 2014 Christopher N. Hesse <raymanfx@gmail.com>
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

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "Exynos5433PowerHAL"
/* #define LOG_NDEBUG 0 */
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define BOOSTPULSE_PATH "/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse"
#define TOUCHSCREEN_PATH "/sys/class/input/input2"

struct exynos5433_power_module {
    struct power_module base;
    pthread_mutex_t lock;
    int boostpulse_fd;
    int boostpulse_warned;
    const char *touchscreen_power_path;
};

static void sysfs_write(const char *path, char *s)
{
    char errno_str[64];
    int len;
    int fd;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error opening %s: %s\n", path, errno_str);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error writing to %s: %s\n", path, errno_str);
    }

    close(fd);
}

static void init_touchscreen_power_path(struct exynos5433_power_module *exynos5433_pwr)
{
    const char dir[] = TOUCHSCREEN_PATH;
    const char filename[] = "enabled";
    struct dirent *de;
    size_t pathsize;
    char errno_str[64];
    char *path;
    DIR *d;

    d = opendir(dir);
    if (d == NULL) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error opening directory %s: %s\n", dir, errno_str);
        return;
    }
    while ((de = readdir(d)) != NULL) {
        if (strncmp("input", de->d_name, 5) == 0) {
            pathsize = strlen(dir) + strlen(de->d_name) + sizeof(filename) + 2;
            path = malloc(pathsize);
            if (path == NULL) {
                strerror_r(errno, errno_str, sizeof(errno_str));
                ALOGE("Out of memory: %s\n", errno_str);
                return;
            }
            snprintf(path, pathsize, "%s/%s/%s", dir, de->d_name, filename);
            exynos5433_pwr->touchscreen_power_path = path;
            goto done;
        }
    }
    ALOGE("Error failed to find input dir in %s\n", dir);
done:
    closedir(d);
}

/*
 * This function performs power management setup actions at runtime startup,
 * such as to set default cpufreq parameters.  This is called only by the Power
 * HAL instance loaded by PowerManagerService.
 */
static void exynos5433_power_init(struct power_module *module)
{
    struct exynos5433_power_module *exynos5433_pwr = (struct exynos5433_power_module *) module;
    struct stat sb;
    int rc;

    /*
     * You get the values reading the hexdump from the biniary power module or
     * strace
     */
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/multi_enter_load", "800");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/single_enter_load", "200");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/param_index", "0");

    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/timer_rate", "20000");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/timer_slack", "20000");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/min_sample_time", "40000");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/hispeed_freq", "900000");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/go_hispeed_load", "84");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/target_loads", "75");
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/above_hispeed_delay", "19000");

    /* was emtpy in hex so a value already defined. */
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse_duration", "500000");

    /* The CPU might not be turned on, then it doesn't make sense to configure it. */
    rc = stat("/sys/devices/system/cpu/cpu4/cpufreq/interactive", &sb);
    if (rc < 0) {
        ALOGE("CPU2 is offline, skip init\n");
        goto out;
    }

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/multi_enter_load", "360");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/multi_enter_time", "99000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/multi_exit_load", "240");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/multi_exit_time", "299000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/single_enter_load", "95");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/single_enter_time", "199000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/single_exit_load", "60");
    /* was emtpy in hex so a value already defined. */
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/single_exit_time", "299000");

    /* was emtpy in hex so a value already defined. */
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/param_index", "0");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/timer_rate", "20000");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/timer_slack", "20000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/min_sample_time", "40000");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/hispeed_freq", "1000000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/go_hispeed_load", "89");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/target_loads", "80 1000000:81 1400000:87 1700000:90");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/above_hispeed_delay", "59000 1200000:119000 1700000:19000");

    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/interactive/boostpulse_duration", "59000");

out:
    init_touchscreen_power_path(exynos5433_pwr);
}

/* This function performs power management actions upon the system entering
 * interactive state (that is, the system is awake and ready for interaction,
 * often with UI devices such as display and touchscreen enabled) or
 * non-interactive state (the system appears asleep, display usually turned
 * off).  The non-interactive state is usually entered after a period of
 * inactivity, in order to conserve battery power during such inactive periods.
 */
static void exynos5433_power_set_interactive(struct power_module *module, int on)
{
    struct exynos5433_power_module *exynos5433_pwr = (struct exynos5433_power_module *) module;
    char buf[80];
    int ret;

    ALOGV("power_set_interactive: %d\n", on);

    /*
     * Lower maximum frequency when screen is off.  CPU 0 and 1 share a
     * cpufreq policy.
     * NOT FOUND IN STOCK POWER HAL. DISABLING.
    sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq",
                on ? "1700000" : "800000");
    sysfs_write("/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq",
                on ? "1700000" : "800000");
     */

    sysfs_write(exynos5433_pwr->touchscreen_power_path, on ? "1" : "0");

    ALOGV("power_set_interactive: %d done\n", on);
}

static int boostpulse_open(struct exynos5433_power_module *exynos5433_pwr)
{
    char errno_str[64];

    pthread_mutex_lock(&exynos5433_pwr->lock);

    if (exynos5433_pwr->boostpulse_fd < 0) {
        exynos5433_pwr->boostpulse_fd = open(BOOSTPULSE_PATH, O_WRONLY);
        if (exynos5433_pwr->boostpulse_fd < 0) {
            if (!exynos5433_pwr->boostpulse_warned) {
                strerror_r(errno, errno_str, sizeof(errno_str));
                ALOGE("Error opening %s: %s\n", BOOSTPULSE_PATH, errno_str);
                exynos5433_pwr->boostpulse_warned = 1;
            }
        }
    }

    pthread_mutex_unlock(&exynos5433_pwr->lock);
    return exynos5433_pwr->boostpulse_fd;
}

/*
 * This functions is called to pass hints on power requirements, which may
 * result in adjustment of power/performance parameters of the cpufreq governor
 * and other controls.
 */
static void exynos5433_power_hint(struct power_module *module, power_hint_t hint,
                             void *data)
{
    struct exynos5433_power_module *exynos5433_pwr = (struct exynos5433_power_module *) module;
    char errno_str[64];
    int len;

    switch (hint) {
        case POWER_HINT_INTERACTION:
            if (boostpulse_open(exynos5433_pwr) >= 0) {
                len = write(exynos5433_pwr->boostpulse_fd, "1", 1);

                if (len < 0) {
                    strerror_r(errno, errno_str, sizeof(errno_str));
                    ALOGE("Error writing to %s: %s\n", BOOSTPULSE_PATH, errno_str);
                }
            }

            break;

        case POWER_HINT_VSYNC:
            break;

        default:
            break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct exynos5433_power_module HAL_MODULE_INFO_SYM = {
    base: {
        common: {
            tag: HARDWARE_MODULE_TAG,
            module_api_version: POWER_MODULE_API_VERSION_0_2,
            hal_api_version: HARDWARE_HAL_API_VERSION,
            id: POWER_HARDWARE_MODULE_ID,
            name: "EXYNOS5433 Power HAL",
            author: "The Android Open Source Project",
            methods: &power_module_methods,
        },

        init: exynos5433_power_init,
        setInteractive: exynos5433_power_set_interactive,
        powerHint: exynos5433_power_hint,
    },

    lock: PTHREAD_MUTEX_INITIALIZER,
    boostpulse_fd: -1,
    boostpulse_warned: 0,
};
