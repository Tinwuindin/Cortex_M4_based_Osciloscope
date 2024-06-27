
In this project, an oscilloscope has been developed using the STM32F429 microcontroller. The device features a graphical user interface (GUI) based on TouchGFX and offers advanced capabilities such as signal generation and signal analysis through Fast Fourier Transform (FFT) implemented with CMSIS DSP.

## Main Features 
Sampling Frequency: Up to 1 MHz
Resolution: 12 bits
Signal Generation: Up to 100 kHz
Signal Analysis: FFT using CMSIS DSP

## Hardware Components
Microcontroller: STM32F429ZIT6
Display: ili9341 2.4" LCD TFT with TouchGFX support
Inputs/Outputs: BNC connectors for signal input and output

## Software Used
IDE: STM32CubeIDE
Libraries: CMSIS DSP, HAL (Hardware Abstraction Layer), TouchGFX

 
### Signal Acquisition
The system can acquire signals at a sampling frequency of up to 1 MHz with a resolution of 12 bits. The internal ADCs (Analog-to-Digital Converters) of the STM32F429 are used to capture the input signals.
