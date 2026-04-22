@echo off
setlocal
set "SCRIPT=C:\Users\Linger\Desktop\git_ai.py"
if not exist "%SCRIPT%" (
  echo [ERROR] 找不到脚本: %SCRIPT%
  exit /b 1
)
where py >nul 2>nul
if %ERRORLEVEL%==0 (
  py "%SCRIPT%" --repo "%~dp0."
) else (
  python "%SCRIPT%" --repo "%~dp0."
)
endlocal
