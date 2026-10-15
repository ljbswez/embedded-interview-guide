@echo off
setlocal
if not exist "%~dp0index.html" (
  echo Missing index.html. Keep the entire folder together.
  pause
  exit /b 1
)
start "" "%~dp0index.html"
exit /b 0
