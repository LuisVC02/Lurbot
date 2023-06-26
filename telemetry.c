/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "telemetry.h"

void telemetry_init()
{
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 15, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, 14, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, 13, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, 12, kPORT_MuxAlt3);
	uart_config_t configuration =
	{
		57600,            			// Baudrate
		kUART_ParityDisabled,     	// Parity
		kUART_OneStopBit,    // Stop bit
		0,					 // FIFO TX wattermark
		1,					 // FIFO RX wattermark
		false,				 // Request to send
		false,				 // Clear to send
		kUART_IdleTypeStopBit, // IDLE
		true,				 // Receptor enable
		true				 // Transmissor enable
	};
	UART_initialization_conf(uart_4, configuration, true, false);
	/*set_dmamux(4, uart4, false);
	DMA_init(4, NULL);
	set_transfer_config(bytes_ptr,
		 1,
		 1,
		 (int8_t)-n_bytes,
		 get_data_adress(uart_4),
		 1,
		 0,
		 0,
		 1,
		 n_bytes,
		 false,
		 4);
	UART_send_byte(uart_4, 0);*/
}

void telemetry_send_unblocking(uint8_t n_bytes, uint8_t* bytes_to_send)
{
	UART_send_array_unblocking(uart_4, n_bytes, bytes_to_send);
}


