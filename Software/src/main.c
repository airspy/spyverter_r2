#include <C8051F330.H>
#include "hw_config.h"

extern void setup(void);
extern void loop(void);

int main(void)
{
	setup();
	while(1)
		loop();
	return 0;
}
