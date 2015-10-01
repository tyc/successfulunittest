#ifndef _PWM_H_
#define _PWM_H_

typedef enum
{
	PWM_CH0,
	PWM_CH1,
	PWM_CH2,
	PWM_CH3,
	PWM_CH_MAX
} pwm_channel_t;

bool_t init_pwm(pwm_channel_t channel);
bool_t deinit_pwm(pwm_channel_t channel);
bool_t set_frequency_pwm(uint16_t frequency, pwm_channel_t channel);
bool_t set_duty_cycle_pwm(uint8_t duty_cycle, pwm_channel_t channel);
uint16_t get_frequency_pwm(pwm_channel_t channel);
uint8_t get_duty_cycle(pwm_channel_t channel);

#endif /* _PWM_H_ */
