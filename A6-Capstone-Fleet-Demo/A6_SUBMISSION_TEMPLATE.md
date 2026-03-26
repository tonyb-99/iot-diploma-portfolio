# Assessment Submission Portfolio

**Assessment A6: Capstone Fleet System**  
**Due:** Week 18 | **Weight:** 30%

---

## Version Control

| Field | Details |
|-------|---------|
| **Assessment Type** | Individual Portfolio Submission |
| **Assessment Code** | A6 |
| **Platform** | GitHub + Blackboard |
| **Document Version** | v1.0 |

---

## Introduction

This assessment submission form documents the completion of Assessment A6 (Capstone Fleet System). This is the final, comprehensive assessment integrating all course concepts: IoT hardware, AWS cloud services, digital twins, analytics, and testing.

**Important:** This form is for submission evidence only. Your actual work stays on GitHub.

---

## Submission Instructions

### Assessment Overview

Demonstrate a complete IoT solution for a haul truck fleet monitoring system in a mining pit. Your system integrates:

**Hardware & Sensors:**
- Real ESP32 haul truck device with all A1-A5 sensors
- Continuous MQTT telemetry to AWS IoT Core
- Functional sensors: DHT11, GY-521, MQ-2, touch sensor

**AWS Cloud Integration:**
- IoT Core (Things, certificates, MQTT topics)
- Device Shadow (offline resilience)
- IoT Rules Engine (routing to DynamoDB, SiteWise, SNS, Lambda)
- IoT SiteWise (asset models and properties)
- DynamoDB (time-series storage)
- SNS (alert notifications)

**Analytics & Visualization:**
- QuickSight dashboard with 5+ visualizations
- Real-time data from actual truck
- Pit Station web dashboard showing fleet health

**Digital Twins & Industry Quest:**
- AWS IoT TwinMaker with 6 truck models (1 real + 5 simulated)
- Real truck telemetry bound to digital model
- Anomaly detection with visual feedback
- Industry Quest labs integration (Weeks 15-17)

### How to Complete This Assessment

1. Ensure A4 device fully functional (all sensors/actuators)
2. Complete AWS integration from A5 (IoT Core, Shadows, Rules)
3. Deploy QuickSight dashboard from A5b with live data
4. Create IoT SiteWise asset models for truck
5. Build TwinMaker digital twin scenes (1 real + 5 simulated trucks)
6. Develop pit station web dashboard
7. Implement anomaly detection and visual feedback
8. Document architecture (2-3 pages)
9. Create portfolio document (20-30 pages)
10. Prepare 5-minute demonstration video
11. Commit all files to GitHub
12. Fill out this form with your submission details
13. Copy completed form into Blackboard by the due date

### What to Submit on GitHub

- ✅ Complete `/A6-Capstone-Fleet-Demo/` folder with all code
- ✅ `/truck` – ESP32 firmware with all sensors + AWS
- ✅ `/pit-station` – Web dashboard code (HTML/CSS/JS)
- ✅ `/aws` – CloudFormation/Lambda/Rules configurations
- ✅ `/docs` – Architecture diagrams, setup guides
- ✅ Portfolio PDF (20-30 pages)
- ✅ 5-minute demonstration video

---

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | [Your full name] |
| **Student ID** | [Your student/enrollment ID] |
| **Assessment** | A6 – Capstone Fleet System |
| **Submission Date** | [Date submitted to Blackboard] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [Paste your GitHub portfolio URL] |
| **Assessment Folder** | `/A6-Capstone-Fleet-Demo/` |
| **Last Commit Date** | [Date of final commit] |

### Work Completed

**Executive Summary (2-3 sentences):**  
Describe your complete fleet monitoring system: hardware, AWS services, digital twins, and business value.

[Your summary here]

---

## Assessment Evidence

### Code Organization

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| ESP32 truck firmware | ☐ Included | `/A6-Capstone-Fleet-Demo/truck/` |
| Pit station web dashboard | ☐ Included | `/A6-Capstone-Fleet-Demo/pit-station/` |
| AWS configurations | ☐ Included | `/A6-Capstone-Fleet-Demo/aws/` |
| Documentation and diagrams | ☐ Included | `/A6-Capstone-Fleet-Demo/docs/` |
| README.md with setup instructions | ☐ Included | `/A6-Capstone-Fleet-Demo/README.md` |

### Hardware & Sensors

| Component | Status | Verified |
|-----------|--------|----------|
| Real ESP32 truck device | ☐ Functional | Sensors publishing MQTT |
| DHT11 (temp/humidity) | ☐ Working | Values in telemetry |
| GY-521 (vibration) | ☐ Working | Values in telemetry |
| MQ-2 (gas) | ☐ Working | Values in telemetry |
| Touch sensor (cabin lock) | ☐ Working | State in telemetry |
| SG90 servo (dump bed) | ☐ Working | Responsive to commands |
| OLED display | ☐ Working | Shows truck status |
| RGB LED indicator | ☐ Working | Color changes on alerts |
| Buzzer | ☐ Working | Sounds on alarms |

### AWS Cloud Services

| Service | Configuration | Status |
|---------|---------------|--------|
| IoT Core | Things, certificates, MQTT topics | ☐ Complete |
| Device Shadow | Desired/reported state sync | ☐ Complete |
| IoT Rules | Routes to DynamoDB, SiteWise, SNS | ☐ Complete |
| IoT SiteWise | Asset models for truck properties | ☐ Complete |
| DynamoDB | Time-series storage | ☐ Complete |
| SNS | Alert notifications | ☐ Complete |
| Lambda | Anomaly detection logic (if used) | ☐ Optional |

### Digital Twins & Analytics

| Component | Evidence | Status |
|-----------|----------|--------|
| **IoT TwinMaker** | 6 truck scene (1 real + 5 simulated) | ☐ Complete |
| **Truck Asset Models** | 6 digital models with properties | ☐ Complete |
| **Real Truck Binding** | Telemetry bound to digital model | ☐ Complete |
| **Anomaly Visualization** | Digital twins respond to alerts | ☐ Complete |
| **QuickSight Dashboard** | 5+ visualizations with live data | ☐ Complete |
| **Pit Station Dashboard** | Web interface showing fleet health | ☐ Complete |

### Documentation

| Document | Pages | Status |
|----------|-------|--------|
| Architecture diagram | Block diagram showing full flow | ☐ Included |
| Architecture document | 2-3 pages explaining each service | ☐ Included |
| Setup guide | Deployment instructions | ☐ Included |
| Unit mapping document | Alignment to ICTIOT503 criteria | ☐ Included |
| Portfolio PDF | 20-30 pages comprehensive report | ☐ Included |

### Demonstration Evidence

| Requirement | Link/Location | Status |
|-------------|--------------|--------|
| **Demo Video** | 5-minute YouTube/Vimeo/OneDrive link | ☐ Complete |
| **Portfolio PDF** | PDF in GitHub repo | ☐ Complete |
| **GitHub Repository** | URL with all code organized | ☐ Complete |

**Demo Video Link:**  
[Paste your YouTube/OneDrive/Vimeo link]

**Portfolio PDF Location:**  
[Path in GitHub repo, e.g., /A6-Capstone-Fleet-Demo/docs/Portfolio.pdf]

---

## Assessment Evidence Checklist

### Hardware & Sensors

| Requirement | Completed |
|-------------|-----------|
| Real ESP32 truck device fully functional | ☐ |
| All A1-A3 sensors integrated and reading | ☐ |
| Actuators functional (servo, OLED, LED, buzzer) | ☐ |
| Sensors publishing continuous telemetry | ☐ |
| Data reaches AWS IoT Core successfully | ☐ |

### AWS Cloud

| Requirement | Completed |
|-------------|-----------|
| IoT Core Thing created with X.509 certs | ☐ |
| MQTT topics structured correctly | ☐ |
| Device Shadow tracking truck state | ☐ |
| IoT Rules routing messages | ☐ |
| DynamoDB storing telemetry | ☐ |
| SNS sending alerts on anomalies | ☐ |
| IoT SiteWise asset models created | ☐ |
| SiteWise properties updated in real-time | ☐ |

### Digital Twins & Analytics

| Requirement | Completed |
|-------------|-----------|
| AWS IoT TwinMaker scene created | ☐ |
| 6 truck models in scene (1 real + 5 simulated) | ☐ |
| Real truck telemetry bound to digital model | ☐ |
| Anomaly detection rules implemented | ☐ |
| Digital twins visually respond to alerts | ☐ |
| QuickSight dashboard displays live data | ☐ |
| 5+ visualizations in dashboard | ☐ |
| Pit station web dashboard operational | ☐ |
| Fleet health grid shows all trucks | ☐ |
| Drill-down to individual truck details | ☐ |

### Testing & Scenarios

| Requirement | Completed |
|-------------|-----------|
| Scenario 1: Normal operation (all green) | ☐ |
| Scenario 2: Sensor anomaly (trigger alert) | ☐ |
| Scenario 3: Offline resilience (disconnect/reconnect) | ☐ |
| Scenario 4: Multi-truck simulation (6 trucks) | ☐ |
| QuickSight responds to anomalies | ☐ |
| Digital twins update in real-time | ☐ |
| Pit station dashboard shows all trucks | ☐ |
| Message latency documented | ☐ |
| Shadow sync time documented | ☐ |

### Documentation

| Requirement | Completed |
|-------------|-----------|
| Architecture diagram showing full flow | ☐ |
| Architecture document (2-3 pages) | ☐ |
| Setup guide with deployment steps | ☐ |
| Unit mapping to ICTIOT503 criteria | ☐ |
| Portfolio PDF (20-30 pages) | ☐ |
| GitHub repository organized and accessible | ☐ |
| All code clean and commented | ☐ |
| Demo video (5 minutes) | ☐ |

---

## Portfolio Document Structure

**Expected Contents (20-30 pages):**

1. **Executive Summary** (1 page)
   - Problem statement (mining haul truck fleet monitoring)
   - Solution overview

2. **System Architecture** (2-3 pages)
   - Block diagram: truck → IoT Core → Analytics → TwinMaker → Pit Station
   - Component descriptions
   - Data flow explanation

3. **Hardware Implementation** (4-6 pages)
   - Truck design with sensors and actuators
   - Wiring diagrams/photos
   - Component specifications
   - Pin configurations

4. **Software Architecture** (4-5 pages)
   - ESP32 firmware structure and functions
   - MQTT topic design
   - Device Shadow schema
   - AWS service interactions

5. **Cloud Integration** (4-5 pages)
   - IoT Core setup and security
   - Device Shadow implementation
   - IoT Rules configuration
   - SiteWise asset models
   - DynamoDB design

6. **Analytics & Visualization** (2-3 pages)
   - QuickSight dashboard design
   - IoT TwinMaker scene and 3D models
   - Anomaly detection logic

7. **Testing & Results** (4-6 pages)
   - Test scenarios and outcomes
   - Performance metrics
   - Anomaly detection examples
   - Multi-truck simulation results

8. **Industry Quest Integration** (1-2 pages)
   - How AWS Industry Quest labs enhanced learning
   - Concepts applied to capstone

9. **Reflection & Learning** (1-2 pages)
   - Challenges encountered and solutions
   - Lessons learned
   - Future improvements

10. **Appendices**
    - Code snippets (key Arduino functions, Lambda, etc.)
    - Datasheets (sensors, ESP32)
    - AWS console screenshots
    - Compliance mapping to ICTIOT503

---

## Demo Video Content

**Structure (5 minutes):**

- **0:00-0:30** Opening: Introduce RockCore mining scenario and system overview
- **0:30-1:00** Hardware demo: Show real truck, sensors working, components visible
- **1:00-1:30** AWS Integration: Console walkthrough (IoT Core, Things, Shadows)
- **1:30-2:00** Cloud Services: SiteWise, DynamoDB, SNS, CloudWatch
- **2:00-2:30** Analytics: QuickSight dashboard with live data, trigger anomaly
- **2:30-3:30** Digital Twins: TwinMaker fleet scene responding to real truck anomaly
- **3:30-4:00** Pit Station: Web dashboard showing fleet health, drill-down details
- **4:00-4:30** Test Scenario: Offline resilience (disconnect WiFi, device queues, syncs)
- **4:30-5:00** Conclusion: Business value (safety, ROI, maintenance efficiency)

---

## Optional Notes

[Add context: advanced features implemented, design challenges, optimization techniques, Industry Quest lessons applied, etc.]

---

## Submission Declaration

By submitting this form, I confirm that:

- ☐ All code in my A6 folder is my own work
- ☐ System integrates all A1-A5 components
- ☐ AWS services are properly configured and secure
- ☐ Digital twins and analytics are fully functional
- ☐ Testing is comprehensive and documented
- ☐ Code follows ICTIOT502/503 assessment requirements
- ☐ I have not plagiarized or breached academic integrity
- ☐ Portfolio document is complete and thorough

---

## For Assessor Use

| Field | Details |
|-------|---------|
| **Assessor Name** | [Assessor completes] |
| **Date Assessed** | [Assessor completes] |
| **Result** | ☐ Satisfactory ☐ Not Yet Satisfactory |
| **Portfolio Quality** | ☐ Excellent ☐ Good ☐ Acceptable |
| **System Integration** | ☐ Complete ☐ Mostly Complete ☐ Partial |
| **Feedback** | [Assessor completes] |

---

**Submission recorded by Blackboard:** [Auto-recorded]

**Your actual work is assessed on GitHub. This form provides proof of submission.**
