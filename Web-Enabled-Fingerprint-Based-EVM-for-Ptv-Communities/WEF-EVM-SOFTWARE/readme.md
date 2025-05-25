voter enrollment and voting system details:

markdown
# Voter Enrollment & Voting System

## Overview
This document outlines the complete process flow and API reference for our voter enrollment and electronic voting system. The system supports voter registration, verification, and secure voting with fingerprint authentication.

## Process Flow

### Main Menu Options
1. **Enroll** - Register new voters
2. **Verify & Vote** - Authenticate voters and cast votes

### 1️⃣ Enrollment Process (New Voter Registration)
1. User selects "Enroll" from the menu
2. System generates a unique `voterId` (e.g., "VTR234")
3. Capture voter's fingerprint data
4. Send enrollment data to backend:

```http
POST /api/voters/enroll
Content-Type: application/json

{
  "voterId": "VTR234",
  "fingerprintData": "[hexadecimal fingerprint template]"
}
On successful response, system returns to main menu

2️⃣ Verification & Voting Process
User selects "Verify & Vote"

Choose verification method:

By Voter ID:

http
GET /api/voters/{voterId}
By Fingerprint:

http
GET /api/voters/by-fingerprint/{fingerprintData}
Combined Verification:

http
GET /api/voters/login/verify-voter/{voterId}/{fingerprintData}
On successful verification:

Fetch candidate list:

http
GET /api/candidates
User selects candidate (gets candidateId)

Cast vote:

http
POST /api/candidates/{candidateId}/vote
Mark voter as voted:

http
POST /api/voters/{voterId}/mark-voted
System returns to main menu

API Reference
Voter Management (/api/voters)
Method	Endpoint	Description
POST	/enroll	Register new voter
GET	/	Get all registered voters
GET	/{voterId}	Get voter by ID
GET	/by-fingerprint/{fingerprintData}	Get voter by fingerprint data
GET	/verify-fingerprint/{fingerprintData}	Validate fingerprint
GET	/login/verify-voter/{voterId}/{fingerprintData}	Verify both voter ID and fingerprint
POST	/{voterId}/mark-voted	Mark voter as having voted
Candidate Management (/api/candidates)
Method	Endpoint	Description
POST	/enroll	Register new candidate
GET	/	List all candidates
GET	/{id}	Get candidate by ID
PUT	/{id}	Update candidate details
DELETE	/{id}	Delete candidate
POST	/{id}/vote	Cast vote for candidate
GET	/position/{position}	Filter candidates by position
System Flow Diagram
Main Menu
├─▶ Enroll
│   ├─ Generate voterId
│   ├─ Capture fingerprint
│   └─ POST /enroll
│
└─▶ Verify & Vote
    ├─ Verification Options
    │   ├─ By voterId
    │   ├─ By fingerprint
    │   └─ Combined
    │
    ├─ GET /candidates
    ├─ Select candidate
    ├─ POST /vote
    └─ POST /mark-voted
Implementation Notes
All API responses should include appropriate HTTP status codes

Fingerprint data should be transmitted as hexadecimal strings

The system should prevent duplicate votes

Voter verification must be completed before allowing vote casting

Example Requests
Voter Enrollment:

http
POST /api/voters/enroll HTTP/1.1
Content-Type: application/json

{
  "voterId": "VTR789",
  "fingerprintData": "A1B2C3D4E5F6..."
}
Cast Vote:

http
POST /api/candidates/5/vote HTTP/1.1
Content-Type: application/json
This documentation should be used as the single source of truth for both frontend and backend implementation teams.