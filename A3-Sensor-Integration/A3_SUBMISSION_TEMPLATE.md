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
| **Student Name** | [Your full name] |
| **Student ID** | [Your student/enrollment ID] |
| **Assessment** | A3 – Vibration Monitoring |
| **Submission Date** | [Date submitted to Blackboard] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [Paste your GitHub portfolio URL] |
| **Assessment Folder** | `/A3-Vibration-Monitoring/` |
| **Code Location** | `/A3-Vibration-Monitoring/code/esp32-arduino/` |
| **Last Commit Date** | [Date of final commit] |

### Work Completed

**Brief Description:**  
Describe your vibration monitoring system: which axes you measured, what filtering you applied, and what vibration thresholds trigger alerts.

[Your description here - 2-3 sentences]

---

## Assessment Evidence

### Code and Documentation

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| Arduino `.ino` file with GY-521 code | ☐ Included | `/A3-Vibration-Monitoring/code/esp32-arduino/` |
| X, Y, Z acceleration measurements | ☐ Working | Raw values logged to serial |
| Moving average filter implementation | ☐ Included | Code comments explain filter window |
| Threshold detection for anomalies | ☐ Included | Thresholds defined for X, Y, Z axes |
| CSV data file (60+ seconds) | ☐ Included | CSV in assessment folder |
| Assessment README.md | ☐ Included | `/A3-Vibration-Monitoring/README.md` |

### Hardware Evidence

| Requirement | Evidence | Provided |
|-------------|----------|----------|
| **Breadboard Photo** | Photo showing GY-521 (MPU6050) correctly wired to ESP32 | ☐ Yes |
| **Raw Data Sample** | CSV showing raw and filtered acceleration values | ☐ Yes |
| **Working System** | Screenshot of serial monitor showing sensor values and anomalies | ☐ Yes |

**Breadboard Photo/Screenshot:**  
[Describe what's shown in your evidence]

**Sample CSV Entry:**  
[Paste 2-3 rows from your CSV showing Timestamp, X_Raw, X_Filtered, Y_Raw, Y_Filtered, Z_Raw, Z_Filtered]

---

## Assessment Evidence Checklist

Confirm all requirements completed before submitting:

| Requirement | Completed |
|-------------|-----------|
| GY-521 sensor reads X, Y, Z acceleration | ☐ |
| Moving average filter reduces noise | ☐ |
| Filtered values are noticeably smoother than raw | ☐ |
| Threshold detection identifies anomalies | ☐ |
| CSV file contains 60+ seconds of data | ☐ |
| CSV includes both raw and filtered values | ☐ |
| Timestamp column in CSV | ☐ |
| Code is clean and commented | ☐ |
| GitHub repository is accessible | ☐ |
| Assessment README documents filter parameters and thresholds | ☐ |

---

## Optional Notes

[Add any additional context: filter window size used, vibration thresholds defined, how you triggered test vibrations, etc.]

---

## Submission Declaration

By submitting this form, I confirm that:

- ☐ All code in my A3 folder is my own work
- ☐ GY-521 accelerometer is correctly wired and functional
- ☐ Moving average filter is properly implemented
- ☐ Threshold detection logic works as designed
- ☐ Code follows ICTIOT502 assessment requirements
- ☐ I have not plagiarized or breached academic integrity

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
