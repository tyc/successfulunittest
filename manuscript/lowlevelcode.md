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

Register DDRB and PORTB resides at address 0x1A and 0x19 respectively.

In your C code, there would a header file that defines all the register's addresses. To make it testable, it could be defined as

	#if defined (__UNITTEST__)
	
	#define PORTB_ADDRESS			0x1A
	#define DDRB_ADDRESS			0x19
	#define MAX_NUMBER_REGISTERS	0x40
	
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
	
When doing unit test, the `__UNITTEST__` macro would be defined and PORTB and DDRB would be accessing an array element. Otherwise, it would addressing the actual registers on the microcontroller. Noticed that I created an array to contain all the registers in the microcontroller. This would only work if the microcontroller is small and simple. Luckily, the ATmega128 addressing of the registers starts at a very  low address and it has only a small amount of registers. The size of all the mocked variables can be fitted to an 8bit value.

For a 32bit microcontroller such as a Freescale MPC5510 PowerPC, there are hundreds of registers and having all the registers' addresses defined is not a suitable strategy. An array to contain all the registers would be very large, and the index into the array would be a 32bit value. For such a microcontroller, the following set up might be better.

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
 
The way the peripherals are mapped will dictate which method is finally used.  Most modern 32bit microcontroller has its peripherals's registers to a region in the memory space. The base address specifies the start address for the region, and the amount of registers specifies the size of the region.


