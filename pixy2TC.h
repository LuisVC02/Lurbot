/*
 * Main Pixy template class.
 *
 *  Created on: 21 jun 2023
 *      Author: otzru
 */

#ifndef PIXY2TC_H_
#define PIXY2TC_H_

#include "pixy2.h"

#define PIXY_DEFAULT_ARGVAL 0x80000000
#define PIXY_BUFFERSIZE 0x104
#define PIXY_CHECKSUM_SYNC 0xc1af
#define PIXY_NO_CHECKSUM_SYNC 0xc1ae
#define PIXY_SEND_HEADER_SIZE 4
#define PIXY_MAX_PROGNAME 33

#define PIXY_TYPE_REQUEST_CHANGE_PROG 0x02
#define PIXY_TYPE_REQUEST_RESOLUTION 0x0c
#define PIXY_TYPE_RESPONSE_RESOLUTION 0x0d
#define PIXY_TYPE_REQUEST_VERSION 0x0e
#define PIXY_TYPE_RESPONSE_VERSION 0x0f
#define PIXY_TYPE_RESPONSE_RESULT 0x01
#define PIXY_TYPE_RESPONSE_ERROR 0x03
#define PIXY_TYPE_REQUEST_BRIGHTNESS 0x10
#define PIXY_TYPE_REQUEST_SERVO 0x12
#define PIXY_TYPE_REQUEST_LED 0x14
#define PIXY_TYPE_REQUEST_LAMP 0x16
#define PIXY_TYPE_REQUEST_FPS 0x18

#define PIXY_RESULT_OK 0
#define PIXY_RESULT_ERROR -1
#define PIXY_RESULT_BUSY -2
#define PIXY_RESULT_CHECKSUM_ERROR -3
#define PIXY_RESULT_TIMEOUT -4
#define PIXY_RESULT_BUTTON_OVERRIDE -5
#define PIXY_RESULT_PROG_CHANGING -6

// RC-servo values
#define PIXY_RCS_MIN_POS 0
#define PIXY_RCS_MAX_POS 1000L
#define PIXY_RCS_CENTER_POS ((PIXY_RCS_MAX_POS - PIXY_RCS_MIN_POS) / 2)


/*!
 * brief  Turns on/off Pixy2's integrated light source.
 *
 * Note Both arguments are binary, zero or non-zero.
 *
 * param upper Controls the two white LEDs along the top edge of Pixy2's PCB.
 * param lower Sets the RGB LED, causing it to turn on all three color channels at full brightness, resulting in white light.
 * retval retval It returns an false if it fails and true if it succeeds.
 */
bool setLamp(bool upper, bool lower);

#endif /* PIXY2TC_H_ */
