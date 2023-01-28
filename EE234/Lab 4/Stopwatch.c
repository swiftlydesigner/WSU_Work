
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_exception.h"

// data type
typedef uint32_t TDATA;
typedef uint32_t* TADDR;

const TADDR LED_BASE = (TADDR) 0x41210000;
const TADDR SW_BASE = (TADDR) 0x41220000;
const TADDR BTN_BASE = (TADDR) 0x41200000;
const TADDR SSD_BASE = (TADDR) 0x43C10000;

const TADDR SLCR_BASE = (TADDR) 0xF8000000;  // System Level Control Registers (SLCR)

const TADDR MIO_PIN_16 = (TADDR) 0xF8000740;
const TADDR MIO_PIN_17 = (TADDR) 0xF8000744;
const TADDR MIO_PIN_18 = (TADDR) 0xF8000748;
const TADDR MIO_PIN_50 = (TADDR) 0xF80007C8;
const TADDR MIO_PIN_51 = (TADDR) 0xF80007CC;
const TADDR GPIO_DIRM_0 = (TADDR) 0xE000A204;  // Direction mode bank 0
const TADDR GPIO_OUTE_0 = (TADDR) 0xE000A208;  // Output enable bank 0
const TADDR GPIO_DIRM_1 = (TADDR) 0xE000A244;  // Direction mode bank 1

const TADDR ICCPMR_BASE_ADDR = (TADDR) 0xF8F00104;  // Interrupt priority mask
const TADDR ICCICR_BASE_ADDR = (TADDR) 0xF8F00100;  // CPU interface control
const TADDR ICDDCR_BASE_ADDR = (TADDR) 0xF8F01000;  // Distributor control
const TADDR ICDISER_BASE_ADDR = (TADDR) 0xF8F01100;  // Interrupt set enable
const TADDR ICDICER_BASE_ADDR = (TADDR) 0xF8F01180;  // Interrupt clear/enable
const TADDR ICDIPR_BASE_ADDR = (TADDR) 0xF8F01400;  // Interrupt priority register
const TADDR ICDIPTR_BASE_ADDR = (TADDR) 0xF8F01800;  // Interrupt processor targets
const TADDR ICDICFR_BASE_ADDR = (TADDR) 0xF8F01C00;  // Interrupt configuration
const TADDR ICCIAR_BASE_ADDR = (TADDR) 0xF8F0010C;  // Interrupt acknowledge
const TADDR ICCEOIR_BASE_ADDR = (TADDR) 0xF8F00110;  // End of interrupt

const TADDR GPIO_MTDATA_OUT_0 = (TADDR) 0xE000A004;  // Maskable data out in bank 0
const TADDR GPIO_INT_DIS_0 = (TADDR) 0xE000A214;  // Interrupt disable bank 0
const TADDR GPIO_INT_EN_1 = (TADDR) 0xE000A250;  // Interrupt enable bank 1
const TADDR GPIO_INT_DIS_1 = (TADDR) 0xE000A254;  // Interrupt disable bank 1
const TADDR GPIO_INT_STAT_1 = (TADDR) 0xE000A258;  // Interrupt status bank 1
const TADDR GPIO_INT_TYPE_1 = (TADDR) 0xE000A25C;  // Interrupt type bank 1
const TADDR GPIO_INT_POL_1 = (TADDR) 0xE000A260;  // Interrupt polarity bank 1
const TADDR GPIO_INT_ANY_1 = (TADDR) 0xE000A264;  // Interrupt any edge sensitive bank 1

// Timer/Counter
const TADDR GTC_REG0 = (TADDR) 0xF8F00200;
const TADDR GTC_REG1 = (TADDR) 0xF8F00204;
const TADDR GTC_CON_REG = (TADDR) 0xF8F00208;
const TADDR GTC_ISR = (TADDR) 0xF8F0020C;
const TADDR GTC_COMP0 = (TADDR) 0xF8F00210;
const TADDR GTC_COMP1 = (TADDR) 0xF8F00214;
const TADDR GTC_AI_REG = (TADDR) 0xF8F00218;

static inline void vset(TADDR addr, TDATA val) {
	*(addr) = val;
}

static inline TDATA vget(TADDR addr) {
	return *(addr);
}

void update_LEDs (TDATA v) {
	vset (LED_BASE, v);
}

void update_7seg (TDATA v) {
	vset (SSD_BASE+1, v);
}

void enable_7seg () {
	TDATA data;
	data = 0x00000001;
	vset (SSD_BASE, data);  // enable 7seg
}

typedef struct {
	TDATA d0, d1, d2, d3, min;
	TDATA state;
} VSW;

/// incTime will update time for the passed in paramter.
///
/// - Precondition: 0.01 seconds have passed and the stopwatch is active.
/// - Postcondition: Time has increased by 0.01 seconds. Downstream values updated as needed.
/// - Parameter sw: The stopwatch data to update.
/// - Returns: None
/// - version: 0.1
void incTime(VSW* sw) {
	TDATA temp = ++(sw->d0); // increment zeroth digit

	if (temp > 9) { // increment first digit if needed [xx:x0], reset zeroth digit
		sw->d0 = 0;
		temp = ++(sw->d1);
	} else { temp = sw->d1; }

	if (temp > 9) { // increment second digit if needed [xx:0x], reset first digit
		sw->d1 = 0;
		temp = ++(sw->d2);
	} else { temp = sw->d2; }

	if (temp > 9) { // increment third digit if needed [x0:xx], reset second digit
		sw->d2 = 0;
		temp = ++(sw->d3);
	} else { temp = sw->d3; }

	if (temp > 5) { // increment minutes and reset to 00:00
		sw->d3 = sw->d2 = sw->d1 = sw->d0 = 0;
		++(sw->min);
	}


}

/// getSSDDataSW will return the value needed to display on the screen.
///
/// - Precondition: Old value displayed
/// - Postcondition: New value returned, waitning to be displayed.
/// - Parameter sw: The stopwatch data.
/// - Returns: An integer that describes the current value for SSD.
/// - version: 0.1
TDATA getSSDDataSW(VSW* sw) {
	TDATA first = 0b0, second = 0b0, third = 0b0, decmials = 0x080008080;
	switch (sw->d1) {
        case 1: first = 0b0000000000000000000100000000; break;
        case 2: first = 0b0000000000000000001000000000; break;
        case 3: first = 0b0000000000000000001100000000; break;
        case 4: first = 0b0000000000000000010000000000; break;
        case 5: first = 0b0000000000000000010100000000; break;
        case 6: first = 0b0000000000000000011000000000; break;
        case 7: first = 0b0000000000000000011100000000; break;
        case 8: first = 0b0000000000000000100000000000; break;
        case 9: first = 0b0000000000000000100100000000; break;
        default:break;
	}

	switch (sw->d2) {
        case 1: second = 0b0000000000010000000000000000; break;
        case 2: second = 0b0000000000100000000000000000; break;
        case 3: second = 0b0000000000110000000000000000; break;
        case 4: second = 0b0000000001000000000000000000; break;
        case 5: second = 0b0000000001010000000000000000; break;
        case 6: second = 0b0000000001100000000000000000; break;
        case 7: second = 0b0000000001110000000000000000; break;
        case 8: second = 0b0000000010000000000000000000; break;
        case 9: second = 0b0000000010010000000000000000; break;
        default: break;
	}

	switch (sw->d3) {
        case 1: third = 0b0001000000000000000000000000; break;
        case 2: third = 0b0010000000000000000000000000; break;
        case 3: third = 0b0011000000000000000000000000; break;
        case 4: third = 0b0100000000000000000000000000; break;
        case 5: third = 0b0101000000000000000000000000; break;
        case 6: third = 0b0110000000000000000000000000; break;
        case 7: third = 0b0111000000000000000000000000; break;
        case 8: third = 0b1000000000000000000000000000; break;
        case 9: third = 0b1001000000000000000000000000; break;
        default:break;
	}

	return sw->d0 | first | second | third | decmials;
}

/// updateSW will check the current state and call mehtods to update the time,
///     display, and leds as needed. RGB status lights are updated in here by
///     enabling/disabling the light.
///
/// - Precondition: A button was pressed. Update state as necessary.
/// - Postcondition: SSD will have the correct value and leds will indicate current state.
/// - Parameter sw: the switch variable to modify and display values from
/// - Returns: None
/// - version: 0.2
void updateSW(VSW* sw) {
    
	*((TADDR)0x43C00020) = 0; // second red disable // This is to disable the second RGB Red light.
    
	switch (sw->state) {
	case 0: // reset
		sw->d0=sw->d1=sw->d2=sw->d3=sw->min=0; // reset everything to zero
		update_7seg(getSSDDataSW(sw)); // update the seven segment display.
		update_LEDs(0); // set all leds to 0 (no need to access vars here)
		*((TADDR)0x43C00040) = *((TADDR)0x43C00030) = 0; // green/blue disable
		*((TADDR)0x43C00050) = 1; // red enable
		*((TADDR)0x43C00020) = 1; // second red enable
		break;
	case 1: // run
		incTime(sw);
		*((TADDR)0x43C00050) = *((TADDR)0x43C00030) = 0; // red/blue disable
		*((TADDR)0x43C00040) = 1; // green enable
		update_7seg(getSSDDataSW(sw)); // update display
		update_LEDs(sw->min); // update minute leds.
		break;
	case 2: // stop
		*((TADDR)0x43C00040) = *((TADDR)0x43C00030) = 0; // green disable
		*((TADDR)0x43C00050) = 1; // red enable
		update_7seg(getSSDDataSW(sw)); // update display
		update_LEDs(sw->min); // update minute leds.
		break;
	case 3: // background count
		incTime(sw);
		*((TADDR)0x43C00040) = 0; // green disable
		*((TADDR)0x43C00030) = *((TADDR)0x43C00050) = 1; // red/blue enable
		break;
	}
}

void configure_IO() {
	vset(SLCR_BASE+2, 0x0000DF0D);  // unlock (enable writing to the SLCR registers)
	vset(MIO_PIN_50, 0x00000600);  // BTN4
	vset(MIO_PIN_51, 0x00000600);  // BTN5
	vset(MIO_PIN_16, 0x00001600);  // RGB LED B
	vset(MIO_PIN_17, 0x00001600);  // RGB LED R
	vset(MIO_PIN_18, 0x00001600);  // RGB LED G
	vset(GPIO_DIRM_0, 0x00070000);
	vset(GPIO_OUTE_0, 0x00070000);
	vset(GPIO_DIRM_1, 0x00000000);
}

void disable_interrupts() {
	TDATA mode = 0xDF;  // system mode [4:0] and IRQ disabled [7]
	TDATA read_cpsr = 0;  // used to read the previous CPSR value
	TDATA bit_mask = 0xFF;  // used to clear the bottom eight bits
	__asm__ __volatile__ ("mrs %0, cpsr\n" : "=r" (read_cpsr));  // execute the assembly instruction MSR
	__asm__ __volatile__ ("msr cpsr,%0\n" : : "r" ((read_cpsr & (~bit_mask)) | mode));  // change the lower eight bits
}

void configure_GIC() {
	vset(ICDIPTR_BASE_ADDR + 13, 0x00000000);  // disable handling interrupt #52
	vset(ICDICER_BASE_ADDR + 1, 0x00000000);  // clear GPIO interrupts
	vset(ICDDCR_BASE_ADDR, 0x00000000);  // disable the distributer of the GIC
	vset(ICDIPR_BASE_ADDR + 13, 0x000000A0);  // Set the priority level of interrupt #54
	vset(ICDIPTR_BASE_ADDR + 13, 0x00000001);  // assign interrupt #52 to CPU0
	vset(ICDICFR_BASE_ADDR + 3, 0x55555555);  // sensitivity for interrupt #52.
	vset(ICDISER_BASE_ADDR + 1, 0x00100000);
	vset(ICCPMR_BASE_ADDR, 0x000000FF);  // processor's priority level
	vset(ICCICR_BASE_ADDR, 0x00000003);
	vset(ICDDCR_BASE_ADDR, 0x00000001);  // enable the distributer of the GIC
}

void initialize_GPIO_interrupts() {
	vset(GPIO_INT_DIS_1, 0xFFFFFFFF);
	vset(GPIO_INT_DIS_0, 0xFFFFFFFF);
	vset(GPIO_INT_STAT_1, 0xFFFFFFFF);  // clear status register
	vset(GPIO_INT_TYPE_1, 0x000C0000);  // type of interrupt rising edge
	vset(GPIO_INT_POL_1, 0x000C0000);  // polarity of interrupt
	vset(GPIO_INT_ANY_1, 0x00000000);  // interrupt any edge sensitivity
	vset(GPIO_INT_EN_1, 0x000C0000);  // enable interrupts in bank 0
}

void enable_interrupts() {
	TDATA read_cpsr = 0;  // used to read the previous CPSR value
	TDATA mode = 0x5F;  // System mode [4:0] and IRQ enabled [7]
	TDATA bit_mask = 0xFF;  // used to clear bottom 8 bits
	__asm__ __volatile__ ("mrs %0, cpsr\n" : "=r" (read_cpsr));
	__asm__ __volatile__ ("msr cpsr, %0\n" : : "r" ((read_cpsr & (~bit_mask)) | mode));  // change the lower eight bits
}

void initialize_global_timer() {
	vset(GTC_CON_REG, 0x00000000);  // disable the interrupt and the timer
	vset(GTC_REG1, 0x00000000);  // counter(high) = 0
	vset(GTC_REG0, 0x00000000);  // counter(low) = 0
	vset(GTC_ISR, 0x00000001);  // clear the event flag
	vset(GTC_COMP0, 3333333);  // set the comparator to 0.01 second under prescale of 1
	vset(GTC_COMP1, 0x00000000);  // the CPU_3x2x freq. is 333.3333MHz, so 1/CPU_3x2x is 10,000us = 10ms = 0.01s
	vset(GTC_CON_REG, 0x00000100);
}

void configure_GIC_GTC() {  // ID #27
	vset(ICDIPTR_BASE_ADDR + 6, 0x00000000);  // disable handling interrupt #27
	vset(ICDICER_BASE_ADDR, 0x08000000);  // clear interrupt #27
	vset(ICDDCR_BASE_ADDR, 0x00000000);  // disable the distributer of the GIC
	vset(ICDIPR_BASE_ADDR + 6, 0x90000000);  // Set the priority level of interrupt #27
	vset(ICDIPTR_BASE_ADDR + 6, 0x01000000);  // assign interrupt #27 to CPU0
	vset(ICDICFR_BASE_ADDR + 1, 0x7DC00000);  // sensitivity for interrupt #27.
	vset(ICDISER_BASE_ADDR, 0x08000000);  // enable interrupt #27
	vset(ICCPMR_BASE_ADDR, 0x000000FF);  // processor's priority level
	vset(ICCICR_BASE_ADDR, 0x00000003);
	vset(ICDDCR_BASE_ADDR, 0x00000001);  // enable the distributer of the GIC
}

void configure_GIC_GPIO() {  // interrupt #52
	vset(ICDIPTR_BASE_ADDR + 13, 0x00000000);  // disable handling interrupt #52
	vset(ICDICER_BASE_ADDR + 1, 0x00000000);  // clear GPIO interrupts
	vset(ICDDCR_BASE_ADDR, 0x00000000);  // disable the distributer of the GIC
	vset(ICDIPR_BASE_ADDR + 13, 0x000000A0);  // Set the priority level of interrupt #54
	vset(ICDIPTR_BASE_ADDR + 13, 0x00000001);  // assign interrupt #52 to CPU0
	vset(ICDICFR_BASE_ADDR + 3, 0x55555555);  // sensitivity for interrupt #52.
	vset(ICDISER_BASE_ADDR + 1, 0x00100000);
	vset(ICCPMR_BASE_ADDR, 0x000000FF);  // processor's priority level
	vset(ICCICR_BASE_ADDR, 0x00000003);
	vset(ICDDCR_BASE_ADDR, 0x00000001);  // enable the distributer of the GIC
}

void initialize_GTC_interrupts() {
	vset(GTC_CON_REG, 0x0000010F);  // restart the timer
	vset(GTC_ISR, 0x00000001);
}

void IRQ_Handler(void* data) {
	// 1. Type casting of the data.
	// 2. Get the interrupt ID using ICCIAR.

	VSW * sw = (VSW*)data; // Type cast the data to use.

	uint32_t interrupt_ID = *ICCIAR_BASE_ADDR; // Get the interrupt ID

	if (interrupt_ID == 52) {  // from GPIO
		// 3. Get the interrupt using GPIO_INT_STAT_1.
		// 4. Process the buttons (BTN4 and BTN5).

		uint32_t button = *GPIO_INT_STAT_1 & 0x000C0000; // Get the button.

		if (button == 0x80000) { // button 5
			switch (sw->state) { // Update the next state based on the current state.
                case 0: break;
                case 1: sw->state = 3; break;
                case 2: sw->state = 0; break;
                case 3: sw->state = 1; break;
                default: break;
			}
		} else if (button == 0x40000) { // button 4
			switch (sw->state) { 
                case 0: sw->state = 1; break;
                case 1: sw->state = 2; break;
                case 2: sw->state = 1; break;
                case 3: sw->state = 2; break;
                default: break;
			}
		}

		vset(GPIO_INT_STAT_1, 0xFFFFFF);  // clear the GPIO interrupt status

	} else if (interrupt_ID == 27) {  // from GTC
				// 5. Process the timer.
				// 6. Reset the timer counter (GTC_REG0, GTC_REG1).
				// 7. Clear the ISR event flag (GTC_ISR).

			updateSW(sw); // Timer, update based on the state.

			vset(GTC_REG1, 0x00000000);  // counter(high) = 0
			vset(GTC_REG0, 0x00000000);  // counter(low) = 0
			vset(GTC_ISR, 0x00000001); // clear event flag

	}

	vset(ICCEOIR_BASE_ADDR, interrupt_ID);  // clear the GIC GPIO interrupt
}



int main()
{
    init_platform();

		// 1. Declare a variable for the stopwatch.
		// 2. Enable the seven-segment display.

    VSW sw = {0,0,0,0,0,0}; // stopwatch. Set time to 00:00:00, State 0.

    enable_7seg();

    configure_IO();
    initialize_global_timer();
    disable_interrupts();
    configure_GIC_GPIO();  // configure the GPIO interrupt (ID #52)
   	configure_GIC_GTC();  // configure the global timer interrupt (ID #27)
    initialize_GPIO_interrupts();
    initialize_GTC_interrupts();
    Xil_ExceptionRegisterHandler(5, IRQ_Handler, (void*) &sw );  // 3. Pass the stopwatch data available.
    enable_interrupts();


    // These are set so the RGB LED will illumunate.
    *((TADDR)0x43C00024) = *((TADDR)0x43C00034) = *((TADDR)0x43C00044) = *((TADDR)0x43C00054) = 1; // Left RGB pers = 1.
    *((TADDR)0x43C00028) = *((TADDR)0x43C00038) = *((TADDR)0x43C00048) = *((TADDR)0x43C00058) = 1; // Left RGB wids = 1.

    while (1) {
    	;
    }

    cleanup_platform();

    return 0;
}
