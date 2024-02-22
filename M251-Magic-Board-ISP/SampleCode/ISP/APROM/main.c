/***************************************************************************//**
 * @file     main.c
 * @version  V3.03
 * @brief    Demonstrate how to update chip flash data through USB HID interface
             between chip USB device and PC.
             Nuvoton NuMicro ISP Programming Tool is also required in this
             sample code to connect with chip USB device and assign update file
             of Flash.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2017-2018 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "targetdev.h"
#include "hid_transfer.h"
//note 
//fix aprom size
//remove dataflash 
#define TRIM_INIT           (SYS_BASE + 0x118)
#define PLL_CLOCK           48000000

/* This is a dummy implementation to replace the same function in clk.c for size limitation. */
uint32_t CLK_GetPLLClockFreq(void)
{
    return PLL_CLOCK;
}

/*--------------------------------------------------------------------------*/
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable Internal RC clock */
    CLK->PWRCTL |= CLK_PWRCTL_HIRCEN_Msk;

    /* Waiting for external XTAL clock ready */
    while (!(CLK->STATUS & CLK_STATUS_HIRCSTB_Msk));

    CLK->CLKSEL0 &= (~CLK_CLKSEL0_HCLKSEL_Msk);
    CLK->CLKSEL0 |= CLK_CLKSEL0_HCLKSEL_HIRC;
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */

    SystemCoreClock = __HIRC;             // HCLK
    CyclesPerUs     = SystemCoreClock / 1000000;  // For SYS_SysTickDelay()
    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;

    /* Enable module clock */
    //CLK->APBCLK0 |= CLK_APBCLK0_USBDCKEN_Msk;
    CLK->AHBCLK |= CLK_AHBCLK_ISPCKEN_Msk | CLK_AHBCLK_GPACKEN_Msk | CLK_AHBCLK_EXSTCKEN_Msk;
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PA.12 ~ PA.14 to input mode */
    PA->MODE &= ~(GPIO_MODE_MODE12_Msk | GPIO_MODE_MODE13_Msk | GPIO_MODE_MODE14_Msk);
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA12MFP_Msk | SYS_GPA_MFPH_PA13MFP_Msk | SYS_GPA_MFPH_PA14MFP_Msk | SYS_GPA_MFPH_PA15MFP_Msk);
}

volatile uint32_t totallen, cksum;
void USBD_IRQHandler(void);
__attribute__((aligned(4))) static uint8_t aprom_buf[FMC_FLASH_PAGE_SIZE];
static uint16_t Checksum (unsigned char *buf, int len)
{
    int i;
    uint16_t c;

    for (c=0, i=0; i < len; i++) {
        c += buf[i];
    }
    return (c);
}

static uint16_t CalCheckSum(uint32_t start, uint32_t len)
{
	int i;
	uint16_t lcksum = 0;
	
	for(i = 0; i < len; i+=FMC_FLASH_PAGE_SIZE)
	{
		ReadData(start + i, start + i + FMC_FLASH_PAGE_SIZE, (uint32_t*)aprom_buf);
		if(len - i >= FMC_FLASH_PAGE_SIZE)
			lcksum += Checksum(aprom_buf, FMC_FLASH_PAGE_SIZE);
		else
			lcksum += Checksum(aprom_buf, len - i);
	}
    
    return lcksum;
    
}
unsigned char usb_detect_flag=0;
/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    uint32_t u32TrimInit;

    /* Unlock write-protected registers to operate SYS_Init and FMC ISP function */
    SYS_UnlockReg();

    /* Init system and multi-function I/O */
    SYS_Init();

usb_detect_flag=0;


        /* Using polling mode and Removed Interrupt Table to reduce code size for M251 */
   
				while(1)	
				{
				
					CLK->APBCLK0 |= CLK_APBCLK0_USBDCKEN_Msk;
					if (USBD_IS_ATTACHED())
					{
						usb_detect_flag++;
					}
					else
					{
					  usb_detect_flag=0;
					}
					if (USBD_IS_ATTACHED()&&usb_detect_flag>2)
					{
						
           /* Open USB controller */
           USBD_Open(&gsInfo, HID_ClassRequest, NULL);

          /*Init Endpoint configuration for HID */
          HID_Init();

        /* Start USB device */
        USBD_Start();

        /* Backup default trim value */
        u32TrimInit = M32(TRIM_INIT);

        /* Clear SOF */
        USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);
					
					
					  //check vbus is plug in
					 while (USBD_IS_ATTACHED())
					 {
					    FMC->ISPCTL |= FMC_ISPCTL_ISPEN_Msk;
              FMC->ISPCTL |= FMC_ISPCTL_APUEN_Msk;
              /* Start USB trim function if it is not enabled. */
              if ((SYS->HIRCTRIMCTL & SYS_HIRCTRIMCTL_FREQSEL_Msk) != 0x1)
              {
                /* Start USB trim only when USB signal arrived */
                if (USBD->INTSTS & USBD_INTSTS_SOFIF_Msk)
                {
                    /* Clear SOF */
                    USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);

                    /*
                        USB clock trim function:
                        HIRC Trimming with boundary function enhances robustility
                        and keeps HIRC in right frequency while receiving unstable USB signal
                    */
                    SYS->HIRCTRIMCTL = (0x1 << SYS_HIRCTRIMCTL_REFCKSEL_Pos)
                                       | (0x1 << SYS_HIRCTRIMCTL_FREQSEL_Pos)
                                       | (0x0 << SYS_HIRCTRIMCTL_LOOPSEL_Pos)
                                       | (0x1 << SYS_HIRCTRIMCTL_BOUNDEN_Pos)
                                       | (10  << SYS_HIRCTRIMCTL_BOUNDARY_Pos);
                }
            }

            /* Disable USB Trim when any error found */
            if (SYS->HIRCTRIMSTS & (SYS_HIRCTRIMSTS_CLKERIF_Msk | SYS_HIRCTRIMSTS_TFAILIF_Msk))
            {
                /* Init TRIM */
                M32(TRIM_INIT) = u32TrimInit;

                /* Disable USB clock trim function */
                SYS->HIRCTRIMCTL = 0;

                /* Clear trim error flags */
                SYS->HIRCTRIMSTS = SYS_HIRCTRIMSTS_CLKERIF_Msk | SYS_HIRCTRIMSTS_TFAILIF_Msk;

                /* Clear SOF */
                USBD_CLR_INT_FLAG(USBD_INTSTS_SOFIF_Msk);
            }

            // polling USBD interrupt flag
            USBD_IRQHandler();

            if (bUsbDataReady == TRUE)
            {
                ParseCmd((uint8_t *)usb_rcvbuf, 64);
                EP2_Handler();
                bUsbDataReady = FALSE;
            }
        }

			}
					CLK->APBCLK0 &= ~CLK_APBCLK0_USBDCKEN_Msk;
			}

    /* Trap the CPU */
    while (1);
}
