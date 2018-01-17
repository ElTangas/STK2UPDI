/*
 * packet.h
 *
 * Created: 12-11-2017 11:10:31
 *  Author: JMR_2
 */ 


#ifndef STK500_H_
#define STK500_H_

#include "stk_io.h"
#include <stdint.h>
#include "flash_vars.h"
#include "NVM.h"

namespace STK500 {
	
	// *** Parameter IDs ***
	constexpr uint8_t PARAM_BUILD_NUMBER_LOW		= 0x80;
	constexpr uint8_t PARAM_BUILD_NUMBER_HIGH		= 0x81;
	constexpr uint8_t PARAM_HW_VER					= 0x90;
	constexpr uint8_t PARAM_SW_MAJOR				= 0x91;
	constexpr uint8_t PARAM_SW_MINOR				= 0x92;
	constexpr uint8_t PARAM_VTARGET					= 0x94;
	constexpr uint8_t PARAM_VADJUST					= 0x95;
	constexpr uint8_t PARAM_OSC_PSCALE				= 0x96;
	constexpr uint8_t PARAM_OSC_CMATCH				= 0x97;
	constexpr uint8_t PARAM_SCK_DURATION			= 0x98;
	constexpr uint8_t PARAM_TOPCARD_DETECT			= 0x9A;
	constexpr uint8_t PARAM_STATUS					= 0x9C;
	constexpr uint8_t PARAM_DATA					= 0x9D;
	constexpr uint8_t PARAM_RESET_POLARITY			= 0x9E;
	constexpr uint8_t PARAM_CONTROLLER_INIT			= 0x9F;
	
	// *** Parameter Values ***
	constexpr uint8_t PARAM_BUILD_NUMBER_LOW_VAL	= 0x02;
	constexpr uint8_t PARAM_BUILD_NUMBER_HIGH_VAL	= 0x00;
	constexpr uint8_t PARAM_HW_VER_VAL				= 0x00;
	constexpr uint8_t PARAM_SW_MAJOR_VAL			= 0x02;
	constexpr uint8_t PARAM_SW_MINOR_VAL			= 0x0A;
	extern uint8_t PARAM_VTARGET_VAL;
	extern uint8_t PARAM_VADJUST_VAL;
	extern uint8_t PARAM_OSC_PSCALE_VAL;
	extern uint8_t PARAM_OSC_CMATCH_VAL;
	extern uint8_t PARAM_SCK_DURATION_VAL;
	constexpr uint8_t PARAM_TOPCARD_DETECT_VAL		= 0x00;
	constexpr uint8_t PARAM_STATUS_VAL				= 0x00;
	constexpr uint8_t PARAM_DATA_VAL				= 0x00;
	extern uint8_t PARAM_RESET_POLARITY_VAL;
	extern uint8_t PARAM_CONTROLLER_INIT_VAL;

	// *** General command constants ***
	constexpr uint8_t CMD_SIGN_ON					= 0x01;
	constexpr uint8_t CMD_SET_PARAMETER				= 0x02;
	constexpr uint8_t CMD_GET_PARAMETER				= 0x03;
	constexpr uint8_t CMD_OSCCAL					= 0x05;
	constexpr uint8_t CMD_LOAD_ADDRESS				= 0x06;
	constexpr uint8_t CMD_FIRMWARE_UPGRADE			= 0x07;
	
	// *** ISP command constants ***
	constexpr uint8_t CMD_ENTER_PROGMODE_ISP		= 0x10;
	constexpr uint8_t CMD_LEAVE_PROGMODE_ISP		= 0x11;
	constexpr uint8_t CMD_CHIP_ERASE_ISP			= 0x12;
	constexpr uint8_t CMD_PROGRAM_FLASH_ISP			= 0x13;
	constexpr uint8_t CMD_READ_FLASH_ISP			= 0x14;
	constexpr uint8_t CMD_PROGRAM_EEPROM_ISP		= 0x15;
	constexpr uint8_t CMD_READ_EEPROM_ISP			= 0x16;
	constexpr uint8_t CMD_PROGRAM_FUSE_ISP			= 0x17;
	constexpr uint8_t CMD_READ_FUSE_ISP				= 0x18;
	constexpr uint8_t CMD_PROGRAM_LOCK_ISP			= 0x19;
	constexpr uint8_t CMD_READ_LOCK_ISP				= 0x1A;
	constexpr uint8_t CMD_READ_SIGNATURE_ISP		= 0x1B;
	constexpr uint8_t CMD_READ_OSCCAL_ISP			= 0x1C;
	constexpr uint8_t CMD_SPI_MULTI					= 0x1D;

	// *** Status constants ***
	// Success
	constexpr uint8_t STATUS_CMD_OK = 0x00;
	// Warnings
	constexpr uint8_t STATUS_CMD_TOUT = 0x80;
	constexpr uint8_t STATUS_RDY_BSY_TOUT = 0x81;
	constexpr uint8_t STATUS_SET_PARAM_MISSING = 0x82;
	// Errors
	constexpr uint8_t STATUS_CMD_FAILED = 0xC0;
	constexpr uint8_t STATUS_CMD_UNKNOWN = 0xC9;
	
	// *** Answer constants ***
	constexpr uint8_t ANSWER_CKSUM_ERROR = 0xB0;

	// *** STK500 packet ***
	constexpr uint8_t MESSAGE_START = 0x1B;
	extern uint8_t prologue[3];
	extern uint8_t & number;
	extern uint8_t & size_hi;
	extern uint8_t & size_lo;
	constexpr uint8_t TOKEN = 0x0E;
	extern uint8_t body [275];
	
	// *** Address ***
	extern uint16_t address;			/* Note: actually this is a 32 bit value. However, for the xtiny we can use just the low 16 bits */
	extern uint8_t (& address_byte)[2];
	
	//  *** Signature response ***
	extern FLASH(uint8_t) sgn_resp[10];
	
	// *** Parameter initialization ***
	void init();

	// *** Packet functions *** 
	bool receive();
	void answer();

	// *** Set status function ***
	void set_status(uint8_t);

	// *** General command functions ***
	void sign_on();
	void get_parameter();
	void set_parameter();
	void load_address();

	// *** ISP command functions ***
	void enter_progmode();
	void leave_progmode();
	void read_signature();
	void read_mem(const uint16_t mem_base = NVM::Flash_base, const uint8_t is_word_address = true);
	void program_nvm(const uint16_t mem_base, const uint8_t buff_size, const uint8_t is_word_address);
	void program_fuse(const uint16_t mem_base = NVM::Fuse_base);
	void chip_erase();
}



#endif /* STK_H_ */