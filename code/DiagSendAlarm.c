
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

