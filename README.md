# Mach4 Bluetooth MPG Handwheel Using ESP32
Mach4 CNC MPG Handwheel using ESP32 via Bluetooth

## Demonstration
https://www.youtube.com/watch?v=MvadVEjZ93Q

## Required Arduino Libraries
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/jakern/ESP32-BLE-Combo

## ESP32 Firmware and Network Setup
1. Upload the ESP32-Bluetooth-MPG.ino file to the ESP32 using the Arduino (or other) IDE.
2. Reboot the ESP32.
3. The ESP32 device should show up in the list of available Bluetooth devices in Windows. Connect to the ESP32 Bluetooth device.

<p align="center">
  <img src="/img/add_bluetooth_dev.png" height="300" width="500"/>
</p>

## Mach4 Software Setup
1. Launch the MPGInstaller.exe from the dist folder of the repo.
2. Select the Mach4 profile and press Install.

<p align="center">
  <img src="/img/installer.png" width="350"/>
</p>

4. To monitor the status of the device, turn on the ESP32 MPG, open Mach4 and navigate to Wizard->Select Wizard->MPGWizBluetooth in the File Menu.
   
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
