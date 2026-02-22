⚠️ IMPORTANT LEGAL DISCLAIMER: This project is created STRICTLY FOR EDUCATIONAL PURPOSES to understand RFID security vulnerabilities. Only test on cards you own or have explicit written permission to test. Unauthorized cloning of access cards may violate laws and property regulations. The developers assume no liability for misuse.

📋 Project Overview
Advanced RFID Cloner Pro is a sophisticated, portable device designed to demonstrate the security vulnerabilities in MIFARE Classic RFID cards commonly used in access control systems. This educational tool showcases how easily these cards can be read, analyzed, and cloned using readily available hardware.

🎯 Educational Objectives
Understand RFID communication protocols (13.56 MHz)

Learn about MIFARE Classic card memory structure

Explore cryptographic weaknesses in CRYPTO1 encryption

Demonstrate dictionary and nested authentication attacks

Practice secure hardware/software integration

Study access control system vulnerabilities

✨ Key Features
🖥️ Hardware Features
Standalone Operation: No computer required - fully portable

OLED Display: 128x64 I2C screen for intuitive menu navigation

Rotary Encoder: Easy menu browsing with built-in button for selection

LED Indicators: Multi-color status LEDs (Green/Yellow/Red)

Battery Powered: Li-ion rechargeable with charging circuit

Voltage Monitoring: Real-time battery level detection

Compact Design: All components fit in a custom 3D-printed case

📱 Software Capabilities
Card Management
Multi-Card Storage: Save up to 8 complete card dumps in EEPROM

Non-Volatile Memory: Preserves data even when powered off

Card Information Display: Shows UID, SAK, ATQA, and card type

Timestamp Tracking: Records when each card was saved

Reading Operations
Full Memory Dump: Reads all 64 blocks (1KB) of MIFARE Classic cards

Sector-by-Sector Reading: Handles authentication per sector

Dictionary Attack: Tests 7+ default manufacturer keys

Partial Read Recovery: Continues even if some sectors fail

Real-time Progress: Visual feedback during reading

Writing Operations
Complete Card Cloning: Writes all data blocks to target cards

Magic Card Support: Detects and utilizes UID-writable cards

Selective Writing: Option to write only specific sectors

Verification: Optional read-back verification of written data

Advanced Security Analysis
Key Extraction Engine: Attempts to recover unknown keys

Nested Attack Framework: Exploits authentication protocol weaknesses

Mfkey32 Placeholder: Framework for advanced nonce capture

Key Storage: Saves discovered keys per sector (Key A and Key B)

Success Statistics: Shows number of keys found per card

User Interface
Intuitive Menu System: 6 main menu options with submenus

Battery Status Icon: Visual battery level indicator

Slot Navigation: Easy browsing through stored cards

Confirmation Dialogs: Prevents accidental operations

Settings Menu: Clear cards, view about info, system reset

🔧 Hardware Requirements
Essential Components
Component	Specification	Quantity
Microcontroller	Arduino Nano/Pro Mini (ATmega328P)	1
RFID Module	MFRC522 (13.56MHz)	1
Display	OLED 128x64 I2C (SSD1306)	1
Rotary Encoder	With push button	1
LEDs	5mm (Green, Yellow, Red)	3 each
Resistors	330Ω for LEDs	3
Battery	Li-ion 18650 (3.7V, 2000mAh+)	1
Charging Module	TP4056 with protection	1
Boost Converter	MT3608 (3.7V → 5V)	1
PCB	Prototyping board or custom	1
Enclosure	3D printed case	1
Optional Components
Voltage divider (10kΩ + 10kΩ) for battery monitoring

Power switch

USB-C breakout board for charging

Tactile switches (optional backup buttons)

🔌 Wiring Diagram

MFRC522 → Arduino
  SDA   → D10
  SCK   → D13
  MOSI  → D11
  MISO  → D12
  RST   → D9
  3.3V  → 3.3V
  GND   → GND

OLED I2C → Arduino
  SDA    → A4
  SCL    → A5
  VCC    → 5V
  GND    → GND

Encoder → Arduino
  CLK    → D2 (interrupt)
  DT     → D3 (interrupt)
  SW     → D4
  +      → 5V
  GND    → GND

LEDs → Arduino (via 330Ω resistors)
  Green  → D5
  Yellow → D6
  Red    → D7

Power System
  Battery+ → TP4056 BAT+ → Boost IN+ → Arduino VIN
  Battery- → TP4056 BAT- → Boost IN- → Arduino GND
  Boost OUT (5V) → Arduino VIN (if no USB)
  Battery+ → Voltage divider → A0

  📖 How It Works
RFID Technology Basics
The device operates at 13.56 MHz, the frequency used by MIFARE Classic cards. These cards contain 1KB of memory divided into 16 sectors, each protected by two 6-byte keys (Key A and Key B). The security relies on the proprietary CRYPTO1 encryption algorithm, which has known vulnerabilities.

Cloning Process
Authentication: The device attempts to authenticate with each sector using known default keys

Reading: Successfully authenticated sectors are read block by block

Storage: Complete card data (UID, keys, data blocks) is saved to EEPROM

Writing: Data is transferred to a blank card using similar authentication

Key Extraction Methods
Dictionary Attack: Tests common manufacturer default keys

Nested Attack: Uses one known key to recover others through protocol analysis

Mfkey32: Advanced method capturing cryptographic nonces during communication

🚀 Getting Started
Assembly Steps
Solder components to PCB according to wiring diagram

Install Arduino IDE and required libraries

Upload the firmware to Arduino

Assemble in 3D-printed case

Connect battery and test

First Use
Power on device (splash screen appears)

Navigate menu with rotary encoder

Select "Read Card" to capture your first card

Follow on-screen prompts

Save to available slot

Test "Write Card" with blank media

📊 Menu Structure
text
MAIN MENU
├── Read Card
│   ├── Place card on reader
│   ├── Reading progress
│   └── Save to slot selection
├── Write Card
│   ├── Select source slot
│   ├── Place blank card
│   └── Writing progress
├── View Cards
│   ├── Browse slots 0-7
│   ├── View UID and details
│   └── Key statistics
├── Extract Keys
│   ├── Select card slot
│   ├── Place original card
│   └── Key search progress
├── Settings
│   ├── Clear All Cards
│   ├── About Information
│   └── Back to Main
└── Battery Status
    ├── Percentage display
    └── Visual bar graph
🔬 Technical Specifications
Card Compatibility
Supported Cards: MIFARE Classic 1K, 4K

Frequency: 13.56 MHz

Memory: Full 1KB dump capability

UID Length: 4-byte or 7-byte UID support

Device Specifications
Power Supply: 3.7V Li-ion battery

Current Draw: ~150mA active, ~50mA idle

Battery Life: 10-15 hours continuous use

Storage: 8 card slots in 1KB EEPROM

Display: 128x64 OLED, 0.96 inch

Dimensions: Customizable via 3D printing

⚠️ Security Considerations
Vulnerabilities Demonstrated
Default manufacturer keys still in use

Weak CRYPTO1 encryption

No mutual authentication requirement

Predictable random numbers in protocol

Backdoors in certain card models

Protection Recommendations
Upgrade to MIFARE DESFire or similar

Use diversified keys per card

Implement mutual authentication

Regular security audits

Multi-factor access control

📚 Educational Resources
Understanding RFID Security
RFID Basics: Electromagnetic coupling, modulation, protocols

MIFARE Architecture: Memory organization, sector structure

CRYPTO1 Cryptanalysis: Known attacks and weaknesses

Protocol Analysis: Authentication flow, nonce generation

Supply Chain Risks: Manufacturer backdoors and magic cards

Project Learning Outcomes
Embedded systems programming

Hardware-software integration

Cryptographic protocol analysis

User interface design

Power management

Data persistence strategies

🔧 Troubleshooting Guide
Common Issues
Problem	Likely Cause	Solution
No display	I2C address wrong	Check OLED address (0x3C or 0x3D)
Card not detected	Wiring issue	Verify SPI connections
Read fails	Unknown keys	Try key extraction first
Write fails	Card locked	Card may be read-only
Battery not charging	TP4056 fault	Check protection circuit
🛠️ Development Environment
Required Libraries
MFRC522 by Miguel Balboa

Adafruit SSD1306

Adafruit GFX

Wire (built-in)

SPI (built-in)

EEPROM (built-in)

Arduino IDE Setup
Install Arduino IDE (1.8.19+)

Add board support for Arduino Nano

Install required libraries via Library Manager

Select correct port and board

Compile and upload

📈 Future Enhancements
Planned Features
Bluetooth connectivity for data export

SD card logging for extended storage

PC software for detailed analysis

Support for more card types (MIFARE Plus, DESFire)

Waveform capture for deep protocol analysis

Mobile app integration

Cloud backup of card dumps

Advanced cryptanalysis tools

📜 License
This project is licensed under the MIT License - see LICENSE file for details. Educational use only.

🤝 Contributing
Contributions for educational purposes are welcome! Please:

Fork the repository

Create a feature branch

Submit a pull request

Include educational documentation

⚖️ Legal Notice
IMPORTANT: This tool is for security research and education only. Users must:

Only test on their own property

Obtain written permission before testing any access control system

Comply with all local, state, and federal laws

Not use for unauthorized access or circumvention of security measures

Accept full responsibility for any misuse

The creators and contributors are not liable for any illegal or unethical use of this information or device.

🙏 Acknowledgments
NXP Semiconductors for MIFARE documentation

Arduino community for libraries and examples

Security researchers who disclosed RFID vulnerabilities

Open source contributors for MFRC522 library

📞 Support
For educational questions and responsible disclosure of vulnerabilities:

Open an issue on GitHub

Join the discussion in Discussions tab

Read the Wiki for detailed tutorials
