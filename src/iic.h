/**
 * @file iic.h
 * @author Michal Piekos (michal.public@wp.pl)
 * @brief IIC library for AVR controllers.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright MIT License. Copyright (c) 2022
 * 
 */
#ifndef MP_IIC_h
#define MP_IIC_h


/*****************************************************************
 * Includes
******************************************************************/

#include <avr/io.h>
#include <util/twi.h>

/*****************************************************************/


/*****************************************************************
 * General definitions
******************************************************************/

#define NEWTWBR(scl, prescaler)     (uint8_t)((F_CPU / scl - 16) / (2 * prescaler))
#define IIC_PRESCALER1      0x00
#define IIC_PRESCALER4      0x01
#define IIC_PRESCALER16     0x02
#define IIC_PRESCALER64     0x03
#define IIC_SEND_ACK        0x01
#define IIC_SEND_NACK       0x00
#define IIC_TIMEOUT         60U
/*****************************************************************/


/*****************************************************************
 * Status definitions
******************************************************************/

#define STATUS_OK                       0xFF
#define STATUS_BUS_DISCONNECTED         0xD0
#define STATUS_TRANSMISSION_ERROR       0xD1

/*****************************************************************/


/*****************************************************************
 * Function prototypes
******************************************************************/

void iic_init(long iic_freq, uint8_t prescaler);
uint8_t iic_start(void);
uint8_t iic_timeout(uint8_t timeout);
uint8_t iic_tx_addr(uint8_t sla, uint8_t mode);
uint8_t iic_tx_data(uint8_t byte);
uint8_t iic_rx_data(uint8_t confirmation);
void iic_stop(void);

/*****************************************************************/

#endif /* MP_IIC_h */