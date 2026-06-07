# Assessment Submission Portfolio

**Assessment A3: Vibration Monitoring System**  
**Due:** Week 8 | **Weight:** 10%

---

## Version Control

| Field | Details |
|-------|---------|
| **Assessment Type** | Individual Portfolio Submission |
| **Assessment Code** | A3 |
| **Platform** | GitHub + Blackboard |
| **Document Version** | v1.0 |

---

## Introduction

This assessment submission form documents the completion of Assessment A3 (Vibration Monitoring). Your code and project work must be completed and committed to your GitHub portfolio repository in the `/A3-Vibration-Monitoring/` folder.

**Important:** This form is for submission evidence only. Your actual code stays on GitHub.

---

## Submission Instructions

### Assessment Overview

Implement a vibration monitoring system using an accelerometer for predictive maintenance:
- **GY-521 (MPU6050)** measuring X, Y, Z acceleration on 3 axes
- **Moving average filtering** to reduce sensor noise
- **Threshold detection** for abnormal vibration patterns
- **CSV data file** with 60 seconds of filtered readings

### How to Complete This Assessment

1. Complete accelerometer code in `/A3-Vibration-Monitoring/code/esp32-arduino/`
2. Implement moving average filter for noise reduction
3. Set vibration thresholds and log anomalies
4. Collect 60 seconds of data and export to CSV
5. Commit all files to GitHub
6. Fill out this form with your submission details
7. Copy completed form into Blackboard by the due date

### What to Submit on GitHub

- ✅ Arduino `.ino` file with GY-521 accelerometer code
- ✅ CSV data file with 60 seconds of X, Y, Z acceleration readings
- ✅ README.md explaining filtering and threshold logic
- ✅ Breadboard photo showing GY-521 wiring

---

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | [Tony Bui] |
| **Student ID** | [20115515] |
| **Assessment** | A3 – Vibration Monitoring |
| **Submission Date** | [7/6/26] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [https://github.com/tonyb-99/iot-diploma-portfolio.git] |
| **Assessment Folder** | `/A3-Vibration-Monitoring/` |
| **Code Location** | `/A3-Vibration-Monitoring/code/esp32-arduino/` |
| **Last Commit Date** | [7/6/26] |

### Work Completed

**Brief Description:**  
Describe your vibration monitoring system: which axes you measured, what filtering you applied, and what vibration thresholds trigger alerts.

The vibration monitoring system measures acceleration in XYZ axes. It uses to moving average filtering which estimates the current value based on a subset of samples prior to it, replacing old values with new values with each new measurement. While system monitors passively, it can also detect abrupt vibrations by setting at a high frequency (5 Hz) to measure fast moving vibrations and a LSB (least significant bit) of 10 which detects sudden movement. 

---

## Assessment Evidence

### Code and Documentation

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| Arduino `.ino` file with GY-521 code | ✅ Included | `/A3-Vibration-Monitoring/code/esp32-arduino/` |
| X, Y, Z acceleration measurements | ✅ Working | Raw values logged to serial |
| Moving average filter implementation | ✅ Included | Code comments explain filter window |
| Threshold detection for anomalies | ✅ Included | Thresholds defined for X, Y, Z axes |
| CSV data file (60+ seconds) | ✅ Included | CSV in assessment folder |
| Assessment README.md | ✅ Included | `/A3-Vibration-Monitoring/README.md` |

### Hardware Evidence

| Requirement | Evidence | Provided |
|-------------|----------|----------|
| **Breadboard Photo** | Photo showing GY-521 (MPU6050) correctly wired to ESP32 | ✅ Yes |
| **Raw Data Sample** | CSV showing raw and filtered acceleration values | ✅ Yes |
| **Working System** | Screenshot of serial monitor showing sensor values and anomalies | ✅ Yes |

**Breadboard Photo/Screenshot:**  
Micro SD module at 5V. Pins: CS = 5, SCK = 18, MOSI = 23, MISO = 19
MPU6050 at 3.3V. Pins: SCL = 22, SDA = 21


**Sample CSV Entry:**  
[Paste 2-3 rows from your CSV showing Timestamp, X_Raw, X_Filtered, Y_Raw, Y_Filtered, Z_Raw, Z_Filtered]
timestamp	 x_raw	 x_filtered	 y_raw	 y_filtered	 z_raw	 z_filtered	 IsAbnormal?
0:00:10	0.043	-0.01	0.036	0.011	9.814	9.803	0
0:00:11	1.805	0.365	1.025	0.225	9.663	9.78	0
0:00:11	6.768	1.713	0.97	0.415	7.791	9.375	1

---

## Assessment Evidence Checklist

Confirm all requirements completed before submitting:

| Requirement | Completed |
|-------------|-----------|
| GY-521 sensor reads X, Y, Z acceleration | ✅ |
| Moving average filter reduces noise | ✅ |
| Filtered values are noticeably smoother than raw | ✅ |
| Threshold detection identifies anomalies | ✅ |
| CSV file contains 60+ seconds of data | ✅ |
| CSV includes both raw and filtered values | ✅ |
| Timestamp column in CSV | ✅ |
| Code is clean and commented | ✅ |
| GitHub repository is accessible | ✅ |
| Assessment README documents filter parameters and thresholds | ✅ |

---

## Optional Notes

[Add any additional context: filter window size used, vibration thresholds defined, how you triggered test vibrations, etc.]
Contains an SD card module for ease of access to the CSV file.
Window size is 5.
Passive vibration set at 2g and 250 degree range as it is mostly idle, 260 Hz to disable its inbuilt filtering according to Adafruit_MPU6050.h specification.
Trigger test vibrations from idle is calibrate by sampling until it is within expected initial range in x,y,z with z = g. Interrupt is triggered when it is not idle.
---

## Submission Declaration

By submitting this form, I confirm that:

- ✅ All code in my A3 folder is my own work
- ✅ GY-521 accelerometer is correctly wired and functional
- ✅ Moving average filter is properly implemented
- ✅ Threshold detection logic works as designed
- ✅ Code follows ICTIOT502 assessment requirements
- ✅ I have not plagiarized or breached academic integrity

---

## For Assessor Use

| Field | Details |
|-------|---------|
| **Assessor Name** | [Assessor completes] |
| **Date Assessed** | [Assessor completes] |
| **Result** | ☐ Satisfactory ☐ Not Yet Satisfactory |
| **Feedback** | [Assessor completes] |

---

**Submission recorded by Blackboard:** [Auto-recorded]

**Your actual work is assessed on GitHub. This form provides proof of submission.**
