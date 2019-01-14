#include <SoftwareSerial.h>

#define M0      7
#define M1      8
#define AUX     12
#define SOFT_RX 10
#define SOFT_TX 11
SoftwareSerial softSerial(SOFT_RX, SOFT_TX);  // RX, TX

// #define SAVE_MODE 0xC0  // To flash
#define SAVE_MODE 0xC2  // To memory

#define UART_MODE 0b00   // 8N1
#define UART_RATE 0b011  // 9600
#define AIR_RATE  0b010  // 5k
#define SPED ((UART_MODE << 6) | (UART_RATE << 3) | (AIR_RATE << 0))

#define TRANS_MODE 0      // Transparent mode
#define IO_MODE    1      // Push-pull outputs, pull-up inputs
#define WAKEUP     0b111  // 2000ms
#define FEC        1      // Enable FEC
// #define TX_POW     0b11   // 21dBm
#define TX_POW     0b00   // 30dBm
#define OPTION ((TRANS_MODE << 7) | (IO_MODE << 6) | (WAKEUP << 3) | (FEC << 2) | (TX_POW << 0))

// -------------------------------------------------------------------
void enter_mode_normal(void) {
    digitalWrite(M0, LOW);
    digitalWrite(M1, LOW);
}

void enter_mode_setup(void) {
    digitalWrite(M0, HIGH);
    digitalWrite(M1, HIGH);
}

// -------------------------------------------------------------------
void send_command_transceiver(const char * cfg, int len) {

    while (true) {
        delay(2);
        if (digitalRead(AUX) == HIGH) break;
    }
    delay(10);

    const char * p = &cfg[0];
    while (p < &cfg[len]) {
        softSerial.print(*p++);
        delay(1);
    }
}

// -------------------------------------------------------------------
void restore_config_transceiver(void) {
    uint8_t cfg[] = {SAVE_MODE, 0x00, 0x00, SPED, 0x17, OPTION};
    send_command_transceiver((char *)cfg, sizeof(cfg));
}

void reset_transceiver(void) { send_command_transceiver("\xC4\xC4\xC4", 3); }

void review_transceiver(void) { send_command_transceiver("\xC1\xC1\xC1", 3); }

void show_version_transceiver(void) { send_command_transceiver("\xC3\xC3\xC3", 3); }

// -------------------------------------------------------------------
void setup_e32(void) {
    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(AUX, INPUT);      // INPUT_PULLUP
    pinMode(SOFT_RX, INPUT);  // INPUT_PULLUP
    pinMode(SOFT_TX, OUTPUT);

    Serial.begin(9600);
    softSerial.begin(9600);

    // Reset transceiver
    enter_mode_setup();
    delay(100);

    restore_config_transceiver();
    show_version_transceiver();
    reset_transceiver();
    delay(500);
    review_transceiver();

    enter_mode_normal();
    delay(100);

    // Delay for returned information
    while (softSerial.available()) softSerial.read();

    Serial.println(">> Ready <<");
}
