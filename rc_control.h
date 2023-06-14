/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef RC_CONTROL_H_
#define RC_CONTROL_H_

#include "uart.h"
#include "pit.h"
#include "dma.h"

#define UART_FOR_CONTROL uart_1
#define CONTROL_BAUDRATE 115200
#define CONTROL_PARITY   kUART_ParityDisabled
#define CONTROL_STOP_BIT kUART_OneStopBit
#define TIME_ERROR_FOR_PIT 200000

typedef struct
{
	 uint8_t start_byte_L;
	 uint8_t start_byte_H;
	 uint8_t direction_L;
	 uint8_t direction_H;
	 uint8_t motor_L;
	 uint8_t motor_H;
	 uint8_t channel_3_L;
	 uint8_t channel_3_H;
	 uint8_t channel_4_L;
	 uint8_t channel_4_H;
	 uint8_t channel_5_L;
	 uint8_t channel_5_H;
	 uint8_t channel_6_L;
	 uint8_t channel_6_H;
	 uint8_t sw1_L;
	 uint8_t sw1_H;
	 uint8_t sw2_L;
	 uint8_t sw2_H;
	 uint8_t sw3_L;
	 uint8_t sw3_H;
	 uint8_t sw4_L;
	 uint8_t sw4_H;
	 uint8_t channel_11_L;
	 uint8_t channel_11_H;
	 uint8_t channel_12_L;
	 uint8_t channel_12_H;
	 uint8_t channel_13_L;
	 uint8_t channel_13_H;
	 uint8_t channel_14_L;
	 uint8_t channel_14_H;
	 uint8_t final_byte_L;
	 uint8_t final_byte_H;
	 bool     good_comunication;
}channel_controller_t;

typedef void(*controller_function_t)(channel_controller_t channel_values);

void RC_init();

void set_control_callback(controller_function_t function);

channel_controller_t get_contol_values();

void input_data_control(struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds);

void set_dma_transfer_conf();

void no_control_request();

#endif /* RC_CONTROL_H_ */
