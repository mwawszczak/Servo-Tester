# RC Servo Tester — STM32G030K8Tx

A hand-held tester for RC hobby servos, and my first solo PCB project. The goal was to go through the whole process — schematic, PCB layout, SMD soldering, and firmware — from scratch, using KiCad and a bare-metal STM32 build.

## What it does

The tester lets you drive a servo manually or run it through a few automatic routines, without needing a full RC receiver/transmitter setup.

- **Manual mode** — a potentiometer sets the servo position live
- **Auto mode** — cycles through center, min, max, and a continuous sweep between the limits
- A mode switch toggles between manual and auto, a button steps through the auto sub-modes, and a start/stop button arms or disarms the PWM output
- Two LEDs show the current mode and whether the output is armed, through different blink patterns

## Hardware

The board is built around an STM32G030K8Tx, powered from micro USB through an AMS1117 3.3V regulator. It reads the potentiometer on an ADC input, generates the servo PWM signal (50Hz, 1–2ms pulse) from a hardware timer, and has three buttons with on-board RC filtering for hardware debounce. Programming and debugging go through a standard 4-pin SWD header. The design was kept deliberately simple — no protection circuitry beyond the basics — to keep the focus on finishing a working first revision rather than maximizing features.

## Firmware

Bare-metal C on the STM32 HAL, no RTOS. The firmware is split into small, self-contained modules — a mode state machine, button handling, an LED blink engine, potentiometer reading, and servo PWM control — each exposing a small public interface and keeping its internal state private. The main loop polls the buttons, feeds events into the state machine, updates the servo output and LEDs based on the current mode, and repeats — all non-blocking, driven off the system tick.