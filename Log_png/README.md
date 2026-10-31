---
g2010 remove&port usb 

---

```ps

# Canon G2010 Cleanup - Windows 11
# Run PowerShell as Administrator
# Unplug the printer USB cable before running.

$ErrorActionPreference = "Continue"

Write-Host "`n=== Canon G2010 Cleanup ===" -ForegroundColor Cyan

# 1. Backup the current driver list
$backup = "$env:USERPROFILE\Desktop\CanonG2010_Backup"
New-Item -ItemType Directory -Path $backup -Force | Out-Null

Get-Printer | Format-List * |
    Out-File "$backup\Printers.txt"

Get-PrinterDriver | Format-List * |
    Out-File "$backup\PrinterDrivers.txt"

pnputil /enum-drivers |
    Out-File "$backup\DriverStore.txt"

# 2. Remove Canon printer queues
Get-Printer | Where-Object {
    $_.Name -match "Canon|G2010" -or
    $_.DriverName -match "Canon|G2010"
} | ForEach-Object {
    Write-Host "Removing printer: $($_.Name)"
    Remove-Printer -Name $_.Name -ErrorAction Continue
}

# 3. Stop the print spooler
Stop-Service Spooler -Force -ErrorAction Continue

# 4. Clear stuck print jobs
Remove-Item "$env:windir\System32\spool\PRINTERS\*" `
    -Force -ErrorAction SilentlyContinue

# 5. Restart the print spooler
Start-Service Spooler -ErrorAction Continue

# 6. Remove Canon printer drivers
Get-PrinterDriver | Where-Object {
    $_.Name -match "Canon|G2010"
} | ForEach-Object {
    Write-Host "Removing driver: $($_.Name)"
    Remove-PrinterDriver -Name $_.Name -ErrorAction Continue
}

# 7. Find Canon USB devices
Write-Host "`n=== Canon USB devices ===" -ForegroundColor Yellow

$devices = Get-PnpDevice -PresentOnly:$false |
    Where-Object {
        $_.FriendlyName -match "Canon|G2010" -or
        $_.InstanceId -match "VID_04A9"
    }

$devices | Format-Table Status, Class, FriendlyName, InstanceId -AutoSize

Write-Host "`nCleanup complete." -ForegroundColor Green
Write-Host "Backup saved at: $backup"
Write-Host "Restart Windows before installing the driver again."


```
---
spoots remove
---
```ps

# Reset Print Spooler - Windows 11
# Run as Administrator

$ErrorActionPreference = "Continue"

# 1. Stop Print Spooler
Stop-Service -Name Spooler -Force
Start-Sleep -Seconds 2

# 2. Clear stuck print jobs
$spoolPath = "$env:windir\System32\spool\PRINTERS"

Get-ChildItem -Path $spoolPath -Force -ErrorAction SilentlyContinue |
    Remove-Item -Force -Recurse -ErrorAction SilentlyContinue

# 3. Reset Print Spooler service
sc.exe config Spooler start= auto
sc.exe failure Spooler reset= 86400 actions= restart/5000/restart/5000/restart/5000

# 4. Start Print Spooler
Start-Service -Name Spooler
Start-Sleep -Seconds 2

# 5. Verify service status
Get-Service -Name Spooler |
    Format-Table Name, Status, StartType -AutoSize

Write-Host "`nSpooler reset complete." -ForegroundColor Green


```
```

# List Canon printer driver packages
pnputil /enum-drivers |
    Select-String -Pattern "Canon" -Context 4,8



```

<img width="2428" height="1476" alt="image" src="https://github.com/user-attachments/assets/aa06c2c6-4e21-4673-9786-3fcb9b7f905d" />



---
log
---
#15
<img width="8160" height="6120" alt="1000011267" src="https://github.com/user-attachments/assets/c1152758-6bbf-4f1e-84ea-bce4a8cde3b1" />
<img width="8160" height="6120" alt="1000011266" src="https://github.com/user-attachments/assets/3fc3d604-6eb0-48a4-ae99-782c867f6135" />
<img width="6120" height="8160" alt="1000011265" src="https://github.com/user-attachments/assets/0095691e-83c8-4495-99a2-c104957dd2ee" />



#13
<img width="1836" height="4080" alt="1000011264" src="https://github.com/user-attachments/assets/c548ccbd-dc5b-49e9-a296-9a0a8d8b2b87" />
<img width="1836" height="4080" alt="1000011263" src="https://github.com/user-attachments/assets/9aca37c0-eb66-4208-8919-02d23468c771" />
<img width="6120" height="8160" alt="1000011262" src="https://github.com/user-attachments/assets/fe310fcc-81da-4eba-ac0f-9946fca3067a" />



#11
<img width="8160" height="6120" alt="1000011259" src="https://github.com/user-attachments/assets/822915e9-7ca2-4817-82b1-04232bc92023" />
<img width="8160" height="6120" alt="1000011260" src="https://github.com/user-attachments/assets/a6b67d92-ea5e-4ab5-9d3d-878a7c79b56f" />
<img width="8160" height="6120" alt="1000011261" src="https://github.com/user-attachments/assets/e56b53a5-1b00-44b1-8851-4cce823111da" />


#1
<img width="6120" height="8160" alt="1000011258" src="https://github.com/user-attachments/assets/4c17c133-618e-473f-8b24-3f1b66a04c85" />


---
PEA 
---

---
Finish
---

#1
<img width="1344" height="1962" alt="แบบแจ้งผลการลงทะเบียนรับสิทธิ-101-0078-516-27" src="https://github.com/user-attachments/assets/b404a80f-425a-4095-9016-b4e1b3918403" />
#2
<img width="1344" height="1962" alt="แบบแจ้งผลการลงทะเบียนรับสิทธิ-101-0559-509-20" src="https://github.com/user-attachments/assets/d58714c5-eabf-49f3-85a9-1ff8d74867df" />

