/*
 * brief  : This module controls the UART
 * date   : 13-March-2023
 * Author : Luis Roberto Vega Campos & Luis Ruben Padilla Ortiz
 */
#include "uart.h"



typedef UART_Type *uart_modules;
typedef struct
{
	clock_ip_name_t clock_port;
	PORT_Type *base;
	uint16_t Rx;
	uint16_t Tx;
}uart_port;

volatile static uint32_t  g_send_index[5]      = {0};
volatile static uint8_t  *g_array_to_send[5]   = {0};
volatile static function  g_callbacks[5]       = {0};

const           uint32_t  uart_clock_sources[] = {
		UART_CLOCK_SOURCE_0,
		UART_CLOCK_SOURCE_1,
		UART_CLOCK_SOURCE_2,
		UART_CLOCK_SOURCE_3,
		UART_CLOCK_SOURCE_4,
};

const uart_port g_ports[5]			   		   = {
		{UART0_PORT_CLOCK, UART0_PORT, UART0_RX, UART0_TX},
		{UART1_PORT_CLOCK, UART1_PORT, UART1_RX, UART1_TX},
		{UART2_PORT_CLOCK, UART2_PORT, UART2_RX, UART2_TX},
		{UART3_PORT_CLOCK, UART3_PORT, UART3_RX, UART3_TX},
		{UART4_PORT_CLOCK, UART4_PORT, UART4_RX, UART4_TX},
};
const       uart_modules  g_uarts[6]             = {UART0, UART1, UART2, UART3, UART4};


// Initialize UART PORTS --------------------------------------------
void PORT_initialization(n_uart_t n_uart)
{
	CLOCK_EnableClock(g_ports[(uint8_t)n_uart].clock_port);
	PORT_SetPinMux(g_ports[(uint8_t)n_uart].base, g_ports[(uint8_t)n_uart].Rx, kPORT_MuxAlt3);
	PORT_SetPinMux(g_ports[(uint8_t)n_uart].base, g_ports[(uint8_t)n_uart].Tx, kPORT_MuxAlt3);
}
// ------------------------------------------------------------------

// UART initialization ----------------------------------------------
void UART_initialization(n_uart_t n_uart, uint32_t baudrate, uart_parity_mode_t parity, uart_stop_bit_count_t stop_bit, bool enable_interrupts, bool dma_enable)
{
	uart_config_t configuration =
	{
		baudrate,            // Baudrate
		parity,     // Parity
		stop_bit,    // Stop bit
		0,					 // FIFO TX wattermark
		1,					 // FIFO RX wattermark
		false,				 // Request to send
		false,				 // Clear to send
		kUART_IdleTypeStopBit, // IDLE
		true,				 // Receptor enable
		true				 // Transmissor enable
	};
	PORT_initialization((uint8_t)n_uart);
	UART_Init(g_uarts[(uint8_t)n_uart], &configuration, CLOCK_GetFreq(uart_clock_sources[(uint8_t)n_uart]));
	if(true == enable_interrupts)
	{
		UART_EnableInterrupts(g_uarts[(uint8_t)n_uart], kUART_RxDataRegFullInterruptEnable); // Interrupt for reception
		if(true ==  dma_enable)
		{
			UART_EnableRxDMA(g_uarts[(uint8_t)n_uart], true);
		}
		else
		{
			UART_EnableRxDMA(g_uarts[(uint8_t)n_uart], false);
		}
	}
}
// -----------------------------------------------------------------------------------------

// UART RX value ----------------------------------------------------------------------------
uint8_t *get_data_adress(n_uart_t n_uart)
{
	return (uint8_t*)&(g_uarts[n_uart]->D);
}
// ------------------------------------------------------------------------------------------

// UART init with configuration -------------------------------------------------------------
void UART_initialization_conf(n_uart_t n_uart, uart_config_t configuration)
{
	PORT_initialization((uint8_t)n_uart);
	UART_Init(g_uarts[(uint8_t)n_uart], &configuration, CLOCK_GetFreq(uart_clock_sources[(uint8_t)n_uart]));
}
// ------------------------------------------------------------------------------------------

// This function is for configure interrupts ------------------------------------------------
void UART_cofig_interrupts(bool enable, uint32_t mask, n_uart_t n_uart)
{
	if(true == enable)
	{
		UART_EnableInterrupts(g_uarts[(uint8_t)n_uart], mask); // Interrupt set
	}
	else
	{
		UART_DisableInterrupts(g_uarts[(uint8_t)n_uart], mask); // Interrupt disable
	}
}
// ------------------------------------------------------------------------------------------

// This function sends only one byte in UART ------------------------
void UART_send_byte(n_uart_t n_uart, uint8_t byte)
{
	UART_WriteByte(g_uarts[(uint8_t)n_uart], byte);
}
// ------------------------------------------------------------------
void UART_send_array_blocking(n_uart_t n_uart, uint8_t *array)
{
	uint8_t value = array[0];
	uint16_t index = 0;

	while(value != 0 && index < MAX_SEND_LENGTH)
	{
		while(!(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(g_uarts[(uint8_t)n_uart])));
		UART_WriteByte(g_uarts[(uint8_t)n_uart], value);
		index ++;
		value = array[index];
	}

}
// This function sends array in UART --------------------------------
// This array have to have end line character (0)
void UART_send_array_unblocking(n_uart_t n_uart, uint8_t *array)
{
	uint8_t value;
	g_send_index[(uint8_t)n_uart] = 0;
	g_array_to_send[(uint8_t)n_uart] = array;
	value = g_array_to_send[(uint8_t)n_uart][g_send_index[(uint8_t)n_uart]];
	if(0 != value)
	{
		UART_WriteByte(g_uarts[(uint8_t)n_uart], value);
		UART_EnableInterrupts(g_uarts[(uint8_t)n_uart], kUART_TxDataRegEmptyInterruptEnable);
		g_send_index[(uint8_t)n_uart] ++;
	}
}
// ------------------------------------------------------------------

// Callback set -----------------------------------------------------
void UART_set_Callback(function callback, n_uart_t n_uart)
{
	g_callbacks[(uint8_t)n_uart] = callback;
}
// ------------------------------------------------------------------

// Interrupts -------------------------------------------------------
void Interrupts(n_uart_t n_uart)
{
	uint32_t status = UART_GetStatusFlags(g_uarts[(uint8_t)n_uart]);
	uint8_t value = 0;
	if(kUART_TxDataRegEmptyFlag == (kUART_TxDataRegEmptyFlag & status))
	{
		if(0 != ((g_array_to_send[(uint8_t)n_uart])[g_send_index[(uint8_t)n_uart]]) && (MAX_SEND_LENGTH > g_send_index[(uint8_t)n_uart]))
		{
			UART_WriteByte(g_uarts[(uint8_t)n_uart], g_array_to_send[(uint8_t)n_uart][g_send_index[(uint8_t)n_uart]]);
			g_send_index[(uint8_t)n_uart] ++;
		}
		else
		{
			UART_DisableInterrupts(g_uarts[(uint8_t)n_uart], kUART_TxDataRegEmptyInterruptEnable);
		}
	}
	if(kUART_RxDataRegFullFlag == (kUART_RxDataRegFullFlag & status))
	{
		if(0 != g_callbacks[(uint8_t)n_uart])
		{
			value = UART_ReadByte(g_uarts[(uint8_t)n_uart]);
			g_callbacks[(uint8_t)n_uart](value);
		}
		UART_ClearStatusFlags(g_uarts[(uint8_t)n_uart], kUART_RxDataRegFullFlag);
	}
}

// Interrupt vector of UART 0 ---------------------------------------
void UART0_RX_TX_IRQHandler()
{
	Interrupts(uart_0);
}
// ------------------------------------------------------------------

// Interrupt vector of UART 1 ---------------------------------------
void UART1_RX_TX_IRQHandler()
{
	Interrupts(uart_1);
}
// ------------------------------------------------------------------

// Interrupt vector of UART 2 ---------------------------------------
void UART2_RX_TX_IRQHandler()
{
	Interrupts(uart_2);
}
// ------------------------------------------------------------------

// Interrupt vector of UART 3 ---------------------------------------
void UART3_RX_TX_IRQHandler()
{
	Interrupts(uart_3);
}
// ------------------------------------------------------------------

// Interrupt vector of UART 4 ---------------------------------------
void UART4_RX_TX_IRQHandler()
{
	Interrupts(uart_4);
}
// ------------------------------------------------------------------
