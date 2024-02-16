# Challenger NA Main yasuo
# Expected

It should be (at least in my mind):
* a simple osciloscope with 100kHz of max freq sample for every type of signals 
* at least 1MHz of clk signals (square with only 2 states)
* include a simple wave generator
* include the CMSIS DSP for dd fourier analyses, and digital filtering

This is the frist prototype, the end can be able to be ported to a BLE MCU based (i got in the eye the STM32WB55xxx with my pcb design)
 # STM32 based Osciloscope

This is a design for a STM discovery board letting u to view in the screen how does AC/DC signals r.
## Features
* Up to 100kHz with very high precision in AC signals
* Up to 1MHz for DC changing signals
* Adjustable zoom for amplitude and time

## How it works

Using a RTOS (freertos cmsis v1.0) and DMA technologies this osciloscope can plot with very high acurrency in voltages and time terms.
There is 3 principales routines
* ADC task: Controls the DMA for the ADC and manage the auto start for prevent data losses.
* DAC task: Controls the DAC signal generator for adjusting the frequency and signal type
* Graphichs task: Controls the plot window for the signal's data and react to the buttons for adjusting the DAC configuration
