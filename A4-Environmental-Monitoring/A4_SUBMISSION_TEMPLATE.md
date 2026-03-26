# Assessment Submission Portfolio

**Assessment A4: Complete Haul Truck Device**  
**Due:** Week 9 | **Weight:** 15%

---

## Version Control

| Field | Details |
|-------|---------|
| **Assessment Type** | Individual Portfolio Submission |
| **Assessment Code** | A4 |
| **Platform** | GitHub + Blackboard |
| **Document Version** | v1.0 |

---

## Introduction

This assessment submission form documents the completion of Assessment A4 (Complete Haul Truck Device). This is the capstone hardware project integrating all sensors, actuators, and I²C peripherals from A1-A3.

**Important:** This form is for submission evidence only. Your actual code stays on GitHub.

---

## Submission Instructions

### Assessment Overview

Integrate all previous sensors and add actuators/displays to create a complete haul truck monitoring system:

**Integrated Components:**
- ✅ DHT11 (temperature/humidity) - from A1
- ✅ MQ-2 (gas sensor) - from A1
- ✅ Flame sensor - from A1
- ✅ RFID-RC522 + DS3231 RTC - from A2
- ✅ GY-521 accelerometer - from A3
- ✅ SG90 servo (dump bed control)
- ✅ OLED display (truck status)
- ✅ RGB LED (status indicator)
- ✅ Buzzer (alarms)

**Requirements:**
- Clean, modular code with functions
- Comprehensive Fritzing diagram or breadboard photo
- README.md documenting all components and pinouts
- 5-minute demonstration video

### How to Complete This Assessment

1. Integrate all A1-A3 sensors into `/A4-Haul-Truck-Integration/code/esp32-arduino/`
2. Add servo, OLED, LED, and buzzer control
3. Organize code with functions for each subsystem
4. Create/photograph complete circuit
5. Prepare 5-minute demo video showing all features
6. Commit all files to GitHub
7. Fill out this form with your submission details
8. Copy completed form into Blackboard by the due date

### What to Submit on GitHub

- ✅ Complete Arduino `.ino` file with all sensors and actuators
- ✅ Fritzing circuit diagram (PNG) or breadboard photo
- ✅ README.md with component list, pins, and system overview
- ✅ Demo video link (YouTube/OneDrive/Vimeo)

---

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | [Your full name] |
| **Student ID** | [Your student/enrollment ID] |
| **Assessment** | A4 – Complete Haul Truck Device |
| **Submission Date** | [Date submitted to Blackboard] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [Paste your GitHub portfolio URL] |
| **Assessment Folder** | `/A4-Haul-Truck-Integration/` |
| **Code Location** | `/A4-Haul-Truck-Integration/code/esp32-arduino/` |
| **Last Commit Date** | [Date of final commit] |

### Work Completed

**Brief Description:**  
Summarize your complete truck system: all integrated sensors, what the OLED displays, how alerts trigger the buzzer, and servo operation.

[Your description here - 3-4 sentences]

---

## Assessment Evidence

### Code and Documentation

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| Complete Arduino `.ino` file | ☐ Included | `/A4-Haul-Truck-Integration/code/esp32-arduino/` |
| All A1-A3 sensors integrated | ☐ Working | DHT11, MQ-2, flame, RFID, RTC, accelerometer |
| SG90 servo control | ☐ Working | Servo responds to commands |
| OLED display showing status | ☐ Working | Display shows real-time truck data |
| RGB LED status indicator | ☐ Working | LED changes color based on system state |
| Buzzer alarm logic | ☐ Working | Buzzer triggers on threshold violations |
| Modular code with functions | ☐ Included | Code organized by subsystem |
| Circuit diagram (Fritzing) | ☐ Included | PNG in assessment folder |
| Assessment README.md | ☐ Included | `/A4-Haul-Truck-Integration/README.md` |

### Hardware & Demonstration Evidence

| Requirement | Evidence | Provided |
|-------------|----------|----------|
| **Circuit Diagram** | Fritzing diagram showing all components and wiring | ☐ Yes |
| **OR Breadboard Photo** | High-quality photo of complete circuit | ☐ Yes |
| **Demo Video** | 5-minute video showing all features working | ☐ Yes |
| **Component List** | README lists all sensors, actuators, and pin assignments | ☐ Yes |

**Circuit Diagram/Breadboard Photo:**  
[Describe what's shown in your evidence]

**Demo Video Link:**  
[Paste your YouTube/OneDrive/Vimeo link]

---

## Assessment Evidence Checklist

Confirm all requirements completed before submitting:

| Requirement | Completed |
|-------------|-----------|
| DHT11 sensor reading temperature/humidity | ☐ |
| MQ-2 sensor reading gas levels | ☐ |
| Flame sensor detecting flame | ☐ |
| RFID-RC522 reading access cards | ☐ |
| DS3231 RTC providing timestamps | ☐ |
| GY-521 accelerometer measuring vibration | ☐ |
| SG90 servo rotating for dump bed | ☐ |
| OLED display showing truck status | ☐ |
| RGB LED color changes based on state | ☐ |
| Buzzer sounds on alarms | ☐ |
| Code is modular with functions | ☐ |
| All components cleanly wired | ☐ |
| Circuit diagram is clear and complete | ☐ |
| README documents all pins and logic | ☐ |
| Demo video shows normal and alarm scenarios | ☐ |

---

## Optional Notes

[Add any additional context: advanced features added, layout improvements, specific threshold values used, etc.]

---

## Submission Declaration

By submitting this form, I confirm that:

- ☐ All code in my A4 folder is my own work
- ☐ All sensors and actuators are correctly integrated and functional
- ☐ Code follows ICTIOT502 assessment requirements
- ☐ I have not plagiarized or breached academic integrity
- ☐ Demo video demonstrates all required features

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
