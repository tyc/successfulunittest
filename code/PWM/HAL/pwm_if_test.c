#include <assert.h>

#include <stdio.h>

#include "std_types.h"
#include "pwm.h"
#include "pwm_if.h"

bool_t init_pwm_return_value = FALSE;
extern bool_t init_pwm_reinit_flag;

/***********************************************************************
 * mock functions
 ***********************************************************************
 */
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


/***********************************************************************
 * test cases
 ***********************************************************************
 */

/** test case for init_pwm_if() */
static void init_pwm_if_test_case(void)
{
	uint8_t index;

	typedef struct
	{
		/* inputs */
		pwm_channel_t channel;
		bool_t init_pwm_return_value;
		bool_t init_pwm_reinit_flag;

		/* output */
		bool_t expected_init_OK;
	} test_data_t;

	test_data_t test_data[] =
	{
		{PWM_CH0-1, 	TRUE, FALSE, FALSE},
		{PWM_CH0,   	TRUE, FALSE, TRUE},
		{PWM_CH0+1, 	TRUE, FALSE, TRUE},
		{PWM_CH_MAX-1,  TRUE, FALSE, TRUE},
		{PWM_CH_MAX, 	TRUE, FALSE, FALSE},
		{PWM_CH_MAX+1, 	TRUE, FALSE, FALSE},
		{PWM_CH0+1, 	FALSE, FALSE, FALSE},
		{PWM_CH0+1, 	FALSE, TRUE, FALSE},
	};

	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		bool_t output;
		init_pwm_return_value = test_data[index].init_pwm_return_value;
		init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
		output = init_pwm_if(test_data[index].channel);

		assert(output == test_data[index].expected_init_OK);
	}
}


bool_t init_pwm_return_value = FALSE;
extern bool_t init_pwm_reinit_flag;

/** test case for set_pwm_if() */
void set_pwm_if_test(void)
{
	uint8 index;

	typedef struct
	{
		/* inputs */
		pwm_signal_t signal;		/* for freq and duty cycle */
		pwm_channel_t channel;
		bool_t return_for_set_pwm;

		/* output */
		bool_t flag_calling_set_pwm;
		bool_t return_from_function_call;
	} test_data_t;

	/* set up the test data. It is based the truth table.
	 */
	test_data_t test_data[] =
	{
		{{50, 	10}, PWM_CH0, TRUE,		TRUE,	TRUE },
		{{49, 	10}, PWM_CH0, TRUE, 	TRUE,	FALSE},
		{{51, 	10}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{499,	10}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{500,	10}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{501,	10}, PWM_CH0, TRUE, 	TRUE,	FALSE},
		{{50,	9 }, PWM_CH0, TRUE, 	TRUE,	FALSE},
		{{50,	11}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{50,	89}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{50,	90}, PWM_CH0, TRUE,     TRUE,	TRUE },
		{{50,	91}, PWM_CH0, TRUE, 	TRUE,	FALSE},
		{{50,	90}, PWM_CH0, FALSE,    TRUE,	FALSE},
	};


	/* set up the loop to iterate over all the each combination
	 * of the test data in the truth table.
	 */
	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		/* setup the inputs into the function under test */
		bool_t return_from_function_call;

		return_value_for_set_pwm = test_data[index].return_for_set_pwm;
		flag_calling_set_pwm = FALSE;

		/* execute the function */
		expected_init_OK = set_pwm_if(
				test_data[index].channel,
				test_data[index].signal);

		/* check that the result is the same as the expected */
		assert(return_from_function_call == test_data[index].return_from_function_call);
		assert(flag_calling_set_pwm == test_data[index].flag_calling_set_pwm);
	}
}



int main(void) {

	init_pwm_if_test_case();

	return (0);
}


