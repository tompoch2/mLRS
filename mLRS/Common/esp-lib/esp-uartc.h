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


//-------------------------------------------------------
// INIT routines
//-------------------------------------------------------

void uartc_init(void)
{
#ifdef ESP32
    UARTC_SERIAL_NO.setTxBufferSize(UARTC_TXBUFSIZE);
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
    UARTC_SERIAL_NO.begin(UARTC_BAUD);
    UARTC_SERIAL_NO.setRxFIFOFull(8);
    UARTC_SERIAL_NO.setRxTimeout(1);
#elif defined ESP8266
    UARTC_SERIAL_NO.setRxBufferSize(UARTC_RXBUFSIZE);
    UARTC_SERIAL_NO.begin(UARTC_BAUD);
#endif
}


#endif // ESPLIB_UARTC_H
