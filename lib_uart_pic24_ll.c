/**
 * @file 
 * @author 	Alexis ROLLAND
 * @date	2022
 * @brief 	
 *  
 *
 */


#include "lib_uart_pic24_ll.h" // Inclusion du fichier .h  renommé

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/



/*	Implémentation du code */
//------------------------------------------------------------------------------
uart_err_t  uart_init(uart_id_t uart_id, const uart_config_t *pUartCFG, uart_desc_t *pUart)
{
    switch(uart_id){
        case _UART1:
            pUart->pUxBRG = (uint16_t*)&U1BRG;
            pUart->pUxMODE = (uint16_t*)&U1MODE;
            pUart->pUxSTA = (uint16_t*)&U1STA;
            pUart->pUxTXREG = (uint16_t*)&U1TXREG;
            pUart->pUxRXREG = (uint16_t*)&U1RXREG;
            break;
        case _UART2:
            pUart->pUxBRG = (uint16_t*)&U2BRG;
            pUart->pUxMODE = (uint16_t*)&U2MODE;
            pUart->pUxSTA = (uint16_t*)&U2STA;
            pUart->pUxTXREG = (uint16_t*)&U2TXREG;
            pUart->pUxRXREG = (uint16_t*)&U2RXREG;
            break;
        default: 
            return UART_UNKNOWN_UART;
            break;
    }
    pUart->uartID = uart_id;
    pUart->RxIrqPrio = pUartCFG->RxIrqPrio;
    *(pUart->pUxBRG) = pUartCFG->UxBRG;
    *(pUart->pUxMODE) = pUartCFG->UxMODE;
    *(pUart->pUxSTA) = pUartCFG->UxSTA;
    
    return UART_OK;
}

//------------------------------------------------------------------------------
uart_err_t  uart_set_rx_interrupt(const uart_desc_t *pUart)
{
    if (pUart->RxIrqPrio > 7) return UART_BAD_PRIO;
    
    switch (pUart->uartID)
    {
        case _UART1:
            IFS0bits.U1RXIF = 0;
            IPC2bits.U1RXIP = pUart->RxIrqPrio;
            IEC0bits.U1RXIE = 1;
            break;
        case _UART2:
            IFS1bits.U2RXIF = 0;
            IPC7bits.U2RXIP = pUart->RxIrqPrio;
            IEC1bits.U2RXIE = 1;
            break;
        default: 
            return UART_UNKNOWN_UART;
            break;
    }
    
    return UART_OK;
}

//------------------------------------------------------------------------------
uart_err_t      uart_putch(const uart_desc_t *pUart, uint8_t Car, uart_tx_blocking_t BlockingMode)
{
    if ( ( (*(pUart->pUxSTA)) & UTX_BF_MASK) == 0){      // If Tx Buffer is free, let's Tx whatever mode is
        *(pUart->pUxTXREG) = Car;
        return UART_OK;
    }
    else
    {
        if (BlockingMode == UART_TX_NON_BLOCKING_MODE) return UART_TX_FIFO_FULL;    // Mode non bloquant, buffer plein, on retourne une erreur
        while ( ( (*(pUart->pUxSTA)) & UTX_BF_MASK) != 0);   // Attente libération buffer
        *(pUart->pUxTXREG) = Car;
    }        
    
    return UART_OK;
}
//------------------------------------------------------------------------------
uart_err_t      uart_puts(const uart_desc_t *pUart, const uint8_t *pString)
{
    while(*pString != '\0'){
        while ( ( (*(pUart->pUxSTA)) & UTX_BF_MASK) != 0);   // Attente libération buffer
        *(pUart->pUxTXREG) = *pString;
        pString++;
    }
    return UART_OK;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------


