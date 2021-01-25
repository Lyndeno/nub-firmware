#include <avr/io.h>

// TODO: Might need this
//#include <avr/pgmspace.h>

// TODO: Figure this out
//#define F_CPU 1000000

int main (void) {

}

void gpio_init (void) {
    // Disable power reduction for USARTS
    PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);
}