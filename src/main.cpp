#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define BTN_A  13  // WiFi button (cycle skimmer name)
#define BTN_B  32  // BLE button (toggle advertising)
#define LED    2   // built-in blue LED

// Skimmer names from VordS3's detection list
static const char* NAMES[] = {
    "HC-05",
    "HC-06",
    "HC-08",
    "BT05",
    "JDY-31",
    "HM-10",
    "CC41A",
    "SPP-CA",
};
static const int NAME_COUNT = sizeof(NAMES) / sizeof(NAMES[0]);

static int  currentName = 0;
static bool advertising = true;
static BLEServer* pServer = nullptr;
static BLEAdvertising* pAdv = nullptr;

static void startAdv() {
    BLEDevice::deinit(false);
    delay(100);
    BLEDevice::init(NAMES[currentName]);
    pServer = BLEDevice::createServer();
    pAdv = BLEDevice::getAdvertising();
    pAdv->setScanResponse(true);
    pAdv->setMinPreferred(0x06);
    pAdv->start();
    advertising = true;
    digitalWrite(LED, HIGH);
    Serial.printf("Advertising as: \"%s\"\n", NAMES[currentName]);
}

static void stopAdv() {
    if (pAdv) pAdv->stop();
    advertising = false;
    digitalWrite(LED, LOW);
    Serial.println("Advertising stopped");
}

void setup() {
    Serial.begin(115200);
    delay(500);

    pinMode(BTN_A, INPUT_PULLUP);
    pinMode(BTN_B, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    Serial.println("=== Skimmer BLE Simulator ===");
    Serial.println("D13 (WiFi button): cycle skimmer name");
    Serial.println("D32 (BLE button):  toggle advertising on/off");
    Serial.println();

    startAdv();
}

void loop() {
    // Button A — cycle to next skimmer name
    if (digitalRead(BTN_A) == LOW) {
        delay(50);
        if (digitalRead(BTN_A) == LOW) {
            currentName = (currentName + 1) % NAME_COUNT;
            if (advertising) {
                startAdv();
            } else {
                Serial.printf("Selected: \"%s\" (not advertising)\n", NAMES[currentName]);
            }
            while (digitalRead(BTN_A) == LOW) delay(10);
        }
    }

    // Button B — toggle advertising on/off
    if (digitalRead(BTN_B) == LOW) {
        delay(50);
        if (digitalRead(BTN_B) == LOW) {
            if (advertising) {
                stopAdv();
            } else {
                startAdv();
            }
            while (digitalRead(BTN_B) == LOW) delay(10);
        }
    }

    delay(20);
}
