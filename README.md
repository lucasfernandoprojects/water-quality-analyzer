# ESP32 Water Quality Analyzer

An open-source water quality analyzer built with an ESP32 that estimates the pollution level of water samples by combining three different sensors. Results are displayed in real time on the Arduino IDE Serial Monitor.

> **This project was kindly sponsored by [Elecrow](https://www.elecrow.com/referral-program/ODI4MDlqMnQ/) - a leading provider of open-source hardware and manufacturing services. They offer everything from ESP32 development boards to professional PCB assembly, CNC machining, and more. Check them out for your next project!**

<div style="display: flex; flex-wrap: wrap;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-1.jpg" width="400" height="250" style="margin: 10px;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-2.jpg" width="400" height="250" style="margin: 10px;">
</div>
</br>
---

## 📺 Watch the Full Build Video

A complete walkthrough of this project, from wiring to real-world testing on urban rivers, is available on [YouTube](https://www.youtube.com/watch?v=ioxa7Pzs5dA). The video covers component selection, sensor calibration, code upload, and field results.

---

## 🧰 Components

| Component | Notes |
|---|---|
| ESP32 | Main microcontroller |
| pH Sensor | Analog output, requires voltage divider |
| Turbidity Sensor | Analog output |
| TDS Sensor | Analog output |
| Step-up Converter | Regulated 5V supply for the system |
| 21700 Li-ion Battery | Power source |
| 47kΩ & 75kΩ Resistors | Voltage divider for pH sensor |
| Breadboard + Jumper Wires | Prototyping |

**Estimated cost (as of June 2026):** ~$50 USD for components, excluding shipping and import fees.

### Calibration Solutions Required

- pH buffer solutions: pH 4, pH 7, and pH 10
- Conductivity solution: 1413 µS/cm

**Estimated cost:** at least ~$30 USD, excluding shipping and taxes.

<div style="display: flex; flex-wrap: wrap;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-3.jpg" width="400" height="250" style="margin: 10px;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-4.jpg" width="400" height="250" style="margin: 10px;">
</div>
</br>

---

## 🔌 Wiring

| Sensor | GPIO Pin |
|---|---|
| pH Sensor (via voltage divider) | GPIO 34 |
| TDS Sensor | GPIO 35 |
| Turbidity Sensor | GPIO 32 |

### Voltage Divider (pH Sensor)

The pH sensor used in this project can output signals up to 5V, which exceeds the safe input voltage for the ESP32. A voltage divider built with a 47kΩ and a 75kΩ resistor reduces the signal to approximately 3V.

- Signal wire → 47kΩ resistor → GPIO 34
- 75kΩ resistor connected between signal line and GND

> If your pH sensor already outputs a voltage within the safe range for the ESP32, you can connect it directly without a voltage divider.

A full wiring schematic is available in the `schematic/` folder of this repository.

---

## ⚙️ Calibration

### pH Sensor

The pH sensor outputs an analog voltage, not a pH value directly. Calibration was performed using buffer solutions at pH 4, 7, and 10, which produced voltages ranging from approximately 1.2V down to 0.9V (voltage decreases as pH increases).

The relationship is linear, described by:

```
pH = m * V + b
```

Where:
- `V` = measured voltage
- `m` = slope (calculated: **-23.6**)
- `b` = offset (calculated: **32.2**)

Update these values in the code if your sensor produces different calibration readings.

### TDS Sensor

The TDS sensor outputs a voltage proportional to water conductivity. Using a 1413 µS/cm conductivity reference solution, a conversion factor was derived to translate sensor voltage into conductivity (µS/cm) and then into TDS (ppm).

<div style="display: flex; flex-wrap: wrap;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-5.jpg" width="400" height="250" style="margin: 10px;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-6.jpg" width="400" height="250" style="margin: 10px;">
</div>
</br>

---

## 💻 Code

The complete Arduino sketch is available in this repository. To get started:

1. Download or clone this repository
2. Open the sketch in the Arduino IDE
3. Install the required libraries (listed in the sketch)
4. Update the calibration constants if needed
5. Select your ESP32 board and port, then click **Upload**
6. Open the Serial Monitor to see live readings

---

## 🧪 How It Works

The analyzer combines three measurements to classify water quality:

- **pH**: Detects acidity or alkalinity. Natural rivers typically fall between pH 6.5 and 8.5. Values outside this range may indicate contamination.
- **TDS (Total Dissolved Solids)**: Estimates the concentration of dissolved minerals and salts.
- **Turbidity**: Measures water clarity. Higher turbidity suggests suspended particles.

Based on these readings, the system outputs a pollution classification (e.g., `CLEAN`, `POLLUTED`) to the Serial Monitor.

---

## 🔬 Field Results

The analyzer was tested on water samples collected from two urban rivers in a large city that don't cross each other.

| Parameter | River 1 | River 2 |
|---|---|---|
| pH | ~12.7 | ~13 |
| Conductivity | ~175 µS/cm | ~175 µS/cm |
| Turbidity | Low | Higher (~0.8V) |
| Classification | POLLUTED | POLLUTED |

Both samples showed highly alkaline pH values (above 12), which is far outside the range of natural water. While hobby-grade sensors have inherent limitations and laboratory testing would be needed to confirm exact values, these results - combined with visual observations such as visible waste, suspicious discharge pipes, and an absence of wildlife - are consistent with significant water pollution.

<div style="display: flex; flex-wrap: wrap;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-7.jpg" width="400" height="250" style="margin: 10px;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-8.jpg" width="400" height="250" style="margin: 10px;">
    <img src="https://github.com/lucasfernandoprojects/water-quality-analyzer/blob/main/8-photos/photo-9.jpg" width="800" height="600" style="margin: 10px;">
</div>
</br>

---

## 🛠️ Custom Shield PCB

After a failed attempt at a fully portable enclosure, a custom **shield PCB** was designed to allow all three sensors to connect directly to the ESP32 without a breadboard or jumper wires.

The PCB design files (Gerber files, position files, and BOM) are included in the `pcb/` folder of this repository and are fully open-sourced for anyone to manufacture or modify.

---

## 📁 Repository Structure

```
water-quality-analyzer/
├── 1-gerber/                         # Gerber files for PCB manufacturing
├── 2-drill/                          # Drill files for PCB manufacturing
├── 3-position/                       # Component position files
├── 4-components/                     # Component/BOM files
├── 5-model/                          # PCB 3D model files
├── 6-schematic/                      # Wiring schematic
├── 7-code/                           # Coding files
├── 8-photos/                         # Images of the project
├── LICENSE
├── README.md
```

---

## ⚠️ Limitations & Disclaimer

- Hobby-grade sensors provide estimated readings, not laboratory-grade measurements.
- Calibration accuracy directly affects result quality.
- Single-location, single-time-point samples are insufficient for definitive pollution assessments.
- Always validate results with certified laboratory equipment for any official or regulatory purpose.

---

## 📄 License

This project is open-source. Feel free to use, modify, and share it. If you build something based on this work, a credit or link back to this repository is always appreciated.

---

## 💬 Feedback & Contributions

Questions, suggestions, and pull requests are welcome! If you'd like to see a fully portable version of this analyzer with a touchscreen enclosure, let me know (it's on the roadmap).
