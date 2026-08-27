#!/usr/bin/env bash
# Desarma el auto-load de proyecto de Unreal al empezar la sesion.
#
# Sin esto, abrir el editor desde el boton Launch del Epic Games Launcher rompe con
# "The game module 'ProjectC' could not be found". El fix se desarma solo cada vez que
# el editor abre bien, asi que hay que reaplicarlo.
#
# Ver design/architecture/09-abrir-el-proyecto.md.

set -u

SCRIPT="tools/Fix-UnrealAutoLoad.ps1"
[ -f "$SCRIPT" ] || exit 0

# PowerShell quiere ruta de Windows.
if command -v cygpath >/dev/null 2>&1; then
    SCRIPT="$(cygpath -w "$(pwd)/$SCRIPT")"
fi

# Nunca frenar la sesion por esto: si PowerShell no esta o falla, se sigue igual.
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "$SCRIPT" 2>/dev/null || true

exit 0
