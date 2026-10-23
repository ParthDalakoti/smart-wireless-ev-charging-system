# Smart Wireless EV Charging System with RFID Authentication & Multi-Sensor Safety Cutoff

[![Microcontroller](https://img.shields.io/badge/MCU-Dual_ESP32-E7352C.svg?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Simulation](https://img.shields.io/badge/Simulation-Wokwi_Verified-4CAF50.svg?style=for-the-badge)](https://wokwi.com/)
[![Protocol](https://img.shields.io/badge/Protocols-SPI_%7C_I2C_%7C_Bluetooth-blue.svg?style=for-the-badge)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

> **Major Engineering Project | Graphic Era Hill University (B.Tech ECE 2023–2027)**  
> **Author:** Parth Dalakoti (*President, Robotics & Automation Club, GEHU*)  
> **Contact:** `parthdalakoti482@gmail.com` | [LinkedIn](https://linkedin.com/in/parthdalakoti)

---

## ⚡ Executive Engineering Summary

This project implements an intelligent, safety-interlocked **Inductive Wireless Electric Vehicle (EV) Charging Station** engineered using a **Dual ESP32 Master-Vehicle Architecture**:
1. **Station Controller (ESP32 #1):** Governs user RFID authentication (RC522 via SPI), automatic motorized servo barrier gate access, high-power primary transmitter coil relay switching, and ambient gas/flame hazard monitoring.
2. **Vehicle Controller (ESP32 #2):** Monitors receiver coil rectification, battery terminal voltage via resistive divider scaling, cell temperature via analog thermistor, and transmits real-time telemetry over Bluetooth Serial to the user dashboard.
3. **Interactive Simulation:** Full virtual hardware prototype tested and verified on **Wokwi Simulation**.

---

## 📸 Media & Hardware Architecture

| System Schematic & Circuit | Hardware Components |
| :---: | :---: |
| ![Circuit](hardware/schematics/system_wiring_diagram.png) | ![Components](hardware/components/ESP32.jpg) |

---

## ⚙️ System Architecture & Data Flow

```mermaid
graph TD
    subgraph Station_Node [Station ESP32 Controller]
        RFID[MFRC522 RFID Reader - SPI] -->|UID Auth| ESP_St[Station ESP32]
        IR1[Entry IR Sensor] -->|Vehicle Detect| ESP_St
        IR2[Bay Position IR Sensor] -->|Alignment OK| ESP_St
        MQ2[MQ-2 Gas Sensor - ADC] -->|Hazard Check| ESP_St
        Flame[Flame Sensor - ADC] -->|Fire Check| ESP_St
        ESP_St -->|PWM| Gate[Barrier Gate Servo]
        ESP_St -->|Switch High Power| Relay[Tx Resonant Coil Relay]
        ESP_St -->|Telemetry| LCD1[16x2 I2C LCD]
        ESP_St -->|Alarm| Buzzer[Acoustic Buzzer]
    end

    subgraph Power_Transfer [Wireless Resonant Inductive Coupling]
        Relay -->|AC Drive| TxCoil[Primary Transmitter Coil]
        TxCoil -.->|Magnetic Flux Transfer| RxCoil[Secondary Receiver Coil]
    end

    subgraph Vehicle_Node [Vehicle ESP32 Controller]
        RxCoil --> Bridge[Bridge Rectifier + Buck Converter]
        Bridge --> Batt[18650 Li-Ion Battery Pack]
        Batt -->|Analog Voltage Sense| ESP_Veh[Vehicle ESP32]
        Therm[Temperature Sensor - LM35/NTC] -->|Thermal Safety| ESP_Veh
        ESP_Veh -->|Status| LCD2[Vehicle 16x2 I2C LCD]
        ESP_Veh -->|Wireless Telemetry| BT[Bluetooth Serial Dashboard]
    end
```

---

## 🛠️ Bill of Materials (BOM)

| Component | Specification / Model | Operating Voltage | Interface | Function |
| :--- | :--- | :--- | :--- | :--- |
| **Microcontroller (x2)** | ESP32-WROOM-32 (Dual Core 240MHz) | 3.3V / 5V | SPI, I2C, ADC, UART | Main logic, telemetry & safety |
| **RFID Reader** | MFRC522 (13.56 MHz RFID) | 3.3V | Hardware SPI | Driver/Vehicle ID authentication |
| **Wireless Power Module** | Inductive Resonant Coil Pair (5V-12V, 2A) | 12V DC Input | Electromagnetic Induction | Wireless energy transfer |
| **Combustible Gas Sensor** | MQ-2 Semiconductor Gas Sensor | 5V | Analog ADC (Pin 34) | Gas leakage / smoke cutoff |
| **Flame Sensor** | IR Phototransistor Module | 5V | Analog ADC (Pin 35) | Spark and fire emergency trip |
| **Display (x2)** | 16x2 Character LCD with PCF8574 Backpack | 5V | I2C (Address 0x27) | Real-time voltage & status telemetry |
| **Barrier Gate Actuator** | SG90 9g Micro Servo | 5V | PWM (50Hz) | Physical bay entry gate |
| **Power Switch** | 5V Optocoupled Relay Module | 5V Coil | GPIO (Pin 26) | Primary transmitter coil activation |
| **Battery Storage** | 2S 18650 Lithium-Ion Battery (7.4V - 8.4V) | 7.4V Nominal | DC Terminals | On-board EV energy storage |

---

## 🔌 Hardware Pinout & Wiring Connections

### Station ESP32 Pin Mapping
* **MFRC522 RFID:** `SS = GPIO 5`, `RST = GPIO 4`, `SCK = GPIO 18`, `MOSI = GPIO 23`, `MISO = GPIO 19`
* **Sensors:** `IR1 = GPIO 14`, `IR2 = GPIO 27`, `MQ-2 = GPIO 34`, `Flame = GPIO 35`
* **Actuators:** `Relay = GPIO 26`, `Buzzer = GPIO 25`, `Servo = GPIO 13`
* **Display:** `I2C SDA = GPIO 21`, `I2C SCL = GPIO 22`

### Vehicle ESP32 Pin Mapping
* **Voltage Sensing:** `Divider 1 = GPIO 34`, `Divider 2 = GPIO 35`
* **Thermal Sensor:** `Temp = GPIO 33`
* **Lighting & Alerts:** `Front Light = GPIO 4`, `Back Light = GPIO 2`, `Buzzer = GPIO 25`
* **Display:** `I2C SDA = GPIO 21`, `I2C SCL = GPIO 22`

---

## 💻 Running the Simulation (Wokwi)

1. Open [Wokwi Simulator](https://wokwi.com/).
2. Load [`simulation/Station_ESP32/diagram.json`](simulation/Station_ESP32/diagram.json) and [`simulation/Station_ESP32/sketch.ino`](simulation/Station_ESP32/sketch.ino).
3. Click **Start Simulation**.
4. Swipe the virtual RFID card or press the IR sensor buttons to simulate vehicle arrival and trigger automatic inductive coil charging.

---

## 📑 Detailed Engineering Documentation
* Complete project report available at [`docs/EV_Wireless_Charging_Project_Report.pdf`](docs/EV_Wireless_Charging_Project_Report.pdf)
* Detailed hardware component guide at [`docs/EV_Component_Reference_Guide.pdf`](docs/EV_Component_Reference_Guide.pdf)

---

## 👨‍💻 Engineering Author
**Parth Dalakoti**  
*B.Tech in Electronics & Communication Engineering (2023–2027)*  
*Graphic Era Hill University (GEHU), Bhimtal Campus*  
*President, Robotics & Automation Club, GEHU*
