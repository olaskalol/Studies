#include "../inc/accelerator.h"
#include <stddef.h>
#include <string.h>
#include "../inc/ui.h"
#include "acc.h"

static int32_t xoff = 0;
static int32_t yoff = 0;
static int32_t zoff = 0;
static int8_t x = 0;
static int8_t y = 0;
static int8_t z = 0;

void accelerator_init(void) {
	acc_init();
	acc_read(&x, &y, &z);
	xoff = -(int32_t)x;
	yoff = -(int32_t)y;
	zoff = (int32_t)64 - (int32_t)z;
}

uint8_t* read_accelerator(void) {
	static uint8_t buf[16];
	acc_read(&x, &y, &z);
	x = x + (int8_t)xoff;
	y = y + (int8_t)yoff;
	z = z + (int8_t)zoff;

	if ((x > 15) || (x < -15) || (y > 15) || (y < -15)) {
		(void)strcpy((char*)buf, "tilted");
	} else {
		(void)strcpy((char*)buf, "stable");
	}
	return buf;
}
