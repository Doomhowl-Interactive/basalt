$BasePath = "$PSScriptRoot/.."
$ReleasePath = "$BasePath/x64/Release"
$DistributionPath = "$BasePath/Distribution"

# remove old distribution if exists
if (Test-Path $DistributionPath) {
    Remove-Item -Recurse -Force $DistributionPath
}
New-Item -ItemType Directory -Force -Path $DistributionPath

# copy files to distribution
Copy-Item -Path "$ReleasePath/*.exe" -Destination $DistributionPath
Copy-Item -Path "$ReleasePath/*.dll" -Destination $DistributionPath
Copy-Item -Path "$ReleasePath/assets" -Destination $DistributionPath -Recurse

Write-Host "Distribution created at $DistributionPath"

# Zip distribution to Distribution.zip
# Compress-Archive -Path $DistributionPath/* -DestinationPath "$BasePath/Distribution.zip" -Force