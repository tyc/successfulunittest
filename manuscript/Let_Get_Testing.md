# Let gets testing

I will go through the concepts used in unit test by going through an example. 

Let's say that I need to write a software stack for controlling the PWM signal out of the microcontroller. The PWM signal is used to drive a DC motor. The duty cycle of the PWM signal. The larger the duty cycle, the faster the DC motor will run.

In a typical application, the software would be used to regulate the speed of the DC motor by continuously adjust the duty cycle. The adjustments is based on the measured speed of the DC motor.

To ease the development of the software, it is a good idea to break the software into two layers. The layer that is closer to the hardware is known as the device driver, and the layer above it is known as the manager. Together, they are known as a software stack.

## Requirements

Let's start by laying out the requirements so that we know what is to be built.

    req_PWM1: The PWM driver shall only allow a frequency from 50Hz to 500Hz.
    req_PWM2: The PWM driver shall only allow a duty cycle from 10% to 90%.
    req_PWM3: When PWM driver receives request out of the allowable frequencies and duty cycles, it shall reject the request and the output signal does not change.
    req_PWM4: The amplitude of the PWM signal is at logic level of the microcontroller.
    req_PWM5: The tolerance of the PWM signal is 5% of the requested.
    req_PWM6: The PWM signal will update its speed based upon the measured speed of the DC motor.
    req_PWM7: The speed of the DC motor is to be regulated to 5% of the intended speed. The relationship between the RPM speed of the motor to duty cycle is PWM speed is the RPM increase at 75% of duty cycle.    
    req_PWM8: The PWM driver shall not consume more than 1KByte of code space.
    req_PWM9: The PWM driver shall not consume more than 5% of its allocate time slot.
    
This set of requirements are only developed to show the benefit of unit tests. Some of the may not actually work in a real world software stack.

Once the requirements are agreed by all the parties, including the customer, requirements engineer, the software validation engineer and the hardware engineering team, the design and the test cases can be develop for each requirement. In many cases, getting agreement on the requirements is a long process. The agreement may not even be reached when the design is needed. However, do let that hold you back. After you have designed a few software modules, you will get a sense of basic structure of a software stack.

Above its functional requirements, there will also be architecture requirements for the software module to meet.

## Design

For the purpose of this book, the following functions to the PWM software stack are created. Here are their function prototypes.

	bool_t init_pwm_if(pwm_if_channel_t channel);
	bool_t deinit_pwm_if(pwm_if_channel_t channel);
	bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal);
	bool_t get_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t *signal);


I am adopted the coding standard of using the format of `<action>_<module>_<layer>()` for the function names. So looking at the function name, it is immediately obvious what its purpose. More context is provided by which layer the module is residing.

So for the function `get_pwm_if()`, you know that it get some data or information about a particular PWM channel and it is in the interface layer, this layer is also known as the hardware abstraction layer. Application code would interact with the hardware via this functions. If PWM signal at 50Hz at 75% duty cycle is needed, it would call `set_pwm_if()`. If the software is exiting sleep mode, it would call `init_pwm_if()`.

The design focus of the functions is to have a good set of functions to affect the driver's characteristics. Here a good set also means not to provide too much functionality. The higher level of functionality, the more complex it is, and the higher risk level. 

## Test Case Design

To develop a test case, a good understanding of the environment which the software module is going to be used in is crucial. Without this understanding, the inputs will be pure nonsense to the testing.

 