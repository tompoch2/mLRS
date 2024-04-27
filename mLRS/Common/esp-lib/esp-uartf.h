//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// ESP UartF
//********************************************************
#ifndef ESPLIB_UARTF_H
#define ESPLIB_UARTF_H


#ifdef UARTF_USE_SERIAL
  #define UARTF_SERIAL_NO       Serial
#elif defined UARTF_USE_SERIAL1
  #define UARTF_SERIAL_NO       Serial1
#elif defined UARTF_USE_SERIAL2
  #define UARTF_SERIAL_NO       Serial2
#endif

#ifndef UARTF_TXBUFSIZE
  #define UARTF_TXBUFSIZE       256 // MUST be 2^N
#endif
#ifndef UARTF_RXBUFSIZE
  #define UARTF_RXBUFSIZE       256 // MUST be 2^N
#endif


IRAM_ATTR void uartf_putbuf(uint8_t* buf, uint16_t len)
{
    UARTF_SERIAL_NO.write((uint8_t*)buf, len);
}


//-------------------------------------------------------
// INIT routines
//-------------------------------------------------------

void uartf_init(void)
{
#ifdef ESP32
    UARTF_SERIAL_NO.setTxBufferSize(UARTF_TXBUFSIZE);
    UARTF_SERIAL_NO.setRxBufferSize(UARTF_RXBUFSIZE);
#if defined UARTF_USE_TX_IO || defined UARTF_USE_RX_IO // both need to be defined
    UARTF_SERIAL_NO.begin(UARTF_BAUD, SERIAL_8N1, UARTF_USE_RX_IO, UARTF_USE_TX_IO);
#else
    UARTF_SERIAL_NO.begin(UARTF_BAUD);
#endif
    UARTF_SERIAL_NO.setRxFIFOFull(8);
    UARTF_SERIAL_NO.setRxTimeout(1);
#elif defined ESP8266
    UARTF_SERIAL_NO.setRxBufferSize(UARTF_RXBUFSIZE);
    UARTF_SERIAL_NO.begin(UARTF_BAUD);
#endif
}


#endif // ESPLIB_UARTF_H
