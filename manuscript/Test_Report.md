# Reporting of the results

After a test run has been executed, and the software module is being prepared to be released, the key document to releasing the software module is the test report.

With a good test report with the necessary evidence, the release of the software module can be executed with confidence. 

## What is in a test report?

With a test report, it is evidence that the testing of the software module was performed. The approving body will examine the test report, and made a decision as to whether the results warrants to the software to be released.

During the examining of the test report, the following data is examined.

* Coverage - The amount of functional coverage is examined. For a software module to be released, all coverage metrics should be at 100%. Coverage metrics would include code, branch and requirements coverage. For software used in a functional safety environment, further metrics may be required.
* Pass Fail ratio - For the test cases that were executed, the pass fail ratio must be at 100%. It is obvious, that the test case selected to be executed needs to be suitable for the release. If the pass fail ratio is not at 100%, maybe the test case selection is not correct, or there are some problems detected by the testing. 
* Test Environment - the test environment must be conducted in a controlled environment, and the details of the environment must be shown in the test report. There must be enough details captured in the test report so that it can be recreated if the testing needs to be repeated. This also includes the test engineer performing the tests.
* Test cases and its results - All the test cases (or a reference to it) executed during the test session must be listed. This created traceability between the test results and the test cases. The traceability is used to double check that the correct test cases are used.
* Details of Software module under test - This shows which version of the software module is being tested. It is obvious that information is required as it specifies what is being tested.

Once all the evidence is available, the engineers that can approve the software module for release will be able to quickly assess the situation and release it.

## Test report generation

The details that goes into the test report can be obtained automatically at the end of the test session. It is worthwhile to create a script to extract these information and generate a test report. 

Automatic generation will reduce the barrier to close off the testing session in a controlled manner.


