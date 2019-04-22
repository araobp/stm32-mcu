/**
  @page BlueNRG Expansion Board for STM32 Nucleo Boards Beacon Demo Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt  
  * @author  MCD & CL/AST
  * @version V1.0.0
  * @date    20-May-2016
  * @brief   This application contains an example which shows how implementing
  *          an Eddystone Beacon device.
  *          The communication is done using a STM32 Nucleo board and a Smartphone
  *          with BLE.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

  
@par Example Description 

This example application shows how to use the BlueNRG Bluetooth Low Energy (BLE)
expansion board to implement an Eddystone Beacon device.

An Eddystone Beacon is a smart Bluetooth Low Energy device that transmits 
a small data payload at regular intervals using Bluetooth advertising packets.

Beacons are used to mark important places and objects. Typically, a beacon 
is visible to a user's device from a range of a few meters, allowing for highly 
context-sensitive use cases. 

Eddystone is an open beacon format from Google that works with Android and iOS. 
Specifications can be found at https://developers.google.com/beacons/

Two different kinds of devices can be selected by defining/undefining the 
#define EDDYSTONE_BEACON_TYPE EDDYSTONE_URL_BEACON_TYPE in file app_x-cube-ble1.c:
- UID: a UID beacon broadcasts a unique ID that provides proximity and general 
  location information.
- URL: a URL beacon broadcasts a packet containing an URL code usable by compatible 
  applications.

To locate the beacon, it is necessary to have a scanner application running
on a BLE-capable smartphone, such as one of the following ones for Android:
- Physical Web, https://play.google.com/store/apps/details?id=physical_web.org.physicalweb
- iBeacon & Eddystone Scanner, https://play.google.com/store/apps/details?id=de.flurp.beaconscanner.app
- Beacon Radar, https://play.google.com/store/apps/details?id=net.beaconradar
An alternative is to use a ‘Physical Web’ compatible browser like Google Chrome (version >=44)

NO SUPPORT WILL BE PROVIDED TO YOU BY STMICROELECTRONICS FOR ANY OF THE
ANDROID .apk FILES INCLUDED IN OR REFERENCED BY THIS PACKAGE.

  
@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with BlueNRG STM32 expansion board
    (X-NUCLEO-IDB04A1 or X-NUCLEO-IDB05A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board

  
@par How to use it? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open IAR toolchain (this firmware has been successfully tested with
   Embedded Workbench V8.30.1).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.26.0) or the System Workbench for
   STM32 (this firmware has been successfully tested with Version 2.7.2).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
