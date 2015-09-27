#Tools

There are several ways to look at the tools used in unit tests. All the tools are geared towards getting your software code to behave as per the requirements.  They can range from self made tools to enterprise level tools. 

One of aspects of selecting a tool is the fact that it will have significant effect if it is to be standardise across many projects over a long period of time. This type of technical debt is hard to combat if it is not carefully considered on its long term effect. The implementation of the test case is locked into the unit test framework. Migration is necessary if a different framework is used in the future. This type of technical debt is present in any tool that you choose. It includes the tools used for developing your code. So choose carefully.

The unit test tool forms a framework which the test case can be created and managed. The test engineer would be able to kick the test run, be able to specify which tests to run and have the results when completed.
 
## Automatic Stub generator

An automatic stub generator analyses the software module under test. From the analysis, it will get a list of lower level functions that it calls. The signature of the lower level function is also obtained.

The stubs are usually obtained from the header file that is included by software module under test.

Once the stubs are generated, it is up to the test engineer to fill in the details of the stubs. The behaviour of the stub is defined.

The thing to remember is that depending upon the test case, the stub may have a different behaviour. In one test case, the test engineer may want the stub to return a value that is out of the expected range for negative testing. In another test case, the test engineer may want to have the stub return the correct values for positive testing.

## Debugging the software module

During the early stages of the developing of the software module, bugs and logical problems will surface. Running the test cases over it will case it to detect a failure.

One method of debugging would be to take the inputs used in the test case and analyse it for the root cause of the bug. If the software module is complex, this debugging strategy is quite intensive. The intensity can lead to many false conclusion. Unless you have plenty of time, the recommendation is to avoid this debugging strategy. However, use it as a last resort if there are no more options.

There are some unit test frameworks around where it mixes languages. In the embedded domain, the software code is usually written in C, C++ or assembly, but the test cases are written in another language such as Python or Java. When a different language is used, it is typical for the C code to be compiled into a shared library. In the Windows world, the shared library would be compiled into a `.dll`. In the Linux world, the shared library would be a compiled into a `.so`.  When the test case is written in a different language and it is linked to the shared library, the resultant binary is difficult to debug. The debugging tool will have to cross the language boundary to support all the languages in the binary.

Having said that, I have found that a test case written in Python, linked with a shared library. This combination can be debugged within Windows if the shared library is created using the GNU tools. They will be other combinations as well.

To have the ability to debug will allow the software module to be tested faster. It will allow you to get to the root cause of the bug a lot faster.

## Tool Langauge

The language of the unit test framework is extremely important. It is just as important as the language of the software module. In fact, the test scripts that are written is a piece of code that needs to be managed. So ideally, the principles of software engineering also applies to the test scripts as well.

To reduce the complexity in your test scripts, it would be ideal if the test scripts are written in the same language that software module. If you are writing your software module in C, your test scripts should also be written in C. If you are using rust, then rust should be used for the test scripts.

The advantage of using the same language for both the software module and test script, it is compiled for testing on the target. The big assumption is that the language is already supported by your target.

## Metric collecting and logging

Collecting metrics about the unit test is an important aspect of the test strategy. The metric is used to learn whether the software module has correct behaviour or not.

The framework is able to execute each test case and to interpret if its results are a pass or a fail for the software module. As the software module matures over time, the percentage of pass results will get higher and higher. When the software is released, the percentage should be at 100%. However, this will need to take into account of the coverage. The software would not be complete if all the tests are passed, but the software module does not cover all its requirements.

During the capturing of the results for interpretation, it must note down several details about test run. As a minimum, it needs to capture the following details

* date and time of test execution
* the details of the test engineer performing the test
* a snapshot of the environment used for the testing
* the unique name of the software module and its version number.
* the unique name of the test case and its version number.

These details provides a clear picture of the environment of the tests. If the tests are to be recreated, all the details are available.

## Integration

To allow integration of the unit test framework into the build chain, it should be able to be triggered via command line. At the moment, the computer environment most open for tools integration is via the command line. The concept is true if the host is a Windows machine, Linux machine, OSX machine or even one hosted on HP-UX. Command line environment with its large amount of tools, it is the most easiest way of doing this.

The typical setup in a build tool chain is to execute all unit test cases when it is used to do a release. It will compile the the software module, its corresponding test cases are automatically executed afterwards. If the unit tests are not successful, the build is aborted and the engineers notified. Successful testing is to be defined by the project. One possible definition is for all unit test to achieve 80% pass rate as the build is for a prototype. Another definition is for all unit test to achieve 100% pass rate as the build is targeting production.

Once written, the unit tests are available for free in the future. So it make sense to include and execute them as often as possible.

Some unit test frameworks are driven via a graphical interface. This will make it problematic when integrating. A deeper investigation is need on the unit test framework on how it can be integrated. There are several GUI tools available that can be used to automate the execution of the unit tests.

The obvious fall back is to trigger the unit test manually. When you want to execute the unit test cases, the unit test framework would start up manually by you.

For unit test on the target, it would be beneficial if the compiled unit test binary can be downloaded onto the target, executed and the result captured automatically. At the time of writing this book, several of the debugger tool companies are going towards this direction. Their debuggers can be controlled via scripts, making it possible for the automatic execution of the unit test binary.  

## Maturity of the framework

There are many unit test frameworks which are designed for a language that is quite new. This usually means that the unit test framework is also quite new. If this is the case, make sure that you pick a unit test framework that is well supported. The support could be in the form of a well informed and helpful user community. The support could also be purchased from technical experts of the unit test framework.

For support via user community, please do the right thing and  contribute your gained knowledge back into the community. They have provided you with valuable help.