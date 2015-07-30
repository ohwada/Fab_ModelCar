/*
 * Bluetooth Motor Controller
 * 2015-06-01 K.OHWADA 
 */

// Bluetooth
// Microchip RN-42 Bluetooth Module

// Motor driver
// TA7291P
// 0 0 Stop
// 1 0 ClockWise
// 0 1 CounterClockWise
// 1 1 Break

#include <AltSoftSerial.h>
AltSoftSerial altSerial;

#define SERIAL_SPEED  9600 
#define BT_SPEED  115200

// Pin
// Serial RX  0
// Serial TX  1
#define P_BT_RST  2
#define P_M01    5 
#define P_M02    6
// AltSoftSerial RX  8
// AltSoftSerial TX  9
#define P_M11    10 
#define P_M12    11
#define P_LED    13

int cnt = 0;
boolean isRun = false;
boolean isLed = false;
boolean isBlink = true;

void setup() {
    Serial.begin( SERIAL_SPEED );
    altSerial.begin( BT_SPEED );
    // LED
    pinMode(P_LED, OUTPUT);
    // motor
    pinMode(P_M01, OUTPUT);
    pinMode(P_M02, OUTPUT);
    pinMode(P_M11, OUTPUT);
    pinMode(P_M12, OUTPUT);
    stop();
    // make the Bluetooth Module reset
    pinMode(P_BT_RST, OUTPUT);
    digitalWrite(P_BT_RST, LOW);
    delay(100);
    digitalWrite(P_BT_RST, HIGH);
    delay(500);
}

void loop() { 
    char c; 
    if( Serial.available() ) {
        c = Serial.read();
        command( c );
    }	
    if( altSerial.available() ) {
        c = altSerial.read();
        command( c );
    }	
    // LED
    cnt ++;
    if (cnt>10) {
        cnt = 0;
        if ( isBlink ) {
            digitalWrite(P_LED, isLed); 
            isLed = !isLed;
        }       
    } 
    delay(100);
}

void command( int c ) { 
    if ( c == '0' ) {
        isBlink = false;
        digitalWrite(P_LED, LOW); 
    } else if ( c == '1' ) {
        isBlink = false;
        digitalWrite(P_LED, HIGH); 
    } else if ( c == '2' ) {
        isBlink = true;
    } else if ( c == 'f' ) {
        // forward
        digitalWrite(P_M01, HIGH);
        digitalWrite(P_M02, LOW);
        digitalWrite(P_M11, HIGH);
        digitalWrite(P_M12, LOW);
    }  else if ( c == 'b' ) {
        // backward
        digitalWrite(P_M01, LOW);
        digitalWrite(P_M02, HIGH);
        digitalWrite(P_M11, LOW);
        digitalWrite(P_M12, HIGH);
    }  else if ( c == 'r' ) {
        // right turn
        digitalWrite(P_M01, HIGH);
        digitalWrite(P_M02, LOW);
        digitalWrite(P_M11, LOW);
        digitalWrite(P_M12, HIGH); 	
    } else if ( c == 'l' ) {
        // left turn
        digitalWrite(P_M01, LOW);
        digitalWrite(P_M02, HIGH);
        digitalWrite(P_M11, HIGH);
        digitalWrite(P_M12, LOW);  	
    } else {
        stop();
    } 
}

void stop() { 
    digitalWrite(P_M01, LOW);
    digitalWrite(P_M02, LOW);
    digitalWrite(P_M11, LOW);
    digitalWrite(P_M12, LOW);
}
