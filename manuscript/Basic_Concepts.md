# Basic Concepts

Before we get started on the core unit tests, I will need to go through some of the concepts of unit test. If you are familiar with the basic concepts of unit test, you can skip this chapter onto the next chapter. In this chapter,  Some of the topics I will be discussing topics are test coverage, traceability and mocks.

This chapter does not just cover basic concepts in unit test. It also covers basic concept around software engineering. Unit test is one of the key components. The basic concepts around software engineering need to be described to give a complete picture.

I am only covering the basic concepts and I encourage you to research the topics further. There are many resources that discusses these topics in detail.

##Software module

A software module is a group of functions that are grouped together based on their complementary functions. There are usually a .c file, and a corresponding .h file. Sometimes, there are several header files that would contain information for configuration and  private functions. 

To help unit test, the software module should be setup so that it can be easily tested. A practical way is to abstract all the configurable parameters to a header file. The abstraction will separate the logical functionality from its configuration. When the unit test is completed on the software module and proven to be correct, it is released. Any change in its behaviour that can be affected via its configuration. The unit test is skipped as the software module is proven from a logical perspective. 

Obviously, for the complete behaviour of the software module to be known, all combinations which can be subjected to the software module must be used.

##Mocks

Depending upon which layer the software module under test is residing, there are usually lower layer functions which are called. Lower layer functions are usually replaced by mock functions that pretends to be the real functions. It is useful when writing software modules targeting specific platform with specific software drivers.

This is much better explained via an example. Say the task is to write a software manager. It is for handling the incoming stream of serial data over the CAN communication bus. The manager interfaces to the CAN transceiver driver and CAN controller interface. The focus is to write the manager is a generic manner. Once written, it can be used on other platforms. The CAN controller interface and the CAN transceiver driver are tied to the platform and the hardware used. When the test script for the manager is being written, two mocks are created to pretend to be the CAN transceiver driver and the CAN controller interface.

One of the major benefits of using mock is the ability to inject erroneous behaviour. Extending the above example of the software manager, the CAN controller interface could pretend to be faulty and not able  to be initialised correctly. This test case would check that the software manager is able to handle error conditions.

Mocks also abstract all the test conditions away from the dependencies of hardware availability. This will allow the software modules to developed on an relatively independent time line from the hardware.

##Correct behaviour of a software module

Before you start writing a single test script, you must have a clear understanding of what is correct behaviour. 

The requirements of your software module defines correct behaviour. If it satisfies those requirements, then the software module is classified as having correct behaviour. If the requirements missing, correct behaviour is undefined and is usually based upon the engineering judgement of the project.

There are several ways of checking if the behaviour is correct. Some of the main characteristics to look for are:

* Lower level functions must be called in a specified order
* The number of call to lower level function is counted. If it is called too many times, it is a sign of incorrect implementation.
* The internal behaviour is verified by checking that the variables are updated correctly.
* The returned values matches the expected. Returned values can be obtained via the returned type, or the passed in pointer.
* The correct lower level code is called with the correct parameters.

Correct behaviour about timing and interaction with other modules are not considered in unit tests. Dynamic behaviour and interactive behaviour are tested with integration tests. This include memory consumption tests.

Correct behaviour is also checked by other tools that focuses on other aspects of the software module. For example, a tool is used to check that the code matches the coding guidelines of the project, or that the software module is structured according to the architecture requirements. This type of discussion is outside the scope of this book.

The majority of the unit test framework uses the `assert()` macro to help with notifying software tester that a non-conformity was detected. The `assert()` macro takes the test result and the expected result, a non-match flags the error. A more complex `assert()` macro will also display a message to give some context to the software tester or the software debugger why an error was detected. 

##Requirements

The requirements of the software module is a set of statements that specifies what the software should do. This set of statements are cohesively created. This means that the requirements are clear and able to be tested. It also means that each of the requirements works together and does not conflict against each other. 

The requirements can specify all the important aspects of the software module. These could include all the items mentioned earlier such as the specific calling order or the value that the types are returned.

There several level of requirements used when describing the software. For the software modules, the requirements are specified at a low level perspective.

For example, if the task is to write a driver for the managing a PWM output, the following items would define the behaviour

    req_PWM1: The PWM driver shall only allow a frequency from 50Hz to 500Hz.
    req_PWM2: The PWM driver shall only allow a duty cycle from 10% to 90%.
    req_PWM3: When PWM driver receives request out of the allowable frequencies and duty cycles, it shall reject the request and the output signal does not change.
    req_PWM5:The amplitude of the PWM signal is at logic level of the microcontroller.
    req_PWM5:The tolerance of the PWM signal is 5% of the requested.
    req_PWM6:The PWM driver shall not consume more than 1KByte of code space.
    req_PWM7:The PWM driver shall not consume more than 5% of its allocate time slot.  

Those seven requirements would define the behaviour of the PWM driver. Any other aspects of the driver that are not specified are not tested. For example, the requirements could specified the  PWM signal behaviour when the microcontroller enters sleep mode. During analysis of the requirements, this gap would hopefully be identified and closed.

Notice that the requirements also specified non functional requirements, but these requirements are not tested using unit tests.

Once all the requirements are specified, it is reviewed with the test engineer. The test engineer should review it and ask himself if the requirements can be tested. If it can't be tested, usually the requirements are too vague and are too general. The requirements will need to be re-written and its behaviour better defined. In most cases, the test engineer create the unit test is also the development engineer. So this iteration loop is quite quick.

The code you create implements functionality that behaves as per the requirements. The unit tests are code that checks if the implementation creates functionality that meets the requirements. This is why getting the requirements right is important. It has enormous impact on the implementation and the unit test if the requirements are not complete or not right.

One important character of requirements is that each requirements is assigned a unique ID. In the 7 requirements, I have used nomenclature of software module name, post fixed by a `req`. This makes it easy for each requirements to be identified. In more complicated nomenclature, version and status of the requirements are also attached to its ID. I think it over complicates the naming structure and does not add much value.

##Version Control

I am not going to go too deep with version control. Yet, it is such an important subject that I want to write a chapter just to remind you of it.

The unit test cases you create needs to be version controlled. It is an integral part of creating your software. It is as important as the C code you write, so it needs to be managed using a version control management system. There are plenty of system available, just choose one that you feel comfortable, just make sure you use one. 

##Coverage

There are many different types of coverage. A coverage is a measure of whether the tests is covering the correctness of the code's behaviour. Getting complete coverage of code is difficult to achieve. A simple approach is to check that every line of the code that can be executed is checked that it has been executed during the test. This is one of the coverages that can be used. Among the other coverages that can be considered are branch coverage, condition coverage and path coverage.

In this chapter, I will go through each type of coverage. I will show how achieving high coverage is essential for a successful test of your code.

### Instruction Coverage

Instruction coverage is a measure of how much of the code was tested. This type of coverage is also known as statement coverage, code coverage or C0 coverage.

The measure for instruction coverage is whether the C code is executed during the test. If the code was executed at least once, it is marked as executed and counts towards the measurement. If the measurement is less than 100%, not all the code in your function was executed during the testing. Not achieving 100% coverage does not always mean that you have uncovered some dead code. It could also show that the stimulus that you have used in the testing is not suitable.

The coverage does not matter how many times the C code have been executed. It also does not care if the code has correct behaviour.

The coverage is calculated by the number lines of code executed divided by the number of available lines of code for execution. To get a better understanding when assessing this metric, it is a good idea to also take into the account of the number of lines of code as well as the percentage. 

Sometimes, your coverage tool measures this metric at assembly language level. It takes a measure of all the addresses which are occupied with code. When executing, it logs all the addresses which have been executed. This is a more accurate measure but it is dependent on the architecture and compiler and platform used. Yet, to keep you code platform independent, your test results should also be platform independent as well.

### Branch Coverage

Branch coverage is a measure of whether branches of code was executed or not. It is relates to decision making and the branches it takes when those decision are made. The `if` clauses are main focus here. The `switch..case` is also in focus here as well. 

This type of coverage is also known as C1 coverage.

This metric is calculated by dividing the number of branch executed by the number of possible branches available. These are dependent on the structure of the `if` statements, `switch` statements and `goto` statements in your C code. Combinations of these statements will increase the complexity of the branches.

During the execution of the test case, if a branch executed, it is marked as executed. The number of times that branch is executed does not affect this count.

The branch coverage does not care about if the correct decision was made to execute a particular branch. It cares if the branch was executed or not. The correctness of the branch execution is determined in a different manner.

One of the purposes for branch coverage is to check if your test inputs are correctly selected. If the test inputs only yields a branch coverage of 50% on an `if` branch, that means that your selection of test inputs is not wide enough. The correctness of the branching is not being evaluate here.  That is checked in a different manner. 

### Functional coverage

Functional coverage is a measure of the unit test covers the functional requirements of the software module. If the coverage is less than 100%, it shows that the unit test does not cover all the functions to be achieve by the software module. Only functional requirements are counted for functional coverage. Non-functional requirements are outside the scope of this measurement. They should be covered through other mechanism such as integration testing or code inspection.

The functional coverage is measured by dividing the number of requirements that are covered by the unit tests by the total number of requirements. From this equation, it is clear that a coverage of less than 100% shows that not all the functionality are tested.

Functional coverage occurs during the planning stage of the your software module. When the software module is being planned, the unit test cases should be planned at the same time.

For requirements that are not covered by the unit tests, it may be only possible to test these requirements in the integration tests because it is testing dynamic behaviour.


