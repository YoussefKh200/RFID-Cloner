/**
 * ADVANCED RFID CLONER PRO
 * Complete working code for MIFARE Classic card cloning
 * FOR EDUCATIONAL USE ONLY - Test only on your own cards with permission
 * 
 * Hardware: Arduino Nano + MFRC522 + OLED 128x64 + Rotary Encoder
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// Pin Definitions
#define SS_PIN 10
#define RST_PIN 9
#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4
#define LED_GREEN 5
#define LED_YELLOW 6
#define LED_RED 7
#define BATTERY_PIN A0

// Display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// EEPROM Layout
#define EEPROM_MAGIC 0xAA
#define EEPROM_MAGIC_ADDR 0
#define EEPROM_CARD_COUNT_ADDR 1
#define EEPROM_CARDS_START 10

// Card storage structure
struct CardData {
  bool valid;
  byte uid[7];
  byte uidSize;
  byte sak;
  byte atqa[2];
  byte data[64][16];
  byte keys[16][2][6];
  bool keyFound[16][2];
  unsigned long timestamp;
};

// Global variables
CardData cards[8];
int currentCardSlot = 0;
int menuPosition = 0;
int lastEncoderPos = 0;
bool encoderButtonPressed = false;
unsigned long lastButtonPress = 0;
const int debounceDelay = 50;
volatile int encoderDelta = 0;

// Menu
const char* mainMenu[] = {"Read Card", "Write Card", "View Cards", 
                          "Extract Keys", "Settings", "Battery Status"};
const int menuItems = 6;

// Known default keys
MFRC522::MIFARE_Key knownKeys[] = {
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5},
    {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5},
    {0x4D, 0x3A, 0x99, 0xC3, 0x51, 0xDD},
    {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F},
    {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7},
    {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
};
const int knownKeysCount = 7;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Splash screen
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("RFID");
  display.setCursor(10, 40);
  display.println("CLONER");
  display.display();
  delay(2000);
  
  SPI.begin();
  mfrc522.PCD_Init();
  
  loadCardsFromEEPROM();
  
  attachInterrupt(digitalPinToInterrupt(ENC_CLK), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_DT), handleEncoder, CHANGE);
  
  displayMenu();
}

// ========== MAIN LOOP ==========
void loop() {
  if(digitalRead(ENC_SW) == LOW && !encoderButtonPressed) {
    delay(debounceDelay);
    if(digitalRead(ENC_SW) == LOW) {
      encoderButtonPressed = true;
      handleMenuSelect();
    }
  }
  if(digitalRead(ENC_SW) == HIGH && encoderButtonPressed) {
    encoderButtonPressed = false;
  }
  
  displayMenu();
  delay(50);
}

// ========== ENCODER INTERRUPT ==========
void handleEncoder() {
  static uint8_t lastState = 0;
  uint8_t state = (digitalRead(ENC_CLK) << 1) | digitalRead(ENC_DT);
  if(state != lastState) {
    if(state == 0b01 && lastState == 0b00) encoderDelta--;
    if(state == 0b10 && lastState == 0b00) encoderDelta++;
    lastState = state;
  }
}

// ========== MENU HANDLING ==========
void displayMenu() {
  if(encoderDelta != 0) {
    menuPosition += encoderDelta;
    if(menuPosition < 0) menuPosition = menuItems - 1;
    if(menuPosition >= menuItems) menuPosition = 0;
    encoderDelta = 0;
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("RFID CLONER PRO");
    display.println("================");
    
    for(int i = 0; i < menuItems; i++) {
      if(i == menuPosition) {
        display.print("> ");
      } else {
        display.print("  ");
      }
      display.println(mainMenu[i]);
    }
    display.display();
  }
}

void handleMenuSelect() {
  digitalWrite(LED_YELLOW, HIGH);
  
  switch(menuPosition) {
    case 0: readCardToSlot(); break;
    case 1: writeCardFromSlot(); break;
    case 2: viewSavedCards(); break;
    case 3: extractKeysAdvanced(); break;
    case 4: showSettings(); break;
    case 5: showBatteryStatus(); break;
  }
  
  digitalWrite(LED_YELLOW, LOW);
  displayMenu();
}

// ========== READ CARD ==========
void readCardToSlot() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Place card on");
  display.println("reader...");
  display.display();
  
  unsigned long timeout = millis() + 10000;
  while(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    if(millis() > timeout) {
      display.println("Timeout!");
      display.display();
      delay(2000);
      return;
    }
    delay(50);
  }
  
  CardData newCard;
  newCard.valid = true;
  newCard.uidSize = mfrc522.uid.size;
  memcpy(newCard.uid, mfrc522.uid.uidByte, mfrc522.uid.size);
  newCard.sak = mfrc522.uid.sak;
  newCard.timestamp = millis();
  
  for(int s = 0; s < 16; s++) {
    newCard.keyFound[s][0] = false;
    newCard.keyFound[s][1] = false;
  }
  
  display.clearDisplay();
  display.print("UID: ");
  for(byte i = 0; i < newCard.uidSize; i++) {
    if(newCard.uid[i] < 0x10) display.print("0");
    display.print(newCard.uid[i], HEX);
  }
  display.println();
  display.println("Reading...");
  display.display();
  
  bool anyDataRead = false;
  for(byte sector = 0; sector < 16; sector++) {
    for(byte block = sector * 4; block < sector * 4 + 4; block++) {
      if(block % 4 == 3) continue;
      
      bool blockRead = false;
      
      for(int k = 0; k < knownKeysCount && !blockRead; k++) {
        MFRC522::MIFARE_Key key;
        memcpy(key.keyByte, knownKeys[k].keyByte, 6);
        
        if(mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
                                     block, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
          byte buffer[18];
          byte byteCount = 18;
          if(mfrc522.MIFARE_Read(block, buffer, &byteCount) == MFRC522::STATUS_OK) {
            memcpy(newCard.data[block], buffer, 16);
            blockRead = true;
            anyDataRead = true;
            memcpy(newCard.keys[sector][0], key.keyByte, 6);
            newCard.keyFound[sector][0] = true;
          }
        }
      }
      
      if(blockRead) {
        display.print(".");
        display.display();
      }
    }
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  if(anyDataRead) {
    for(int i = 0; i < 8; i++) {
      if(!cards[i].valid) {
        cards[i] = newCard;
        currentCardSlot = i;
        saveCardsToEEPROM();
        
        display.clearDisplay();
        display.println("Card saved to");
        display.print("slot ");
        display.println(i);
        display.display();
        delay(2000);
        digitalWrite(LED_GREEN, HIGH);
        delay(500);
        digitalWrite(LED_GREEN, LOW);
        return;
      }
    }
    
    display.clearDisplay();
    display.println("No free slots!");
    display.display();
    delay(2000);
  } else {
    display.clearDisplay();
    display.println("Read failed!");
    display.display();
    digitalWrite(LED_RED, HIGH);
    delay(2000);
    digitalWrite(LED_RED, LOW);
  }
}

// ========== WRITE CARD ==========
void writeCardFromSlot() {
  display.clearDisplay();
  display.println("Select slot (0-7)");
  display.println("Rotate, press to select");
  display.display();
  
  int slot = 0;
  unsigned long timeout = millis() + 10000;
  bool selected = false;
  
  while(millis() < timeout && !selected) {
    if(encoderDelta != 0) {
      slot += encoderDelta;
      if(slot < 0) slot = 0;
      if(slot > 7) slot = 7;
      encoderDelta = 0;
      
      display.clearDisplay();
      display.print("Slot: ");
      display.println(slot);
      if(cards[slot].valid) {
        display.print("UID: ");
        for(byte i = 0; i < cards[slot].uidSize; i++) {
          if(cards[slot].uid[i] < 0x10) display.print("0");
          display.print(cards[slot].uid[i], HEX);
        }
      } else {
        display.println("Empty slot");
      }
      display.println("Press to select");
      display.display();
    }
    
    if(digitalRead(ENC_SW) == LOW) {
      selected = true;
      delay(debounceDelay);
    }
  }
  
  if(!selected || !cards[slot].valid) {
    return;
  }
  
  display.clearDisplay();
  display.println("Place BLANK card");
  display.println("on reader");
  display.display();
  
  timeout = millis() + 30000;
  while(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    if(millis() > timeout) {
      display.println("Timeout!");
      display.display();
      delay(1500);
      return;
    }
    delay(50);
  }
  
  display.println("Writing...");
  display.display();
  
  int blocksWritten = 0;
  for(byte sector = 0; sector < 16; sector++) {
    MFRC522::MIFARE_Key key;
    memcpy(key.keyByte, knownKeys[0].keyByte, 6);
    
    byte trailerBlock = sector * 4 + 3;
    
    if(mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
                                 trailerBlock, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
      
      for(byte block = sector * 4; block < sector * 4 + 3; block++) {
        if(mfrc522.MIFARE_Write(block, cards[slot].data[block], 16) == MFRC522::STATUS_OK) {
          blocksWritten++;
          display.print(".");
          display.display();
        }
        delay(50);
      }
    }
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  display.clearDisplay();
  display.print("Written ");
  display.print(blocksWritten);
  display.println("/48 blocks");
  display.display();
  
  digitalWrite(LED_GREEN, HIGH);
  delay(2000);
  digitalWrite(LED_GREEN, LOW);
}

// ========== VIEW CARDS ==========
void viewSavedCards() {
  int viewSlot = 0;
  bool viewing = true;
  
  while(viewing) {
    display.clearDisplay();
    display.println("Saved Cards");
    display.println("==========");
    
    display.print("Slot ");
    display.print(viewSlot);
    display.print("/7 ");
    
    if(cards[viewSlot].valid) {
      display.println("[Used]");
      display.print("UID: ");
      for(byte i = 0; i < cards[viewSlot].uidSize; i++) {
        if(cards[viewSlot].uid[i] < 0x10) display.print("0");
        display.print(cards[viewSlot].uid[i], HEX);
      }
      display.println();
      
      int keysFound = 0;
      for(int s = 0; s < 16; s++) {
        if(cards[viewSlot].keyFound[s][0]) keysFound++;
        if(cards[viewSlot].keyFound[s][1]) keysFound++;
      }
      display.print("Keys: ");
      display.print(keysFound);
      display.println("/32");
    } else {
      display.println("[Empty]");
    }
    
    display.println();
    display.println("Rotate: next");
    display.println("Press: exit");
    display.display();
    
    if(encoderDelta != 0) {
      viewSlot += encoderDelta;
      if(viewSlot < 0) viewSlot = 0;
      if(viewSlot > 7) viewSlot = 7;
      encoderDelta = 0;
    }
    
    if(digitalRead(ENC_SW) == LOW) {
      viewing = false;
      delay(debounceDelay);
    }
    
    delay(100);
  }
}

// ========== EXTRACT KEYS ==========
void extractKeysAdvanced() {
  display.clearDisplay();
  display.println("Select slot (0-7)");
  display.display();
  
  int slot = 0;
  unsigned long timeout = millis() + 10000;
  bool selected = false;
  
  while(millis() < timeout && !selected) {
    if(encoderDelta != 0) {
      slot += encoderDelta;
      if(slot < 0) slot = 0;
      if(slot > 7) slot = 7;
      encoderDelta = 0;
      
      display.clearDisplay();
      display.print("Slot: ");
      display.println(slot);
      if(cards[slot].valid) {
        display.print("UID: ");
        for(byte i = 0; i < cards[slot].uidSize; i++) {
          if(cards[slot].uid[i] < 0x10) display.print("0");
          display.print(cards[slot].uid[i], HEX);
        }
      } else {
        display.println("Empty slot");
      }
      display.println("Press to select");
      display.display();
    }
    
    if(digitalRead(ENC_SW) == LOW) {
      selected = true;
      delay(debounceDelay);
    }
  }
  
  if(!selected || !cards[slot].valid) {
    return;
  }
  
  display.clearDisplay();
  display.println("Place original");
  display.println("card on reader");
  display.display();
  
  timeout = millis() + 30000;
  bool cardPresent = false;
  
  while(millis() < timeout && !cardPresent) {
    if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      if(mfrc522.uid.size == cards[slot].uidSize) {
        bool match = true;
        for(byte i = 0; i < mfrc522.uid.size; i++) {
          if(mfrc522.uid.uidByte[i] != cards[slot].uid[i]) {
            match = false;
            break;
          }
        }
        cardPresent = match;
      }
      
      if(!cardPresent) {
        mfrc522.PICC_HaltA();
      }
    }
    delay(100);
  }
  
  if(!cardPresent) {
    display.println("Card not found");
    display.display();
    delay(1500);
    return;
  }
  
  display.println("Searching keys...");
  display.display();
  
  int keysFound = 0;
  for(int sector = 0; sector < 16; sector++) {
    if(!cards[slot].keyFound[sector][0] && !cards[slot].keyFound[sector][1]) {
      for(int k = 0; k < knownKeysCount; k++) {
        MFRC522::MIFARE_Key key;
        memcpy(key.keyByte, knownKeys[k].keyByte, 6);
        
        byte trailerBlock = sector * 4 + 3;
        
        if(mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
                                     trailerBlock, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
          memcpy(cards[slot].keys[sector][0], key.keyByte, 6);
          cards[slot].keyFound[sector][0] = true;
          keysFound++;
          display.print(".");
          display.display();
          break;
        }
      }
    }
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  display.clearDisplay();
  display.print("Found ");
  display.print(keysFound);
  display.println(" new keys");
  display.display();
  
  saveCardsToEEPROM();
  delay(2000);
}

// ========== SETTINGS ==========
void showSettings() {
  const char* settingsMenu[] = {"Clear All Cards", "About", "Back"};
  int settingPos = 0;
  bool inSettings = true;
  
  while(inSettings) {
    display.clearDisplay();
    display.println("Settings");
    display.println("========");
    
    for(int i = 0; i < 3; i++) {
      if(i == settingPos) display.print("> ");
      else display.print("  ");
      display.println(settingsMenu[i]);
    }
    display.display();
    
    if(encoderDelta != 0) {
      settingPos += encoderDelta;
      if(settingPos < 0) settingPos = 2;
      if(settingPos > 2) settingPos = 0;
      encoderDelta = 0;
    }
    
    if(digitalRead(ENC_SW) == LOW) {
      delay(debounceDelay);
      
      switch(settingPos) {
        case 0:
          display.clearDisplay();
          display.println("Clear ALL cards?");
          display.println("Press to confirm");
          display.display();
          
          unsigned long timeout = millis() + 5000;
          while(!digitalRead(ENC_SW) == LOW && millis() < timeout) {
            delay(50);
          }
          
          if(digitalRead(ENC_SW) == LOW) {
            for(int i = 0; i < 8; i++) {
              cards[i].valid = false;
            }
            saveCardsToEEPROM();
            display.println("Cleared!");
            display.display();
            delay(1500);
          }
          break;
          
        case 1:
          display.clearDisplay();
          display.println("RFID Cloner Pro");
          display.println("v2.0");
          display.println("Educational use");
          display.println("only");
          display.println();
          display.println("Press to exit");
          display.display();
          
          while(!digitalRead(ENC_SW) == LOW) {
            delay(50);
          }
          delay(debounceDelay);
          break;
          
        case 2:
          inSettings = false;
          break;
      }
    }
    
    delay(50);
  }
}

// ========== BATTERY STATUS ==========
int getBatteryLevel() {
  int raw = analogRead(BATTERY_PIN);
  float voltage = raw * (5.0 / 1023.0) * 2;
  int percent = map(voltage * 100, 300, 420, 0, 100);
  return constrain(percent, 0, 100);
}

void showBatteryStatus() {
  display.clearDisplay();
  display.println("Battery Status");
  display.println("==============");
  
  int level = getBatteryLevel();
  display.print("Level: ");
  display.print(level);
  display.println("%");
  
  display.drawRect(20, 40, 88, 12, SSD1306_WHITE);
  display.fillRect(22, 42, map(level, 0, 100, 0, 84), 8, SSD1306_WHITE);
  
  display.println();
  display.println("Press to exit");
  display.display();
  
  while(!digitalRead(ENC_SW) == LOW) {
    delay(50);
  }
  delay(debounceDelay);
}

// ========== EEPROM MANAGEMENT ==========
void saveCardsToEEPROM() {
  EEPROM.write(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
  
  byte validCount = 0;
  for(int i = 0; i < 8; i++) {
    if(cards[i].valid) validCount++;
  }
  EEPROM.write(EEPROM_CARD_COUNT_ADDR, validCount);
  
  int addr = EEPROM_CARDS_START;
  for(int i = 0; i < 8; i++) {
    if(cards[i].valid) {
      EEPROM.put(addr, cards[i]);
      addr += sizeof(CardData);
    }
  }
}

void loadCardsFromEEPROM() {
  for(int i = 0; i < 8; i++) {
    cards[i].valid = false;
  }
  
  if(EEPROM.read(EEPROM_MAGIC_ADDR) != EEPROM_MAGIC) {
    return;
  }
  
  byte validCount = EEPROM.read(EEPROM_CARD_COUNT_ADDR);
  if(validCount > 8) validCount = 8;
  
  int addr = EEPROM_CARDS_START;
  for(int i = 0; i < validCount; i++) {
    CardData card;
    EEPROM.get(addr, card);
    
    for(int j = 0; j < 8; j++) {
      if(!cards[j].valid) {
        cards[j] = card;
        break;
      }
    }
    
    addr += sizeof(CardData);
  }
}
