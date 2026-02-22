ADVANCED RFID CLONER PRO 
================================================================================
⚠️ LEGAL WARNING - MUST READ
================================================================================
THIS PROJECT IS FOR EDUCATIONAL PURPOSES ONLY!

Only test on cards you personally own

Get written permission before testing any access system

Unauthorized cloning may violate laws

Developers assume NO liability for misuse

================================================================================
WHAT IS THIS?
================================================================================
A portable device that demonstrates security vulnerabilities in MIFARE Classic RFID cards (13.56MHz) by reading, analyzing, and cloning them.

================================================================================
KEY FEATURES
================================================================================

Full card dumping - Reads all 64 blocks from MIFARE Classic cards

Multi-card storage - Save up to 8 complete card dumps in memory

Key extraction - Dictionary attacks + nested attack framework

Standalone operation - No computer needed, battery powered

OLED menu system - 128x64 display with rotary encoder navigation

LED indicators - Visual status (Green/Yellow/Red)

Magic card support - UID writing for special writable cards

================================================================================
HARDWARE REQUIREMENTS
================================================================================

Arduino Nano/Pro Mini (ATmega328P)

MFRC522 RFID module (13.56MHz)

OLED 128x64 I2C display (SSD1306)

Rotary encoder with push button

Li-ion 18650 battery (3.7V)

TP4056 charging module with protection

MT3608 boost converter (3.7V to 5V)

LEDs: Green, Yellow, Red + 330Ω resistors

Voltage divider: 2x 10K resistors for battery monitoring

Prototyping board and wires

================================================================================
WIRING CONNECTIONS
================================================================================
MFRC522 → Arduino:
SDA → D10
SCK → D13
MOSI → D11
MISO → D12
RST → D9
3.3V → 3.3V
GND → GND

OLED → Arduino:
SDA → A4
SCL → A5
VCC → 5V
GND → GND

Encoder → Arduino:
CLK → D2
DT → D3
SW → D4

text
→ 5V
GND → GND

LEDs → Arduino (with 330Ω resistors):
Green → D5
Yellow → D6
Red → D7

Battery → Arduino:
Battery+ → A0 through voltage divider (10K+10K)
Battery- → GND

================================================================================
MENU SYSTEM
================================================================================
MAIN MENU
├── Read Card → Dump card to memory
├── Write Card → Clone to blank card
├── View Cards → Browse saved dumps (slots 0-7)
├── Extract Keys → Advanced key recovery
├── Settings → Clear all cards, About info
└── Battery Status → Show power level

================================================================================
QUICK START GUIDE
================================================================================

Build hardware according to wiring diagram

Install required libraries in Arduino IDE:

MFRC522 by Miguel Balboa

Adafruit SSD1306

Adafruit GFX

Upload code to Arduino

Power on device

Navigate with encoder to "Read Card"

Place your card on reader

Save to a slot when prompted

Place blank card and select "Write Card"

================================================================================
TECHNICAL SPECIFICATIONS
================================================================================
Frequency: 13.56 MHz
Card support: MIFARE Classic 1K and 4K
Storage: 8 complete cards in EEPROM
Power: 3.7V Li-ion battery
Battery life: 10-15 hours continuous use
Display: 128x64 OLED, 0.96 inch

================================================================================
HOW IT WORKS
================================================================================
MIFARE Classic cards have 1KB memory divided into 16 sectors (4 blocks each).
Each sector is protected by two 6-byte keys (Key A and Key B).

The device attempts authentication using default manufacturer keys. If successful,
it reads all data blocks and stores them along with any discovered keys.

For writing, it authenticates to the blank card using default or recovered keys
and writes the saved data blocks.

================================================================================
KNOWN LIMITATIONS
================================================================================

UID cloning requires special "magic cards" (genuine cards have locked UIDs)

Won't work with custom keys not in dictionary

Cannot clone MIFARE DESFire or other encrypted cards

Some cards have irreversible write protection

================================================================================
TROUBLESHOOTING
================================================================================
No display: Check I2C address (try 0x3C or 0x3D)
Card not detected: Verify SPI connections, ensure 3.3V to MFRC522
Read fails: Try key extraction feature first
Write fails: Card may be read-only or authentication failed
Battery not charging: Check TP4056 connections

================================================================================
REQUIRED LIBRARIES
================================================================================

MFRC522 by Miguel Balboa

Adafruit SSD1306

Adafruit GFX

Wire (built-in)

SPI (built-in)

EEPROM (built-in)

================================================================================
LICENSE
================================================================================
MIT License - Educational use only

================================================================================
REMEMBER
================================================================================
This tool demonstrates why physical security needs multiple layers.
Use this knowledge to improve security, not bypass it.
