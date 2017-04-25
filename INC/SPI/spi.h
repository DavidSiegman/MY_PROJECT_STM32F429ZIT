#ifndef __SPI_H 
#define __SPI_H

#include "stm32f4xx.h"

#define SPI5_SCK_PORT  GPIOF
#define SPI5_MISO_PORT GPIOF
#define SPI5_MOSI_PORT GPIOF

#define SPI5_SCK_PIN   7U
#define SPI5_MISO_PIN  8U
#define SPI5_MOSI_PIN  9U

void SPI5_INIT(void);
void SPI5_SendByte(unsigned char byte);
unsigned char SPI5_ReadByte(void);
void SPI5_Enable(void);
void SPI5_Disable(void);
void SPI5_BIDI_Enable(void);
void SPI5_BIDI_Disable(void);
void SPI5_BIDIO_Enable(void);
void SPI5_BIDIO_Disable(void);

#endif /* end __SPI_H */
