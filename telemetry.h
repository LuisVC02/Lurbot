/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "uart.h"
#include "pit.h"
#include "dma.h"
#include "fsl_clock.h"
#include "fsl_port.h"

#define PIT_CHANNEL kPIT_Chnl_2

void telemetry_init();

void telemetry_send_unblocking(uint8_t n_bytes, uint8_t* bytes_to_send);

#endif /* TELEMETRY_H_ */
