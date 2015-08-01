# Types of Test

The types of tests are classified as to the type of coverage it provides. The most obvious type of coverage is statement coverage. These ranges from state coverage to path coverage. The type of coverage is denote via C value. 

* Instruction coverage
* Branch coverage
* Conditon coverage
* Path coverage

Each type of coverage aims to cover the code's functionality as well as the code's execution execution path.

## Instruction coverage
Instruction coverage or statement coverage is a measure of how much of the code was tested. If the measure is less than 100%, not all the C code in your function was executed during the testing.

The measure for instruction coverage is whether the C code is executed during the test. If the code was executed at least once, it is marked as executed.

The coverage does not matter how many times the C code have been executed. It also does not care if the it the correct behaviour.



	uint8 validateDigitalInput(uint8 in1, uint8 in2)
	{
		uint8 retVal = 0;
		
		if (in1 != in2)
		{
			retVal = in2;
		}
		else
		{
			retVal = in1;
		}
		
		return (retVal);
	}


 