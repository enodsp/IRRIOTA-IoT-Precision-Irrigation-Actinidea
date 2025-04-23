## Agricultural Monitoring System with ESP32 and Google Sheets

This project is a complete environmental monitoring system designed for agricultural applications. It features automatic sensor data collection, local processing via ESP32, and seamless data submission to Google Sheets using the Google Cloud API. It is ideal for supporting irrigation decision-making.

The system was **field-tested at a Kiwi producers' association** and is part of a **scientific article currently under review for publication**.  
The article includes:
- Detailed specifications of the **hardware used**, with direct links to purchase each component  
- The full **electronic circuit schematic**  
- The **installation process in actual production fields**

It also explores the **agronomic applications** of the system, with practical examples of how to interpret the data for **kiwi crop management**, supporting precision irrigation and soil moisture monitoring.

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

## Notes

- All logic for reading, sending, and resetting is **well-documented** and has been **field-tested in a professional kiwi production environment**, under the supervision of an expert agronomic team from a leading fruit company.
