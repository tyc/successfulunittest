
During the construction of code, have you ever wonder if you code works without testing with the other code, or how close your code is what you are designing against. I have always wonder about the same thing. In my formative years after graduating from University in 1992, the notion doing unit testing was quite alien. It is even more so if it is applied to the world of practicing software engineering in the embedded space.

With the explosion of the internet and the all the technologies that go with enabling the thousands of apps, applying unit test to your code is occuring almost by default. However, in the embedded space, the practice is not so. It is almost as if it is not needed.

I notice a change about 10 years ago when consumer electronics started to get smart and applications written in Java were started to be deployed in them. 

In this book, it will go into some of the history of the unit testing. A good understanding of where unit test was derived from will show where it is heading.

It will show how to write code that is testable. To that end, it will suggest a project structure that will allow the project to unit test the code every time the build.

### Show me the money
  
||this is a test||thest||


## Purpose of unit test

## why is unit test on embedded so hard?

Testing functions of code that are targetted for an embedded system is hard, sometimes it is impossible to fully test it correctly. By definition, an embedded system is close, and getting data into it and out of it is not possible unless it has a human interface attached to it. The majority of embedded system are electronics modules that are found in your rice cookers, your washing machines, your cars and motorcycles and your aeroplanes. They are used in a wire variety of tasks such as controlling the cooking temperature for your rice, or monitoring the brake system in your car. These modules lack a human interface where data can be injected and data extracted to check if it is working correctly.

The traditional way of testing embedded software is to build the software as a whole, programmed it into the non-volatile memory, put it into the target board and power it up. Stimulus would be injected and its behaviour noted. If the behaviour is the same as the expected, then it was give a pass, otherwise a fail is detected. This testing is quite unforgiving as it is error prone. The test execution is not consistent over the time as it is typically tested manaully. The capture of the results could also be not so accurate.

By using a modular design in the architecture, it is possible to test the software in a much more control manner.

## mocking units

## designing your code for unit test

The code you are writing must be design for unit test in mind, otherwise it may be possible for your function to be untestable. If it is not able to be tested correctly, the correctness of its implementation will be in doubt.

The key to writing code that can be tested is to keep the functions as simple as possible. The higher level of complexity, the harder it will be to test it. The complexity will be driven from the number of combinations and permutations the function can behave as its inputs changes A good rule of thumb for design is for the function to only do one function and to do it very well.

The complexity of the functions is a good measure of how well it can be tested. Using the complexity model of Cyclomatic Complexity, a value of 10 is typically recognised as a safe limit. There are studies to show that a value of 10 has received substantial corrobating evidence. If it is higher than 10, it is recommended that the modules be broken down into smaller modules which will handle simpler functions.

### non-blocking function

A function that blocks is very difficult to test. Unless there is a very good logical reason, blocking operation in your code design is a very bad idea.

If you are unable to avoid blocking operation, the best way to test blocking operation is have unit test code within your software module to unblock the operation.

A simple implementation of a round robin scheduler is a while(1) loop. Within the `while(1)` loop, it checks for the expiration of a timer tick. Once the expiration is confirmed, the function is called.

 
	uint8 scheduler(void)
	{
	#ifndef(UNITTEST)	
		while(1)
	#endif		
		{
			if (isTimeTick(1ms) != FALSE)
			{
				function[function_index]();
				function_index++;
				function_index %= FUNCTION_MAX;			
			}
		}
	}


The unit test harness calls `scheduler()` a number of times to check on its behaviour. The `while(1)` is abstracted by conditionally removed by the compiled time macro of `UNITTEST`.

An alternative is to use the `break` statement just before the closing bracket to the `while(1)` loop. The alternative has less risk than the obvious solution as it does not remove any code from being tested. It adds the `break` statement. The obvious solution removes the `while(1)` for the unit test.

	uint8 scheduler(void)
	{
		while(1)
		{
			if (isTimeTick(1ms) != FALSE)
			{
				function[function_index]();
				function_index++;
				function_index %= FUNCTION_MAX;			
			}
	#ifndef(UNITTEST)	
			break;
	#endif		
		}
	}




### reduce the number of parameters for the interface

There are several ways which a function can take its values and return its values. For inputs, the parameters could be passed in as function arguments, or as a global variables. For return values, it could passed back via global variables, returned via the passed in pointer, or via the return type.

	unsigned int voltage_reading;

	uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
	{
		uint8 retVal = ERET_OK;

		if ((raw_count != NULL) &&
			(channel < MAX_ADC_CHANNELS))
		{
			*raw_count = adcRaw[channel];
			voltage_reading = *raw_count * VOLTAGE_SCALE_FACTOR;
		}
		else
		{
			retVal = ERET_BAD_ARG;
		}

		return retVal;
	}
	
The above example shows there are four variables that must be tested for. Each of these variables has their own influence on how the function behaves. This example is relatively simple and easy to understand. 

To contrast the complexity, the following function is a lot harder to test. The raw ADC reading was coverted to a voltage reading after it has been filtered. It is also ranged checked. 

	unsigned int voltage_reading;

	uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
	{
		uint8 retVal = ERET_OK;

		if ((raw_count != NULL) &&
			(channel < MAX_ADC_CHANNELS))
		{
			*raw_count = adcRaw[channel];
			voltage_reading = filterAdc(*raw_count) * VOLTAGE_SCALE_FACTOR;

			if (voltage_reading > VOLTAGE_READ_MAX)
			{
				voltager_reading = VOLTAGE_READ_MAX;
			}
			else if (voltage_reading < VOLTAGE_READ_MIN)
			{
				voltage_reading = VOLTAGE_READ_MIN;
			}
		}
		else
		{
			retVal = ERET_BAD_ARG;
		}

		return retVal;
	}
	
The complexity can be reduced by encapsulating the filtering and range checking into the one function `filterAdc_to_Voltage`. The following example illustrate the change.

	unsigned int voltage_reading;

	uint16 filterAdc_to_Voltage(uint16 raw)
	{
		uint16 voltage = filterAdc(*raw_count) * VOLTAGE_SCALE_FACTOR;

		if (voltage > VOLTAGE_READ_MAX)
		{
			voltager_reading = VOLTAGE_READ_MAX;
		}
		else if (voltage < VOLTAGE_READ_MIN)
		{
			voltage = VOLTAGE_READ_MIN;
		}

		return (voltage);
	}

	uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
	{
		uint8 retVal = ERET_OK;

		if ((raw_count != NULL) &&
			(channel < MAX_ADC_CHANNELS))
		{
			*raw_count = adcRaw[channel];
			voltage_reading = filterAdc_to_Voltage(*raw_count);
		}
		else
		{
			retVal = ERET_BAD_ARG;
		}

		return retVal;
	}
	
Even though there is an extra function `filterAdc_to_Voltage` to test, both functions are simple enough that a high level of coverage is ensured.