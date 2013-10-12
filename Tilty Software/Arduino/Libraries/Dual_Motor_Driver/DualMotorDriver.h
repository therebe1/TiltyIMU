/*
DualMotorDriver.h - Software library to program and control the TiltyIMU Dual Motor Driver Shield
Copyright (C) 2013-2014 Alex Beattie <alexbeattie at tiltyimu dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the version 2 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DualMotorDriver_h
#define DualMotorDriver_h

#include "Arduino.h"

#ifdef CORE_TEENSY && __arm__
//==========================================================================================//
// Teensy class data
#include <i2c_t3.h>

#ifndef MOTOR_DRIVER_I2C_ADDRESS
	#define DEFAULT_MOTOR_DRIVER_I2C_ADDRESS 0x03
	char I2C_ADDRESS = 0x03;
#endif

// End Teensy/Master class information
//==========================================================================================//
#else
//==========================================================================================//
// Add-on/shield class information

// ========== Library includes ==========
#include "Wire.h"
#include "EEPROM.h"
#ifndef NO_ENCODERS
	#include "Encoder.h"
#endif
// ========== Library includes ==========

// ========== I2C Settings ==========
#define MOTOR_DRIVER_RX_BUFFER_SIZE 14
#define MOTOR_DRIVER_TX_BUFFER_SIZE 14
#define DEFAULT_MOTOR_DRIVER_I2C_ADDRESS 0x03
// ========== I2C Settings ==========

// ========== EEPROM data addresses ==========
#define MOTOR_DRIVER_I2C_EEPROM_ADDRESS 0
// ========== EEPROM data addresses ==========

// ========== Motor control pins ==========
// Motor PWM pins
#define M1 6
#define M2 5
// Motor direction control pins
#define M1A 4
#define M1B 9
#define M2A 7
#define M2B 8
// ========== Motor control pins ==========

// ========== Encoder pins ==========
#define ENC1A 2 // Interrupt pin
#define ENC1B A1
#define ENC2A A0
#define ENC2B 3 // Interrupt pin
// ========== Encoder pins ==========

// ========== Command identifiers ==========

// ========== Command identifiers ==========


class MotorDriver {
	public:
		// Constructors
		MotorDriver();
		~MotorDriver();
		
		// Encoder objects
	#ifndef NO_ENCODERS
		Encoder m1Encoder;
		Encoder m2Encoder;
	#endif
		
		// Initialization functions
		void init();
		
		// I2C data handlers
		int getData(int bytes);
		int parseCommands();
		
		// Update functions
		int update();
		
		// Motor control functions
		int parseMotorCommand();
		
		// Motor power values (0-255)
		uint8_t M1_power;
		uint8_t M2_power;
		
		bool M1_direction;
		bool M2_direction;
		
		bool invertM1;
		bool invertM2;
		
		// Encoder values
		long encoder1;
		long encoder2;

	private:
		// Command Values
		static const char DIRECTION = 0x01;// Sets motor direction
		static const char ENABLE = 0x02;// Motor brake/cost. 0 brakes, 1 coasts.
		static const char MOTOR_NUM = 0x04;// Motor or Throttle/Steering selection
		static const char SPEED = 0x08;// Speed/Power setting
		static const char DRIVE = 0x10;// Tank/Arcade style control. Set to 1 to enable Tank Control, 0 to enable Arcade.
				
		// I2C data buffers
		uint8_t rxBuffer[MOTOR_DRIVER_RX_BUFFER_SIZE];
		uint8_t txBuffer[MOTOR_DRIVER_TX_BUFFER_SIZE];
		
		// I2C data buffer indexes
		uint8_t rxBufferIndex;
		uint8_t txbufferIndex;
	
};
// End add-on class information
//==========================================================================================//
#endif
#endif