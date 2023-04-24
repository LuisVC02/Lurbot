/*
 * brief   : This module has functions for remote controller
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "controller.h"

volatile static channel_controller_t g_control_values = {0};
volatile static controller_function_t  g_control_callback = 0;

void RC_init()
{
	UART_initialization(UART_FOR_CONTROL, CONTROL_BAUDRATE, CONTROL_PARITY, CONTROL_STOP_BIT, true, true);
	set_dmamux(0, uart1_rx, false);
	DMA_init(0, input_data_control);
	set_transfer_config(get_data_adress(uart_1),
							 1,
							 0,
							 0,
							 (channel_controller_t*)&g_control_values,
							 1,
							 1,
							 -32,
							 1,
							 32,
							 false,
							 0);
	PIT_Initialization(PIT_module, false, T0, TIME_ERROR_FOR_PIT, u_seconds, true);
	PIT_callback_init(T0, no_control_request);
}

void set_control_callback(controller_function_t function)
{
	g_control_callback = function;
}

channel_controller_t get_contol_values()
{
	return g_control_values;
}

void input_data_control(struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
	stop_Timer(PIT_module, T0);
	setTime(PIT_module , T0, TIME_ERROR_FOR_PIT, u_seconds);
	g_control_values.good_comunication = true;
	if(g_control_values.start_byte_H != 0x40 || g_control_values.start_byte_L != 0x20)
	{
		set_transfer_config(get_data_adress(uart_1),
			 1,
			 0,
			 0,
			 (channel_controller_t*)&g_control_values,
			 1,
			 1,
			 -32,
			 1,
			 32,
			 false,
			 0);
	}
	else
	{
		if(0 != g_control_callback)
		{
			g_control_callback(g_control_values);
		}
	}
	start_Timer(PIT_module, T0);
}

void no_control_request()
{
	g_control_values.good_comunication = false;
	if(0 != g_control_callback)
	{
		g_control_callback(g_control_values);
	}
}
