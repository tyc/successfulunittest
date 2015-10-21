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

When a interrupt occurs in a microcontroller, it would load its program counter with the address of the interrupt service routine. It would get the address from a predefined address location in the memory space. Once the program counter is loaded, it start executing the interrupt service routine. 

From a unit test perspective, it is difficult to simulate the dynamic nature of the interrupt service routine. However, from the code perspective, it is just like it is being called. The only difference is that no values are being passed into it.

Let's take an example where the analog to digital convert interrupt is to be unit tested. It is a simple routine that would take converted value and store it into a buffer.

The simple requirements are

	req_ADC1: When the interrupt is triggered, the interrupt service routine shall copy the converted value into the FIFO of the ADC Manager.
	req_ADC2: At the end of the interrupt service routine, the interrupt flag is cleared.
  
From the requirements, the inputs are

* The interrupt flag (in1)
* The converted data (in2)

From the requirements, the outputs are

* the interrupt flag status (out1)
* the FIFO buffer containing the converted data (out2)


| in1    | in2     | out1    | out2    |
| ------ |:-------:|:-------:|:-------:|
| false  | 0x55    | false   | 0xff    |
| true   | 0x55    | false   | 0x55    |


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
		
		/* setup the Status register before calling the ISR */
		if (test_data[index].interrupt_flag_input != FALSE)
		{
			ADC0_SR |= (0x01 << ADC0_EOC_BIT);
		}
		else
		{
			ADC0_SR &= ~(0x01 << ADC0_EOC_BIT);
		}
		ADC0_DATA = test_data[index].converted_val;
		
		/* execute the ISR for the conditions setup above */
		adc_ISR();
		
		/* extract the resultant data */
		interrupt_flag_out = ADC0_SR & (0x01 << ADC0_EOC_BIT);
		data_in_fifo = get_adc_fifo_data();
		
		/* test if the resultant data is the same as the expected */
		assert(ADC0_SR & (0x01 << ADC0_EOC_BIT) == test_data[index].interrupt_flag_out);
		assert(get_adc_fifo_data() == test_data[index].data_in_fifo);
	}
}
```

The two asserts are used to check that the interrupt service routine satisfies the `req_ADC1` and `req_ADC2`. During the test, the interrupt service routine will appear to be execute as though a real interrupt has occurs, however there are some differences that must be kept in mind.

* When an interrupt service routine is called, the context of the microcontroller is saved onto the stack. WHen the interrupt service routine is completed, the context is restored from the stack. Context typically means the all the values of the microcontroller's registers. The unit test's version will not do this action as it is just being treated as a function. 
* When the interrupt service routine is completed, it is returned normal execution with a special return instruction, something like a RETI. In most microcontroller, executing the RETI instruction will trigger some other action to clear interrupts flags.

These differences are not present in your unit test, so be aware of these if it might have some influence of your software behaviour.

There will other interrupt strategies that are not easily unit tested. An example would be nested interrupts, especially the scenario of returning from servicing an interrupt to servicing the routine that was interrupted. 

### Accessing external memory

External memory are memory devices that are outside of the microcontroller. They can either be used for storing of code or data, and are connected via a number of ways. Traditionally, they are memory mapped and access the content is just de-referencing a pointer to it, or maybe just let the linker handler the memory allocation.

If the memory devices are not accessed via a memory mapped bus and access to its content need software intervention, then the memory devices needs to be mocked. Some examples of access is via a SPI bus for relatively fast memory access, or via I2C for slower memory access.

One of the common memory functions in an embedded is the non volatile memory manager. This software manager deals the accessing of data on an external memory. Think of it as saving data onto the harddisk on your PC. There are many algorithms to managing the memory, and it is beyond the scope of this book to go into its detail.

For the unit test of the non volatile memory manager, a mocked memory device is needed to simulate the behaviour of the external memory device. The actual manager code will be sitting on top of the device driver of the bus connecting to the external memory device. The mocking of the external memory device is only needed to test the device driver.

To easiest way is to mock the external memory access is to abstract the access to a set of macros. Even though accessing the external memory will be just like accessing a set of memory addresses, you should bear in mind that the content of the external memory also needs to be mocked. So if the device writes a value of 0x5A to address 0x10FA in the external memory device, that data in the device needs to be persistent. This is particular important if you want to test your non volatile memory manager for unwanted access and causing corruption.

### Dealing with watchdog driver

A watchdog device is used to reset the microcontroller if you software is not behaving correctly. This misbehaviour would include a stack overflow and causing your software returning to an unintended address after exiting a function. The watchdog would detect this misbehaviour and reset microcontroller.

The detection comes in a form an exhausted timeout period in an external device. The timeout period is reset to the zero by the software, usually by driving a logic high to the external device.

Because the true testing of the watchdog is dynamic in nature, it is only possible to test the logic of the watchdog manager with unit test. Dynamic aspects of the watchdog is within the scope of integration testing.

A well designed watchdog function is triggered from a scheduler task that is triggered periodically. If the time period is shorten or lengthen, the watchdog tickling will occur on the wrong time and cause the watchdog to generate the reset signal to the microcontroller.

The test case for a watchdog would be to execute the watchdog task the correct number of times and check that the watchdog tickling function is called at the right time. For example, your watchdog function is called every 100uS, and the watchdog needs to be tickled every between 200ms and 250ms. So the ideal tickle period is 225ms. Doing the maths, after the watchdog task has been called 2250 times, the watchdog is tickled. 

### How to do unit test functions that has assembly code

Doing unit test on software the contains assembly code is very difficult to get full coverage. The basic fact that the assembler is only specific to the microcontroller and is not executable on the host computer.

The only way is to abstract the assembler code into a function. By using the `inline` attribute for the function declaration, the linker will put the code together as though the assembler is not abstracted to a function.


```

```



### Items that are not possible to unit tested