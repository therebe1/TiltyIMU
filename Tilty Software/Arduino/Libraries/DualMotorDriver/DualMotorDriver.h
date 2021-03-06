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

#if defined(__MK20DX128__) || defined(__MK20DX256__)
#include "DualMotorDriverAddOn.h"

#else

#include "Arduino.h"

//==========================================================================================//
// Add-on/shield class information

// ========== Code Settings ==========
// You can change/comment these values
#define DEBUG_MOTOR_DRIVER				// Enable to include debug code in program
//#define I2C_FREQ 100000				// Enable to set I2C to 100kHz frequency
#define I2C_FREQ 200000					// Enable to set I2C to 200kHz frequency
//#define I2C_FREQ 400000				// Enable to set I2C to 400kHz frequency
#define REFRESH_FREQ 100 				// Frequency at which to update encoder and motor if in any auto-control mode, cannot be less than 62 and should not be greater than 1561 (technically it can be, but don't do it)
#define ENABLE_WATCHDOG_TIMER			// Watchdog timer will cause a system reset if any functions freeze, preventing the motor driver from freezing for longer than the watchdog timer limit (which is set below)
#define ENCODER_RESOLUTION SINGLE		// Change to DOUBLE to read the rising and falling edge of the encoder, which doubles the resolution, but also the CPU load
#define WHO_AM_I 0x11					// Arbitrary value set so that the motor driver can be recognized as such
#define CURRENT_SENSE_SENSITIVITY HIGH	// Change to LOW if current sensing seems to exceed one byte (255)
// ========== Code Settings ==========


// ========== I2C Settings ==========
// Neither buffer size can exceed 32 bytes! That is the limit imposed by the Wire library
#define RX_BUFFER_SIZE 30
#define TX_BUFFER_SIZE 30
#define DEFAULT_MOTOR_DRIVER_I2C_ADDRESS 0x03
// ========== I2C Settings ==========


// ========== Library includes ==========
//#include "Wire.h"
#include "CustomWire.h"
#include <avr/eeprom.h>
// ========== Library includes ==========


// ========== Debug Defines ==========
#ifdef DEBUG_MOTOR_DRIVER
	#define m1_scaled_power 101
	#define m1_current_power 102
	#define m1_power 103
	#define m1_encoder 104
	#define m1_rate 105
	#define m1_target_rate 106
	#define ms 107
	#define m1_p 108
	#define m1_i 109
	#define m1_d 110

	#define m2_scaled_power 111
	#define m2_current_power 112
	#define m2_power 113
	#define m2_encoder 114
	#define m2_rate 115
	#define m2_target_rate 116
	#define m2_p 117
	#define m2_i 118
	#define m2_d 119

	#define pin1s 140
	#define pin2s 141
	#define pin1H 142
	#define pin1L 143
	#define pin2H 144
	#define pin2L 145

	#define mcusr 150
	#define admux 151
	#define adcsra 152
	#define adcsrb 153

	#define pid_kp 
	#define pid_ki 
	#define pid_kd 
	
	#define elapsed_millis 200
	#define led	255
#endif
// ========== Debug Defines ==========


// ========== Motor current sense pins ==========
#define M1_SENSE A3
#define M2_SENSE A2
// ========== Motor current sense pins ==========


// ========== Encoder pins ==========
#define ENC1A 3 // Interrupt pin
#define ENC1B A0
#define ENC2A A1
#define ENC2B 2 // Interrupt pin
// ========== Encoder pins ==========

// ========== Encoder resolutions ==========
#define SINGLE 0x0F
#define DOUBLE 0x05
// ========== Encoder resolutions ==========


// ========== LED pin ==========
#define LED 10
// ========== LED pin ==========


// ========== Default Motor Characteristics ==========
#ifndef TICKS_PER_REV
#define TICKS_PER_REV 334 // Encoder pulses per axle revolution
#endif
#ifndef GEAR_RATIO
	#define GEAR_RATIO 9.28	// Motor gear ratio, set to 1 if encoder is on gearbox output instead of motor output
#endif
#if ENCODER_RESOLUTION == SINGLE
	#define ENCODER_MULTIPLIER 1
#elif ENCODER_RESOLUTION == DOUBLE
	#define ENCODER_MULTIPLIER 2
#endif
#define TICKS_PER_ROT ((TICKS_PER_REV * GEAR_RATIO) * ENCODER_MULTIPLIER) // Encoder pulses per rotation of the output shaft
// ========== Default Motor Characteristics ==========


// ========== Timer Characteristics ==========
#define OCR2A_VALUE uint8_t((16000000 / 1024) / REFRESH_FREQ)	// Automatically sets value for OCR2A which controls motor and encoder update timing
#define TIMER2_PRESCALER 0x07									// Prescaler of 1024 for TCCR2B
#define TIMER2_IMSK 0x06										// Sets interrupts for OCR2A and OCR2B
#define TIMER2A_MODE 0x03										// Fast PWM count up mode settings for TCCR2A
#define TIMER2B_MODE 0x08										// Fast PWM count up mode settings for TCCR2B
// ========== Timer Characteristics ==========


// ========== ADC Characteristics ==========
#if CURRENT_SENSE_SENSITIVITY
	#define ADC_REFERENCE 0xC0				// Sets ADC reference voltage to internal 1.1v reference
#else
	#define ADC_REFERENCE 0x40				// Sets ADC reference voltage to Vcc (5v)
#endif
#define M1_SENSE_MASK (ADC_REFERENCE | 0x03)// Motor 1 sense pin ADC mux bit mask
#define M2_SENSE_MASK (ADC_REFERENCE | 0x02)// Motor 2 sense pin ADC mux bit mask
// ========== ADC Characteristics ==========


// ========== Watchdog Timer Characteristics ==========
#ifdef ENABLE_WATCHDOG_TIMER
#define WATCHDOG_TIME 6 // Change this to change time before reset if code freezes
#define LOW_WDP (WATCHDOG_TIME & 0x07)
#define HIGH_WDP (WATCHDOG_TIME & 0x08) << 5
// WATCHDOG_TIME values and their approximate delay-till-reset times
// 0 => 16ms    3 => 0.125s    6 => 1.0s    9 => 8.0s
// 1 => 32ms    4 => 0.250s    7 => 2.0s
// 2 => 64ms    5 => 0.500s    8 => 4.0s
#endif
// ========== Watchdog Timer Characteristics ==========


// ========== EEPROM Addresses ==========
#define SAVED_VALS_ADDRESS		0x00
#define M1_CONTROL_ADDRESS		0x01
#define M2_CONTROL_ADDRESS		0x02
#define MIN_POWER_ADDRESS		0x03
#define RAMPING_RATE_ADDRESS	0x04
#define MIN_POWER_ADDRESS		0x05
#define I2C_ADDR_ADDRESS		0x06
#define PID_SCALARS_ADDRESS		0x07
// ========== EEPROM Addresses ==========


// ========== Default Register Values ==========
#define DEFAULT_CONTROL		ENCODER | BRAKE | SPEED
#define DEFAULT_M1_POWER		0
#define DEFAULT_M2_POWER		0
#define DEFAULT_M1_ENCODER		0
#define DEFAULT_M2_ENCODER		0
#define DEFAULT_M1_CURRENT		0
#define DEFAULT_M2_CURRENT		0
#define DEFAULT_M1_RATE			0
#define DEFAULT_M2_RATE			0
#define DEFAULT_PID_KP			0.0
#define DEFAULT_PID_KI			0.05
#define DEFAULT_PID_KD			0.25
#define DEFAULT_MIN_POWER		55
#define DEFAULT_TICKS_REV		1336
#define DEFAULT_LOOP_TIME		5
#define DEFAULT_DMD_ADDRESS		0x03
#define DEFAULT_EEPROM_SAVE		0
#define DEFAULT_EEPROM_LOAD		0
// ========== Default Register Values ==========

class DualMotorDriver {
	public:
		// Constructors
		DualMotorDriver();
		~DualMotorDriver();
		
		void init();
		
		uint8_t getData(int bytes);
		void sendData();
		
		void updateVars();

		float PID_kP;				// PID D scalar for RPM control
		float PID_kI;				// PID D scalar for RPM control
		float PID_kD;				// PID D scalar for RPM control
		
		uint8_t min_power;			// Minimum PWM to apply to the motor in all modes except when SPEED and MODE bits are 0
		uint8_t ramping_rate;		// Amount to add/subtract each time ramping code is checked
		
		typedef struct Pin {
			uint8_t *out_port;
			uint8_t *in_port;
			uint8_t bit_mask;
			uint8_t bit;
			uint8_t number;
			int8_t  TIMSK2_mask;
		} Pin;
		
		typedef struct MotorState {
			uint8_t control;
			uint8_t set_power;
			uint8_t scaled_power;
			uint8_t current_power;
			
#if CURRENT_SENSE_SENSITIVITY
			uint16_t current_draw;	// High sensitivity motor 1 current draw (16-bit)
#else
			uint8_t current_draw;	// Low sensitivity motor 1 current draw (8-bit)
#endif
			
			int16_t target_rate;
			float current_rate;
			
			float PID_P;
			float PID_I;
			float PID_D;
			
			int32_t encoder_value;
		} MotorState;
		
		Pin M1, M2;// PWM pins
		Pin M1H, M1L, M2H, M2L;// Direction/braking control pins
		
		typedef struct Motor {
			MotorState state;	// Motor state holder, holds all data relevant to the user
			
			uint8_t *OCR0x;// Pointer to pin output compare register
			uint8_t COM0x;// Pin setup in TCCR0A
			
			int32_t old_enc;
			
			Pin *speed_pin;// PWM/enable pin
			Pin *high_pin;// Direction/braking control pin 1
			Pin *low_pin;// Direction/braking control pin 2
		} Motor;
		
		Motor motor1;
		Motor motor2;
		
		void updateMotor(Motor *motor);
		
		void saveSettings(uint8_t vals);
		void loadSettings();
		
		inline void reset();
		
		inline void updateMotorControl(Motor *motor);
		inline void updateMotorRPM(Motor *motor);
		inline void updateMotorPower(Motor *motor);
		inline void updateEncoder(Motor* motor);
		
		inline void setMotorPWM(Motor *motor);
		inline void setMotorDirection(Motor *motor);
		inline void setMotorBraking(Motor *motor);
		inline void setTimerB(Motor *motor);
		
		void wireToVar(uint8_t *var);
		void wireToVar(int16_t *var);
		void wireToVar(uint16_t *var);
		void wireToVar(int32_t *var);
		void wireToVar(float *var);
		void wireToVar(MotorState *var);
		
		// Counter variable for timing
		uint8_t reset_led_counter;
		
	private:
		uint8_t active_var;
		void *active_var_ptr;
		
		uint16_t updated_vars;//	Indicates which variables were changed in the last I2C update

		// Data union for transferring different 4 byte types to/from data register
		union data_union {
			uint8_t bytes[4];
			float float32;
		} data_union;
		
		// Motor control register bit values
		static const uint8_t DIRECTION = 0x01;// Sets motor direction. 0 is forwards, 1 is reverse.
		static const uint8_t BRAKE = 0x02;// Motor brake/cost. 1 brakes, 0 coasts.
		static const uint8_t SPEED = 0x04;// Enables/Disables automatic speed control
		static const uint8_t MODE = 0x08;// Sets speed control type. Ramping power or RPM control
		static const uint8_t ENCODER = 0x10;// Sets whether to enable the encoder
		static const uint8_t CURRENT = 0x20;// Sets whether to read motor currents or not
		static const uint8_t CURD = 0x40;// Set to 1 when new current sense data is available
		//======== Motor control type settings ========
		//	|  SPEED  |  MODE  |  RESULT
		//  |    0    |    0   |  No automatic control, power is written directly to PWM
		//  |    0    |    1   |  No automatic power smoothing, but power is mapped 0%-100% to min_power-255
		//  |    1    |    0   |  Automatic power ramping for smoother transitions between speeds, also mapped min_power-255
		//  |    1    |    1   |  RPM control via the motor rate registers, will automatically enable encoders
		
		// Active variable values, control which variable is modified when an I2C write is performed
		static const uint8_t M1_CONTROL = 0x00;
		static const uint8_t M2_CONTROL = 0x01;
		static const uint8_t M1_POWER = 0x02;
		static const uint8_t M2_POWER = 0x03;
		static const uint8_t M1_CURRENT = 0x04;
		static const uint8_t M2_CURRENT = 0x05;
		static const uint8_t RAMPING_RATE = 0x06;
		static const uint8_t MIN_POWER = 0x07;
		static const uint8_t M1_RATE = 0x08;
		static const uint8_t M2_RATE = 0x09;
		static const uint8_t M1_ENCODER = 0x0A;
		static const uint8_t M2_ENCODER = 0x0B;
		static const uint8_t PID_KP = 0x0C;
		static const uint8_t PID_KI = 0x0D;
		static const uint8_t PID_KD = 0x0E;
		static const uint8_t EEPROM_SAVE = 0x0F;
		static const uint8_t EEPROM_LOAD = 0x10;
		static const uint8_t DEVICE_ADDRESS = 0x20;
		static const uint8_t RESET = 0x30;
		static const uint8_t M1_STATE = 0x31;
		static const uint8_t M2_STATE = 0x32;
};

extern DualMotorDriver MotorDriver;

void receiveEvent(int bytes);
void requestEvent();

void readEncoder1();
void readEncoder2();

void delayMillis(unsigned long time);

extern volatile unsigned long blahblah;


#ifdef DEBUG_MOTOR_DRIVER
		void ledOn();
		void ledOff();
		void ledToggle();
#endif

#endif
		
#endif