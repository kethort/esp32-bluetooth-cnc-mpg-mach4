# Mach4 Bluetooth MPG Handwheel Using ESP32
Mach4 CNC MPG Handwheel using ESP32 via Bluetooth

<p align="center">
  <img src="/img/IMG-0914.jpg" width="500"/>
</p>

<p align="center">
  <img src="/img/IMG-0915.jpg" width="500"/>
</p>

## Demonstration
https://www.youtube.com/watch?v=MvadVEjZ93Q

## ESP32 Firmware and Network Setup
1. Download NodeMCU-PyFlasher.exe program from here: https://github.com/marcelstoer/nodemcu-pyflasher/releases/tag/v5.0.0
2. Connect a USB to FTDI or CP2102 to FTL programmer to the ESP32 PCB at the programmer port. 
3. Select the COM port of the programmer in the NodeMCU program.
4. Select the ESP32-Bluetooth-MPG.ino.bin or ESP32-Bluetooth-MPG-ILI9341-Display.ino.bin file from the fw-build folder and press the Flash NodeMCU button to flash the firmware to the ESP32.
5. Reboot the ESP32.

<p align="center">
  <img src="/img/fw-upload.PNG" width="500"/>
</p>

6. The ESP32 device should show up in the list of available Bluetooth devices in Windows. Connect to the ESP32 Bluetooth device.

<p align="center">
  <img src="/img/add_bluetooth_dev.png" height="400" width="500"/>
</p>

## Mach4 Software Setup
1. Launch the MPGInstaller.exe from the dist folder of the repo.
2. Select the Mach4 profile and press Install.

<p align="center">
  <img src="/img/installer.png" width="350"/>
</p>

3. To monitor the status of the device, turn on the ESP32 MPG, open Mach4 and navigate to Wizard->Select Wizard->MPGWizBluetooth in the File Menu.
   
<p align="center">
  <img src="/img/wizard.PNG" width="350"/>
</p>
 
## Wiring
MPG PCB: <br>
    ESP32 GND    -> COM <br>
    ESP32 Pin 2  -> Encoder A+ <br>
    ESP32 Pin 4  -> Encoder B+ <br>
    ESP32 Pin 13 -> X Axis <br>
    ESP32 Pin 14 -> Y Axis <br>
    ESP32 Pin 27 -> Z Axis <br>
    ESP32 Pin 25 -> MPG Enable Switch <br>
    ESP32 Pin 32 -> A Axis <br>
    ESP32 Pin 35 -> B Axis (8k ohm pullup to 3.3v) <br>
    ESP32 Pin 34 -> C Axis (8k ohm pullup to 3.3v) <br>
    ESP32 Pin 15 -> Inc 1 <br>
    ESP32 Pin 21 -> Inc 2 <br>
    ESP32 Pin 22 -> Inc 3 <br> <br>
