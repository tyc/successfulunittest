# Basic Concepts

Before we get started on the core unit tests, I will need to go through some of the concepts of unit test. If you are familiar with the basic concepts of unit test such as the different kinds of test coverage, or how important traceability is, you can skip this chapter onto the next chapter.

## Correct behaviour

Before you start writing a single test script, you must have a clear understanding of what is correct behaviour. The correct behaviour could be dependent upon what the project aim. 

The usual manner is for the requirements of your software module defined. The definitions will be used as what is defined as correct behaviour. If it meets that requirements, then the software module is classified as a pass. Its tests script and test telephone is returned.

There are several ways of checking behaviour is correct. Some of the main points to look for are:

* lower level functions must be called in a specified order
* the number of call to lower level function is counted. If it is called too many times, it is a sign of incorrect implementation.
* The returned values matches the expected. Returned values can be obtained via the returned type, or the passed in pointer.
* The correct lower level code is called with the correct parameters.

Correct behaviour about timing and interaction other modules are not considered in unit tests. Dynamic behaviour and interactive behaviour are tested for integration tests.

## Coverage

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

