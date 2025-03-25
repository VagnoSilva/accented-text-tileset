#include <genesis.h>
#include "text.h"
#include "resources.h"


int main(bool hard) //--- MAIN ---//
{
    //inicializacao da VDP (Video Display Processor)
	SYS_disableInts();
	 VDP_init();
	 VDP_setScreenWidth320();
	 VDP_setScreenHeight224();
	  VDP_setPlaneSize(64, 64, FALSE);
	SYS_enableInts();

	PAL_setPalette(PAL0, customFont_PAL.data, DMA);
	initFont(&customFont, 0);
	drawText(2, 10, "Uma nova organização criminosa vem");
	drawText(2, 12, "ganhando força e corrompendo tudo o");
	drawText(2, 14, "que há de bom na cidade.");

    while(TRUE) //MAIN LOOP
    {

        SYS_doVBlankProcess();// // Sincroniza com o VBlank
    }

    return 0;
}
