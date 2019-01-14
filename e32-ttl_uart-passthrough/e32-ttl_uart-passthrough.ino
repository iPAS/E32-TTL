/**
 * E32-TTL UART Passthrough
 *
 * @author Pasakorn Tiwatthanont, AKA. iPAS.
 * @date 19 May 2017
 *
 * This code is adopted from:
 * @see http://spacetinkerer.blogspot.com/2011/02/serial-port-echo-with-arduino.html, and
 * @see https://github.com/tienfuc/E32-TTL.
 *
 * If you need a soft serial (UART) function, please follow
 * @see http://www.arduino.cc/en/Tutorial/SoftwareSerialExample.
 */

#define MAX_TX_SIZE 58

#include "e32.h"

// -------------------------------------------------------------------
// The setup function runs once when you press reset or power the board
void setup() {
  setup_e32();
}

// -------------------------------------------------------------------
// The loop function runs over and over again forever
void loop() {

  /**
   * Coming from normal UART
   */
  if (Serial.available() && digitalRead(AUX)) {
    char buf[MAX_TX_SIZE];
    int cnt = 0;

    do {
      buf[cnt++] = Serial.read();
    } while (Serial.available() && cnt < MAX_TX_SIZE);

    softSerial.write(buf, cnt);
  }

  /**
   * Coming from software UART
   */
  if (softSerial.available()) {
    char buf[MAX_TX_SIZE];
    int cnt = 0;

    do {
      buf[cnt++] = softSerial.read();
    } while (softSerial.available() && cnt < MAX_TX_SIZE);

    Serial.write(buf, cnt);
  }

}
