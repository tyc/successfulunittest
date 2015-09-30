#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <assert.h>

#include <stdio.h>

#include "std_types.h"
#include "pwm.h"
#include "pwm_if.h"

bool_t init_pwm_return_value = FALSE;

bool_t init_pwm(pwm_channel_t channel)
{
	bool_t retVal = FALSE;

	if (channel < PWM_CH_MAX)
	{
		retVal = init_pwm_return_value;
	}

	return (retVal);
}

bool_t deinit_pwm(pwm_channel_t channel)
{
	bool_t retVal = FALSE;
	if (channel < PWM_CH_MAX)
	{
		retVal = TRUE;
	}
	return (retVal);
}

bool_t set_frequency_pwm(uint16_t frequency, pwm_channel_t channel)
{
	bool_t retVal = FALSE;
	if (channel < PWM_CH_MAX)
	{
		frequency = 0;
		retVal = TRUE;
	}
	return (retVal);
}

bool_t set_duty_cycle_pwm(uint8_t duty_cycle, pwm_channel_t channel)
{
	bool_t retVal = FALSE;
	if (channel < PWM_CH_MAX)
	{
		duty_cycle = 0;
		retVal = TRUE;
	}
	return (retVal);
}

uint16_t get_frequency_pwm(pwm_channel_t channel)
{
	uint16_t retVal = FALSE;
	if (channel < PWM_CH_MAX)
	{
		retVal = TRUE;
	}
	return (retVal);
}

uint8_t get_duty_cycle(pwm_channel_t channel)
{
	uint8_t retVal = 0;
	if (channel < PWM_CH_MAX)
	{
		retVal = 12;
	}
	return (retVal);
}



/** test case for init_pwm_if() */
static void init_pwm_if_test_case(void)
{
	uint8_t index;

	typedef struct
	{
		/* inputs */
		pwm_channel_t input1;
		bool_t input2;

		/* output */
		bool_t output;
	} test_data_t;

	test_data_t test_data[] =
	{
		{PWM_CH1-1, 	TRUE, FALSE},
		{PWM_CH1,   	TRUE, TRUE},
		{PWM_CH1+1, 	TRUE, TRUE},
		{PWM_CH_MAX-1,  TRUE, TRUE},
		{PWM_CH_MAX, 	TRUE, FALSE},
		{PWM_CH_MAX+1, 	TRUE, FALSE},
		{PWM_CH1+1, 	FALSE, FALSE},
	};

	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		bool_t output;
		init_pwm_return_value = test_data[index].input2;
		output = init_pwm_if(test_data[index].input1);

		assert(output == test_data[index].output);
	}
}


int main(void) {
	init_pwm_if_test_case();

	return (0);
}


