#include <string.h>
#include "buffer.h"
#include "comms.h"
#include "io.h"

// This stuff needs to be defined in its own header for everything
#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define PWRHUM		PORTE1

ISR(WIFI_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_wifi_rx.buff[buff_wifi_rx.tail++] = UDR_WIFI;
    buff_wifi_rx.free--;

    if (buff_wifi_rx.tail >= buff_wifi_rx.size) {
        buff_wifi_rx.tail = 0;
    }

    //PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);
}

ISR(TRANS_RX_vect){
	// when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_trans_rx.buff[buff_trans_rx.tail++] = UDR_WIFI;
    buff_trans_rx.free--;

    if (buff_trans_rx.tail >= buff_trans_rx.size) {
        buff_trans_rx.tail = 0;
    }
	
}

void delay_ms(int t){
	int intervals = t/50;
	
	for (int i = 0; i < intervals; i++){
		
		_delay_ms(10);
	}
	
}

/** 
 * Initializes a given buffer for use. This function sets the baud rate, enables transmission and reception,
 * and enables a receive interrupt for UART.
 */
void UART_init (unsigned int ubrr) {
    // Disable power reduction for USARTS
    //PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);

    // Enable WiFi UART
    UBRRH_WIFI = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_WIFI = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    //UCSRA_WIFI = (1<<UDRE_WIFI)|(1<<U2X_WIFI);
    UCSRB_WIFI = (1<<RXEN_WIFI)|(1<<TXEN_WIFI); // Enable receiving and transmission on UART
    UCSRB_WIFI |= (1<<RXCIE_WIFI); // enable receive interrupt for circular buffer
    //UCSRC_WIFI = (1<<USBS_WIFI)|(3<<UCSZ0_WIFI);			// 2 stop bits & 8 bit character size
    init_buffer(&buff_wifi_rx, BUFFER_LENGTH_BYTES);

    // Enable Transceiver UART
    UBRRH_TRANS = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_TRANS = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    //UCSRA_TRANS = (1<<UDRE_TRANS)|(1<<U2X_TRANS);
    UCSRB_TRANS = (1<<RXEN_TRANS)|(1<<TXEN_TRANS); // Enable receiving and transmission on UART
    UCSRB_TRANS |= (1<<RXCIE_TRANS); // enable receive interrupt for circular buffer
    //UCSRC_TRANS = (1<<USBS_TRANS)|(3<<UCSZ0_TRANS);			// 2 stop bits & 8 bit character size
    init_buffer(&buff_trans_rx, BUFFER_LENGTH_BYTES);

    sei(); //enable interrupts
}

/** 
 * Transmit byte to WiFi modules. This function waits for the ATmega to finish transmitting the last byte before
 * attempting another transfer.
 */
void UART_WiFi_TX (uint8_t byte) {
    while(!( UCSRA_WIFI & (1<<UDRE_WIFI))); // Wait for TX to finish
    UDR_WIFI = byte; // Write for TX
}

// Taking input characters of size size and sending them through the UART TX without storing the values in a buffer first
void TXWrite(unsigned char c[], uint8_t size, uart_dev dest ){
	
	// Writing to the transmit port the amount of characters defined by size
	uint16_t count = 0;
	while (size > 0 ){
		if (dest == WiFi){
			while (!(UCSRA_WIFI & (1<<UDRE_WIFI)));
			UDR_WIFI = c[count];
			count++;
			size --;
		}
		else if (dest == Transceiver) {
			while (!(UCSRA_TRANS & (1<<UDRE_TRANS)));
			UDR_TRANS = c[count];
			count++;
			size --;
		}	
	}
}

// Setting up ports for HUMPRO900 transceiver
uint8_t * setupTransceiver(){
	
	sei();		// Enabling interrupts
	DDRB |= (1 << DDB7);	//PD4 PWR High for normal op, low for low power operation (power down)
	//DDRC &= ~(1 << DDC3);	//PC3 CRESP_BAR
	DDRC &= ~(1 << DDC4);	//PC4 CTS_BAR (high when busy, low when module is ready)
	DDRB |= (1 << DDB6);	//PC5 Reset, resets when low
	
	
	//DDRD &= ~(1 << DDD0);	//PD2 Buffer empty status (high when buffer is empty) input
	//DDRD &= ~(1 << DDD4);	//PD3 Exception status
	DDRD |= (1 << DDD5);	//PD5 CMD_BAR (high for data transmit, low for commands)
	
	
	PORTB &= ~(1 << PORTB6);		// Reset norm operation
	_delay_ms(100);
	PORTB |= (1 <<PORTB6);
	_delay_ms(1000);

	//TODO: Should we clear the buffer and update the free value?
	buff_trans_rx.head = buff_trans_rx.tail;	// Ignoring reset message
	
	PORTB |= (1 << PORTB7);		// PWR, norm operation
	PORTD &= ~(1 << PORTD5);	// Command mode
	
	
	uint8_t hopTable[4]	= {0xFF,0x02,0x00,0x00};
	uint8_t TXPower[4]	= {0xFF,0x02,0x4D,0x01};		// Setting the power to 25dBm for initial search/ default value
	uint8_t UARTBaud[4]	= {0xFF,0x02,0x03,0x02};		// Setting BAUD rate to 19.2 kb/s ( also means this is the transmission speed)
	uint8_t addMode[4]	= {0xFF,0x02,0x4F,0x04};		// Setting address mode as DSN (Volatile address)
	uint8_t showVer[4]	= {0xFF,0x02,0x0A,0x00};		// Do not show version at start up
	uint8_t CMDHold[4]	= {0xFF,0x02,0x6E,0x00};		// If CMD_BAR is low, wait until it goes high to send to mcu
	uint8_t compat[4]	= {0xFF,0x02,0x70,0x00};		// Compatibility mode to enable broadcast function for DSN
	uint8_t autoAdd[4]	= {0xFF,0x02,0x71,0x04};		// Auto addressing for DNS mode
	uint8_t myDSN3[4]	= {0xFF,0x02,0xFE,0x34};				// Read command for preprogrammed DNS values, DSN3 is MSB
	uint8_t myDSN2[4]	= {0xFF,0x02,0xFE,0x35};
	uint8_t myDSN1[4]	= {0xFF,0x02,0xFE,0x36};
	uint8_t myDSN0[4]	= {0xFF,0x02,0xFE,0x37};
	uint8_t LStatus[5]	= {0xFF,0x03,0xFE,0xFE,0x46};	// Status of output lines
	
	// Send all defined commands through uart
	//TX0Write(hopTable[0],4);
	delay_ms(50);
	TXWrite(TXPower[0],4, Transceiver);
	delay_ms(50);
	getChar(0);
	//TX0Write(UARTBaud[0],4);
	
	
	TXWrite(addMode,4, Transceiver);
	delay_ms(50);
	getChar(0);
	
	//TX0Write(showVer[0],4);
	TXWrite(CMDHold,4,Transceiver);
	delay_ms(50);
	getChar(0);
	TXWrite(compat,4,Transceiver);
	delay_ms(50);
	getChar(0);
	//TX0Write(autoAdd[0],4);
	
	
	// Getting local DNS info (third char returned)
	uint8_t *myDSNVal = malloc(4*sizeof(uint8_t));
	
	TXWrite(myDSN3,4,Transceiver);
	_delay_ms(100);
	while((buff_trans_rx.size - buff_trans_rx.free) < 3);
	
	buff_trans_rx.head = buff_trans_rx.tail - 1;		// Ignoring other bytes received, only care about DSN value

	myDSNVal[0] = getChar(0);
	
	
	TXWrite(myDSN2,4,Transceiver);
	_delay_ms(100);
	while((buff_trans_rx.size - buff_trans_rx.free) < 3);
		
	buff_trans_rx.head = buff_trans_rx.tail - 1;
	myDSNVal[1] = getChar(0);
	
	
	TXWrite(myDSN1,4,Transceiver);
	_delay_ms(100);
	while((buff_trans_rx.size - buff_trans_rx.free) < 3);
	buff_trans_rx.head = buff_trans_rx.tail - 1;
	myDSNVal[2] = getChar(0);
	
	
	TXWrite(myDSN0,4,Transceiver);
	_delay_ms(100);
	while((buff_trans_rx.size - buff_trans_rx.free) < 3);
	buff_trans_rx.head = buff_trans_rx.tail - 1;
	myDSNVal[3] = getChar(0);
	
	_delay_ms(3000);
	
	TXWrite(myDSNVal,4,Transceiver);
	_delay_ms(1000);
	PORTD |= (1 << PORTD5);	// Data transmit mode
	// Resetting HUMPRO to have non volatile registers update
	//PORTC &= ~(1 << PORTC5);
	
	//PORTC |= (1 << PORTC5);
	
	return myDSNVal;
	
}

