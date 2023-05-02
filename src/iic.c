/**
 * @file iic.c
 * @author Michal Piekos (michal.public@wp.pl)
 * @brief IIC library for AVR controllers.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright MIT License. Copyright (c) 2022
 * 
 */
#include "iic.h"

/**
 * @brief Initialize IIC Interface
 * 
 * @param iic_freq IIC Frequency, e.g. 100000 for 100kHz
 * @param prescaler Prescaler value to be used in frequency calculation.
 * Possible values are [IIC_PRESCALER1 | IIC_PRESCALER4 | IIC_PRESCALER16 | IIC_PRESCALER64].
 * If in doubt use IIC_PRESCALER1.
 */
void iic_init(long iic_freq, uint8_t prescaler)
{
  TWSR = prescaler;
  switch (prescaler) {
    case 0x00:
      TWBR = NEWTWBR(iic_freq, 1);
      break;
    case 0x01:
      TWBR = NEWTWBR(iic_freq, 4);
      break;
    case 0x02:
      TWBR = NEWTWBR(iic_freq, 16);
      break;
    case 0x03:
      TWBR = NEWTWBR(iic_freq, 64);
      break;  
  }
  TWCR = _BV(TWEN);       /* Enable TWI */
}


/**
 * @brief Transmit START condition and return status
 * 
 * @return uint8_t Return statuses are defined in `iic.h`
 */
uint8_t iic_start(void)
{
  uint8_t status = STATUS_OK;
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
  loop_until_bit_is_set(TWCR, TWINT);
  
  if ((TW_STATUS != TW_START) && (TW_STATUS != TW_REP_START)) {
    status = TW_STATUS;
  }
  return status;
}


/**
 * @brief Transmit SLA + R/W and returns status
 * 
 * @param sla Slave address
 * @param mode TW_READ | TW_WRITE
 * @return uint8_t status. STATUS_OK or error code.
 */
uint8_t iic_tx_addr(uint8_t sla, uint8_t mode)
{
  uint8_t status = STATUS_OK;
  TWDR = (sla << 1) | mode;
  TWCR = _BV(TWINT) | _BV(TWEN);
  loop_until_bit_is_set(TWCR, TWINT);

  if ((TW_STATUS != TW_MT_SLA_ACK) && (TW_STATUS != TW_MR_SLA_ACK)) {
    status = TW_STATUS;
  }
  return status;
}


/**
 * @brief Transmit 1 byte of data and return status
 * 
 * @param byte data to transfer
 * @return uint8_t status. STATUS_OK or error code.
 */
uint8_t iic_tx_data(uint8_t byte)
{
  uint8_t status = STATUS_OK;
  TWDR = byte;
  TWCR = _BV(TWINT) | _BV(TWEN);
  loop_until_bit_is_set(TWCR, TWINT);

  if (TW_STATUS != TW_MT_DATA_ACK) {
    status = TW_STATUS;
  }
  return status;
}


/**
 * @brief Timeout wait for TWINT flag change
 * 
 * @param timeout Timeout 0-255
 * @return uint8_t STATUS_OK | STATUS_BUS_DISCONNECTED 
 */
uint8_t iic_timeout(uint8_t timeout)
{
  uint8_t status = STATUS_OK;
  while (bit_is_clear(TWCR, TWINT)) {
    timeout--;
    if (timeout <= 0) {
      status = STATUS_BUS_DISCONNECTED;
      break;
    }
  }
  return status;
}


/**
 * @brief Receive data from IIC bus
 * 
 * @param confirmation IIC_SEND_ACK | IIC_SEND_NACK
 * @return uint8_t. A byte of data or error code
 */
uint8_t iic_rx_data(uint8_t confirmation)
{
  uint8_t status;
  
  if (confirmation == IIC_SEND_ACK) {
    TWCR |= _BV(TWEA);
  } else {
    TWCR &= ~_BV(TWEA);
  }
  TWCR |= _BV(TWINT);

  if (iic_timeout(IIC_TIMEOUT) == STATUS_OK) {
    if ((TW_STATUS != TW_MR_DATA_ACK) && (TW_STATUS != TW_MR_DATA_NACK)) {
      status = TWDR;
    } else {
      status = STATUS_TRANSMISSION_ERROR;
    }
  } else {
    status = STATUS_BUS_DISCONNECTED;
  }
  return status;
}


/**
 * @brief Transmit STOP condition
 * 
 */
void iic_stop(void)
{ 
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); 
}