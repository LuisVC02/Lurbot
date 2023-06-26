 /*
 *	brief : This module controls GPIO, this include interrupts
 *
 *	author: Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
 *	date  : 17-feb-2023
 */

#ifndef GPIO_H_
#define GPIO_H_


#include "stdint.h"
#include "stdbool.h"
#include "fsl_gpio.h"


/*! These constants are used to select an specific port in the different API functions*/
typedef enum
{
	GPIO_A, /*!< Definition to select GPIO A */
	GPIO_B, /*!< Definition to select GPIO B */
	GPIO_C, /*!< Definition to select GPIO C */
	GPIO_D, /*!< Definition to select GPIO D */
	GPIO_E, /*!< Definition to select GPIO E */
	GPIO_F  /*!< Definition to select GPIO F */
} gpio_name_t;

typedef struct
{
	uint32_t flag_port_a;
	uint32_t flag_port_b;
	uint32_t flag_port_c;
	uint32_t flag_port_d;
	uint32_t flag_port_e;
	uint32_t flag_port_f;
} gpio_interrupt_flags_t;


typedef enum
{
	InterrupDisable,	/*!< Interrupt/DMA request is disabled. */
	DMARisingEdge, 		/*!< DMA request on rising edge. */
	DMAFallingEdge, 	/*!< DMA request on falling edge. */
	DMAEitherEdge,		/*!< DMA request on either edge. */
	LogicZero, 			/*!< Interrupt when logic zero. */
	RisingEdge, 		/*!< Interrupt on rising edge. */
	FallingEdge, 		/*!< Interrupt on falling edge. */
	EitherEdge, 		/*!< Interrupt on either edge. */
	LogicOne  			/*!< Interrupt when logic one. */
} intr_logic_t;


typedef struct
{
	gpio_name_t gpio;				//Stands for GPIO
	uint8_t pin;					//Stands for pin number
	bool in_out;					//High in_out stands for output
	bool interrupt_en;				//High when interrupt enable
	intr_logic_t interrupt_logic;	//Interrupt logic
} gpio_pin_conf_t;


// Initialize GPIO --------------------------
void GPIO_init(gpio_pin_conf_t );
// ------------------------------------------

void GPIO_set_values(gpio_name_t gpio, uint32_t value);
void GPIO_clear_values(gpio_name_t gpio, uint32_t value);
void GPIO_toggle_values(gpio_name_t gpio, uint32_t value);

void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags));

uint32_t get_gpio(gpio_name_t gpio);

bool get_pin_gpio(gpio_name_t gpio, uint8_t pin);

// GPIO interrupts --------------------------
void PORTA_IRQHandler(void);

void PORTB_IRQHandler(void);

void PORTC_IRQHandler(void);

void PORTD_IRQHandler(void);
// ------------------------------------------


#endif /* GPIO_H_ */
