#define ENABLE_BASIC
#define ENABLE_SERIAL
#define ENABLE_INTERRUPT
#include "framework.h"

static void INT0Callback(){
	// TODO:	
		serialWrite('z');
}

static void INT1Callback(){
	// TODO:
	serialWrite('j');
	    
}

int main(void) {
    pinMode(13, OUTPUT);
	serialInit(9600);
	interruptOn();
	interruptInit(2, 'f', INT0Callback);
	interruptInit(3, 'c', INT1Callback);
	
    while (1) {		
		digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }

    return 0;
}
