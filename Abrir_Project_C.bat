@echo off
REM Abre Project_C pasando el .uproject en la linea de comandos.
REM
REM Eso NO es opcional: un proyecto con modulo C++ que arranca sin esa ruta no
REM encuentra su propio modulo y muere con "The game module 'ProjectC' could not
REM be found". El boton Launch del Epic Games Launcher arranca sin ruta y por eso
REM rompia. Ver design/architecture/09-abrir-el-proyecto.md.
start "" "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0Project_C.uproject"
