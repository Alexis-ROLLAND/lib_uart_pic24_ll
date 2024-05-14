/**
 * @file    lib_uart_pic24_ll.h 
 * @author 	Alexis ROLLAND
 * @date	2024-02
 * @brief 	Header file for low level uart driver
 *          Target : PIC24 (PIC24FJ128GA010)
 */

//------------------------------------------------------------------------------
#ifndef	__LIB_UART_PIC24LL_H__
#define	__LIB_UART_PIC24LL_H__
#include <xc.h>

#ifndef	NULL
#define	NULL    (void*)0
#endif

#define UTX_BF_MASK (0x0001<<9) /**< Location of the UTXBF bit (UxSTA:9 */

typedef enum    {   
    _UART1,     /**< Value for UART1 */
    _UART2      /**< Value for UART2 */
} uart_id_t;
               
typedef enum    {
    UART_TX_NON_BLOCKING_MODE,  /**< Tx mode is non Blocking mode (no wait)  */
    UART_TX_BLOCKING_MODE       /**< Tx mode is Blocking mode (wait until Tx buffer is free) */
} uart_tx_blocking_t;

/** Type uart_err_t
 * Liste les divers codes de retour des fonctions uart low level
 */
typedef enum    {   
    UART_OK,            /**< Succes value */
    UART_UNKNOWN_UART,  /**< The UART ID is unknown  */
    UART_TX_FIFO_FULL,  /**< TX_FIFO is full */
    UART_BAD_PRIO,      /**< Interrupt priority level outside the 0-7 range */
    UART_ERROR          /**< Non specific Error */
} uart_err_t; 

 /** Type uart_config_t
 * Contient les valeurs à stocker dans les registres 
 * UxMODE, UxSTA et UxBRG pour un UART donné
 */
typedef struct{
    uart_id_t   uartID;                 /**< UART ID                */
    uint16_t    UxMODE;                 /**< UxMODE value */
    uint16_t    UxSTA;                  /**< UxSTA value */
    uint16_t    UxBRG;                  /**< UxBRG value */
    uint8_t     RxIrqPrio;              /**< Rx Interrupt priority level (between 0 and 7) */
}   uart_config_t;

 /** Type uart_desc_t
 * UART descritor type
 */
typedef struct{
    uart_id_t   uartID;         /**< UART ID                */
    uint8_t     RxIrqPrio;      /**< RX IRQ priority level  */
    uint16_t    *pUxMODE;       /**< Real UxMODE address    */
    uint16_t    *pUxSTA;        /**< Real UxSTA address     */
    uint16_t    *pUxBRG;        /**< Real UxBRG address     */
    uint16_t    *pUxTXREG;      /**< Real UxTXREG address   */
    uint16_t    *pUxRXREG;      /**< Real UxRXREG address   */
}   uart_desc_t;

//------------------------------------------------------------------------------

/**
 * @brief   Configuration d'un UART  
 * 
 * @param[out]  pUart : Adress of an uart_desc_t variable (descriptor, set by the init function) 
 * @param[in]   pUartCFG : Adress of an uart_config_t variable (holding the desired configuration)  
 * 
 * @retval      UART_OK on success, 
 * @retval      UART_UNKNOWN_UART if the UART is not listed (_UART1 / _UART2)
 * @retval      UART_BAD_PRIO if the given priority is outside [0;7]
 */
uart_err_t  uart_init(uart_desc_t *pUart, const uart_config_t *pUartCFG);

 /**
  * @brief  Send one character on a choosen UART 
  * 
  * @param[in]      pUart : Address of an initialized uart descriptor
  * @param          Car : Character to send
  * @param          BlockingMode : Send in blocking mode or non blocking mode 	
  *  
  * @return     UART_OK on success or UART_TX_FIFO_FULL in non blocking mode if Tx FIFO is full
  */
uart_err_t      uart_putch(const uart_desc_t *pUart, uint8_t Car, uart_tx_blocking_t BlockingMode);

 /**
 * @brief  Sends a C Formatted string, blocking mode only
 * 
 * @param[in]   pUart : Address of an initialized uart descriptor
 * @param[in]   pString : Address of the string to send	
 * 
 * @return   UART_OK
 */
uart_err_t      uart_puts(const uart_desc_t *pUart, const uint8_t *pString);

#endif  /*  __LIB_UART_PIC24LL_H__  */


