/**
 * E32-TTL Transceiver Interface
 *
 * @author Pasakorn Tiwatthanont, AKA. iPAS.
 * @date 8 May 2017
 *
 * This code is adopted from:
 * @see http://spacetinkerer.blogspot.com/2011/02/serial-port-echo-with-arduino.html, and
 * @see https://github.com/tienfuc/E32-TTL.
 *
 * If you need a soft serial (UART) function, please follow
 * @see http://www.arduino.cc/en/Tutorial/SoftwareSerialExample.
 */

#define MAX_TX_SIZE 58
//#define MAX_TX_SIZE 36
#include "Queue.h"
Queue<char> queue(MAX_TX_SIZE * 3);

#include "e32.h"

// -------------------------------------------------------------------
// The setup function runs once when you press reset or power the board
void setup() {
  setup_e32(); 
}

// -------------------------------------------------------------------
// The loop function runs over and over again forever
void loop() {

  if (softSerial.available()) {
    while (softSerial.available()) {
      char c = softSerial.read();
      queue.push(c);
      Serial.print(c, HEX);
      Serial.print(' ');      
    }

    Serial.print("> ");
    Serial.println(queue.count(), DEC);
  }

  if (queue.count() > 0 && digitalRead(AUX)) {
    char c;
    static char s[MAX_TX_SIZE];
    static int cnt = 0;
    
    do {
      char c = queue.pop();  // The oldest
      s[cnt++] = c;

      if (cnt >= MAX_TX_SIZE || c == '\n') {
        //softSerial.write(s, cnt);
        int i;
        for (i = 0; i < cnt; i++) {
          delayMicroseconds(1);
          softSerial.write(s[i]);
        }

        Serial.write(s, cnt);  // Display
        if (c != '\n' && c != '\r')  // Add 'newline'
          Serial.println();
        Serial.print("Send ");
        Serial.print(cnt);
        Serial.println(" bytes.");
        Serial.println();
        
        cnt = 0;
        break;
      }
    } while (queue.count() > 0);

  }
}
