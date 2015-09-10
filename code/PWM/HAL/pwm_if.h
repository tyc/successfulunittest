#ifndef _PWM_IF_H_
#define _PWM_IF_H_

typedef enum
{
	PWM_IF_CH1,
	PWM_IF_CH2,
	PWM_IF_CH3,
	PWM_IF_CH4,
	PWM_IF_CH_MAX,
} pwm_if_channel_t;

typedef struct
{
	uint16_t frequency;
	uint8_t duty_cycle;
} pwm_if_signal_t;

bool_t init_pwm_if(pwm_if_channel_t channel);
bool_t deinit_pwm_if(pwm_if_channel_t channel);
bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal);
bool_t get_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t *signal);

#endif /* _PWM_IF_H_ */
