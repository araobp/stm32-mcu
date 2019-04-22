/**
  @page BlueNRG Expansion Board for STM32 Nucleo Boards Sensor Demo Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt  
  * @author  CL/AST
  * @version V0.0.1
  * @date    28-May-2014
  * @brief   This application contains an example which shows how implementing
  *          a proprietary Bluetooth Low Energy profile: the sensor profile.
  *          The communication is done using a STM32 Nucleo board and a Smartphone
  *          with BTLE.
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

Main function to show how to use the BlueNRG Bluetooth Low Energy 
expansion board to send data from an STM32 Nucleo board to a smartphone with
the support BLE and the "BlueNRG" app freely available on both GooglePlay and iTunes.
The URL to the iTunes for BlueNRG is http://itunes.apple.com/app/bluenrg/id705873549?uo=5
The URL to the GooglePlay is https://play.google.com/store/apps/details?id=com.st.blunrg
The source code of the "BlueNRG" app, both for iOS and Android, is freely downloadable 
from the developer website at http://software.g-maps.it/

NO SUPPORT WILL BE PROVIDED TO YOU BY STMICROELECTRONICS FOR ANY OF THE
ANDROID .apk FILES INCLUDED IN OR REFERENCED BY THIS PACKAGE.

The board will act as Server-Peripheral.
After establishing the connection:
- by pressing the USER button on the board, the cube showed by
  the app on the smartphone in the MOTION tab will rotate on the
  three axes (x,y,z).
- in the ENVIRONMENT tab of the app the temperature, pressure
  and humidity emulated values sent by the STM32 Nucleo to the smartphone are shown.
- in the OTHER tab of the app the RSSI value is shown.
For testing this sample application, a smartphone (acting as Central device) with the
"BlueNRG" app running is needed.

The communication is done using a vendor specific profile.

  
@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with BlueNRG STM32 expansion board
    (X-NUCLEO-IDB04A1 or X-NUCLEO-IDB05A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board
    and can be easily tailored to any other supported device and development board.
    This example runs also on the NUCLEO-F411RE RevC board, even if the chip could
    be not exploited at its best since the projects are configured for the
    NUCLEO-F401RE target board.

  
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
