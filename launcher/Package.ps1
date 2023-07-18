if ($args.Length -ne 1) {
    Write-Host "Usage: .\CreateDistribution.ps1 <path to solution directory>"
    exit 1
}

$BasePath = $args[0]
$ReleasePath = "$BasePath/x64/Release"
$DistributionPath = "$BasePath/Distribution"

# check if deploy.ini exists
if (-not (Test-Path "$BasePath/deploy.ini")) {
    Write-Host "deploy.ini not found in solution folder"
    exit 1
}

# load deploy.ini
$INI = ConvertFrom-StringData((Get-Content $BasePath\deploy.ini) -join "`n")
foreach($item in $INI)
{
    Write-Output $item
}

# remove old distribution if exists
if (Test-Path $DistributionPath) {
    Remove-Item -Recurse -Force $DistributionPath
}
New-Item -ItemType Directory -Force -Path $DistributionPath

# copy files to distribution
Copy-Item -Path "$ReleasePath/*.exe" -Destination $DistributionPath
Copy-Item -Path "$ReleasePath/*.dll" -Destination $DistributionPath
Copy-Item -Path "$ReleasePath/assets" -Destination $DistributionPath -Recurse

# rename files
$GameNameWithoutSpaces = $INI["GameName"].Replace(" ", "")
Rename-Item -Path "$DistributionPath/$($INI["GameExe"])" -NewName "$DistributionPath/game.exe"
Rename-Item -Path "$DistributionPath/launcher.exe" -NewName "$DistributionPath/$GameNameWithoutSpaces.exe"

# Create shortcut to launcher
$WshShell = New-Object -ComObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut("$DistributionPath/$($INI["GameName"]).lnk")
$Shortcut.TargetPath = "$DistributionPath/$GameNameWithoutSpaces.exe"
$Shortcut.Save()

Write-Host "Distribution created at $DistributionPath"

# Zip distribution to Distribution.zip
Compress-Archive -Path $DistributionPath/* -DestinationPath "$BasePath/Distribution.zip" -Force