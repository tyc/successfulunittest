unsigned int voltage_reading;

uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
{
	uint8 retVal = ERET_OK;

	if ((raw_count != NULL) &&
		(channel < MAX_ADC_CHANNELS))
	{
		*raw_count = adcRaw[channel];
		voltage_reading = *raw_count * VOLTAGE_SCALE_FACTOR;
	}
	else
	{
		retVal = ERET_BAD_ARG;
	}

	return retVal;
}




unsigned int voltage_reading;

uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
{
	uint8 retVal = ERET_OK;

	if ((raw_count != NULL) &&
		(channel < MAX_ADC_CHANNELS))
	{
		*raw_count = adcRaw[channel];
		voltage_reading = filterAdc(*raw_count) * VOLTAGE_SCALE_FACTOR;

		if (voltage_reading > VOLTAGE_READ_MAX)
		{
			voltager_reading = VOLTAGE_READ_MAX;
		}
		else if (voltage_reading < VOLTAGE_READ_MIN)
		{
			voltage_reading = VOLTAGE_READ_MIN;
		}
	}
	else
	{
		retVal = ERET_BAD_ARG;
	}

	return retVal;
}




unsigned int voltage_reading;

uint16 filterAdc_to_Voltage(uint16 raw)
{
	uint16 voltage = filterAdc(*raw_count) * VOLTAGE_SCALE_FACTOR;

	if (voltage > VOLTAGE_READ_MAX)
	{
		voltager_reading = VOLTAGE_READ_MAX;
	}
	else if (voltage < VOLTAGE_READ_MIN)
	{
		voltage = VOLTAGE_READ_MIN;
	}

	return (voltage);
}

uint8 getAdcRaw(uint8 channel, uint16 *raw_count)
{
	uint8 retVal = ERET_OK;

	if ((raw_count != NULL) &&
		(channel < MAX_ADC_CHANNELS))
	{
		*raw_count = adcRaw[channel];
		voltage_reading = filterAdc_to_Voltage(*raw_count);
	}
	else
	{
		retVal = ERET_BAD_ARG;
	}

	return retVal;
}

