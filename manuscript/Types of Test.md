# Types of Coverage

The types of tests are classified about the type of coverage it provides. The most obvious type of coverage is statement coverage. These ranges from state coverage to path coverage. The type of coverage is denote via C value.

* Instruction coverage
* Branch coverage
* Conditon coverage
* Path coverage

Each type of coverage aims to cover the code's functionality as well as the code's execution execution path.

## Instruction coverage

Instruction coverage or statement coverage is a measure of how much of the code was tested. If the measure is less than 100%, not all the C code in your function was executed during the testing.

The measure for instruction coverage is whether the C code is executed during the test. If the code was executed at least once, it is marked as executed.

The coverage does not matter how many times the C code have been executed. It also does not care if the code has correct behaving.

## Instruction coverage - example

We have just written a function to validate that a digital input on a input pin of a microcontroller is the same as that read on another input pin. If the pin values are different, it will take the value of the 2nd digital input, otherwise it will return the value from 1st digital input.

The function is below.

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


The test case for testing this function is

### << Add the code for the test case here >>.

The test case only stimulates the `validateDigitalInput` where the values for `in1` and `in2` are the same. This will cause the the if clause to be evaluated as true. The functionality for false is not executed at all.

So for this function and it corresponding test case, not 100% instruction coverage was achieved. To get 100% instruction coverage, the values for `in1` and `in2` must be different so the functionality for false is also executed.


## Branch coverage

Branch coverage is a measure of whether branches of code was executed or not. It is relates to decision making and the branches it takes when those decision are made. The if clauses are main focus here.

At the start of the analysis, the number of possible branch is counted. These are dependent on the structure of the `if` statements, `switch` statements and `goto` statements in your C code. Combinations of these statements will increase the complexity of the branches in the code.

During the execution of the test case, if a branch executed, it is marked as executed. The number of times that branch is executed does not affect this count.

The branch coverage does not care about if the behaviour is correct or not, just that the branch was executed.

The purpose of branch coverage is to check the logic in the code is making the correct decision for that branch to be executed. The path to making the decision may be complex, but the coverage of the complexity will be checked in condition coverage.


## Branch coverage - example

We will reuse the example from instruction coverage. Here it is again.

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
 
This function has a branch count of two.

### << Add the code for the test case here >>.

The test case `validateDigitalInput`passes in same values for `in1` and `in2`. This will only execute of one of the two branches. Branch coverage is only 50%. For the branch coverage to be at 100%, the test case must pass in different values of `in1` and `in2`. The execution of the branch with the different values will cause the branch coverage to 100%.   