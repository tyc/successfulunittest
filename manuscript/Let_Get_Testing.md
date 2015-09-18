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

Taking the function `init_pwm_if()`, it returns a bool_t of TRUE if it was initialised successfully. The truth table for its operation is 

| input1             | output     |
| `pwm_if_channel_t` | `bool_t`   |
| ------------------ |:----------:|
| `PWM_CH0 - 1`      | `FALSE`    |
| `PWM_CH0`          | `TRUE`     |
| `PWM_CH0 + 1`      | `TRUE`     |
| `PWM_CH_MAX - 1`   | `TRUE`     |
| `PWM_CH_MAX`       | `FALSE`    |
| `PWM_CH_MAX + 1`   | `FALSE`    |

The left hand column is the input values, and the right hand side is the return value. This truth table shows that the whole range of possible values are used. It starts from the lowest value of `PWM_CH0`, up to the maximum value of `PWM_CH_MAX`. With the C program language being used, it is based with index starting at 0. This is why a `FALSE` is returned when the input is PWM_CH_MAX. A good test is to test 1 item on either of the maximum and minimum value. This would catch the one-off bugs.

The function description of the `init_pwm_if()` indicates the lower level MCAL is also initialised. In the unit test, a mock function is used to simulate the initialisation of the PWM peripheral. In your mock function, a flag is set to indicate that it has been called. There could also be a counter in the mock function to count the number of times it has been called. This is check that the PWM peripheral is initialised the correct number of times.

So far, we have only consider inputs coming into the software module via the caller. There is also inputs coming into the software module on lower level functions. As mentioned before, it calls the microcontroller's peripheral. In this case, it calls `init_pwm()` to initialise peripheral. `init_pwm()` returns a boolean. It has the function prototype of

    bool_t init_pwm(pwm_channel_t channel);
    
The truth table now becomes

| input1             | input2                 | output       |
| `channel`          | return from init_pwm() | `bool_t`     |
| ------------------ |:----------------------:|:------------:|
| `PWM_CH0 - 1`      | `TRUE`     		      | `FALSE`      |
| `PWM_CH0`          | `TRUE`                 | `TRUE`       |
| `PWM_CH0 + 1`      | `TRUE`                 | `TRUE`       |
| `PWM_CH_MAX - 1`   | `TRUE`                 | `TRUE`       |
| `PWM_CH_MAX`       | `TRUE`                 | `FALSE`      |
| `PW‚M_CH_MAX + 1`   | `TRUE`                 | `FALSE`      |
| `don't care`       | `FALSE`                | `FALSE`      |

When the return value from `init_pwm()` is a FALSE, the output from `init_pwm_if()` is always a FALSE. It does not matter what value is passed into `init_pwm_if()`, this is indicated as a `don't care`.

## Test Script

The test script is the implementation for the test case. To implemented, the test case should be close to complete.

```C

bool_t init_pwm_return_value = FALSE;

/** Mock function for init_pwm() */
bool_t init_pwm(pwm_channel_t channel)
{
	bool_t retVal = FALSE;
	
	if (channel < PWM_CH_MAX)
	{
		retVal = init_pwm_return_value;
	}
	
	return (retVal);
}

/** test case for init_pwm_if() */
void init_pwm_if_test_case(void)
{
	uint8 index;
	
	typedef struct 
	{
		/* inputs */
		pwm_channel_t input1;
		bool_t input2;
		
		/* output */
		bool_t output;
	} test_data_t;
	
	test_data_t test_dat[] =
	{
		{PWM_CH0-1, 	TRUE, FALSE},
		{PWM_CH0,   	TRUE, TRUE},
		{PWM_CH0+1, 	TRUE, TRUE},
		{PWM_CH_MAX-1,  TRUE, TRUE},
		{PWM_CH_MAX, 	TRUE, FALSE},
		{PWM_CH_MAX+1  	TRUE, FALSE},
		{PWM_CH0+1, 	FALSE, FALSE},
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
```

