# Challenger NA Main yasuo
# Expected

It should be (i ask my nuts and say it will be able ):
* a simple osciloscope with 100kHz of max freq sample for every type of signals 
* at least 1MHz of clk signals (square with only 2 states)
* include a simple wave generator
* include the CMSIS DSP for fourier analyses and digital filtering

This is the frist prototype, the end can be able to be ported to a BLE MCU based (i got in the eye the STM32WB55xxx with my pcb design)


 # STM32 based Osciloscope
This is a design for a STM discovery board letting you to analyse how does AC/DC signals is.

## Features
* Up to 100kHz bandwidth with 0.73mV resolution in AC signals
* Up to 1MHz bandwidth for DC signals
* Adjustable zoom for amplitude and time

## How it works
Using a RTOS (freertos cmsis v1.0) and DMA technologies this osciloscope can plot with very high acurrency in voltages and time terms.
There is 3 principales routines
* ADC task: Controls the DMA for the ADC and manage the auto start for prevent data losses.
* DAC task: Controls the DAC signal generator for adjusting the frequency and signal type
* Graphichs task: Controls the plot window for the signal's data and react to the buttons for adjusting the DAC configuration
