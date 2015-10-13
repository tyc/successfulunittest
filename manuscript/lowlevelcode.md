# Testing Low Level Code

Low level code are code that works closely with the hardware. They also known as device drivers. Because the code works very close to the hardware, it is very specific to the hardware it is targeting. From a reuse perspective, the best scenario is if a standard microcontroller or standard circuit is used for all projects, now and in the future. Although it is the best scenario, it is not a realistic scenario. Typically, the microcontroller is chosen for the project and most projects have their unique requirements that would cause a different microcontroller to be chosen for each project. For example, for a project where power consumption is a key requirement, the Texas Instrument MSP430 is a good candidate as it has a low power consumption during sleep mode, however for a project that needs more processing power, an microcontroller with a ARM core in it might more suitable as it provides high processing power.

To test the code, it can be written to be tested with an debugger attached to the hardware. This would tests the code the actual device and would get the true behaviour. There several reasons why this scenario may not work as well.

The debugger hardware may be expensive and not every developer can have access to one. The testing procedure is also likely to be manually executed. The electronic circuitry may not even be designed for you to start interfacing to. So the low level code you are creating now must be tested on a mocked platform.

## Mocking the hardware

To test the low level code, the concept of mock is used. The behaviour of the microcontroller and the electronic circuitry are simulated in the mocked functions. The key to accessing these is getting access to the registers of the microcontroller. The registers are the conduits between the software and the hardware. Even memory devices can be seen registers.   

### Accessing to the registers

Most microcontrollers functions are accessible via memory mapped registers. These registers contain bit fields that you can use to affect the behaviour, and can also be use to read status of inputs or functions.

For example, on a Atmel ATMega128 micrcontroller, to set the pin PB0 as an output and write a logic one to it. The  sequence is

	set bit DDB0 of register DDRB to 1
	set bit PORTB0 of register PORTB to 1

Register `DDRB` and `PORTB` resides at address `0x1A` and `0x19` respectively.

In your C code, there would a header file that defines all the register's addresses. To make it testable, it could be defined as


	
	#define PORTB_ADDRESS			0x1A
	#define DDRB_ADDRESS			0x19
	#define MAX_NUMBER_REGISTERS	0x40
	
	#if defined (__UNITTEST__)
	
	uint8 register[MAX_NUMBER_REGISTERS];
	
	#define PORTB		(&registers[PORTB_ADDRESS])
	#define DDRB		(&ddrb[DDRB_ADDRESS])
	
	#else
	
	#define PORTB		(*(volatile uint8 *)(PORTB_ADDRESS))
	#define DDRB		(*(volatile uint8 *)(DDRB_ADDRESS))
	
	#endif /* __UNITTEST__ */


In your software module, you can use it in the following manner

	DDRB = DDRB | 0x01;			/* writing 1 for direction of pin PB0 */
	PORTB = PORTB & 0xFE;		/* writing 0 to PB0 */ 
	
Noticed that I also do a read before setting the value.
	
When doing unit test, the `__UNITTEST__` macro would be defined and PORTB and DDRB would be accessing an array element. Otherwise, it would addressing the actual registers on the microcontroller. Noticed that I created an array to contain all the registers in the microcontroller. This would only work if the microcontroller is small and simple. Luckily, the ATMega128 addressing of the registers starts at a very  low address and it has only a small amount of registers. The size of all the mocked variables can be fitted to an 8bit value.

For a 32bit microcontroller such as a Freescale MPC5510 PowerPC, there are hundreds of registers and having all the registers' addresses simulated as an array is not a suitable strategy. An array to contain all the registers would be very large, and the index into the array would be a 32bit value. For such a microcontroller, the following set up might be better.

	#if defined (__UNITTEST__)
	
	uint32 siu_pcr0;
	uint32 siu_pcr1;
	#define PCR0		(&siu_pcr0)
	#define PCR1		(&siu_pcr1)
	
	#else
	
	#define SIU_BASE_ADDR		0xFFFE8000
	#define SIU_PCR0_ADDR		(SIU_BASE_ADDR + 0x0040)
	#define SIU_PCR1_ADDR		(SIU_BASE_ADDR + 0x0041)
	
	#define PCR0		(*(volatile uint32 *)(SIU_PCR0_ADDR))
	#define PCR1		(*(volatile uint32 *)(SIU_PCR1_ADDR))
	
	#endif /* __UNITTEST__ */
 
When the lower level code is using the registers, it does not have to consider whether it is doing a unit test or it is using it on the target.

	...
	/* clearing the lower nibble */
	PCR0 &= 0x0f;
	
	/* checking if bit 3 if PCR1 is set */
	if (PCR1 & (1<<PIN3) != 0x00)
	{
		....
	}
	
	
### Dealing with interrupts

In embedded system, the microcontroller would sometimes have to deal with interrupt. Interrupt is used for events that must be serviced and can not be missed. It is also used to handle non-periodic events.

In a simple system, the interrupt would be captured and be serviced by the software immediately. For example, an interrupt is triggered when analog to digital converter has completed a conversion. The interrupt service routine would be copy the converted value, clear the interrupt flag and trigger another the analog to digital converter to perform another conversion. 

In a complex system, the interrupt design is divided into two halves. The top half is used to capture the interrupt event and capture the necessary information. The bottom half which is executed periodically will poll the flag and continue to process the data. For example, when data is received by the CAN peripheral, an interrupt is triggered. The interrupt service routine will copy that data from the data register of the CAN peripheral in a receive data buffer and clear the receive interrupt. That is the top half of the interrupt service routine. The bottom half would poll the "data in buffer" flag to be raised. Once detected, it would process the data in the buffer. The top half is fast code that does not take up took put too much CPU load. The bottom half is usually a lot slower and is executed periodically. This strategy has many variations and is derived from the Unix system.   

From a unit test perspective, it is difficult to simulate the dynamic nature of the interrupt service routine. So each portion of the interrupt service would have to be tested separately.

Let's take an example where the analog to digital convert interrupt is to be unit tested. It is a simple routine that would take converted value and store it into a buffer.

The simple requirements are

	req_ADC1: When the interrupt is triggered, the ISR shall copy the converted value into the FIFO of the ADC Manager.
	req_ADC2: At the end of the ISR, the interrupt flag is cleared.
  
From the requirements, the inputs are

* The interrupt flag (in1)
* The converted data (in2)

From the requirements, the outputs are

* the interrupt flag status (out1)
* the FIFO buffer containing the converted data (out2)


| in1    | in2     | out1    | out2    |
| ------ |:-------:|:-------:|:-------:|
| false  |:0x55   :|:false  :|:0xff   :|
| true   |:0x55   :|:false  :|:0x55   :|


The value used for the converted data (in2) is really a don't care, but I put in a value in there to specify that the value of copied value (out2) into the FIFO buffer needs to be the same.

```
void adc_ISR_test_case(void)
{
	typedef struct 
	{
		/* inputs */
		bool_t interrupt_flag_input;
		uint16_t converted_val;
		
		/* output */
		bool_t interrupt_flag_output;
		uint16_t data_in_fifo;
	} test_data_t;
	
	test_data_t test_data[] =
	{
		{FALSE, 	0x55, FALSE, 0xFF},
		{TRUE,   	0x55, FALSE, 0x55}
	};
	
	for ( 	index = 0;
			index < (sizeof(test_data)/sizeof(test_data[0]));
			index++
		)
	{
		bool_t expected_init_OK;
		
		if (test_data[index].interrupt_flag_input != FALSE)
		{
			ADC0_SR |= (0x01 << ADC0_EOC_BIT);
		}
		else
		{
			ADC0_SR &=
		init_pwm_return_value = test_data[index].init_pwm_return_value;
		init_pwm_reinit_flag = test_data[index].init_pwm_reinit_flag;
		expected_init_OK = init_pwm_if(test_data[index].channel);
		
		assert(expected_init_OK == test_data[index].expected_init_OK);
	}
}
```
