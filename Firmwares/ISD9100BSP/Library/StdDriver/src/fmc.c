/**************************************************************************//**
 * @file     fmc.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 14/07/15 2:52p $
 * @brief    ISD9100 FMC driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "ISD9100.h"

// Must force optimization to make sure FMC write success.
#if   defined ( __CC_ARM )
  #pragma O0
#elif defined ( __ICCARM__ )
  #pragma optimize=low
#endif

/** @addtogroup ISD9100_Device_Driver ISD9100 Device Driver
  @{
*/

/** @addtogroup ISD9100_FMC_Driver FMC Driver
  @{
*/


/** @addtogroup ISD9100_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/

#if defined(__ICCARM__)
void FMC_ISPGO(void) 
{
    asm("CPSID I \n"       
        "MOVS  R0,#0x01 \n"  
        "LDR   R1,[pc,#12] \n"  
        "NOP \n"	 
        "NOP \n"	 
		"STR	R0,[R1] \n"  
		"ISB.W \n"
		"CPSIE I \n"
		"BX lr \n"  
		"DCW 0xC010 \n"
        "DCW 0x5000 \n"    
		);
}
#elif defined ( __CC_ARM )
__asm void FMC_ISPGO(void) 
{
	CPSID I				// Don't allow NVIC to interrupt a FMC write operation.
	MOVS    R0,#0x01 
	LDR     R1, =0x5000C010	
	NOP					// insert 2 NOP instruction to pipeline
	NOP					
	STR		R0,[R1]		// Trigger ISP command, 
	ISB.W				// Flush M0 pipeline.
    CPSIE I
	BX	lr
}
#endif

/**
  * @brief    Set boot source of next software reset
  * @param    i32BootSrc     1: will boot from LDROM; 0: will boot from APROM
  * @retval   None
  */
void FMC_SetBootSource (int32_t i32BootSrc)
{
    if (i32BootSrc == 1)
        FMC->ISPCTL |= FMC_ISPCTL_BS_Msk;
    else
        FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk;
}

/**
  * @brief    Disable all FMC functions
  */
void FMC_Close(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk;
}

/**
  * @brief    Disable User Configuration update function
  */
void FMC_DisableConfigUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk;
}

/**
  * @brief    Disable LDROM update function
  */
void FMC_DisableLDUpdate(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_LDUEN_Msk;
}

/**
  * @brief    Enable User Configuration update function
  */
void FMC_EnableConfigUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_CFGUEN_Msk;
}

/**
  * @brief    Enable LDROM update function
  */
void FMC_EnableLDUpdate(void)
{
    FMC->ISPCTL |= FMC_ISPCTL_LDUEN_Msk;
}

int32_t FMC_Erase(uint32_t u32PageAddr)
{
    FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADDR = u32PageAddr;
	
   	FMC_ISPGO();	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
	if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
	{
		FMC->ISPCTL = (FMC->ISPCTL & (~FMC_ISPCTL_ISPFF_Msk)) | FMC_ISPCTL_ISPFF_Msk;
		return -1;
	}
    return 0;
}

/**
  * @brief    get the current boot source
  * @retval   0   This chip is currently booting from APROM
  * @retval   1   This chip is currently booting from LDROM
  */
int32_t FMC_GetBootSource (void)
{
    if (FMC->ISPCTL & FMC_ISPCTL_BS_Msk)
        return 1;
    else
        return 0;
}

/**
  * @brief    Enable FMC ISP function
  */
void FMC_Open(void)
{
    FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk;
}

/**
  * @brief    Read a word from specified flash address.
  * @param    u32Addr   Flash word address. Must be a word aligned address.
  * @retval   The word data stored in the flash address "u32Addr".
  */
uint32_t FMC_Read(uint32_t u32Addr)
{
    FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADDR = u32Addr;

   	FMC_ISPGO();	

	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
    return FMC->ISPDAT;
}

/**
  * @brief    Read company ID.
  * @retval   The company ID.
  */
uint32_t FMC_ReadCID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_CID;
    FMC->ISPADDR = 0x0;

    FMC_ISPGO();
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
	return FMC->ISPDAT;
}

/**
  * @brief    Read device ID.
  * @retval   The device ID.
  */
uint32_t FMC_ReadDID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_DID;
    FMC->ISPADDR = 0;
	
    FMC_ISPGO();
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
    return FMC->ISPDAT;
}

/**
  * @brief    Get the base address of Data Flash if enabled.
  * @retval   The base address of Data Flash
  */
uint32_t FMC_ReadDataFlashBaseAddr(void)
{
    return FMC->DFBA;
}

/**
  * @brief    Writes a word data to specified flash address.
  * @param    u32Addr   Destination address
  * @param    u32Data   Word data to be written
  */
void FMC_Write(uint32_t u32Addr, uint32_t u32Data)
{
    FMC->ISPCMD = FMC_ISPCMD_WRITE;
    FMC->ISPADDR = u32Addr;
    FMC->ISPDAT = u32Data;
	
    FMC_ISPGO();
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
}

/**
  * @brief    Read the User Configuration words.
  * @param    u32Config: The word array to store data.
  * @param    u32Count: Maximum length of "u32Config".
  * @retval   0    Success
  * @retval   -1   User Configuration CRC check error
  */
int32_t FMC_ReadConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int         i;

    for (i = 0; i < u32Count; i++) {
        u32Config[i] = FMC_Read(FMC_CONFIG_BASE + i*4);
    }

    //if (FMC->ISPSTS & FMC_ISPSTS_CFGCRCF_Msk)
      //  return -1;

    return 0;
}

/**
  * @brief    Write User Configuration
  * @param    u32Config   The word array to store data. MUST be a four word array.
  * @param    u32Count    MUST be 4.
  * @retval   0    Success
  * @retval   -1   Failed
  */
int32_t FMC_WriteConfig(uint32_t *u32Config, uint32_t u32Count)
{
    uint32_t  i;

    FMC_Erase(FMC_CONFIG_BASE);

    for (i = 0; i < 4; i++) {
        FMC_Write(FMC_CONFIG_BASE + i * 4, u32Config[i]);
        if(FMC_Read(FMC_CONFIG_BASE + i * 4) != u32Config[i])
            return -1;
    }

    return 0;
}

/*@}*/ /* end of group ISD9100_FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group ISD9100_FMC_Driver */

/*@}*/ /* end of group ISD9100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/


