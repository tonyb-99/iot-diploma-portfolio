# Assessment Submission Portfolio

**Assessment A5: AWS IoT Integration & Testing**  
**Due:** Week 14 | **Weight:** 18%

---

## Version Control

| Field | Details |
|-------|---------|
| **Assessment Type** | Individual Portfolio Submission |
| **Assessment Code** | A5 |
| **Platform** | GitHub + Blackboard |
| **Document Version** | v1.0 |

---

## Introduction

This assessment submission form documents the completion of Assessment A5 (AWS IoT Integration & Testing). Your code and testing documentation must be completed and committed to your GitHub portfolio repository in the `/A5-AWS-IoT-Integration/` folder.

**Important:** This form is for submission evidence only. Your actual code stays on GitHub.

---

## Submission Instructions

### Assessment Overview

Connect your haul truck device to AWS IoT Core and implement cloud features with comprehensive testing documentation:

**Cloud Integration:**
- X.509 certificate authentication to AWS IoT Core
- MQTT publish to `truck/{truckID}/telemetry`
- Device Shadow for offline resilience and state sync
- IoT Rules Engine routing messages to SNS alerts
- CloudWatch metrics and alarms

**Testing Requirements (ICTIOT503):**
- Test plan (unit, integration, system tests)
- Test cases with expected vs actual results
- Bug log and resolutions
- Performance testing (message latency, offline sync)

### How to Complete This Assessment

1. Set up AWS IoT Core Thing with X.509 certificates
2. Complete Arduino MQTT code in `/A5-AWS-IoT-Integration/code/esp32-arduino/`
3. Configure Device Shadow and IoT Rules
4. Create SNS alerts and CloudWatch alarms
5. Document all testing in PDF report
6. Commit code, screenshots, and testing report to GitHub
7. Fill out this form with your submission details
8. Copy completed form into Blackboard by the due date

### What to Submit on GitHub

- ✅ Arduino `.ino` file with MQTT/AWS code
- ✅ AWS console screenshots (IoT Core, Rules, CloudWatch)
- ✅ Testing report (PDF, 5-10 pages)
- ✅ README.md with AWS setup instructions
- ✅ Demo video link showing MQTT, Shadow, SNS, CloudWatch

---

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | [Your full name] |
| **Student ID** | [Your student/enrollment ID] |
| **Assessment** | A5 – AWS IoT Integration |
| **Submission Date** | [Date submitted to Blackboard] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [Paste your GitHub portfolio URL] |
| **Assessment Folder** | `/A5-AWS-IoT-Integration/` |
| **Code Location** | `/A5-AWS-IoT-Integration/code/esp32-arduino/` |
| **Last Commit Date** | [Date of final commit] |

### Work Completed

**Brief Description:**  
Describe your AWS integration: which services you used (IoT Core, Shadows, Rules), what telemetry you publish, and testing scenarios you covered.

[Your description here - 3-4 sentences]

---

## Assessment Evidence

### Code and Documentation

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| Arduino `.ino` with MQTT/AWS code | ☐ Included | `/A5-AWS-IoT-Integration/code/esp32-arduino/` |
| X.509 certificate authentication | ☐ Included | Code shows cert/key loading |
| MQTT publish to telemetry topic | ☐ Working | Messages visible in AWS test console |
| Device Shadow implementation | ☐ Working | Shadow syncs desired/reported state |
| IoT Rules Engine configuration | ☐ Included | Rules route to SNS, CloudWatch |
| SNS alert setup | ☐ Working | Alerts sent on threshold violation |
| CloudWatch alarms | ☐ Working | Alarms trigger on anomalies |
| Testing report (PDF) | ☐ Included | PDF in assessment folder |
| Assessment README.md | ☐ Included | `/A5-AWS-IoT-Integration/README.md` |

### Testing & Demonstration Evidence

| Requirement | Evidence | Provided |
|-------------|----------|----------|
| **Test Plan** | PDF documenting unit, integration, system tests | ☐ Yes |
| **Test Cases** | Expected vs actual results documented | ☐ Yes |
| **Bug Log** | Issues found and resolutions | ☐ Yes |
| **Performance Data** | MQTT latency, Shadow sync time measured | ☐ Yes |
| **AWS Screenshots** | IoT Core, Rules, CloudWatch, SNS console | ☐ Yes |
| **Demo Video** | 5 minutes showing MQTT, Shadow, SNS, offline sync | ☐ Yes |

**Testing Report Location:**  
[Path to PDF in your GitHub repo]

**Demo Video Link:**  
[Paste your YouTube/OneDrive/Vimeo link]

---

## Assessment Evidence Checklist

Confirm all requirements completed before submitting:

| Requirement | Completed |
|-------------|-----------|
| X.509 certificates created and loaded in code | ☐ |
| MQTT messages publishing to AWS IoT Core | ☐ |
| Messages visible in AWS IoT Core test console | ☐ |
| Device Shadow created and syncing | ☐ |
| IoT Rules Engine routing messages correctly | ☐ |
| SNS alerts sending on thresholds | ☐ |
| CloudWatch metrics collecting data | ☐ |
| CloudWatch alarms triggering appropriately | ☐ |
| Test plan documented (unit/integration/system) | ☐ |
| Test cases with expected vs actual results | ☐ |
| Bug log showing issues and fixes | ☐ |
| Performance testing completed and documented | ☐ |
| Offline connectivity tested (disconnect/reconnect) | ☐ |
| Code is clean and commented | ☐ |
| GitHub repository is accessible | ☐ |

---

## Testing Report Summary

**Test Plan Coverage:**
- ☐ Unit tests (individual functions)
- ☐ Integration tests (sensors → MQTT → AWS)
- ☐ System tests (full device workflow)

**Key Test Scenarios:**
1. Normal operation: All sensors → telemetry → AWS
2. Threshold violation: Trigger alert, verify SNS email/SMS
3. Offline resilience: Disconnect WiFi, device queues data, reconnects and syncs
4. Shadow sync: Update desired state, device reports actual state

**Performance Metrics:**
- Average MQTT publish latency: [___] ms
- Device Shadow sync time: [___] seconds
- Offline buffer capacity: [___] messages

---

## Optional Notes

[Add context: AWS region used, certificate handling, testing tools used (AWS IoT test console, Mosquitto, etc.), challenges overcome, etc.]

---

## Submission Declaration

By submitting this form, I confirm that:

- ☐ All code in my A5 folder is my own work
- ☐ AWS integration follows security best practices
- ☐ Testing report is thorough and accurate
- ☐ Code follows ICTIOT503 assessment requirements
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
