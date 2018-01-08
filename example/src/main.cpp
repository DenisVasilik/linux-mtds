#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "ProtoDefs.h"
#include "MtdsCore.h"
#include "MtdsHal.h"
#include "MyDisp.h"

extern void setup();
extern void loop();

int main ()
{
	printf("MTDS Test\r\n");

	setup();
	while(1)
	{
		loop();
	}

	return 0;
}
