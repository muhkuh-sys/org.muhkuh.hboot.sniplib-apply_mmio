#include "netx_io_areas.h"

#define __IRQ_LOCK__ {__asm__ volatile ("cpsid   i, #0x13");}
#define __IRQ_UNLOCK__ {__asm__ volatile ("cpsie   i, #0x13");}


const unsigned char * const pucMmioSel = (const unsigned char*)0x00024b54;
const unsigned char * const pucMmioInv = (const unsigned char*)0x00024bbf;


void start(void);
void __attribute__ ((section (".init_code"))) start(void)
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptMmioCtrlArea);
	unsigned int sizMmioCnt;
	unsigned long ulMmioCfg;


	__IRQ_LOCK__;
	/* Set all MMIOs to the default values. */
	for(sizMmioCnt=0; sizMmioCnt<(sizeof(ptMmioCtrlArea->aulMmio_cfg)/sizeof(ptMmioCtrlArea->aulMmio_cfg[0])); ++sizMmioCnt)
	{
		ulMmioCfg  = pucMmioSel[sizMmioCnt];
		/* Add the mmio_in_inv and mmio_out_inv information. */
		ulMmioCfg |= ((unsigned long)(pucMmioInv[sizMmioCnt])) << HOSTSRT(mmio0_cfg_mmio_out_inv);
		ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;
		ptMmioCtrlArea->aulMmio_cfg[sizMmioCnt] = ulMmioCfg;
	}
	__IRQ_UNLOCK__;
}

