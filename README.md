# CH32_433Mhz_Library

A highly robust, noise-filtering Arduino communication library designed specifically for cheap **433MHz superregenerative RF modules**. 

Unlike standard libraries that fail when flooded with ambient RF static or require hardware interrupts, **CH32_433Mhz_Library** implements an **Asynchronous Time-Slice Density Modulation** algorithm. It sifts high-density pulse clusters right out of background noise floors dynamically, performing with the stability of a wired connection.

---

## 🚀 Features

* **Noise Immunity:** Statistical density filtering completely eliminates random static pops and background interference.
* **Interrupt-Free:** Built entirely using linear, non-blocking timing routines—no hardware timer conflicts.
* **Anti-Freeze Guard:** Internal 50ms timeouts ensure code never locks up when a signal gets blocked or masked.
* **Built-in Alignment Protection:** Integrated preamble signature matching strips away bit-shifting errors automatically.
* **Ultra-Lightweight:** Low flash memory footprint, ideal for memory-constrained MCUs like the **CH32V003**.

---

## 🛠️ Installation

### Method 1: Arduino Library Manager (Recommended)
1. In the Arduino IDE, open **Tools** -> **Manage Libraries...**
2. Search for `CH32_433Mhz_Library`.
3. Click **Install**.

### Method 2: Manual Installation
1. Download this repository as a `.zip` file.
2. Inside your Arduino IDE, go to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
3. Select the downloaded file.

---

## 📐 Connection Diagram

Connect your 433MHz modules to any digital input/output pins on your microcontroller.

```text
  [Microcontroller Unit]            [433MHz Receiver / Transmitter]
  ┌──────────────────┐               ┌────────────────────────┐
  │         INPUT PIN│◄──────────────┤DATA PIN       (Receiver)│
  │        OUTPUT PIN├──────────────►│DATA PIN    (Transmitter)│
  │               GND├──────────────┤GND                     │
  │               VCC├──────────────┤VCC                     │
  └──────────────────┘               └────────────────────────┘
```
*Note: For the best range, solder a 17.3cm straight wire to the "ANT" pad on both RF modules.*

---

## 📜 License

This library is open-source and released under the **MIT License**. Feel free to use, modify, and distribute it for both commercial and personal projects.
