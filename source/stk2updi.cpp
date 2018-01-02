/*
 * STK2UPDI.cpp
 *
 * Created: 11-11-2017 22:29:58
 * Author : JMR_2
 */ 

// Includes
//#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "STK500.h"
#include "updi_io.h"
#include "stk_io.h"
#include "UPDI_lo_lvl.h"
#include "NVM.h"

// Prototypes
void setup();
void loop();
void sys_init();

int main(void)
{
	setup();
	loop();
}

inline void setup() {
	/* Initialize MCU */
	sys_init();
	
	/* Initialize serial links */
	STK_io::init();
	UPDI_io::init();
	
	/* Initialize STK500v2 variables */
	STK500::PARAM_VTARGET_VAL = 55;
	STK500::PARAM_VADJUST_VAL = 32;

	using namespace UPDI;
	/* Initialize or enable UPDI */
	UPDI_io::put(UPDI_io::double_break);
	stcs(reg::Control_A, 6);
	
	/* Perform chip erase if D5 is low */
	if ((PIND & (1 << PIND5)) == 0)
		STK500::chip_erase();
	
	// Stop if incorrect initial state
	if (lcds(reg::ASI_System_Status) != 0x82) {
		// Dump status/control registers for analysis
		for (int i = 0; i < 16; i++) {
			STK_io::put(lcds(static_cast<reg::reg_t>(i)));
		}
		// Start blinking
		DDRB |= 1 << DDB5;
		while (1) {
			PINB |= 1 << PINB5;
			_delay_ms(500);
		}
	}
}


inline void loop() {
	while (1) {
		uint8_t current_fuse;
		
		// Receive command
		while(!STK500::receive());
	
		// Process command
		switch (STK500::body[0]) {

			// *** Special case: checksum error ***
			case STK500::ANSWER_CKSUM_ERROR:
				STK500::set_status(STK500::ANSWER_CKSUM_ERROR);
				break;

			// *** General commands ***
			case STK500::CMD_SIGN_ON:
				STK500::sign_on();
				break;
			case STK500::CMD_GET_PARAMETER:
				STK500::get_parameter();
				break;
			case STK500::CMD_SET_PARAMETER:
				STK500::set_parameter();
				break;
			case STK500::CMD_LOAD_ADDRESS:
				STK500::load_address();
				break;
			// *** ISP commands ***
			case STK500::CMD_ENTER_PROGMODE_ISP:
				STK500::enter_progmode();	
				break;
			case STK500::CMD_LEAVE_PROGMODE_ISP:
				STK500::leave_progmode();
				break;
			case STK500::CMD_READ_FLASH_ISP:
				STK500::read_mem(STK500::PARAM_VADJUST_VAL << 10);
				break;
			case STK500::CMD_READ_EEPROM_ISP:
				STK500::read_mem(STK500::PARAM_VTARGET_VAL == 55 ? NVM::EEPROM_base : NVM::User_base, false);
				break;
			case STK500::CMD_READ_FUSE_ISP:
				// Only lfuse actually causes a read, the others send the currently selected fuse
				if (STK500::body[3] == 0x00){
					// Select fuse from the current_fuse variable
					STK500::address = current_fuse;
					// Set number of bytes to read as 1
					STK500::body[1] = 0;
					STK500::body[2] = 1;
					// Read fuse byte
					STK500::read_mem(NVM::Fuse_base, false);
					break;			
				}
				// else, carry on to next case
			case STK500::CMD_READ_LOCK_ISP:			
				// send fake fuse value (this is an hack for Atmel Studio)
				STK500::size_hi = 0;
				STK500::size_lo = 4;
				STK500::body[2] = current_fuse;
				STK500::body[1] = STK500::body[3] = STK500::STATUS_CMD_OK;
				break;
			case STK500::CMD_READ_SIGNATURE_ISP:
				// Select requested signature byte from command data
				if ((STK500::address = STK500::body[4]) == 2) {
					// send fake 3rd signature byte (this is an hack for Atmel Studio)
					STK500::size_hi = 0;
					STK500::size_lo = 4;
					STK500::body[2] = 0x0A;
					STK500::body[1] = STK500::body[3] = STK500::STATUS_CMD_OK;
					break;		
				}
				else {
					// Set number of bytes to read as 1
					STK500::body[1] = 0;
					STK500::body[2] = 1;
					// Read signature byte
					STK500::read_mem(NVM::Sig_base, false);
					break;
				}
			case STK500::CMD_PROGRAM_FLASH_ISP:
				STK500::program_nvm(NVM::Flash_base, 64, true);
				break;
			case STK500::CMD_PROGRAM_EEPROM_ISP:
				STK500::program_nvm(STK500::PARAM_VTARGET_VAL == 55 ? NVM::EEPROM_base : NVM::User_base, 32, false);
				break;
			case STK500::CMD_PROGRAM_LOCK_ISP:
				current_fuse = STK500::body[4] & 0x3F;
			case STK500::CMD_PROGRAM_FUSE_ISP:
				// Only lfuse actually causes a write, the others are ignored
				if (STK500::body[2] == 0xA0){
					// Select fuse from the PARAM_VADJUST parameter
					STK500::address = current_fuse;
					STK500::program_fuse();
					break;
				}		
				// Pretend fuses were written (this is an hack for Atmel Studio)
				STK500::size_hi = 0;
				STK500::size_lo = 3;
				STK500::body[1] = STK500::body[2] = STK500::STATUS_CMD_OK;
				break;
			case STK500::CMD_CHIP_ERASE_ISP:
				STK500::chip_erase();
				break;
			case STK500::CMD_OSCCAL:
			case STK500::CMD_FIRMWARE_UPGRADE:
			default:
				STK500::set_status(STK500::STATUS_CMD_UNKNOWN);
				break;
		}
		// send response
		STK500::answer();
	}
}

void sys_init() {
	/* Disable digital input buffers on port C */
	DIDR0 = 0x3F;
	/* Enable all port D pull-ups */
	PORTD = 0xFF;
	/* Enable all port B pull-ups, except for LED */
	PORTB = 0xFF - (1 << PORTB5);
	/* Disable unused peripherals */
	ACSR = 1 << ACD;		// turn off comparator
	PRR =
		(1 << PRTWI) |		// turn off 2 wire interface
		(1 << PRTIM2) |		// turn off timer 2
		(1 << PRTIM1) |		// turn off timer 1
		(1 << PRSPI) |		// turn off SPI interface
		(1 << PRADC);		// turn off the ADC		
