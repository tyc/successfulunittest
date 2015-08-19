# Basic Concepts

Before we get started on the core unit tests, I will need to go through some of the concepts of unit test. If you are familiar with the basic concepts of unit test such as the different kinds of test coverage, or how important traceability is, you can skip this chapter onto the next chapter.

Basic concepts does not just cover basic concepts in unit test, but basic concept around software engineering. Unit test is one of the key components and basic concepts around software engineering need to be described to give a complete picture. 

##Correct behaviour

Before you start writing a single test script, you must have a clear understanding of what is correct behaviour. The correct behaviour could be dependent upon what the project aim. 

The usual manner is for the requirements of your software module defined. The definitions will be used as what is defined as correct behaviour. If it meets that requirements, then the software module is classified as hvaing correct behaviour.

There are several ways of checking behaviour is correct. Some of the main points to look for are:

* lower level functions must be called in a specified order
* the number of call to lower level function is counted. If it is called too many times, it is a sign of incorrect implementation.
* The returned values matches the expected. Returned values can be obtained via the returned type, or the passed in pointer.
* The correct lower level code is called with the correct parameters.

Correct behaviour about timing and interaction other modules are not considered in unit tests. Dynamic behaviour and interactive behaviour are tested for integration tests.

##Requirements

The requirements of the software module is a set of statements that specifies what the software should do. This set of statements are cohesively created, it can specify all the important aspects of the software module. These could include all the items mentioned earlier such as the specific calling order or the value that the types are returned.

There several level of requirements used when describing the software. For the software modules, the requirements are specified at a low level perspective. 

For example, if the task is to write a driver for the managing a PWM output, the following items would define the behaviour

    The PWM driver shall only allow a frequency from 50Hz to 500Hz.
    The PWM driver shall only allow a duty cycle from 10% to 90%.
    When PWM driver receives request out of the allowable frequencies and duty cycles, it shall reject the request and the output signal does not change.
    The amplitude of the PWM signal is at logic level of the microcontroller.
    The tolerance of the PWM signal is 5% of the requested.
    The PWM driver shall not consume more than 1KByte of code space.
    The PWM driver shall not consumre more than 5% of its allocate time slot.  

Those five requirements would define the behaviour of the PWM driver. Any other aspects of the driver that are not specified are not tested. For example, the requirements could specified the  PWM signal behaviour when the microcontroller enters sleep mode. During analysis of the reqiurements, this gap would hopefully be identified and closed. 

Notice that the requirements also specified non functional requirements, but this requirenments are not tested using unit tests.

Once all the requirements are specified, it is reviewed with the test engineer. The test engineer should review it and ask if the requiremetns can be tested. If it can't be tested, usually the requirements is vague and is too general. The requirements will need to be re-written and its behaviour better defined. In most cases, the test engineer create the unit test is also the development engineer. So this iteration loop is quite quick.

The code you create implements functionality that behaves as per the requirements. The unit tests is code that checks if the implementation creates functionality that meets the requirements. This is why getting the requirements right is important as it has enormous impact on the implementation and the unit test if the requirements are not complete or not right.

##Version Control

I am not going to go too deep with version control. However, it is such an important subject that I want to read a chapter just to remind you of it.

The unit test cases you create needs to be version controlled. It is an integral part of creating your software. It is as important as the C code you write, so it needs to be managed using a version control management system. There are plenty of system available, just choose one that you feel comfortable, just make sure you use one.  

##Coverage

There are many different types of coverage. A coverage is a measure of whether the tests is covering the correctness of the code's behaviour. Getting complete coverage of code is very difficult to achieve. A simple approach is to check that every line of the code that can be executed is checked that it has been executed during the test, and that is one of the coverages that can be used. Among the other coverages that can be considered are branch coverage, condition coverage and path coverage. 

In this chapter, I will go through each type of coverage. I will show how achieving high coverage is essential for a successful test of your code.

### Instruction Coverage

Instruction coverage is a measure of how much of the code was tested. This type of coverage is also known as statement coverage, code coverage or C0 coverage. 

The measure for instruction coverage is whether the C code is executed during the test. If the code was executed at least once, it is marked as executed and counts towards the measure. If the measure is less than 100%, not all of the code in your function was executed during the testing. Not achieving 100% coverage does not always mean that you have uncovered some dead code. It could also indicate that the stimulus that you have used in the testing is not suitable.

The coverage does not matter how many times the C code have been executed. It also does not care if the code has correct behaving.

The coverage is calculated by the number lines of code executed divided by the number of available lines of code for execution. To get a better understanding when assess this metric, it is a good idea to also take into the account of the number of lines of code as well as the percentage. 

Sometimes, your coverage tool measures this metric at assembly language level. It takes a measure of all the addresses which are occupied with code, and logs all the addresses which have been executed. This is a more accurate measure but it is highly dependent on the architecture and compiler and platform used. However, to keep you code platform independent, your test results should also be platform independent as well.

###Branch Coverage

Branch coverage is a measure of whether branches of code was executed or not. It is relates to decision making and the branches it takes when those decision are made. The `if` clauses are main focus here.

This type of coverage is also known as C1 coverage.

This metric is calculated by dividing the number of branch executed by the number of possible branches available. These are dependent on the structure of the `if` statements, `switch` statements and `goto` statements in your C code. Combinations of these statements will increase the complexity of the branches.

During the execution of the test case, if a branch executed, it is marked as executed. The number of times that branch is executed does not affect this count.

The branch coverage does not care about if the correct decision was made to execute a particular branch. It cares if the branch was executed or not. The correctness of the branch execution is determined in a different manner.

The purpose of branch coverage is used as a metric to check if your test inputs are correctly selected. If the test inputs only yields a branch coverage of 50% on an if branch, that means that your selection of test inputs is not wide enough.


###Modified Condition Decision Coverage

