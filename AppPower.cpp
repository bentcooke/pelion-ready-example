#include "AppPower.h"
#include "platform/mbed_debug.h"
#include "DigitalOut.h"
#include "platform/mbed_wait_api.h"

#if defined(MBED_CONF_APP_SWO_ENABLED) && (MBED_CONF_APP_SWO_ENABLED == 1)
#include "SerialWireOutput.h"

mbed::FileHandle *mbed::mbed_override_console(int fd)
{
    static mbed::SerialWireOutput swo_serial;
    return &swo_serial;
}
#endif // MBED_CONF_APP_SWO_ENABLED

#if defined(MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT)
#define AVNET_BG96 1
#define GEMALTO_CINTERION 2
#endif

#if MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT == AVNET_BG96
void avnet_bg96_power(void)
{
	mbed::DigitalOut BG96_RESET(D7, 1);
	mbed::DigitalOut BG96_PWRKEY(D10, 0);
	mbed::DigitalOut BG96_VBAT_3V8_EN(D11, 0);

	BG96_VBAT_3V8_EN = 1;
	BG96_PWRKEY = 1;

	wait_ms(300);

	BG96_RESET = 0;
}
#endif

#if MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT == GEMALTO_CINTERION
void gemalto_cinterion_power(void)
{
	mbed::DigitalOut cell_power(D9, 0);

	// power off the cellular modem
	cell_power.write(0);
	wait(3);

	cell_power.write(1);

	wait_ms(500);
	// reset
	cell_power.write(0);

	wait_ms(500);
	cell_power.write(1);
}
#endif

void init_cellular_power(void)
{
	printf("%s\r\n", __FUNCTION__);
#if MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT == AVNET_BG96
	avnet_bg96_power();
#elif MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT == GEMALTO_CINTERION
	gemalto_cinterion_power();
#endif
	wait_ms(5000); // wait for the modem to establish a connection
	printf("done\n");
}

extern "C" void mbed_main(void)
{
#if defined(MBED_CONF_APP_PROVIDE_DEFAULT_VARIANT)
	printf("%s\r\n", __FUNCTION__);
	init_cellular_power();
#endif
}
