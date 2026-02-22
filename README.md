🔐 Advanced RFID Cloner Pro
<div align="center">
https://img.shields.io/badge/version-2.0-blue
https://img.shields.io/badge/status-educational%2520only-orange
https://img.shields.io/badge/Arduino-IDE-00979D
https://img.shields.io/badge/license-MIT-green

A sophisticated portable device for understanding RFID security vulnerabilities in MIFARE Classic cards

Features • Hardware • Quick Start • Documentation

</div>
⚠️ CRITICAL LEGAL NOTICE
This project is STRICTLY FOR EDUCATIONAL PURPOSES only.

Only test on cards you personally own

Obtain written permission before testing any access control system

Unauthorized cloning may violate laws and property regulations

The developers assume NO liability for misuse

✨ Features
Core Capabilities
Full Card Dumping – Read all 64 blocks (1KB) from MIFARE Classic cards

Multi-Card Storage – Save up to 8 complete card dumps in non-volatile EEPROM

Dictionary Attacks – Tests 7+ default manufacturer keys automatically

Key Extraction – Advanced nested attack framework for unknown keys

Magic Card Support – UID writing capability for special writable cards

Hardware Highlights
Standalone Operation – No computer needed, fully portable

OLED Display – 128x64 I2C screen with intuitive menu system

Rotary Encoder – Easy navigation with push-button selection

LED Indicators – Visual status (Green/Yellow/Red)

Battery Powered – Li-ion rechargeable with monitoring

User Interface
6 Main Menu Options with submenus

Real-time Progress indicators during operations

Battery Status with visual bar graph

Card Details – View UID, type, key statistics

🔧 Hardware Requirements
Essential Components
Component	Specs	Purpose
Arduino	Nano/Pro Mini (ATmega328P)	Main controller
RFID Module	MFRC522 (13.56MHz)	Read/write cards
OLED Display	128x64 I2C (SSD1306)	User interface
Rotary Encoder	With push button	Menu navigation
Battery	Li-ion 18650 (3.7V)	Portable power
TP4056 Module	With protection	Battery charging
MT3608	Boost converter	3.7V → 5V conversion
Quick Wiring Reference
text
MFRC522 → D10(DTA), D13(SCK), D11(MOSI), D12(MISO), D9(RST)
OLED    → A4(SDA), A5(SCL)
Encoder → D2(CLK), D3(DT), D4(SW)
LEDs    → D5(Green), D6(Yellow), D7(Red)
Battery → A0 (via voltage divider)
🚀 Quick Start
1️⃣ Build Hardware
Assemble components per wiring diagram

Double-check 3.3V for MFRC522 (never 5V!)

Test power delivery before connecting RFID

2️⃣ Install Software
bash
# Required Libraries (install via Arduino Library Manager)
- MFRC522 by Miguel Balboa
- Adafruit SSD1306
- Adafruit GFX
3️⃣ First Use
Power on device

Navigate with encoder to "Read Card"

Place your card on reader

Save to slot when prompted

Place blank card and select "Write Card"

📱 Menu System
text
MAIN MENU
├── Read Card      → Dump card to memory
├── Write Card     → Clone to blank card
├── View Cards     → Browse saved dumps
├── Extract Keys   → Advanced key recovery
├── Settings       → Clear data, About
└── Battery Status → Power level
📊 Technical Specs
Parameter	Value
Frequency	13.56 MHz
Card Support	MIFARE Classic 1K/4K
Storage	8 cards (1KB EEPROM)
Power	3.7V Li-ion
Battery Life	10-15 hours
Display	128x64 OLED
🔍 How It Works
RFID Card Structure
16 sectors × 4 blocks = 64 blocks total

Each block = 16 bytes (1KB total memory)

Sector trailer blocks contain access keys

Block 0 contains factory-locked UID

Attack Methods
Dictionary – Tests common default keys

Nested – Uses known keys to extract others

Mfkey32 – Framework for nonce capture attacks

⚠️ Known Limitations
UID Cloning – Requires special "magic cards" for full UID copy

Encrypted Cards – Won't work without keys

Locked Cards – Some cards have irreversible write protection

Newer Cards – MIFARE DESFire not supported

🛠️ Troubleshooting
Issue	Solution
No display	Check I2C address (0x3C or 0x3D)
Card not detected	Verify SPI connections
Read fails	Try key extraction first
Write fails	Card may be read-only
Battery not charging	Check TP4056 connections
📚 Learning Resources
Understanding RFID Security
How electromagnetic coupling works

MIFARE memory organization

CRYPTO1 encryption weaknesses

Authentication protocol flow

Manufacturer backdoors

Project Learning Outcomes
Embedded systems programming

Hardware-software integration

Cryptographic analysis

Power management design

Data persistence strategies

🤝 Contributing
Educational contributions only:

Fork repository

Create feature branch

Submit pull request

Include documentation

📜 License
MIT License – Educational use only

<div align="center">
⚠️ Remember: This is for learning security, not bypassing it ⚠️

Report Bug • Request Feature • Documentation

</div>
