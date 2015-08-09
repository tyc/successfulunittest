# Importance of Unit Test

Unit testing a software unit is one of the corner stones of controlling the quality of your code. Properly used, it can eliminate all bugs associated with this code.

If you are not testing your code, these are the questions that you should ask yourself

* if you are not unit testing your code, how confident are you that your code will behave correctly for all the possible inputs it will receive? 
* if you are not unit testing your code, how do you know that it is calling lower level functions correctly?
* if you are not unit testing your code, how do you know it is logically suitable for the platform you are deploying it on?
* if you are not unit testing your code, how confident are you that it is returning the correct values?

For the inputs for your software unit, it will process it or use it to as decision point. The process can be calling lower level functions, react upon the returns of the lower level functions, or internal logical paths based upon the inputs. Each of these functions needs to be tested before the your software unit is declared to be tested.

## testing the software for correct interface

When the architect is defining the software modules that are used in the software, the interface to the software modules are also defined. An interface is how the software module accepts and returns data to other software modules.

* Data can be passed in either as global variables or as function parameters. For C, function parameters are the preferred method as its scope are better controlled.
* Data can be returned either as global variables, returned data or via function parameters. In C, data can be returned via function parameters if the address of the variables are passed in through a function parameter.

For a language like C, which is a strongly type at the interface level, the interface can be tested to check that correct types are used in its implementation. If the incorrect types are used, the interface contract is violated. This would not allow the software module to be easily interfaced.

## testing for software for correct behaviour

This test is to ensure that the software module has the correct behaviour. This is the main focus of the unit test.

Correct behaviour ensures that the when the software is integrated with the other software modules, its behaviour is known and is dependable. The developer will not have to spend a lot of effort in understanding how it is to be interfaced for the correct behaviour.

If the software's behaviour is well known, when software is being debugged during integration, the root cause can be narrowed down quickly. Unknown behaviour is a major road block to narrowing down to the root cause of a bug.

The knowledge of the correct behaviour also allows the software module to be modified quickly. The test harness can also be quickly modified to check that the software module is modified correctly. For the software engineer modifying the software module, it gives her confidence that the modification can be checked very quickly. For the software developers that are integration the modified software module into their code, it also gives them confidence that the modification will have minimal impact for them.


## testing for efficient implementation

In the embedded world, efficient implementation of a software module means that either frugal with the amount of memory it uses, the amount of cpu resources it consumes, or both. 

This is particularly important for an embedded system as it is typically resourced limited. Imagine that the software is deployed on a 8 bit microcontroller running at 1Mhz, have 8Kbytes ROM and 512 bytes of RAM to work with. For a real world application, this configuration is very challenging.

The unit test can profile the code to see the most inefficient part of the code. The efficiency can be tested by stressing it with different inputs and the amount of memory it would used for those inputs. If the test harness can be coupled to a profiler, measurements can be taken and used in a comparison on improvements.
   

## DiagSendAlarm example

Lets take a more detail analysis of why each of these behaviour is important via an example. Consider a service level software module for handling a serial protocol, lets call this module `DiagSendAlarm`. Its only function is to send an alarm signal over the diagnostic databus. It also return a boolean to indicate if data was successfully queued.

	bool DiagSendAlarm(unsigned int Alarm)
	{
		bool retVal = FALSE;

		if (alarmEnable == TRUE)
		{
			switch (alarm);
			{
			case FIRE :
				sendAlarm(MASTER_NODE, FIRE, urgent);
				retVal = TRUE;
				break;
			case WATER :
				sendAlarm(MASTER_NODE, WATER, normal);
				retVal = TRUE;
				break;
			default :
				sendAlarm(MASTER_NODE, FALSE_ALARM, normal);
				break;
			}
		}

		return (retVal);
	}

### lower level functions

In `DiagSendAlarm`, the lower function `sendAlarm` is used to send a specific type of alarm to the master node. The priority of the message is also sent. If the alarm was due to fire, the alarm was despatched with urgent priority.

The unit test harness will check that the correct parameters are passed into the lower level function. If the function for a water alarm is called with an urgent priority, a warning message should show up. 


