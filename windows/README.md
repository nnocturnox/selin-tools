1. Artifact Cleanup (cleanup.ps1)
This script automates the removal of digital footprints left behind by daily Windows usage. It is specifically designed to run at user logoff to ensure a fresh start for the next session.

Key Features:

Browser Purge: Clears history and cache for Chrome and Edge.

System Reset: Wipes "Jump Lists" and "Recent Files" records.

Logoff Automation: Cleans up all traces during the logoff sequence.

Setup: Running at Logoff (Task Scheduler)
To ensure the script triggers exactly when you log off, follow these steps:

Open Task Scheduler and click Create Basic Task.

Trigger: Select "When a specific event is logged".

Log: Select Security.

Source: Select Microsoft-Windows-Security-Auditing.

Event ID: Type 4647 (This is the Windows code for User Logoff).

Action: Select Start a program.

Program/script: powershell.exe

Add arguments: -ExecutionPolicy Bypass -File "C:\path\to\cleanup.ps1"

Note: This configuration ensures that every time you sign out, nnocturnox-tools sweeps the system clean.

Requirements & Disclaimer
OS: Windows 10 or 11.

Permissions: Must be configured with appropriate execution policies.

Disclaimer: Use at your own risk. Deletion is permanent.
