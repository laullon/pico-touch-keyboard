#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
#define BLINK_NOT_MOUNTED 250
#define BLINK_MOUNTED 1000
#define BLINK_SUSPENDED 2500
