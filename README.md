## Agricultural Monitoring System with ESP32 and Google Sheets

This project is a complete environmental monitoring system designed for agricultural applications. It features automatic sensor data collection, local processing via ESP32, and seamless data submission to Google Sheets using the Google Cloud API. It is ideal for supporting irrigation decision-making.

The system was **field-tested at a Kiwi producers' association** and is part of a **scientific article currently under review for publication** titled _"Integrating Low-Cost IoT Sensors with Community-Accessible Online Tools for Irrigation Management: a Case Study in Actinidia Orchards"_.  

The article includes:
- Detailed specifications of the **hardware used**, with direct links to purchase each component  
- The full **electronic circuit schematic**  
- The **installation process in actual production fields**  
- A practical example of how to apply **agronomic knowledge** to manage irrigation using the data collected by the system

---

## Main Features

✅ Automatic readings from:
- **Air Temperature and Humidity** (DHT11)  
- **Soil Moisture Content** (4 Capacitive Sensors V2.0)  
- **Soil Water Tension** (Watermark Sensor with MUX and current inversion)  
- **Soil Temperature** (DS18B20 Sensor)  
- **Accumulated Rainfall** (Tipping Bucket Rain Gauge)

✅ Automatic data submission to **Google Sheets**:
- Every **30 minutes** (`00` and `30` past each hour)  
- Special submission at **23:59** to record the complete daily data  
- **Automatic rain gauge reset** after the 23:59 submission  
- Initial test submission on system startup

✅ **NTP-based time synchronization**  
✅ **Sensor reading validation**, with error detection and fallback values  
✅ **Calibrated algorithms** for soil sensors  
✅ **Modular and well-commented code**, optimized for robustness and clarity

---

## Google Sheets Structure

- **Column A**: Date and Time (automatically generated)  
- **Columns B–N**: Sensor readings (Temperature, Humidity, Rainfall, Soil Electrical Resistance, Soil Water Tension, Soil Moisture by depth and interpolation)

---

## Agronomic Context and Applications

The system was evaluated under the supervision of an expert agronomic team from a leading fruit company specialized in Kiwi production. All logic for reading, sending, and resetting is **well-documented** and was **field-tested in a professional production environment**.

The scientific article also explores the **agronomic applications** of the system, demonstrating how the collected data can be manipulated and interpreted for effective **kiwi crop management**, particularly for optimizing **precision irrigation** and monitoring **soil water availability**.

---

## Additional Applications

While the system was primarily focused on irrigation monitoring, its sensors and data structure also support broader applications:

- **Environmental monitoring**: Detect microclimate conditions that influence plant health, such as heat stress (e.g., identifying leaf scorch events)  
- **Pest and disease prediction**: Anticipate favorable conditions for fungal diseases and pest outbreaks  
- **Cross-crop compatibility**: Easily adaptable to other crops such as **vineyards, olive groves, and fruit orchards**, by adjusting installation depth and crop-specific thresholds


## Notes

- All logic for reading, sending, and resetting is **well-documented** and has been **field-tested in a professional kiwi production environment**, under the supervision of an expert agronomic team from a leading fruit company.
