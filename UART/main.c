#include <xc.h>
#include "config.h"
#include "UART.h"

#define rx      (LATBbits.LATB6)
#define tx      (LATBbits.LATB7)
#define SYS_FREQ 120000000UL

void _delay_us(unsigned int us) {
    // Convert microseconds us into how many clock ticks it will take
    us *= SYS_FREQ / 1000000; // Core Timer updates every 2 ticks

    _CP0_SET_COUNT(0); // Set Core Timer count to 0

    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void _delay_ms(int ms) {
    _delay_us(ms * 1000);
}

int main(void) {  
    
    // Initialize UART
    UART1_Init();
    
    // String to be printed
    const char* message = "Hello from PIC32MK!\r\n";
    
 
    while (1) {
    // Send the string via UART
      write_UART1(message);
      _delay_ms(1000);
    }
    
    return 0;
}
