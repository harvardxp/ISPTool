/**************************************************************************//**
 * @file     wdt.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 2018/04/23 11:32 $
 * @brief    NM1230 WDT driver header file
 *
 * @note
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NM1230_Device_Driver NM1230 Device Driver
  @{
*/

/** @addtogroup NM1230_WDT_Driver WDT Driver
  @{
*/

/** @addtogroup NM1230_WDT_EXPORTED_CONSTANTS WDT Exported Constants
  @{
*/
#define WDT_TIMEOUT_2POW4           (0UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^4 * WDT clocks */
#define WDT_TIMEOUT_2POW6           (1UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^6 * WDT clocks */
#define WDT_TIMEOUT_2POW8           (2UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^8 * WDT clocks */
#define WDT_TIMEOUT_2POW10          (3UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^10 * WDT clocks */
#define WDT_TIMEOUT_2POW12          (4UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^12 * WDT clocks */
#define WDT_TIMEOUT_2POW14          (5UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^14 * WDT clocks */
#define WDT_TIMEOUT_2POW16          (6UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^16 * WDT clocks */
#define WDT_TIMEOUT_2POW18          (7UL << WDT_CTL_TOUTSEL_Pos) /*!< WDT setting for timeout interval = 2^18 * WDT clocks */

/*@}*/ /* end of group NM1230_WDT_EXPORTED_CONSTANTS */


/** @addtogroup NM1230_WDT_EXPORTED_FUNCTIONS WDT Exported Functions
  @{
*/

/**
  * @brief This macro clear WDT time-out reset system flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_RESET_FLAG()  (WDT->CTL = (WDT->CTL & ~(WDT_CTL_IF_Msk | WDT_CTL_WKF_Msk | WDT_CTL_WTTOF_Msk)) | WDT_CTL_RSTF_Msk)

/**
  * @brief This macro clear WDT time-out interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_TIMEOUT_INT_FLAG() (WDT->CTL = (WDT->CTL & ~(WDT_CTL_RSTF_Msk | WDT_CTL_WKF_Msk | WDT_CTL_WTTOF_Msk)) | WDT_CTL_IF_Msk)


/**
  * @brief This macro clear WDT time-out wake-up system flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_TIMEOUT_WAKEUP_FLAG() (WDT->CTL = (WDT->CTL & ~(WDT_CTL_RSTF_Msk | WDT_CTL_IF_Msk | WDT_CTL_WTTOF_Msk)) | WDT_CTL_WKF_Msk)

/**
  * @brief This macro clear WDT time-out flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_TIMEOUT_TIMEOUT_FLAG() (WDT->CTL = (WDT->CTL & ~(WDT_CTL_RSTF_Msk | WDT_CTL_IF_Msk | WDT_CTL_WKF_Msk)) | WDT_CTL_WTTOF_Msk)


/**
  * @brief This macro indicate WDT time-out to reset system or not.
  * @return WDT reset system or not
  * @retval 0 WDT did not cause system reset
  * @retval 1 WDT caused system reset
  * \hideinitializer
  */
#define WDT_GET_RESET_FLAG() (WDT->CTL & WDT_CTL_RSTF_Msk ? 1 : 0)

/**
  * @brief This macro indicate WDT time-out interrupt occurred or not.
  * @return WDT time-out interrupt occurred or not
  * @retval 0 WDT time-out interrupt did not occur
  * @retval 1 WDT time-out interrupt occurred
  * \hideinitializer
  */
#define WDT_GET_TIMEOUT_INT_FLAG() (WDT->CTL & WDT_CTL_IF_Msk ? 1 : 0)

/**
  * @brief This macro indicate WDT time-out waked system up or not
  * @return WDT time-out waked system up or not
  * @retval 0 WDT did not wake up system
  * @retval 1 WDT waked up system
  * \hideinitializer
  */
#define WDT_GET_TIMEOUT_WAKEUP_FLAG() (WDT->CTL & WDT_CTL_WKF_Msk ? 1 : 0)


/**
  * @brief This macro indicate WDT time-out or not
  * @return WDT time-out time-out or not
  * @retval 0 WDT did not timeout
  * @retval 1 WDT timeout
  * \hideinitializer
  */
#define WDT_GET_TIMEOUT_TIMEOUT_FLAG() (WDT->CTL & WDT_CTL_WTTOF_Msk ? 1 : 0)


/**
  * @brief This macro is used to reset 18-bit WDT counter.
  * @details If WDT is activated and enabled to reset system, software must reset WDT counter
  *  before WDT time-out plus reset delay reached. Or WDT generate a reset signal.
  * \hideinitializer
  */
#define WDT_RESET_COUNTER() (WDT->CTL  = (WDT->CTL & ~(WDT_CTL_IF_Msk | WDT_CTL_WKF_Msk | WDT_CTL_RSTF_Msk | WDT_CTL_WTTOF_Msk)) | WDT_CTL_RSTCNT_Msk)

/**
 * @brief This function stops WDT counting and disable WDT module
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_Close(void)
{
    WDT->CTL = 0;
    return;
}

/**
 * @brief This function enables the WDT time-out interrupt
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_EnableInt(void)
{
    WDT->CTL |= WDT_CTL_INTEN_Msk;
    return;
}

/**
 * @brief This function disables the WDT time-out interrupt
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_DisableInt(void)
{
    /* Do not touch write 1 clear bits */
    WDT->CTL &= ~(WDT_CTL_INTEN_Msk | WDT_CTL_RSTF_Msk | WDT_CTL_IF_Msk | WDT_CTL_WKF_Msk | WDT_CTL_WTTOF_Msk) ;
    return;
}

void  WDT_Open(uint32_t u32TimeoutInterval,
               uint32_t u32ResetDelay,
               uint32_t u32EnableReset,
               uint32_t u32EnableWakeup);

/*@}*/ /* end of group NM1230_WDT_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NM1230_WDT_Driver */

/*@}*/ /* end of group NM1230_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif 

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
