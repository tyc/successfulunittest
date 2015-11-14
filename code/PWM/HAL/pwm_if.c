/**
 * include the system headers
 */
#include "std_types.h"

/**
 * include the software module headers, types definitions and driver
 * level functions
 */
#include "pwm.h"
#include "pwm_if.h"

#define MIN_FREQUENCY_PWM_IF		50
#define MAX_FREQUENCY_PWM_IF		500
#define MIN_DUTY_CYCLE_PWM_IF		10
#define MAX_DUTY_CYCLE_PWM_IF		90
#define FREQUENCY_OFFSET			20

STATIC bool_t init_pwm_reinit_flag = FALSE;

/* initialise the PWM software stack
 */
bool_t init_pwm_if(pwm_if_channel_t channel)
{
	bool_t retVal = FALSE;

	if (init_pwm_reinit_flag == FALSE)
	{
		if (channel < PWM_CH_MAX)
		{
			retVal = init_pwm((pwm_channel_t)(channel));

			if (retVal == TRUE)
			{
				init_pwm_reinit_flag  = TRUE;
			}
		}
	}

	return (retVal);
}


bool_t deinit_pwm_if(pwm_if_channel_t channel)
{
	bool_t retVal = FALSE;

	if (channel < PWM_CH_MAX)
	{
		retVal = deinit_pwm(channel);
	}

	return (retVal);
}

bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal)
{
	bool_t retVal = FALSE;

	if (channel < PWM_CH_MAX)
	{
		if ((signal.frequency >= MIN_FREQUENCY_PWM_IF) &&
			(signal.frequency <= MAX_FREQUENCY_PWM_IF))
		{
			if ((signal.duty_cycle >= MIN_DUTY_CYCLE_PWM_IF) &&
				(signal.duty_cycle <= MAX_DUTY_CYCLE_PWM_IF))
			{
				signal.frequency = signal.frequency + FREQUENCY_OFFSET;
				if ((set_frequency_pwm(signal.frequency, channel) == TRUE) &&
					(set_duty_cycle_pwm(signal.duty_cycle, channel) == TRUE))
				{
					retVal = TRUE;
				}
				else
				{
					retVal = FALSE;
				}
			}
		}
	}

	return (retVal);
}

bool_t get_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t *signal)
{
	bool_t retVal = FALSE;

	if ((channel < PWM_CH_MAX) &&
		(signal != NULL))
	{
		signal->frequency = get_frequency_pwm(channel);
		signal->duty_cycle = get_duty_cycle(channel);

		retVal = TRUE;
	}

	return (retVal);
}
