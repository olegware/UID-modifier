# UID-modifier
This file can be used to modify the UID of any writable IC card in a simple 2 step process using an Arduino and an RFID reader module. It can be used in cases where authentication cards must be cloned, for example in home or office settings. 

I do **not** condone the use of this tool for illegal or destructive purposes and I am not responsible for its use in such cases. 
### The RFID module is connected to the Arduino Uno using the following connections: 
* VCC → 3.3V
* GND → GND
* SDA (SS) → Pin 10
* SCK → Pin 13
* MOSI → Pin 11
* MISO → Pin 12
* RST → Pin 9
  
Then install the MFRC522 library (used for parsing and dumping card memory including UID). This can be used independently to obtain the UID of a card you wish to clone.
### Using the modifier 
#### Step 1

After uploading changeUID.ino to your Arduino, make sure to alter the bytes in line 10 to match what you wish your new UID to be. Include 0x to indicate hexadecimal. 
E.g. If I wish to write the UID ` 77 8C 24 B7 `to my new card:

 ``` byte newUID[] = {0x..., 0x..., 0x..., 0x...}; // New UID (4 bytes)  ```

 Becomes 

``` byte newUID[] = {0x77, 0x8C, 0x24, 0xB7}; // New UID (4 bytes) ```

#### Step 2

Touch card that you wish to alter to RFID reader and read output in Serial Monitor. If the card scanned is a writable card, the UID will be altered. 

