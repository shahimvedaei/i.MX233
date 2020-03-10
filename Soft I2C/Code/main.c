#include "SoftI2CMaster.h"

int main(void)
{
	i2c_init();
	
	beginTransmission(0x5A);
	my_write(0xAA, 1);
	endTransmission();

	return 0;
}
