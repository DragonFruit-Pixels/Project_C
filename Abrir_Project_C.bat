@echo off
REM Abre Project_C pasando el .uproject en la linea de comandos.
REM
REM Eso NO es opcional: un proyecto con modulo C++ que arranca sin esa ruta no
REM encuentra su propio modulo y muere con "The game module 'ProjectC' could not
REM be found". El boton Launch del Epic Games Launcher arranca sin ruta y por eso
REM rompia. Ver design/architecture/09-abrir-el-proyecto.md.
REM
REM Antes de abrir compila el modulo: Binaries/ no viaja en git, y el editor carga
REM sin quejarse una DLL vieja aunque el pull haya traido C++ nuevo. Si no hubo
REM cambios en Source/ la compilacion es incremental y tarda segundos.
set "UE=C:\Program Files\Epic Games\UE_5.8"
set "PROJECT=%~dp0Project_C.uproject"

tasklist /FI "IMAGENAME eq UnrealEditor.exe" | find /I "UnrealEditor.exe" >nul
if not errorlevel 1 (
    echo El editor ya esta abierto: cerralo y volve a correr este script para recompilar.
    pause
    exit /b 1
)

call "%UE%\Engine\Build\BatchFiles\Build.bat" Project_CEditor Win64 Development -Project="%PROJECT%" -WaitMutex
if errorlevel 1 (
    echo.
    echo No compilo el modulo C++. Hace falta Visual Studio 2022 con
    echo "Desarrollo de juegos con C++". Pasale este error a quien toco Source/.
    pause
    exit /b 1
)

start "" "%UE%\Engine\Binaries\Win64\UnrealEditor.exe" "%PROJECT%"
