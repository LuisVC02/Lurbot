/*
 * brief   : This module has a main program of robot
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "config.h"
#include "traction.h"
#include "direction.h"
#include "controller.h"

int main()
{
	CLOCK_SetSimSafeDivs();
	traction_init();
	traction_t traction =
	{
			forward_t,
			50
	};
	set_speed(traction);
	traction.speed = 100;
	set_speed(traction);
	traction.speed = 50;
	set_speed(traction);
	traction.speed = 0;
	set_speed(traction);

	while(1)
	{

	}
	return 0;
}
