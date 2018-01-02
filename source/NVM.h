#ifndef NVM_H_
#define NVM_H_

#include <stdint.h>

namespace NVM {
	// *** Base Addresses ***
	constexpr uint16_t NVM_base		= 0x1000;		/* Base address of the NVM controller */
	constexpr uint16_t Flash_base		= 0x8000;		/* Base address of the flash memory */
	constexpr uint16_t Sig_base		= 0x1100;		/* Base address of the signature */
	constexpr uint16_t Fuse_base		= 0x1280;		/* Base address of the fuses */
	constexpr uint16_t User_base		= 0x1300;		/* Base address of the User Row EEPROM */
	constexpr uint16_t EEPROM_base		= 0x1400;		/* Base address of the main EEPROM */
	
	// *** NVM Register Offsets
	constexpr uint8_t CTRLA			= 0x00;
	constexpr uint8_t CTRLB			= 0x01;
	constexpr uint8_t STATUS		= 0x02;
	constexpr uint8_t INTCTRL		= 0x03;
	constexpr uint8_t INTFLAGS		= 0x04;
	constexpr uint8_t DATA_lo		= 0x06;
	constexpr uint8_t DATA_hi		= 0x07;
	constexpr uint8_t ADDR_lo		= 0x08;
	constexpr uint8_t ADDR_hi		= 0x09;
	
	// *** NVM Commands (write to CTRLA to execute) ***
	constexpr uint8_t NOP			= 0x00;		/* Does nothing */
	constexpr uint8_t WP			= 0x01;		/* Write page buffer to memory */
	constexpr uint8_t ER			= 0x02;		/* Erase page */
	constexpr uint8_t ERWP			= 0x03;		/* Erase and write page */
	constexpr uint8_t PBC			= 0x04;		/* Page buffer clear */
	constexpr uint8_t CHER			= 0x05;		/* Chip erase: erase Flash and EEPROM */
	constexpr uint8_t EEER			= 0x06;		/* EEPROM Erase */
	constexpr uint8_t WFU			= 0x07;		/* Write fuse */

}

#endif /* NVM_H_ */
