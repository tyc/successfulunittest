# Testing Low Level Code

Low level code works closely with the hardware. They also known as device drivers. Because the code works very close to the hardware, it is very specific to the hardware it is targeting. From a reuse perspective, the best scenario is if a standard microcontroller or standard circuit is used for all projects, now and in the future. Although it is the best scenario, it is not realistic. Typically, the microcontroller is chosen for the project and most projects have their unique requirements that would cause a different microcontroller to be chosen. For example, for a project where power consumption is a key requirement, the Texas Instrument MSP430 is a good candidate as it has a low power consumption during sleep mode, however for a project that needs more processing power, an microcontroller with a ARM core or more in it might more suitable as it provides high processing power.

To test the code, it can be written to be tested with an debugger attached to the hardware. This would tests the code on the actual device and would get the true behaviour. There several reasons why this scenario may not work as well.

The debugger hardware may be expensive and not every developer can have access to one. The testing procedure is also likely to be manually executed. Manual testing can be difficult to maintain a consistent environment between testing sessions. 

The electronic circuitry may not even be designed for you to start interacting with when you starts to design the code, so the low level code you are creating now must be tested on a mocked platform.

## Mocking the hardware

To test the low level code, the concept of mock is used. The definition of mock is "Make a replica or imitation of something".

The behaviour of the microcontroller and the electronic circuitry are simulated in the mocked functions. The key to accessing these is getting access to the registers of the microcontroller. The registers are the conduits between the software and the hardware. Even memory devices can be seen registers.   

### Accessing to the registers

Most microcontroller's functions are accessible via memory mapped registers. These registers contain bit fields that you can use to affect the behaviour, and can also be use to read status of inputs or functions.

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
	PORTB = PORTB | 0x01;		/* writing 1 to PB0 */ 
	
Just be aware that in my code, I am doing a read, followed by a logical ORing of the lowest bit and finally the result is written back to register. This is true for both DDRB and PORTB registers.
	
When doing unit testing, the `__UNITTEST__` macro would be defined and PORTB and DDRB would be accessing an array element. Otherwise, it would addressing the actual registers on the microcontroller. Noticed that I created an array to contain all the registers in the microcontroller. This would only work if the microcontroller is small and simple. Luckily, the ATMega128 addressing of the registers starts at a very low address and it has only a small amount of registers. The size of all the mocked variables can be fitted to an 8bit value.

For a 32bit microcontroller such as a Freescale MPC5510 PowerPC, there are hundreds of registers and simulating all the registers' addresses as an array is not a suitable strategy. An array to contain all the registers would be very large, and the index into the array would be a 32bit value. For such a microcontroller, the following set up might be better.

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

	....
	/* clearing the lower nibble */
	PCR0 = PCR0 & 0xf0;
	
	/* checking if bit 3 if PCR1 is set */
	if (PCR1 & (1<<PIN3) != 0x00)
	{
		....
	}
	....
	
With some planning after careful examination the register structure of the microcontroller, performing unit tests on low level drivers can occur in a robust manner.  

At the start of the project, it would save significant amount of time if the file that contains all the register is created. This can be used for unit testing as well as for production code.
	
## Dealing with interrupts

In an embedded system, the microcontroller would sometimes have to deal with interrupts. Interrupts are used for events that must be serviced and can not be missed. It is also used to handle non-periodic events where a polling strategy is not suitable.

When an interrupt occurs in a microcontroller, it would load its program counter with the address of the interrupt service routine. This is achieved by getting the address from a predefined address location in the memory space, this memory space is typically known as interrupt vector table. Once the program counter is loaded, the stack would be loaded with context and it start executing the interrupt service routine.

From an unit testing perspective, it is difficult to simulate the dynamic nature of the interrupt service routine. However, from the code perspective, the interrupt service routine is just like it is being called as a function. The only difference is that no values are being passed into it.

Let's take an example where the analog to digital convert interrupt is to be unit tested. It is a simple routine that would take converted value and store it into a buffer.

The simple requirements are

	req_ADC1: When the end of conversion interrupt is triggered, the interrupt service routine shall copy the converted value into the FIFO of the ADC Manager.
	req_ADC2: At the end of the interrupt service routine, the interrupt flag is cleared.
  
From the requirements, the inputs are

* The interrupt flag (in1)
* The converted data (in2)

From the requirements, the outputs are

* the interrupt flag status (out1)
* the FIFO buffer containing the converted data (out2)

```
	| in1   | in2  | out1  | out2  |
	|-------|------|-------|-------|
	| false | 0x55 | false | 0xff  |
	| true  | 0x55 | false | 0x55  |
```

The actual value used for the converted data (in2) is really a don't care, but I put in a value in there to specify that the value of copied value (out2) into the FIFO buffer needs to be the same. It just need to be different from the value in out2.

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
		{FALSE, 0x55, FALSE, 0xFF},
		{TRUE,  0x55, FALSE, 0x55}
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
		
		/* Execute the ISR for the conditions setup above.
		 * This simulates a jumping to the ISR after the 
		 * it address is loaded from the interrupt vector
		 * table. 
		 */
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

* When an interrupt service routine is really called, the context of the microcontroller is saved onto the stack. When the interrupt service routine is completed, the context is restored from the stack. Context typically means that all the values of the microcontroller's registers, but it is dependent on the implementation from the compiler. The unit test's version will not save the context as it is just being treated as a function. This needs to be kept in mind if you are testing functions that perform stack unrolling, or nested interrupts.
* When the interrupt service routine is completed, it returns to normal execution with a special return instruction, something like a RETI. In most microcontroller, executing the RETI instruction will trigger some other action to clear interrupts flags. Obviously, this is not the case in the unit test situation.

These differences are not present in your unit tests, so be aware of these if it might have some influence of your software behaviour.

There will other interrupt strategies that are not easy to unit test. An example would be nested interrupts, especially the scenario of returning from servicing an interrupt to servicing the routine that was interrupted. Stack unrolling that is typically used in pre-emptive system is usually hard to unit test fully with this method.

## Accessing external memory

External memory are memory devices that are outside of the microcontroller. They can either be used for storing of code or data, and are connected via a number of ways. Traditionally, they are memory mapped and access the content is just de-referencing a pointer to it, or maybe just let the linker handler the memory allocation. So this is just simply reading from a memory location, as well as writing to a memory location.

If the memory devices are not accessed via a memory mapped bus and access to its content need software intervention, then the memory devices needs to be mocked. Some examples of access is via a SPI bus for relatively fast memory access, or via I2C for slower memory access.

One of the common memory functions in an embedded is the non-volatile memory manager. This software manager deals the accessing of data on an external memory and deals with situations such as wear leveling, error corrections and the list goes on. Think of it as saving data onto the harddisk on your PC. There are many algorithms to managing the memory, and it is beyond the scope of this book to go into its detail.

For the unit testing of the non volatile memory manager to simulate access to the device, a mocked memory device is needed to simulate the behaviour of the external memory device. The actual manager code will be sitting on top of the device driver of the bus connecting to the external memory device. The mocking of the external memory device is only needed to test the device driver.

To easiest way is to mock the external memory access is to abstract the access to a set of macros. Even though accessing the external memory will be just like accessing a set of memory addresses, you should bear in mind that the content of the external memory also needs to be mocked. So if the device writes a value of 0x5A to address 0x10FA in the external memory device, that data in the device needs to be persistent. This is particular important if you want to test your non volatile memory manager for unwanted access and causing corruption.

If the memory size is large, a good strategy is to limit your location on the boundary conditions, such as

* the regions around start address and the end address.
* if the address space is beyond the size of the microcontroller, test around the region beyond the size of the address. For example, if the external FLASH you are using is 64KByte and your microcontroller is an 8bit microcontroller, test around the regions of 256. This will test if the software handles of moving from a 8bit address space to a 16 address space.

## Dealing with watchdog driver

A watchdog device is used to reset the microcontroller if your software is not behaving correctly. This misbehaviour would include a stack overflow and causing your software returning to an unintended address after exiting a function. The watchdog would detect this misbehaviour and reset microcontroller.

The detection comes in a form an exhausted timeout period in an external device. The timeout period is reset to the zero by the software, usually by driving a logic high to the external device.

Because the true testing of the watchdog is dynamic in nature, it is only possible to test the logic of the watchdog manager with unit testing. Dynamic aspects of the watchdog is within the scope of integration testing, at least if you want to get real time behaviour or to take into account of component tolerances.

A well designed watchdog function is triggered from a scheduler task that is executed periodically. If the time period is shorten or lengthen, the watchdog tickling will occur on the wrong time and cause the watchdog to generate the reset signal to the microcontroller. A shorten period would only affect a windowed watchdog. 

The test case for a watchdog would be to execute the watchdog task the correct number of times and check that the watchdog tickling function is called at the right time. For example, your watchdog function is called every 100uS, and the watchdog needs to be tickled every between 200ms and 250ms. So the ideal tickle period is 225ms. Doing the maths, after the watchdog task has been called 2250 times, the watchdog is tickled. Your test case would be to check that at the 2250th execution of the watchdog task, the watchdog is tickled.

### How to do unit test functions that has assembly code

Doing unit test on software the contains assembly code is very difficult to get full coverage. The basic fact that the assembler is only specific to the microcontroller and is not executable on the host computer. So the assembler code cannot be unit tested off target, and will have to be tested on the target.

For off target testing, the only way is to abstract the assembler code into a function. From the unit testing perspective, your function that contains the assembler code would look just like a function that needs to be mocked. Be sure that it only contains assembler code.

By using the `inline` attribute for the function declaration, the linker will put the assembler code into the function as though the assembler is not abstracted to a function. This will remove any artefacts from the abstraction of the assembler code such as the usage of stack to store return addresses. 
 
For example, in your software, there is a function that executes a series of assembler instructions, like the following. This assembler code is extracted to a function.

```
/* a function containing some assembler code
 */
inline void asm_some_code(void)
{
	asm("mov x,z");
	asm("bne x, 0x55");
}
```

If the header file that contains its function prototype, it would be

```
inline void asm_some_code(void);
```

The unit testing code will include the header file, and have a mocked function for it. The mocked function will be linked into the unit test binary for execution, and not use the assembler function.


