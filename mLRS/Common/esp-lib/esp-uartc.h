//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP UartC
//********************************************************
#ifndef ESPLIB_UARTC_H
#define ESPLIB_UARTC_H


#ifdef UARTC_USE_SERIAL
  #define UARTC_SERIAL_NO       Serial
#elif defined UARTC_USE_SERIAL1
  #define UARTC_SERIAL_NO       Serial1
#elif defined UARTC_USE_SERIAL2
  #define UARTC_SERIAL_NO       Serial2
#else
  #error UARTC_SERIAL_NO must be defined!
#endif

#ifndef UARTC_TXBUFSIZE
  #define UARTC_TXBUFSIZE       256 // MUST be 2^N
#endif
#ifndef UARTC_RXBUFSIZE
  #define UARTC_RXBUFSIZE       256 // MUST be 2^N
#endif


IRAM_ATTR void uartc_putbuf(uint8_t* buf, uint16_t len)
{
    UARTC_SERIAL_NO.write((uint8_t*)buf, len);
}

IRAM_ATTR char uartc_getc(void)
{
    return (char)UARTC_SERIAL_NO.read();
}


IRAM_ATTR void uartc_rx_flush(void)
{
    while (UARTC_SERIAL_NO.available() > 0) UARTC_SERIAL_NO.read();
}


IRAM_ATTR void uartc_tx_flush(void)
{
    UARTC_SERIAL_NO.flush();
}


IRAM_ATTR uint16_t uartc_rx_bytesavailable(void)
{
    return (UARTC_SERIAL_NO.available() > 0) ? UARTC_SERIAL_NO.available() : 0;
}


IRAM_ATTR uint16_t uartc_rx_available(void)
{
    return (UARTC_SERIAL_NO.available() > 0) ? 1 : 0;
}


//-------------------------------------------------------
// INIT routines
//-------------------------------------------------------

void uartc_init(void)
{
#ifdef ESP32
    UARTC_SERIAL_NO.setTxBufferSize(UARTC_TXBUFSIZE);
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
#if defined UARTC_USE_TX_IO || defined UARTC_USE_RX_IO // both need to be defined
    UARTC_SERIAL_NO.begin(UARTC_BAUD, SERIAL_8N1, UARTC_USE_RX_IO, UARTC_USE_TX_IO);
#else
    UARTC_SERIAL_NO.begin(UARTC_BAUD);
#endif
    UARTC_SERIAL_NO.setRxFIFOFull(8);
    UARTC_SERIAL_NO.setRxTimeout(1);
#elif defined ESP8266
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
    UARTC_SERIAL_NO.begin(UARTC_BAUD);
#endif
}


#endif // ESPLIB_UARTC_H
