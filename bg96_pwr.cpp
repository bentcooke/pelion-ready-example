#include "bg96_pwr.h"
#include "platform/mbed_debug.h"
#include "DigitalOut.h"
#include "platform/mbed_wait_api.h"

void avnet_bg96_power(void)
{
	mbed::DigitalOut BG96_RESET(D7, 1);
	mbed::DigitalOut BG96_PWRKEY(D10, 0);
//	mbed::DigitalOut BG96_VBAT_3V8_EN(D11, 0);
//  3V8_EN is pulled to 5V and not connected as the reg is always enabled
//	BG96_VBAT_3V8_EN = 1;
	BG96_PWRKEY = 1;

	wait_ms(300);

	BG96_RESET = 0;
}

void init_cellular_power(void)
{
	printf("%s\r\n", __FUNCTION__);
	avnet_bg96_power();
	wait_ms(5000); // wait for the modem to establish a connection
	printf("done\n");
}

extern "C" void mbed_main(void)
{
	printf("%s\r\n", __FUNCTION__);
	init_cellular_power();
}
