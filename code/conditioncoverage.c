

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
