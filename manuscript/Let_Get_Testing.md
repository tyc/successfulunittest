# Lets start testing

I will go through the concepts used in unit test by going through an example. 

Let's say that I need to write a software stack for controlling the PWM signal out of the microcontroller. The PWM signal is used to drive a DC motor. The duty cycle of the PWM signal. The larger the duty cycle, the faster the DC motor will run.

In a typical application, the software would be used to regulate the speed of the DC motor by continuously adjust the duty cycle. The adjustments are based on the measured speed of the DC motor.

To ease the development of the software, it is a good idea to break the software into two layers. The layer that is closer to the hardware is usually known as the device driver, and the layer above it is known as the manager or the handler. Together, they are known as a software stack. In a more complex software stack, there could be more software modules or layers.

For this example, I am not going to be use a unit test framework such as Cmocka or Ceedling. I want to show the basics of constructing a unit test with all the inner workings.  

## Requirements

Let's start by laying out the requirements so that we know what is to be built.

    req_PWM1: The PWM driver shall only allow a frequency from 50Hz to 500Hz.
    req_PWM2: The PWM driver shall only allow a duty cycle from 10% to 90%.
    req_PWM3: When PWM driver receives request out of the allowable frequencies and duty cycles, it shall reject the request and the output signal does not change.
    req_PWM4: The amplitude of the PWM signal is at logic level of the microcontroller.
    req_PWM5: The tolerance of the PWM signal is 5% of the requested.
    req_PWM6: The PWM signal will update its speed based upon the measured speed of the DC motor.
    req_PWM7: The speed of the DC motor is to be regulated to 5% of the intended speed. The relationship between the RPM speed of the motor to duty cycle is PWM speed is the RPM increase at 75% of duty cycle. 
    req_PWM9: The PWM driver shall not consume more than 1KByte of code space.
    req_PWM10: The PWM driver shall not consume more than 5% of its allocate time slot.
    
This set of requirements are only developed to show the benefit of unit tests. Some of the may not actually work in a real world software stack.

Looking closer at the requirements, the majority of them can be implemented in the manager. The frees up the implementation of the device driver from any requirements. However, it is should keep the basic software engineering practice such as keeping it as lean as possible, and also as simple as possible.

`req_PWM8` and `req_PWM9` are not functional requirements, so it can not be tested during the unit tests. Furthermore, it is highly dependent on platform it is deployed upon, and the compiler used.

Once the requirements are agreed by all the parties, including the customer, requirements engineer, the software validation engineer and the hardware engineering team, the design and the test cases can be develop for each requirement. In many cases, getting agreement on the requirements is a long process. The agreement may not even be reached when the design is needed. However, do let that hold you back. After you have designed a few software modules, you will get a sense of basic structure of a software stack.

Above its functional requirements, there will also be architecture requirements for the software module to meet.

## Design of the PWM interface

For the purpose of this book, the following functions to the PWM software stack are created. Here are their function prototypes.

	bool_t init_pwm_if(pwm_if_channel_t channel);
	bool_t deinit_pwm_if(pwm_if_channel_t channel);
	bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal);
	bool_t get_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t *signal);


I have adopted the format of `<action>_<module>_<layer>()` for the function names. So looking at the function name, it is immediately obvious what its purpose. More context is provided by which layer the module is residing.

So for the function `get_pwm_if()`, you know that it get some data or information about a particular PWM channel and it is in the interface layer, this layer is also known as the hardware abstraction layer. Application code would interact with the hardware via this functions. If PWM signal at 50Hz at 75% duty cycle is needed, it would call `set_pwm_if()`. If the software is exiting sleep mode, it would call `init_pwm_if()`.

The design focus of the functions is to have a good set of functions to affect the driver's characteristics. Here a good set also means not to provide too much functionality. The higher level of functionality, the more complex it is, and the higher risk level. The dependency between the functions should also be kept to a minimum. This design rule would also reduce the complexity of the unit tests.

## Test Case Design for `init_pwm_if()`

To develop a test case, a good understanding of the environment which the software module is going to be used in is crucial. Without this understanding, the inputs will be pure nonsense to the testing.

Taking the function `init_pwm_if()`, it returns a `bool_t` of TRUE if it was initialised successfully. The truth table for its operation is 

| input1             | output     |
| `pwm_if_channel_t` | `bool_t`   |
| ------------------ |:----------:|
| `PWM_CH0 - 1`      | `FALSE`    |
| `PWM_CH0`          | `TRUE`     |
| `PWM_CH0 + 1`      | `TRUE`     |
| `PWM_CH_MAX - 1`   | `TRUE`     |
| `PWM_CH_MAX`       | `FALSE`    |
| `PWM_CH_MAX + 1`   | `FALSE`    |

The left hand column are the input values, and the right hand side are the return values. This truth table shows that the whole range of possible values are used. It starts from the lowest value of `PWM_CH0`, up to the maximum value of `PWM_CH_MAX`. With the C program language being used, it is based with index starting at 0. This is why a `FALSE` is returned when the input is PWM_CH_MAX. A good test is to test 1 item on either of the maximum and minimum value. This would catch the one-off bugs.

The function description of the `init_pwm_if()` indicates the lower level MCAL is also initialised. In the unit test, a mock function is used to simulate the initialisation of the PWM peripheral. In your mock function, a flag is set to indicate that it has been called.

So far, we have only consider inputs coming into the software module via the caller. There is also inputs coming into the software module on lower level functions. As mentioned before, it calls the microcontroller's peripheral. In this case, it calls `init_pwm()` to initialise peripheral. `init_pwm()` returns a boolean. It has the function prototype of

    bool_t init_pwm(pwm_channel_t channel);
    
The truth table now becomes

| input1             | input2                 | output       |
| `channel`          | return from init_pwm() | `bool_t`     |
| ------------------ |:----------------------:|:------------:|
| `PWM_CH0 - 1`      | `don't care`           | `FALSE`      |
| `PWM_CH0`          | `TRUE`                 | `TRUE`       |
| `PWM_CH0 + 1`      | `TRUE`                 | `TRUE`       |
| `PWM_CH_MAX - 1`   | `TRUE`                 | `TRUE`       |
| `PWM_CH_MAX`       | `don't care`           | `FALSE`      |
| `PWM_CH_MAX + 1`   | `don't care`           | `FALSE`      |
| `don't care`       | `FALSE`                | `FALSE`      |

When the return value from `init_pwm()` is a FALSE, the output from `init_pwm_if()` is always a FALSE. It does not matter what value is passed into `init_pwm_if()`, this is indicated as a `don't care`.

The requirement for `init_pwm_if()` is that it is must only be initialised once before it it is de-initialised. Further initialisation is forbidden. In a real world example, if the PWM is initialsed when it is operating, it could cause serious damage if it was controlling machinery. So it is critical if there is a catch in the code for this scenario. Internal of `init_pwm_if()` is a flag that indicates if initialisation had occurred. If it is TRUE when initialisation is called, `init_pwm_if()` will return a FALSE.

The truth table now becomes

| input1             | input2                 | input3             | output       |
| `channel`          | return from init_pwm() | flag for reinit    | `bool_t`     |
| ------------------ |:----------------------:|:------------------:|:------------:|
| `PWM_CH0 - 1`      | `don't care`           | `FALSE`            | `FALSE`      |
| `PWM_CH0`          | `TRUE`                 | `FALSE`            | `TRUE`       |
| `PWM_CH0 + 1`      | `TRUE`                 | `FALSE`            | `TRUE`       |
| `PWM_CH_MAX - 1`   | `TRUE`                 | `FALSE`            | `TRUE`       |
| `PWM_CH_MAX`       | `don't care`           | `FALSE`            | `FALSE`      |
| `PWM_CH_MAX + 1`   | `don't care`           | `FALSE`            | `FALSE`      |
| `don't care`       | `FALSE`                | `FALSE`            | `FALSE`      |
| `don't care`       | `dont't care`          | `TRUE`             | `FALSE`      |

Instead of a reinit flag, there could also be a counter in the mock function to count the number of times it has been called. This is check that the PWM peripheral is initialised the correct number of times.


## Test Script for `init_pwm_if()`

The test script is the implementation for the test case. 

```
bool_t init_pwm_return_value = FALSE;

/** Mock function for init_pwm() */
bool_t init_pwm(pwm_channel_t channel)
{
	bool_t retVal = FALSE;
	
	if (init_pwm_reinit_flag == FALSE)
	{
		if (channel < PWM_CH_MAX)
		{
			retVal = init_pwm_return_value;
		}
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
		pwm_if_channel_t channel;
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
		{PWM_CH_MAX+1,  	TRUE, FALSE, FALSE},
		{PWM_CH0+1, 	FALSE, FALSE, FALSE},
		{PWM_CH0+1, 	FALSE, TRUE, FALSE},
	};
	
	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		bool_t expected_init_OK;
		init_pwm_return_value = test_data[index].init_pwm_return_value;
		init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
		expected_init_OK = init_pwm_if(test_data[index].channel);
		
		assert_true(expected_init_OK == test_data[index].expected_init_OK);
	}
}
```

So pattern for writing a unit test is pretty straight forward. The concept is to iterate a set of inputs and check that the response from it is the same as the expected. The test code above has this pattern.

So breaking the test script to its parts, at the start of the test script, a struct is created to contain the inputs and the expected output. Give the members meaningful names rather than `input1`, `input2` and `input3`. The meaningful names will allow the test code to be read easily. In this example, there are three inputs and one output. Ensure that the member the same type of the tested parameters. The struct is local to this particular test case. For each test case, create a struct suitable for your test.

	typedef struct 
	{
		/* inputs */
		pwm_channel_t channel;
		bool_t init_pwm_return_value;
		bool_t init_pwm_reinit_flag;
		
		/* output */
		bool_t expected_init_OK;
	} test_data_t;

The variable `init_pwm_return_value` is used to configure the return value of the mock function. In this case, the mock function `init_pwm()` will return the value `init_pwm_return_value`. So make sure that the type of `init_pwm_return_value` has the same type as the return type of `init_pwm()`.

The variable `init_pwm_reinit_value` is used to configure the mock function if it has been initialised or not. If it is set to TRUE, the mock function will behave as if it has been initalised.

The construction of the test code should be next. It is constructed as a `for` loop where the test data is iterated over by the test code. It iterates for the entire dataset.
 
	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		bool_t expected_init_OK;
		init_pwm_return_value = test_data[index].init_pwm_return_value;
		init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
		expected_init_OK = init_pwm_if(test_data[index].channel);
		
		assert(expected_init_OK == test_data[index].expected_init_OK);
	}

The code within the `for` loop does the actual testing. At the start of the loop, it configures the mocked objects for the return values, calls the function under tests. It checks that the return values are correct. The checking is via the `assert_true()` macro, an assertion is generated if the test clause evaluates to a FALSE. 

## Building the code

To execute the test, it is necessary to build the binary. I have adopted a simple makefile to build the binary. It is clear and simple to see what is going on.

Assuming that you are using gcc, you can compile the binary using the following command.

	gcc -I../../include -I../MCAL pwm_if_test.c pwm_if.c -o unittest
  

The include paths as specified by the `-I` option specifies where the headers are. Feel free to change them for your setup. In this example, it is including the header for the` pwm.h `by including the `MCAL` directory, and it is including `std_types.h` from the `include` directory.

If the unit test passes, no assertion will be generated. An assertion triggered off is a failed test case.

## Code coverage

For this test case, it is important to check the code coverage is adequate. This is a question that needs to be constantly asked, and the initial target is always 100% coverage for code and branch. If neither of these two metrics are at 100% coverage when you release the software module, there are gaps in the code that were not tested.

The analysis for the code coverage is highly dependent on the implementation of the code. 

The implementation for `init_pwm_if()` is as follows

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
	
Performing manual code coverage measurements is impossible to get right once the software module implementation gets complicated. It is better if the unit test frame work measures code coverage. You are in luck if your unit test frame work builds its test binaries using GNU tools. By specifying your build with the `-ftest-coverage -fprofile-arcs`, the coverage will be automatically measured. This is using the `gcov` features of the GNU toolchain.

## Getting access to internal variables

In the function `init_pwm_if()`