# Importance

Unit testing a software unit is one of the corner stones of controlling the quality of your code. Properly used, it can eliminate all bugs associated with this code.

If you are not testing your code, these are the questions that you should ask yourself

* if you are not unit testing your code, how confident are you that your code will behave correctly for all the possible inputs it will receive? 
* if you are not unit testing your code, how do you know that it is calling lower level functions correctly?
* if you are not unit testing your code, how do you know it is logically suitable for the platform you are deploying it on?
* if you are not unit testing your code, how confident are you that it is returning the correct values?

For the inputs for your software unit, it will process it or use it to as decision point. The process can be calling lower level functions, react upon the returns of the lower level functions, or internal logical paths based upon the inputs. Each of these functions needs to be tested before the your software unit is declared to be tested.

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

The unit test harness will check that the correct parameters are passed into the lower level function. If the function for a water alarm is called with an urgent priority, a warning mesage should show up. 


