/***************************************************************************//**
 * @file     isp_user.c
 * @brief    ISP Command source file
 * @version  0x32
 * @date     14, June, 2017
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2017-2018 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "string.h"
#include "isp_user.h"

__attribute__((aligned(4))) uint8_t response_buff[64];
__attribute__((aligned(4))) static uint8_t aprom_buf[FMC_FLASH_PAGE_SIZE];
uint32_t bUpdateApromCmd;
uint32_t g_apromSize, g_dataFlashAddr, g_dataFlashSize;

static uint16_t Checksum(unsigned char *buf, int len)
{
    int i;
    uint16_t c;

    for (c = 0, i = 0 ; i < len; i++)
    {
        c += buf[i];
    }

    return (c);
}

static uint16_t CalCheckSum(uint32_t start, uint32_t len)
{
    int i;
    register uint16_t lcksum = 0;

    for (i = 0; i < len; i += FMC_FLASH_PAGE_SIZE)
    {
        ReadData(start + i, start + i + FMC_FLASH_PAGE_SIZE, (uint32_t *)aprom_buf);

        if (len - i >= FMC_FLASH_PAGE_SIZE)
        {
            lcksum += Checksum(aprom_buf, FMC_FLASH_PAGE_SIZE);
        }
        else
        {
            lcksum += Checksum(aprom_buf, len - i);
        }
    }

    return lcksum;
}

int ParseCmd(unsigned char *buffer, uint8_t len)
{
    static uint32_t StartAddress, StartAddress_bak, TotalLen, TotalLen_bak, LastDataLen, g_packno = 1;
    uint8_t *response;
    uint16_t lcksum;
    uint32_t lcmd, srclen, i;
    unsigned char *pSrc;
    static uint32_t gcmd;
    response = response_buff;
    pSrc = buffer;
    srclen = len;
    lcmd = inpw(pSrc);
    outpw(response + 4, 0);
    pSrc += 8;
    srclen -= 8;


    if (lcmd == CMD_SYNC_PACKNO)
    {
        g_packno = inpw(pSrc);
    }

    if ((lcmd) && (lcmd != CMD_RESEND_PACKET))
    {
        gcmd = lcmd;
    }

    if (lcmd == CMD_GET_FWVER)
    {
        response[8] = FW_VERSION;//version 2.3
    }
    else if (lcmd == CMD_GET_DEVICEID)
    {
        outpw(response + 8, SYS->PDID);
        goto out;
    }
   
    else if (lcmd == CMD_CONNECT)
    {
        g_packno = 1;
        goto out;
    }
   
    else if (lcmd == CMD_GET_FLASHMODE)
    {
        //return 1: APROM, 2: LDROM
        outpw(response + 8, (FMC->ISPCTL & 0x2) ? 2 : 1);
    }
		else if (lcmd == CMD_APROM_DATA_ERASE)
    {
     //ERASE AP PAGE  
		  EraseAP(user_aprom_data_address, 512);		//it is fix in 0x20000	
    }
		else if (lcmd == CMD_APROM_DATA_WRITE)
    {
       WriteData(user_aprom_data_address, user_aprom_data_address + 48, (uint32_t *)pSrc); 
    }
		else if (lcmd == CMD_APROM_IMAGE_ERASE)
    {
     //ERASE AP PAGE  
		  EraseAP(user_aprom_image_address, 512);			
    }
		else if (lcmd == CMD_APROM_IMAGE_WRITE)
    {
       WriteData(user_aprom_image_address, user_aprom_image_address + 48, (uint32_t *)pSrc); 
    }
		else if (lcmd == CMD_RTC_UPDATE)
    {
      printf("0x%x\n\r",(uint8_t *)pSrc); //year 
			printf("0x%x\n\r",(uint8_t *)pSrc+1); //month
			printf("0x%x\n\r",(uint8_t *)pSrc+2); //day
			printf("0x%x\n\r",(uint8_t *)pSrc+3); //hour
			printf("0x%x\n\r",(uint8_t *)pSrc+4); //sec
			printf("0x%x\n\r",(uint8_t *)pSrc+5); //min
    }
		else if (lcmd == CMD_RUN_APROM || lcmd == CMD_RUN_LDROM || lcmd == CMD_RESET)
    {
        outpw(&SYS->RSTSTS, 3);//clear bit

        /* Set BS */
        if (lcmd == CMD_RUN_APROM)
        {
            i = (FMC->ISPCTL & 0xFFFFFFFC);
        }
        else if (lcmd == CMD_RUN_LDROM)
        {
            i = (FMC->ISPCTL & 0xFFFFFFFC);
            i |= 0x00000002;
        }
        else
        {
            i = (FMC->ISPCTL & 0xFFFFFFFE);//ISP disable
        }

        outpw(&FMC->ISPCTL, i);
        NVIC_SystemReset();

        /* Trap the CPU */
        while (1);
    }

out:
    lcksum = Checksum(buffer, len);
    outps(response, lcksum);
    ++g_packno;
    outpw(response + 4, g_packno);
    g_packno++;
    return 0;
}

