/*
 * brief  : This module controls the UART
 * date   : 13-March-2023
 * Author : Luis Roberto Vega Campos & Luis Ruben Padilla Ortiz
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_uart.h"
#include "fsl_clock.h"
#include "fsl_port.h"

// Source clock of UART ---------
#define UART_CLOCK_SOURCE_0 UART0_CLK_SRC
#define UART_CLOCK_SOURCE_1 UART1_CLK_SRC
#define UART_CLOCK_SOURCE_2 UART2_CLK_SRC
#define UART_CLOCK_SOURCE_3 UART3_CLK_SRC
#define UART_CLOCK_SOURCE_4 UART4_CLK_SRC
// ------------------------------

// Limit of data to send --------
#define MAX_SEND_LENGTH 100
// ------------------------------

// Ports for UARTS --------------
#define UART0_PORT PORTB
#define UART1_PORT PORTC
#define UART2_PORT PORTC
#define UART3_PORT PORTC
#define UART4_PORT PORTC
// ------------------------------

// Pins for UARTS ---------------
#define UART0_RX  16u
#define UART0_TX  17u
#define UART0_CTS 15u
#define UART0_RTS 14u

#define UART1_RX 3u
#define UART1_TX 4u
#define UART1_CTS 2u
#define UART1_RTS 1u

#define UART2_RX 3u
#define UART2_TX 4u
#define UART2_CTS 2u
#define UART2_RTS 1u

#define UART3_RX 16u
#define UART3_TX 17u
#define UART3_CTS 15u
#define UART3_RTS 14u

#define UART4_RX 14u
#define UART4_TX 15u
#define UART4_CTS 13u
#define UART4_RTS 12u

// ------------------------------

// CLOCK for UART ports ---------
#define UART0_PORT_CLOCK kCLOCK_PortB
#define UART1_PORT_CLOCK kCLOCK_PortC
#define UART2_PORT_CLOCK kCLOCK_PortC
#define UART3_PORT_CLOCK kCLOCK_PortC
#define UART4_PORT_CLOCK kCLOCK_PortC
// ------------------------------

typedef enum
{
	uart_0 = 0u,
	uart_1 = 1u,
	uart_2 = 2u,
	uart_3 = 3u,
	uart_4 = 4u,
}n_uart_t;

typedef void(*function)(uint8_t receptor_value);

// Initialize UART PORTS --------------------------------------------
void PORT_initialization(n_uart_t n_uart);
// ------------------------------------------------------------------

// UART initialization ----------------------------------------------
void UART_initialization(n_uart_t n_uart, uint32_t baudrate, uart_parity_mode_t parity, uart_stop_bit_count_t stop_bit, bool enable_interrupts, bool dma_enable);
// ------------------------------------------------------------------

// UART RX value ----------------------------------------------------------------------------
uint8_t *get_data_adress(n_uart_t n_uart);
// ------------------------------------------------------------------------------------------

// UART init with configuration -------------------------------------
void UART_initialization_conf(n_uart_t n_uart, uart_config_t configuration, bool enable_interrupts, bool dma_enable);
// ------------------------------------------------------------------

// This function is for configure interrupts ------------------------
void UART_cofig_interrupts(bool enable, uint32_t mask, n_uart_t n_uart);
// ------------------------------------------------------------------

// This function sends only one byte in UART ------------------------
void UART_send_byte(n_uart_t n_uart, uint8_t byte);
// ------------------------------------------------------------------

// This function sends array in UART --------------------------------
// This array have to have end line character (0)
void UART_send_array_blocking(n_uart_t n_uart, uint8_t *array);
// ------------------------------------------------------------------

// This function sends array in UART --------------------------------
void UART_send_array_unblocking(n_uart_t n_uart, uint8_t n_bytes, uint8_t *array);
// ------------------------------------------------------------------

// Callback set -----------------------------------------------------
void UART_set_Callback(function callback, n_uart_t n_uart);
// ------------------------------------------------------------------

// Interrupt vector of UART 0 ---------------------------------------
void UART0_RX_TX_IRQHandler();
// ------------------------------------------------------------------

// Interrupt vector of UART 1 ---------------------------------------
void UART1_RX_TX_IRQHandler();
// ------------------------------------------------------------------

// Interrupt vector of UART 2 ---------------------------------------
void UART2_RX_TX_IRQHandler();
// ------------------------------------------------------------------

// Interrupt vector of UART 3 ---------------------------------------
void UART3_RX_TX_IRQHandler();
// ------------------------------------------------------------------

// Interrupt vector of UART 4 ---------------------------------------
void UART4_RX_TX_IRQHandler();
// ------------------------------------------------------------------

// Interrupt vector of UART 5 ---------------------------------------
void UART5_RX_TX_IRQHandler();
// ------------------------------------------------------------------

#endif /* UART_H_ */
