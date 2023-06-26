/*
 *	brief : This module controls GPIO, this include interrupts
 *
 *	author: Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
 *	date  : 17-feb-2023
 */

#include "gpio.h"
#include "fsl_port.h"
#include "fsl_clock.h"

typedef GPIO_Type *GPIO_modules;
typedef PORT_Type *PORT_modules;

const gpio_pin_config_t gpio_input_config = {
		        kGPIO_DigitalInput,
		        0,
		    };



const port_pin_config_t button_config = {
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	 };

const gpio_pin_config_t gpio_output_config = {
		        kGPIO_DigitalOutput,
		        0,
		    };

const port_interrupt_t port_intr_config[] = {
	kPORT_InterruptOrDMADisabled,
	kPORT_DMARisingEdge,
	kPORT_DMAFallingEdge,
	kPORT_DMAEitherEdge,
	kPORT_InterruptLogicZero,
	kPORT_InterruptRisingEdge,
	kPORT_InterruptFallingEdge,
	kPORT_InterruptEitherEdge,
	kPORT_InterruptLogicOne
};

const GPIO_modules gpios [] = {
		GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE
};

const PORT_modules ports[] = {
		PORTA,
		PORTB,
		PORTC,
		PORTD,
		PORTE
};

const clock_ip_name_t clocks[] = {
		kCLOCK_PortA,
		kCLOCK_PortB,
		kCLOCK_PortC,
		kCLOCK_PortD,
		kCLOCK_PortE
};

volatile static gpio_interrupt_flags_t g_intr_status_flag = {0};

static void (*gpio_A_callback)(uint32_t flags) = 0;
static void (*gpio_B_callback)(uint32_t flags) = 0;
static void (*gpio_C_callback)(uint32_t flags) = 0;
static void (*gpio_D_callback)(uint32_t flags) = 0;
static void (*gpio_E_callback)(uint32_t flags) = 0;


// Initialize GPIO --------------------------------------------------------------
void GPIO_init(gpio_pin_conf_t pin_conf)
{
	if(true == pin_conf.in_out)										//Outputs
	{
		CLOCK_EnableClock(clocks[pin_conf.gpio]);
		PORT_SetPinMux(ports[pin_conf.gpio], pin_conf.pin, kPORT_MuxAsGpio);
		GPIO_PinInit(gpios[pin_conf.gpio], pin_conf.pin, &gpio_output_config);

	}
	else															//Inputs
	{
		CLOCK_EnableClock(clocks[pin_conf.gpio]);
		GPIO_PinInit(gpios[pin_conf.gpio], pin_conf.pin, &gpio_input_config);
		PORT_SetPinConfig(ports[pin_conf.gpio], pin_conf.pin, &button_config);
		if(true == pin_conf.interrupt_en){
			PORT_SetPinInterruptConfig(ports[pin_conf.gpio], pin_conf.pin, port_intr_config[pin_conf.interrupt_logic]);
		}
	}
}

void GPIO_set_values(gpio_name_t gpio, uint32_t value)
{
	GPIO_PortSet(gpios[gpio], value);
}
void GPIO_clear_values(gpio_name_t gpio, uint32_t value)
{
	GPIO_PortClear(gpios[gpio], value);
}
void GPIO_toggle_values(gpio_name_t gpio, uint32_t value)
{
	GPIO_PortToggle(gpios[gpio], value);
}

void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags))
{
	if(GPIO_A == gpio)
	{
		gpio_A_callback = handler;
	}
	else if(GPIO_B == gpio)
	{
		gpio_B_callback = handler;
	}
	else if(GPIO_C == gpio)
	{
		gpio_C_callback = handler;
	}
	else if(GPIO_D == gpio)
	{
		gpio_D_callback = handler;
	}
	else if(GPIO_E == gpio)
	{
		gpio_E_callback = handler;
	}

}

uint32_t get_gpio(gpio_name_t gpio)
{
	return gpios[gpio]->PDIR;
}

bool get_pin_gpio(gpio_name_t gpio, uint8_t pin)
{
	if(1 == GPIO_PinRead(gpios[gpio], pin))
	{
		return true;
	}
	return false;
}

// GPIO interrupts --------------------------------------------------------------
void PORTA_IRQHandler(void)
{
	g_intr_status_flag.flag_port_a = GPIO_PortGetInterruptFlags(GPIOA);

	if(gpio_A_callback)
	{
		gpio_A_callback(g_intr_status_flag.flag_port_a);
	}

	GPIO_PortClearInterruptFlags(GPIOA, 0xFFFF);
}

void PORTB_IRQHandler(void)
{
	g_intr_status_flag.flag_port_b = GPIO_PortGetInterruptFlags(GPIOB);

	if(gpio_B_callback)
	{
		gpio_B_callback(g_intr_status_flag.flag_port_b);
	}

	GPIO_PortClearInterruptFlags(GPIOB, 0xFFFF);
}

void PORTC_IRQHandler(void)
{
	g_intr_status_flag.flag_port_c = GPIO_PortGetInterruptFlags(GPIOC);

	if(gpio_C_callback)
	{
		gpio_C_callback(g_intr_status_flag.flag_port_c);
	}

	GPIO_PortClearInterruptFlags(GPIOC, 0xFFFF);
}

void PORTD_IRQHandler(void)
{
	g_intr_status_flag.flag_port_d = GPIO_PortGetInterruptFlags(GPIOD);

	if(gpio_D_callback)
	{
		gpio_D_callback(g_intr_status_flag.flag_port_d);
	}

	GPIO_PortClearInterruptFlags(GPIOD, 0xFFFF);
}
// ------------------------------------------------------------------------------

uint32_t GPIO_get_irq_status(gpio_name_t gpio)
{
	uint32_t status = 0;

	if(GPIO_A == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	else if(GPIO_B == gpio)
	{
		status = g_intr_status_flag.flag_port_b;
	}
	else if(GPIO_C == gpio)
	{
		status = g_intr_status_flag.flag_port_c;
	}
	else if(GPIO_D == gpio)
	{
		status = g_intr_status_flag.flag_port_d;
	}
	else if(GPIO_E == gpio)
	{
		status = g_intr_status_flag.flag_port_e;
	}

	return(status);
}

void GPIO_clear_irq_status(gpio_name_t gpio)
{

	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = 0;
	}
	else if(GPIO_B == gpio)
	{
		g_intr_status_flag.flag_port_b = 0;
	}
	else if(GPIO_C == gpio)
	{
		g_intr_status_flag.flag_port_c = 0;
	}
	else if(GPIO_D == gpio)
	{
		g_intr_status_flag.flag_port_a = 0;
	}
	else if(GPIO_E == gpio)
	{
		g_intr_status_flag.flag_port_e = 0;
	}
}
