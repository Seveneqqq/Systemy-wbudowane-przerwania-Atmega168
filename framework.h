#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>    
#include <avr/power.h>    
#include <avr/sleep.h>  
#include <avr/pgmspace.h>   
#define delay(time)	_delay_ms(time);
#define interruptOn() sei();
#define interruptOff() cli();
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

enum m{
	OUTPUT, 
	INPUT
};
enum s{
	HIGH, 
	LOW
};

static void pinMode(uint8_t pin, uint8_t m){
	if(m)
	{
		if(pin <= 7)
		{
			DDRD |= (1 << pin);
		}
		else if(pin <= 13)
		{
			DDRB |= (1 << (pin - 8));
		}
		else
		{
			DDRC |= (1 << (pin - 14));
		}
	}
	else
	{
		if(pin <= 7)
		{
			DDRD &= ~(1 << pin);
		}
		else if(pin <= 13)
		{
			DDRB &= ~(1 << (pin - 8));
		}
		else
		{
			DDRC &= ~(1 << (pin - 14));
		}
	}
}
static void digitalWrite(uint8_t pin, uint8_t s){ 
	if(s)
	{
		if(pin <= 7)
		{
			PORTD |= (1 << pin);
		}
		else if(pin <= 13)
		{
			PORTB |= (1 << (pin - 8));
		}
		else
		{
			PORTC |= (1 << (pin - 14));
		}
	}
	else
	{
		if(pin <= 7)
		{
			PORTD &= ~(1 << pin);
		}
		else if(pin <= 13)
		{
			PORTB &= ~(1 << (pin - 8));
		}
		else
		{
			PORTC &= ~(1 << (pin - 14));
		}
	}
}
static uint8_t digitalRead(uint8_t pin){
	if(pin <= 7)
	{
		return ((PIND & (1 << pin)) >> pin);
	}
	else if(pin <= 13)
	{
		return ((PIND & (1 << (pin - 8))) >> (pin - 8));
	}
	else
	{
		return ((PIND & (1 << (pin - 14))) >> (pin - 14));
	}
}

static void serialInit(uint16_t baud){
	// TODO:
	
	uint8_t _ubr = (8000000/16/baud-1);

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0H = (_ubr << 8);
	UBRR0L = _ubr;
}
static void serialWrite(uint16_t data){
 	while(!((UCSR0A)&(1 << UDRE0)));
	UDR0 = data;
}
static void serialPrintStr(char *text){
	// TODO:
	int i=0;
	while( *(text+i) != '\0')
	{
		serialWrite(text[i]);
		++i;
	}
}
static void serialPrintInt(uint16_t value){
	// TODO:
	char buffer[6];
	itoa(value,buffer,10);
	serialPrintStr(buffer);
	
	//char number[5] = {0, 0, 0, 0, 0};
    //int8_t idx = 0;
    //uint16_t pow = 10;
    //while(value > 0)
    //{
    //    number[idx] = (value % pow) + '0';
    //    value = value / pow;
    //    idx++;
    //}
    //for(idx--; idx >= 0; idx--)
    //{
    //    serialWrite(number[idx]);

}

//----------------------------------------------------------------

static void (*_int0_func)(); // Mechanizm wskaĹşnikĂłw funkcyjnych
static void (*_int1_func)(); // Mechanizm wskaĹşnikĂłw funkcyjnych

static void interruptInit(uint8_t pin, char conf, void (*f)()) {
    if (pin == 2) {
        _int0_func = f;
        if (conf == 'r') {
            EICRA |= (1 << ISC00);
        }
        else if (conf == 'f') {
            EICRA |= (1 << ISC01);
        }
        else if (conf == 'c') {
            EICRA |= (1 << ISC01) | (1 << ISC00);
        }
        EIMSK |= (1 << INT0);
    }
    else if (pin == 3) {
        _int1_func = f;
        if (conf == 'r') {
            EICRA |= (1 << ISC10);
        }
        else if (conf == 'f') {
            EICRA |= (1 << ISC11);
        }
        else if (conf == 'c') {
            EICRA |= (1 << ISC10) | (1 << ISC11);
		
        }
        EIMSK |= (1 << INT1);
    }
}
ISR(INT0_vect) {
    _int0_func();
}
ISR(INT1_vect) {
    _int1_func();    
}
