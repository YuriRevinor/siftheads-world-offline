@echo off
rem 构建 SaveManager.exe（需要 Visual Studio 的 C++ 工具，目标 .NET Framework 4.7.2）
rem   build.bat        生成游戏根目录下的 SaveManager.exe
rem   build.bat test   生成命令行测试版，输出到 %TEMP%\shw_savemanager_build
setlocal
cd /d "%~dp0"
for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set "VS=%%i"
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set "R=%ProgramFiles(x86)%\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.7.2"
set REFS=/FU"%R%\mscorlib.dll" /FU"%R%\System.dll" /FU"%R%\System.Drawing.dll" /FU"%R%\System.Windows.Forms.dll"
set "OBJ=%TEMP%\shw_savemanager_build"
if not exist "%OBJ%" mkdir "%OBJ%"

if /i "%~1"=="test" (
  cl /nologo /utf-8 /clr /EHa /O2 /DSELFTEST %REFS% /Fo"%OBJ%\SaveManager.obj" SaveManager.cpp /Fe"%OBJ%\SaveManager_test.exe" /link /SUBSYSTEM:CONSOLE || exit /b 1
  echo built %OBJ%\SaveManager_test.exe
) else (
  cl /nologo /utf-8 /clr /EHa /O2 %REFS% /Fo"%OBJ%\SaveManager.obj" SaveManager.cpp /Fe"%~dp0..\..\SaveManager.exe" /link /SUBSYSTEM:WINDOWS /ENTRY:main || exit /b 1
  rmdir /s /q "%OBJ%"
  echo built SaveManager.exe
)
endlocal
