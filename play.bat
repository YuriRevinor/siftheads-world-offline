@echo off
rem Portable Ruffle launcher - all config/cache/saves stay inside this folder
set "HERE=%~dp0"
set "P=%HERE%player"
if not exist "%P%\data\config" mkdir "%P%\data\config"
if not exist "%P%\data\cache" mkdir "%P%\data\cache"
if not exist "%P%\data\saves" mkdir "%P%\data\saves"
if "%~1"=="" (
  start "" "%P%\ruffle.exe" --config "%P%\data\config" --cache-directory "%P%\data\cache" --storage disk --save-directory "%P%\data\saves" --open-url-mode deny
) else (
  start "" "%P%\ruffle.exe" --config "%P%\data\config" --cache-directory "%P%\data\cache" --storage disk --save-directory "%P%\data\saves" --open-url-mode deny "%~f1"
)
