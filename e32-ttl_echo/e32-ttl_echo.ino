/**
 * This code is adopted from:
 * http://spacetinkerer.blogspot.com/2011/02/serial-port-echo-with-arduino.html.
 *
 * If you need a soft serial (UART) function, please follow
 * http://www.arduino.cc/en/Tutorial/SoftwareSerialExample.
 *
 * E32-TTL module interfacing --> a example for this work
 * https://github.com/tienfuc/E32-TTL.
 */


/*********************************
 * E32-TTL Transceiver Interface *
 *********************************/

#include <SoftwareSerial.h>

#define M0      7
#define M1      8
#define AUX     12

#define SOFT_RX 10
#define SOFT_TX 11
SoftwareSerial softSerial(SOFT_RX, SOFT_TX);  // RX, TX

#define MAX_TX_SIZE 58
//#define MAX_TX_SIZE 36
#include "Queue.h":
Queue<char> queue(MAX_TX_SIZE * 3);


// The setup function runs once when you press reset or power the board
void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  pinMode(SOFT_RX, INPUT);
  pinMode(SOFT_TX, OUTPUT);

  Serial.begin(9600);
  softSerial.begin(9600);

  // Mode 0 | normal operation
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}


// The loop function runs over and over again forever
void loop() {

  if (softSerial.available()) {
    while (softSerial.available())
      queue.push(softSerial.read());

    Serial.print("Queue: ");
    Serial.print(queue.count(), DEC);
    Serial.println(" bytes");
  }

  if (queue.count() > 0 && digitalRead(AUX)) {
    char c;
    static char s[MAX_TX_SIZE];
    static int cnt = 0;

    do {
      char c = queue.pop();  // The oldest
      s[cnt++] = c;

      if (cnt == MAX_TX_SIZE || c == '\n') {
        softSerial.write(s, cnt);

        Serial.write(s, cnt);  // Display
        if (c != '\n' && c != '\r')  // Add 'newline'
          Serial.println();
        Serial.print("Send: ");
        Serial.print(cnt);
        Serial.println(" bytes.");
        cnt = 0;
        break;
      }
    } while (queue.count() > 0);

  }
}

