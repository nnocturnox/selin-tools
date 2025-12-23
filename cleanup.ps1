# ============================================
# Windows User Artefact Cleanup Script
# Runs at every user logoff
# ============================================

# Get current username
$User = $env:USERNAME

Write-Output "Cleaning artefacts for user: $User"

# --- 1. Browser History (Chrome + Edge) ---
$chromePaths = @(
    "C:\Users\$User\AppData\Local\Google\Chrome\User Data\Default\History",
    "C:\Users\$User\AppData\Local\Google\Chrome\User Data\Default\Cache\*"
)

$edgePaths = @(
    "C:\Users\$User\AppData\Local\Microsoft\Edge\User Data\Default\History",
    "C:\Users\$User\AppData\Local\Microsoft\Edge\User Data\Default\Cache\*"
)

foreach ($path in $chromePaths + $edgePaths) {
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
        Write-Output "Deleted: $path"
    }
}

# --- 2. Jump Lists ---
$jumpListPaths = @(
    "C:\Users\$User\AppData\Roaming\Microsoft\Windows\Recent\AutomaticDestinations\*",
    "C:\Users\$User\AppData\Roaming\Microsoft\Windows\Recent\CustomDestinations\*"
)

foreach ($path in $jumpListPaths) {
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
        Write-Output "Deleted Jump List files in: $path"
    }
}

# --- 3. Recent Files ---
$recentPath = "C:\Users\$User\AppData\Roaming\Microsoft\Windows\Recent\*"
if (Test-Path $recentPath) {
    Remove-Item $recentPath -Recurse -Force -ErrorAction SilentlyContinue
    Write-Output "Cleared Recent Files list"
}

Write-Output "Cleanup completed."
