#Tools

There are several ways to look at the tools used in unit tests. All of the tools are geared towards to the getting your software code to behave as per the reqiurements.  They can range from self made tools to enterprise level tools. 

One of aspects of selecting a tool is the fact that it is wide ranging if it is to be standardise across many projects over a long period of time. This type of techincal debt is present in any tool that you choose. It includes the tools used for devloping your code. So choose carefully.

The unit test tool forms a framework which the test case can be created and managed. The test engineer would be able to kick the test run, be able to specify which tests to run and have the results when completed.
 
## Automatic Stub generator

An automatic stub generator analyses the software module under test. From the analysis, it will get a list of lower level functions that it calls. The signature of the lower level function is also obtained.

The stubs are usually obtained from the header file that is included by software module under test.

Once the stubs have been generated, it is up to the test engineer to fill in the details of the stubs. The behaviour of the stub is defined.

The thing to remember is that depending upon the test case, the stub may have a different behaviour. In one test case, the test engineer may want the stub to return a value that is out of the expected range of acceptable values for negative testing. In another test case, the test engineer may want to have the stub return the correct values for positive testing.

## Debuggable

During the early stages of the developing of the software module, bugs and logical prolems will surface. Running the test cases over it will case it to detect a failure.

One of the many methods of debugging it would be to take the inputs used in the test case and analyse it for the root cause of the bug. If the software module is complex, it is quite intensive and can lead to many false conclusion. Unless you have plenty of time, it is recommended to avoid this debugging option. However, use it as a last resort.

There are some unit test frame work around where mixes languages. In the embedded domain, the software code is usually written in C, C++ or assembly, but the test cases are written in another language such as Python or Java. When a different language is used, it is typical for the C code to be compiled into a shared library. In the Windows world, the shared library would be compiled into a `.dll`. In the Linux world, the shared library would be a compiled into a `.so`.  When the test case that is written in a different language is linked to the shared library, the resultant binary is difficult to debug. The debugging tool will have to cross the a language boundary to support all the languages in the binary. 

Having said that, I have found that a test case written in Python, linked with a shared library. This combination can be debugged within Windows if the shared library is created using the GNU tools. They will be other combinations as well.

To have the ability to debug the software module will allow it to be tested faster. It will allow you to get to the root cause of the bug a lot faster.

## Metric collecting and logging

Collecting metrics about the unit test is a very important aspect of the over test strategy. The metric is used to ascertain whether the software module has correct behaviour or not.

The framework is able to execute each test case and to intepret if its results is a pass or a fail for the software module. As the software module matures over time, the percentage of pass result will get higher and higher. When the sofware is released, the percentage should be at 100%. However, this will need to take into account of the coverage. The software would not be complete if all the tests are passed, but the software module does not cover all of its requirements.

During the capturing of the results for intepretation, it must note down several details about test run. As a minimum, it needs to capture the following details

* date and time when the test was executed
* the name of of the test engineer performing the test
* a snapshot of the computer environment that the test occurred on
* the unique name of the software module and its version number.
* the unique name of the test case and its version number.

These details provides a clear picture of the enviroment of the tests. If the tests are to be recreated, all the details are available.

## integratable

To allow the easy integration of the unit test framework into the overall build chain, it should be able to be triggered via command line. As the moment, the computer enviroment most open for tools integration is via the command line. The concept is true if the host is a Windows machine, Linux machine, OSX machine or even one hosted on HP-UX. Command line environment with its numerious amount of tools, is the most easiest way of doing this.

The typical setup in a build tool chain is to execute all unit test cases when it is used to do a release. It will compile the the software module is compiled, its corresponding test cases are automatically executed afterwards. If the unit tests are not successful, the build is aborted and the engineers are notified. Successful is to be defined. One possible definition is for all unit test to achieve 80% pass rate as the build is for a prototype. Another definition is for all unit test to achieve 100% pass rate as the build is targeted for production.

Once written, the unit tests are available to be used for free. So it make sense to include and execute them as often as possible.

Some unit test framework are driven via a graphical inteface. This will make it problematic when integrating. A deeper investigation is need on this tool as how it can be integrated. There are several GUI tools available that can be used to automate the execution of the unit tests.

The obvious fall back is to trigger the unit test manually. When you want to execute the unit test cases, the unit test framework would start up manually by you. 

## maturity

There are many unit test framework which are designed for the use for a language that is quite new. The also usually means that the unit test framework is also quite new. If this is the case, make sure that you pick a unit test framework that is well supported. The support could be in the form of a well informed and helpful user community, or it could be bought support from technical experts of the unit test framework.

For support via user community, please do the right thing and make some effort to contribute your gained knowledge back into the community as they have provided you with valuable assistance.