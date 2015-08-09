

uint8 setAlarmHighSideDriver(uint8 volt, bool enable)
{
	if ((enable != FALSE) &&
		(VALID_VOLT_MIN < volt)
		)
	{
		setDO(FET1_HSD);
	}
	else
	{
		setDO(FET2_HSD);
	}
}



uint8 scheduler(void)
{
#ifndef(UNITTEST)
	while(1)
#endif
	{
		if (isTimeTick(1ms) != FALSE)
		{
			function[function_index]();
			function_index++;
			function_index %= FUNCTION_MAX;
		}
	}
}
