# Assessment Submission Portfolio

**Assessment A5b: QuickSight Analytics Dashboard**  
**Due:** Week 14 | **Weight:** 7%

---

## Version Control

| Field | Details |
|-------|---------|
| **Assessment Type** | Individual Portfolio Submission |
| **Assessment Code** | A5b |
| **Platform** | GitHub + Blackboard |
| **Document Version** | v1.0 |

---

## Introduction

This assessment submission form documents the completion of Assessment A5b (QuickSight Analytics Dashboard). Your dashboard, architecture documentation, and deployment files must be committed to your GitHub portfolio repository in the `/A5-AWS-IoT-Integration/dashboards/` folder.

**Important:** This form is for submission evidence only. Your actual work stays on GitHub.

---

## Submission Instructions

### Assessment Overview

Create a real-time analytics dashboard for haul truck monitoring using AWS QuickSight and IoT Analytics:

**Dashboard Requirements:**
- **Minimum 3 visualizations:**
  1. Temperature Trend (line chart, 24-hour history)
  2. Vibration Status (gauge/card vs. alert threshold)
  3. Alerts Summary (table of recent alerts)

**Data Integration:**
- Connect QuickSight to IoT Core telemetry via IoT Analytics pipeline OR S3/DynamoDB
- Live/near-real-time data (minimum 1 hour historical)
- Auto-refresh every 5-15 minutes

**Optional Enhancement:**
- AWS IoT SiteWise asset model for truck properties
- SiteWise properties: engine_temperature, vibration_level, cabin_lock_state
- Link SiteWise model to QuickSight (+2 bonus points)

### How to Complete This Assessment

1. Set up AWS IoT Analytics pipeline or S3 backend
2. Create QuickSight dataset from truck telemetry data
3. Design dashboard with 3+ visualizations
4. Configure auto-refresh and interactive filters
5. (Optional) Create SiteWise asset model
6. Document architecture (1 page)
7. Export QuickSight dashboard definition JSON
8. Create 3-minute video walkthrough
9. Commit all files to GitHub
10. Fill out this form with your submission details
11. Copy completed form into Blackboard by the due date

### What to Submit on GitHub

- ✅ QuickSight dashboard URL (live link) OR full-page screenshot (PDF/PNG)
- ✅ QuickSight dashboard definition JSON export
- ✅ IoT Analytics pipeline configuration JSON
- ✅ One-page architecture document (PDF)
- ✅ 3-minute video walkthrough

---

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | [Your full name] |
| **Student ID** | [Your student/enrollment ID] |
| **Assessment** | A5b – QuickSight Analytics Dashboard |
| **Submission Date** | [Date submitted to Blackboard] |

---

## Assessment Summary

### GitHub Portfolio Repository

| Field | Details |
|-------|---------|
| **Repository URL** | [Paste your GitHub portfolio URL] |
| **Dashboard Folder** | `/A5-AWS-IoT-Integration/dashboards/` |
| **Last Commit Date** | [Date of final commit] |

### Work Completed

**Brief Description:**  
Describe your dashboard: which visualizations you created, what data sources they use, refresh rate, and what business insights they reveal.

[Your description here - 3-4 sentences]

---

## Assessment Evidence

### Dashboard & Documentation

| Requirement | Evidence Provided | Location in Repository |
|-------------|-------------------|------------------------|
| QuickSight dashboard created | ☐ Live/Screenshot | URL or PDF provided |
| Temperature Trend visualization | ☐ Included | Line chart showing 24-hour history |
| Vibration Status visualization | ☐ Included | Gauge or card with threshold indicator |
| Alerts Summary visualization | ☐ Included | Table showing recent alerts |
| IoT Analytics pipeline | ☐ Configured | JSON export in dashboards folder |
| Data refreshes every 5-15 min | ☐ Configured | Auto-refresh enabled |
| One-page architecture document | ☐ Included | PDF explaining data flow |
| Dashboard definition JSON | ☐ Exported | JSON file in dashboards folder |
| Assessment README.md | ☐ Included | Instructions for accessing dashboard |

### Optional Enhancements

| Enhancement | Included | Location |
|------------|----------|----------|
| AWS IoT SiteWise asset model | ☐ Yes (Bonus) | `/A5-AWS-IoT-Integration/dashboards/` |
| SiteWise properties (temp, vibration, lock state) | ☐ Yes (Bonus) | SiteWise asset definition JSON |

### Demonstration Evidence

| Requirement | Evidence | Provided |
|-------------|----------|----------|
| **Dashboard Live Link** | URL to live QuickSight dashboard | ☐ Yes |
| **OR Dashboard Screenshot** | Full-page PNG/PDF showing all visualizations | ☐ Yes |
| **Video Walkthrough** | 3-minute video showing dashboard in action | ☐ Yes |
| **Architecture Document** | One-page PDF explaining data flow | ☐ Yes |

**QuickSight Dashboard URL:**  
[Paste your live dashboard URL or screenshot link]

**Video Walkthrough Link:**  
[Paste your YouTube/OneDrive/Vimeo link]

---

## Assessment Evidence Checklist

Confirm all requirements completed before submitting:

| Requirement | Completed |
|-------------|-----------|
| IoT Analytics pipeline created and configured | ☐ |
| Telemetry data flowing from truck to QuickSight | ☐ |
| QuickSight dataset created from truck data | ☐ |
| Temperature Trend chart displays 24-hour history | ☐ |
| Vibration Status shows current value vs threshold | ☐ |
| Alerts Summary table shows recent anomalies | ☐ |
| Dashboard has 3+ visualizations | ☐ |
| Auto-refresh configured (every 5-15 minutes) | ☐ |
| Interactive filters working (if configured) | ☐ |
| Architecture document explains data flow | ☐ |
| Dashboard definition JSON exported | ☐ |
| Video shows dashboard loading with live data | ☐ |
| Video demonstrates interactive features | ☐ |
| (Optional) SiteWise asset model created | ☐ |
| (Optional) SiteWise linked to QuickSight | ☐ |

---

## Data Insights

**Temperature Visualization:**
- What temperature trends does the chart reveal? [_______________]
- Alert threshold set at: [___] °C

**Vibration Visualization:**
- Baseline vibration level: [___] m/s²
- Alert threshold: [___] m/s²

**Alerts Summary:**
- Most common alert type: [_______________]
- Frequency of anomalies: [___] per [hour/day]

---

## Architecture Notes

**Data Flow:**
1. Truck MQTT → AWS IoT Core
2. IoT Core → [IoT Analytics / S3 / DynamoDB]
3. Data Store → QuickSight Dataset
4. Dataset → Dashboard Visualizations

**Refresh Strategy:**
- [IoT Analytics / S3 / DynamoDB] updates every [___] minutes
- QuickSight refreshes every [___] minutes
- Justification: [_______________]

---

## Optional Notes

[Add context: design decisions, visualization choices, challenges integrating IoT Analytics, SiteWise features implemented, etc.]

---

## Submission Declaration

By submitting this form, I confirm that:

- ☐ All dashboard work in my A5b folder is my own work
- ☐ QuickSight is correctly connected to truck telemetry
- ☐ Dashboard visualizations provide meaningful business insights
- ☐ Code follows ICTIOT503 assessment requirements
- ☐ I have not plagiarized or breached academic integrity

---

## For Assessor Use

| Field | Details |
|-------|---------|
| **Assessor Name** | [Assessor completes] |
| **Date Assessed** | [Assessor completes] |
| **Result** | ☐ Satisfactory ☐ Not Yet Satisfactory |
| **SiteWise Bonus** | ☐ Yes (+2 points) ☐ No |
| **Feedback** | [Assessor completes] |

---

**Submission recorded by Blackboard:** [Auto-recorded]

**Your actual work is assessed on GitHub. This form provides proof of submission.**
