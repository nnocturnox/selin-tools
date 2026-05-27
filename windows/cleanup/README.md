# 🧹 Cleanup — Artifact Cleanup Script

A PowerShell script that automates the removal of digital footprints left behind by daily Windows usage. Designed to run automatically at user logoff to ensure a clean slate for the next session.

---

## 📄 Files

| File | Description |
|---|---|
| `cleanup.ps1` | Removes browser history, cache, Jump Lists, and Recent Files at logoff |

---

## 🔍 What It Cleans

| Target | Details |
|---|---|
| **Chrome History & Cache** | `AppData\Local\Google\Chrome\User Data\Default\` |
| **Edge History & Cache** | `AppData\Local\Microsoft\Edge\User Data\Default\` |
| **Jump Lists** | `AutomaticDestinations\*` and `CustomDestinations\*` |
| **Recent Files** | `AppData\Roaming\Microsoft\Windows\Recent\*` |

---

## 🚀 Usage

### Manual Run

Open PowerShell as Administrator and run:

```powershell
powershell.exe -ExecutionPolicy Bypass -File "C:\path\to\cleanup.ps1"
```

### Automatic Run at Logoff (Task Scheduler)

To trigger the script automatically every time you sign out:

1. Open **Task Scheduler** → click **Create Basic Task**
2. **Trigger:** Select *When a specific event is logged*
   - Log: `Security`
   - Source: `Microsoft-Windows-Security-Auditing`
   - Event ID: `4647` *(User Logoff)*
3. **Action:** Select *Start a program*
   - Program/script: `powershell.exe`
   - Arguments: `-ExecutionPolicy Bypass -File "C:\path\to\cleanup.ps1"`

---

## 📊 Sample Output

```
Cleaning artefacts for user: selin
Deleted: C:\Users\selin\AppData\Local\Google\Chrome\User Data\Default\History
Deleted: C:\Users\selin\AppData\Local\Microsoft\Edge\User Data\Default\History
Deleted Jump List files in: ...AutomaticDestinations\*
Cleared Recent Files list
Cleanup completed.
```

---

## ⚙️ Requirements

- **OS:** Windows 10 or Windows 11
- **Privileges:** Must be run with appropriate execution policy (`Bypass` or `RemoteSigned`)

---

## ⚠️ Disclaimer

Deletion performed by this script is **permanent**. Use at your own risk. Always verify the paths before running in a new environment.

---

## 📄 License

MIT License — see the root [`LICENSE`](../../LICENSE) file for details.
