#include <xc.h>
#include "config.h"

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
// Function to configure UART
void UART1_Init() {
    // Configure U1MODE register for UART setup
    U1MODE = 0; // Clear UART1 mode register
    U1MODEbits.BRGH = 0; // Set baud rate to standard speed
    U1BRG = ((4000000 / 9600)/16)-1;
    //U1BRG = BRGVAL; // Set the baud rate (BRGVAL is a predefined constant)
    
    //Configure RX and TX pin uart communication  
    U1RXRbits.U1RXR = 0b0000; // receiver pin as RPB6
    RPB7Rbits.RPB7R = 0b00001; // transmitter pin as RPB7
    
    // Configure input and output pin 
    TRISBbits.TRISB6 = 1;
    LATBbits.LATB6 = 1;
    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;
    
    // Configure U1STA register for UART setup
    U1STA = 0; // Clear UART1 status and control register
    U1STAbits.UTXEN = 1; // Enable UART1 transmitter
    U1STAbits.URXEN = 1; // Disable UART1 receiver (optional)
    
    //8 BIT, no parity bit, and 1 stop bit (8N1 setup)
    U1MODEbits.PDSEL = 1;
    U1MODEbits.STSEL = 0;
    
    // Enable the UART1 module
    U1MODEbits.ON = 1;
}

// Function to send a character via UART
void UART1_SendChar(char data) {
    while (U1STAbits.UTXBF); // Wait while buffer is full
    U1TXREG = data; // Transmit data
}

// Function to send a string via UART
void write_UART1(const char* str) {
    while (*str != '\0') {
        UART1_SendChar(*str);
        str++;
    }
}

void read_UART1(char *message, int maxLength) {
    char data = 0;
    int complete = 0, num_bytes = 0;

    while (!complete) {
        if (U1STAbits.URXDA) {
            data = U1RXREG;
            if ((data == '\n') || (data == '\r')) {
                complete = 1;
            } else {
                message[num_bytes] = data;
                ++num_bytes;
                if (num_bytes >= maxLength) {
                    num_bytes = 0;
                }
            }
        }
    }
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