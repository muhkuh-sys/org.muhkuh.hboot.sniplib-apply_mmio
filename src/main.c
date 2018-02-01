#include "netx_io_areas.h"

#include <string.h>


#define __IRQ_LOCK__ {__asm__ volatile ("cpsid   i, #0x13");}
#define __IRQ_UNLOCK__ {__asm__ volatile ("cpsie   i, #0x13");}


const unsigned char * const pucNetx4000Relaxed_MmioSel = (const unsigned char*)0x00024b54;
const unsigned char * const pucNetx4000Relaxed_MmioInv = (const unsigned char*)0x00024bbf;

const unsigned char * const pucNetx4000Full_MmioSel = (const unsigned char*)0x00024bc4;
const unsigned char * const pucNetx4000Full_MmioInv = (const unsigned char*)0x00024c2f;


#define ID_NETX4000_RELAXED 0x00108004
#define ID_NETX4000_FULL    0x0010b004

const unsigned long * const pulROMId = (const unsigned long*)0x04100020;



void start(void);
void __attribute__ ((section (".init_code"))) start(void)
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptMmioCtrlArea);
	const unsigned char *pucMmioSel;
	const unsigned char *pucMmioInv;
	unsigned long ulValue;
	unsigned int sizMmioCnt;
	unsigned long ulMmioCfg;


	pucMmioSel = NULL;
	pucMmioInv = NULL;

	ulValue = *pulROMId;
	if( ulValue==ID_NETX4000_RELAXED )
	{
		pucMmioSel = pucNetx4000Relaxed_MmioSel;
		pucMmioInv = pucNetx4000Relaxed_MmioInv;
	}
	else if( ulValue==ID_NETX4000_FULL )
	{
		pucMmioSel = pucNetx4000Full_MmioSel;
		pucMmioInv = pucNetx4000Full_MmioInv;
	}

	if( pucMmioSel!=NULL && pucMmioInv!=NULL )
	{
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
}

