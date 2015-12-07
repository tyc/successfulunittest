# Lets start testing

I will go through the concepts used in unit test by going through an example. 

Let's say that I need to write a software stack for controlling the PWM signal out of the microcontroller. The PWM signal is used to drive a DC motor. 

To ease the development of the software, it is a good idea to break the software into two layers. The layer that is closer to the hardware is usually known as the device driver, and the layer above it is known as the manager or the handler. Together, they are known as a software stack. In a more complex software stack, there could be more software modules or layers.

In general, the duty cycle of the PWM signal is what controls the speed of the DC motor. The larger the duty cycle, the faster the DC motor will run. In a typical application, the software would be used to regulate the speed of the DC motor by continuously adjust the duty cycle. The adjustments are based on the measured speed of the DC motor. To write a total software stack to control a DC motor via PWM is a complex task, so I will only implement the setting of the PWM's frequency and duty cycle.

For this example, I am not going to be use a unit test framework such as Cmocka or Ceedling. I want to show the basics of constructing a unit test with all the inner workings, so I will build the unit test using standard development methods. I will use `clang` as the compiler and will be calling it via the command line.

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
    
This set of requirements are only developed to show the benefit of unit tests. Some of the requirements may not actually work in a real world software stack.

Looking closer at the requirements, the majority of them can be implemented in the manager. This frees up the implementation of the device driver from any requirements. However, it is should be designed using the good software engineering practice such as keeping it as lean as possible, and also as simple as possible.

`req_PWM8` and `req_PWM9` are not functional requirements, so it can not be tested during the unit tests. Furthermore, it is highly dependent on platform it is deployed upon, and the compiler used.

Once the requirements are agreed by all the parties, including the customer, requirements engineer, the software validation engineer and the hardware engineering team, the design and the test cases can be develop for each requirement. In many cases, getting agreement on the requirements is a long process. The agreement may not even be reached when the design is needed. However, do let that hold you back. After you have designed a few software modules, you will get a sense of basic structure of a software stack.

Above its functional requirements, there will also be architecture requirements for the software module to meet but for this purpose of this book, the architecture requirements will not be considered.

## Design of the PWM interface

For the purpose of this guide, the following functions to the PWM software stack are created. Here are their function prototypes.

	bool_t init_pwm_if(pwm_if_channel_t channel);
	bool_t deinit_pwm_if(pwm_if_channel_t channel);
	bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal);
	bool_t get_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t *signal);


I have adopted the format of `<action>_<module>_<layer>()` for the function names. So looking at the function name, it is immediately obvious what its purpose. More context is provided by which layer the module is residing. It is good software engineering practice to have a naming standard for your project. Having a standard makes spotting mistakes easier and allows the code to better understood.  

So for the function `get_pwm_if()`, you know that it will get some data or information about a particular PWM channel and it is in the interface layer, this layer is also known as the Hardware Abstraction Layer. Application code would interact with the hardware via this functions. For example, if PWM signal at 50Hz at 75% duty cycle is needed, it would call `set_pwm_if()`. If the software is exiting sleep mode, it would call `init_pwm_if()`.

The design focus of the functions is to have a good set of functions to affect the driver's characteristics. A good set also means not to provide too much functionality. The higher level of functionality, the more complex it is, and the higher risk level. The dependency between the functions should also be kept to a minimum. This design rule would also reduce the complexity of the unit tests.

## Test Case Design for `init_pwm_if()`

To develop a test case, a good understanding of the environment which the software module is going to be used in is crucial. Without this understanding, the inputs will be pure nonsense to the testing.

Taking the function `init_pwm_if()`, it returns a `bool_t` of TRUE if it was initialised successfully. At the first attempt at test case design, the truth table for its operation is 

	| input1           | output |  
	| pwm_if_channel_t | bool_t |  
	|------------------|--------|  
	| PWM_CH0 - 1      | FALSE  |  
	| PWM_CH0          | TRUE   |  
	| PWM_CH0 + 1      | TRUE   |  
	| PWM_CH_MAX - 1   | TRUE   |  
	| PWM_CH_MAX       | FALSE  |  
	| PWM_CH_MAX + 1   | FALSE  |  

The left hand column are the input values, and the right hand side are the return values. This truth table shows that the whole range of possible values are used. It starts from the lowest value of `PWM_CH0`, up to the maximum value of `PWM_CH_MAX`. With the C program language being used, it is based with index starting at 0. This is why a `FALSE` is returned when the input is PWM_CH_MAX. A good test is to test one item on either side of the maximum and minimum value. This would catch the "one-off" bugs.

The function description of the `init_pwm_if()` indicates the lower level MCAL is also initialised. In the unit test, a mock function is used to simulate the initialisation of the PWM peripheral. In your mock function, a flag is set to indicate that it has been called.

So far, we have only consider inputs coming into the software module via the caller. There is also inputs coming into the software module on lower level functions. So the initial truth table only consider the inputs and outputs from the function signature's perspective. The influences on returns from called functions need to be considered as well.

As mentioned before, it initialises the microcontroller's peripheral. In this case, it calls `init_pwm()` to initialise the peripheral, `init_pwm()` returns a boolean. It has the function prototype of

    bool_t init_pwm(pwm_channel_t channel);
    
In the truth table, a new row is added to handle this scenario. The truth table now becomes

	| input1         | input2      | output   |
	| channel        | return from | bool_t   |
	|                | init_pwm()  |          |
	|----------------|-------------|----------|
	| PWM_CH0 - 1    | don't care  | FALSE    |
	| PWM_CH0        | TRUE        | TRUE     |
	| PWM_CH0 + 1    | TRUE        | TRUE     |
	| PWM_CH_MAX - 1 | TRUE        | TRUE     |
	| PWM_CH_MAX     | don't care  | FALSE    |
	| PWM_CH_MAX + 1 | don't care  | FALSE    |
	| don't care     | FALSE       | FALSE    |

When the return value from `init_pwm()` is a FALSE, the output from `init_pwm_if()` is always a FALSE. It does not matter what value is passed into `init_pwm_if()`, this type of inputs is indicated as a `don't care`.

The requirement for `init_pwm_if()` is it must only be initialised once before it it is de-initialised. Further initialisation is forbidden. In a real world example, if the PWM is initialised when it is operating, it could cause serious damage if it was controlling machinery. So it is critical that there is a filter in the code for this scenario. Internal of `init_pwm_if()` there is a flag that indicates if initialisation had occurred. If it is TRUE when initialisation is called, `init_pwm_if()` will return a FALSE.

A new row is added to the truth table to handle that reinit flag is set when `init_pwm()` is called. The truth table must be extended to become


	| input1         | input2       | input3    | output |
	| channel        | return from  | flag for  | bool_t |
	|                | init_pwm()   | reinit    |        |
	|----------------|--------------|-----------|--------|
	| PWM_CH0 - 1    | don't care   | FALSE     | FALSE  |
	| PWM_CH0        | TRUE         | FALSE`    | TRUE   |
	| PWM_CH0 + 1    | TRUE         | FALSE`    | TRUE   |
	| PWM_CH_MAX - 1 | TRUE         | FALSE`    | TRUE   |
	| PWM_CH_MAX     | don't care   | FALSE`    | FALSE  |
	| PWM_CH_MAX + 1 | don't care   | FALSE`    | FALSE  |
	| don't care     | dont't care  | TRUE`     | FALSE  |

Instead of a reinit flag, there could also be a counter in the mock function to count the number of times it has been called. This is check that the PWM peripheral is initialised the correct number of times.

## Getting access to internal variables

In the function `init_pwm_if()`, the variable `init_pwm_reinit_flag` is used by PWM stack to block its re-initialisation. The proper implementation is for this variable to be local to the PWM stack, so for the purpose unit test, this makes `init_pwm_reinit_flag` inaccessible. There are options to get around this problem

### Helper set/get function

Two unit test helper functions can be created to set the value of `init_pwm_reinit_flag`

	#if defined(__UNITTEST__)
	void set_init_pwm_reinit_flag(bool_t data)
	{
		init_pwm_reinit_flag = data;
	}
	
	bool_t get_init_pwm_reinit_flag(void)
	{
		return (init_pwm_reinit_flag);
	} 
	#endif /* __UNITTEST__ */

These two functions are only compiled when `__UNITTEST__` are defined.

### A macro for STATIC

The variable `init_pwm_reinit_flag` is declared with the `static` keyword to keep it local to the software module. 

	static bool_t init_pwm_reinit_flag = FALSE;
	
To make it accessible to other software modules such as the unit test code, it needs to be declared without the `static` keyword such as 

	bool_t init_pwm_reinit_flag = FALSE;
	
To satisfy both requirement, `static` can be replaced with a macro. So in `std_types.h`, there is a definition of 

    #if defined(__UNITTEST__)
	
	#define STATIC
	
	#else
	
	#define STATIC	static
	
	#endif /* __UNITTEST__ */ 

The definition is declared as in `pwm_if.c` 

	STATIC bool_t init_pwm_reinit_flag = FALSE;
	
To use it in `pwm_if_test.c`, it is added to the source via

	extern bool_t init_pwm_reinit_flag;

### use logic to test behaviour

For this example, the whole point is to check that the PWM peripheral is not initialised when it is already initialised. So if getting access to the internal variable is not possible, this will have to be tested based on the behaviour. 

The first time `init_pwm_if()` is called with valid parameters, the return should be TRUE, and the called flag for `init_pwm()` is TRUE as well. Upon the second time it is called, the return should be a FALSE and the called flag for `init_pwm()` is also FALSE as it was not called. By using this combination, this requirement can be tested.

## Test Script for `init_pwm_if()`

The test script is the implementation for the test case. For this example, I have implemented test case as shown in the following code. I have chosen to use the macro method of accessing the internal variable. It was an arbitrary decision.

```
bool_t init_pwm_return_value = FALSE;
extern bool_t init_pwm_reinit_flag;

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
		{PWM_CH0-1,    TRUE, FALSE, FALSE},
		{PWM_CH0,      TRUE, FALSE, TRUE},
		{PWM_CH0+1,    TRUE, FALSE, TRUE},
		{PWM_CH_MAX-1, TRUE, FALSE, TRUE},
		{PWM_CH_MAX,   TRUE, FALSE, FALSE},
		{PWM_CH_MAX+1, TRUE, FALSE, FALSE},
		{PWM_CH0+1,    FALSE, FALSE, FALSE},
		{PWM_CH0+1,    FALSE, TRUE, FALSE},
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
		bool_t expected_init_OK;
		init_pwm_return_value = test_data[index].init_pwm_return_value;
		init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
		
		/* execute the function */
		expected_init_OK = init_pwm_if(test_data[index].channel);
		
		/* check that the result is the same as the expected */ 
		assert(expected_init_OK == test_data[index].expected_init_OK);
	}
}
```

So pattern for writing a unit test is pretty straight forward. The concept is to iterate a set of inputs and check that the response from it is the same as the expected. The test code above has this pattern.

So breaking the test script to its parts, at the start of the test script, a struct is created to contain the inputs and the expected outputs. Give the members meaningful names rather than `input1`, `input2` and `input3`. The meaningful names will allow the test code to be read easily. In this example, there are three inputs and one output. Ensure that the member is the same type of the tested parameters. The struct is local to this particular test case. For each test case, create a struct suitable for your test.

```
typedef struct
{
	/* inputs */
	pwm_channel_t channel;
	bool_t init_pwm_return_value;
	bool_t init_pwm_reinit_flag;

	/* output */
	bool_t expected_init_OK;
} test_data_t;
```

The variable `init_pwm_return_value` is used to configure the return value of the mock function. In this case, the mock function `init_pwm()` will return the value `init_pwm_return_value`. So make sure that the type of `init_pwm_return_value` has the same type as the return type of `init_pwm()`.

The variable `init_pwm_reinit_value` is used to configure the mock function if it has been initialised or not. If it is set to TRUE, the mock function will behave as if it has been initialised.

The construction of the test code should be next. It is constructed as a `for` loop where the test data is iterated over by the test code. It iterates for the entire dataset.

``` 
/* set up the loop to iterate over all the each combination
 * of the test data in the truth table.
 */
for ( 	index = 0;
		index < (sizeof(test_data)/sizeof(test_data[0]));
		index++
	)
{
	/* setup the inputs into the function under test */
	bool_t expected_init_OK;
	init_pwm_return_value = test_data[index].init_pwm_return_value;
	init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
	
	/* execute the function */
	expected_init_OK = init_pwm_if(test_data[index].channel);
	
	/* check that the result is the same as the expected */ 
	assert(expected_init_OK == test_data[index].expected_init_OK);
}
```

The code within the `for` loop does the actual testing. At the start of the loop, it configures the mocked objects for the return values, calls the function under tests. It checks that the return values are correct. The checking is via the `assert()` macro, an assertion is generated if the test clause evaluates to a FALSE. 

## Building the code

To execute the test, it is necessary to build the executable. Assuming that you are using `clang`, you can compile the binary using the following command.

	clang -I../../include -I../MCAL pwm_if_test.c pwm_if.c -o unittest
  
The include paths as specified by the `-I` option specifies where the headers are. Feel free to change them for your setup. In this example, it is including the header for the `pwm.h` by including the `MCAL` directory, and it is including `std_types.h` from the `include` directory.

If the unit test passes, no assertion will be generated. An assertion triggered off is a failed test case.

For a complex project, the building of the test executable would be captured into a `makefile`. This makes dealing with dependencies a much easier tasks.

## Test coverage

For this test case, it is important to check the code coverage is adequate. This is a question that needs to be constantly asked, and the initial target is always 100% coverage for code and branch. If neither of these two metrics does not have 100% coverage when you release the software module, there are gaps in the code that are not tested.

The analysis for the test coverage is highly dependent on the implementation of the code. 

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
	
Performing manual test coverage measurements is impossible to get right once the software module implementation gets complicated. It is better if the unit test frame work measures code coverage.

You are in luck if your unit test frame work builds its test binaries using GNU tools, or tools that are compatible with GNU. By specifying your build with the `--coverage`, your software module will be linked with code coverage data, and it will be automatically measured when the software module is executed. This is using the `gcov` features of the GNU toolchain. By the way, even though `clang` is not part of the GNU toolchain, it is compatible with `gcov`.

With the command line build, it would become 

	clang -I../../include -I../MCAL pwm_if_test.c pwm_if.c -o unittest --coverage

After the execution of the binary, the file `pwm_if.gcno` and `pwm_if.gcda` are generated. These two files contain details about the coverage, but they are in binary format. To convert it into human readable format, `gcov` is needed. Use the `-f` and `-b` options to get a quick summary.
	
	localhost:HAL tehnyitchin$ gcov -f -b pwm_if.gcno
	Function 'init_pwm_if'
	Lines executed:100.00% of 10
	Branches executed:100.00% of 4
	Taken at least once:100.00% of 4
	No calls
	
	Function 'deinit_pwm_if'
	Lines executed:0.00% of 5
	No branches
	No calls
	
	Function 'set_pwm_if'
	Lines executed:0.00% of 12
	Branches executed:0.00% of 8
	Taken at least once:0.00% of 8
	No calls
	
	Function 'get_pwm_if'
	Lines executed:0.00% of 7
	Branches executed:0.00% of 2
	Taken at least once:0.00% of 2
	No calls
	
	File 'pwm_if.c'
	Lines executed:29.41% of 34
	Branches executed:28.57% of 14
	Taken at least once:28.57% of 14
	No calls
	pwm_if.c:creating 'pwm_if.c.gcov'

From the output, it shows that the `init_pwm_if()` is fully covered from a code coverage and branch coverage perspective, both metrics are measured at 100%. Code coverage is reported as "Line executed". Branch coverage is reported as "Branches executed". The other functions in `pwm_if.c` were not tested, so their metrics are at 0%.

The file `pwm_if.c.gcov` has the details per line of code. Showing the details for `init_pwm_if()`, 

	        -:   21:/* initialise the PWM software stack
	        -:   22: */
	function init_pwm_if called 8 returned 100% blocks executed 100%
	        -:   23:bool_t init_pwm_if(pwm_if_channel_t channel)
	        -:   24:{
	        8:   25:	bool_t retVal = FALSE;
	        -:   26:
	        8:   27:	if (init_pwm_reinit_flag == FALSE)
	        -:   28:	{
	        7:   29:		if (channel < PWM_CH_MAX)
	branch  0 taken 57%
	branch  1 taken 43%
	        -:   30:		{
	        4:   31:			retVal = init_pwm((pwm_channel_t)(channel));
	        -:   32:
	        4:   33:			if (retVal == TRUE)
	branch  0 taken 75%
	branch  1 taken 25%
	        -:   34:			{
	        3:   35:				init_pwm_reinit_flag  = TRUE;
	        3:   36:			}
	        4:   37:		}
	        7:   38:	}
	        -:   39:
	        8:   40:	return (retVal);
	        -:   41:}
	        
	        
The number of times the line of code has executed is shown next to code's line number in the source code. One of the interesting metric is the branch metric. It must add up to 100% for all the different branches, and none of the branches must be 0% taken. If one of the branch is at 0%, the branch coverage is not 100% and more inputs must be created to achieve 100% branch coverage.

With a more complex function, this type of code coverage measurement tools will help reduce effort required. It is much better and less error prone when compared measuring it manually. 

## Getting better coverage

As the summary indicated, other than `init_pwm()`, the rest of the functions in the PWM stack are not even executed. Lets continue designing the test cases for the rest of the functions with `set_pwm_if()`. Looking at the requirements, the following three requirements are allocated to `set_pwm_if()`.

    req_PWM1: The PWM driver shall only allow a frequency from 50Hz to 500Hz.
    req_PWM2: The PWM driver shall only allow a duty cycle from 10% to 90%.
    req_PWM3: When PWM driver receives request out of the allowable frequencies and duty cycles, it shall reject the request and the output signal does not change.
    
and the prototype for `set_pwm_if()` is 

	bool_t set_pwm_if(pwm_if_channel_t channel, pwm_if_signal_t signal);
	
The data type of `pwm_if_signal_t` contains two members, `frequency` and `duty_cycle`. These two struct members affects the signal, so it is logical for them to be together. What this really means is that this is actually two inputs rather than the one input.

	typedef struct
	{
		uint16_t frequency;
		uint8_t duty_cycle;
	} pwm_if_signal_t;


Other than the `bool_t` being returned which indicates if the signal was set correctly, the other outputs are the calling of `set_pwm()` to the driver layer. In the mocked `set_pwm()`, a flag will need to be set so that its calling is captured, and this also forms part of the output in the truth table.

	| input1    | input2    |input3   |input4      |output flag | output return |
	| frequency | duty_cycle|channel  |return flag |for calling | from function |
	|           |           |         |set_pwm()   |set_pwn()   | bool_t        |
	|-----------|-----------|---------|------------|------------|---------------|
	| 50        | 10        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 49        | 10        | PWM_CH0 | don't care | don't care | FALSE         |
	| 51        | 10        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 499       | 10        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 500       | 10        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 501       | 10        | PWM_CH0 | don't care | don't care | FALSE         |
	| 50        | 9         | PWM_CH0 | don't care | don't care | FALSE         |
	| 50        | 11        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 50        | 89        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 50        | 90        | PWM_CH0 | TRUE       | TRUE       | TRUE          |
	| 50        | 91        | PWM_CH0 | don't care | don't care | FALSE         |
	| 50        | 90        | PWM_CH0 | FALSE      | TRUE       | FALSE         |

The truth table is constructed based upon the requirements of `req_PWM1`, `req_PWM2` and `req_PWM3`. The values for frequency and duty cycle are chosen for 1 on either side of the limits. The limits are classified to be valid as per the requirements.

With these test cases, the test code is shown. The structure is the same as `init_pwm_if_test_case()`. It follows the following pattern.

1. A `struct` defined to contain all data in the test case.
2. A loop is created to iterate through each test case. 
3. For a test case, the inputs are configured with its data. 
4. The function under test is called.
5. The return is checked via an assert against the return from the function under test. The activity flag from the calling of `set_pwm()` is also checked via an assert.

In the test code below, the `set_pwm()` is actually made up of two functions, one for setting the frequency and another for setting the duty cycle. For each of these functions, a flag is used to indicated that the function has been called and a flag to indicate what value to returned from its calling.

```
/** test case for set_pwm_if() */
void set_pwm_if_test_case(void)
{
	uint8_t index;

	typedef struct
	{
		/* inputs */
		pwm_if_signal_t signal;		/* for freq and duty cycle */
		pwm_channel_t channel;
		bool_t return_for_set_frequency;
		bool_t return_for_set_duty_cycle;

		/* output */
		bool_t flag_calling_set_frequency;
		bool_t flag_calling_set_duty_cycle;
		bool_t return_from_function_call;
	} test_data_t;

	/* set up the test data. It is based the truth table.
	 */
	test_data_t test_data[] =
	{
		{{50,  10}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{49,  10}, PWM_CH0, TRUE, TRUE,  FALSE, FALSE, FALSE},
		{{51,  10}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{499, 10}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{500, 10}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{501, 10}, PWM_CH0, TRUE, TRUE,  FALSE, FALSE, FALSE},
		{{50,  9 }, PWM_CH0, TRUE, TRUE,  FALSE, FALSE, FALSE},
		{{50,  11}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{50,  89}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{50,  90}, PWM_CH0, TRUE, TRUE,  TRUE, TRUE,   TRUE },
		{{50,  91}, PWM_CH0, TRUE, TRUE,  FALSE, FALSE, FALSE},
		{{50,  90}, PWM_CH0, FALSE,FALSE, TRUE, FALSE,  FALSE},
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

		return_for_set_frequency = test_data[index].return_for_set_frequency;
		return_for_set_duty_cycle = test_data[index].return_for_set_duty_cycle;
		flag_calling_set_frequency = FALSE;
		flag_calling_set_duty_cycle = FALSE;

		/* execute the function */
		return_from_function_call = set_pwm_if(
				test_data[index].channel,
				test_data[index].signal);

		/* check that the result is the same as the expected */
		assert(return_from_function_call == test_data[index].return_from_function_call);
		assert(flag_calling_set_frequency == test_data[index].flag_calling_set_frequency);
		assert(flag_calling_set_duty_cycle == test_data[index].flag_calling_set_duty_cycle);
	}
}
```

This test code satisfies the three requirements of `req_PWM1`, `req_PWM2` and `req_PWM3`. The rest of the requirements would be tested within the software integration testing as it involves several of the functions.

When I run the coverage tool to see the code coverage, it shows that line executed coverage has increased to 65%. 

	localhost:HAL tehnyitchin$ gcov -f -b pwm_if.gcno 
	Function 'init_pwm_if'
	Lines executed:100.00% of 11
	Branches executed:100.00% of 4
	Taken at least once:100.00% of 4
	No calls
	
	Function 'deinit_pwm_if'
	Lines executed:0.00% of 6
	No branches
	No calls
	
	Function 'set_pwm_if'
	Lines executed:100.00% of 15
	Branches executed:100.00% of 12
	Taken at least once:91.67% of 12
	No calls
	
	Function 'get_pwm_if'
	Lines executed:0.00% of 8
	Branches executed:0.00% of 2
	Taken at least once:0.00% of 2
	No calls
	
	File 'pwm_if.c'
	Lines executed:65.00% of 40
	Branches executed:88.89% of 18
	Taken at least once:83.33% of 18
	No calls
	pwm_if.c:creating 'pwm_if.c.gcov'


The test case for `get_pwm_if()` would also go through a similar process. All the different inputs and its corresponding expected outputs have been analysed and determined, it becomes quite easy to develop the test code.

I have left the test code as an exercise for the reader so I won't show what my implementation might have been.

 