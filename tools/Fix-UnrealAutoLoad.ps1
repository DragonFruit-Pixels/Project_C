<#
.SYNOPSIS
    Desarma el auto-load de proyecto de Unreal, que rompe proyectos con modulo C++.

.DESCRIPTION
    Cuando UnrealEditor.exe arranca SIN un .uproject en la linea de comandos (que es lo que
    hace el boton Launch del Epic Games Launcher), el engine intenta reabrir solo el ultimo
    proyecto leyendolo de %LOCALAPPDATA%\UnrealEngine\<version>\Saved\AutoLoadProject.txt.

    Ese camino esta roto para proyectos con modulo C++: el ModuleManager arma su cache de
    rutas antes de que la ruta del proyecto exista, y nunca la invalida. El resultado es
    "The game module 'ProjectC' could not be found".

    Dejando ese archivo vacio y de solo lectura, el engine saltea el auto-load y muestra el
    Project Browser, que si relanza el editor con la ruta correcta.

    De solo lectura es la parte importante: UEditorEngine::InitEditor reescribe ese archivo
    cada vez que un proyecto abre bien, asi que sin el read-only el fix se desarma solo.

    Ver design/architecture/09-abrir-el-proyecto.md.

.PARAMETER Undo
    Revierte: le saca el read-only para que Unreal vuelva a manejarlo.

.EXAMPLE
    powershell -NoProfile -File tools\Fix-UnrealAutoLoad.ps1
#>
[CmdletBinding()]
param(
    [switch]$Undo
)

$ErrorActionPreference = 'Stop'

# Versiones que todavia no existen. Se pre-desarman para que el dia que instales una nueva
# el editor ya la encuentre neutralizada, sin depender de que este script corra antes.
$FutureVersions = @('5.9', '6.0', '6.1', '6.2')

$Root = Join-Path $env:LOCALAPPDATA 'UnrealEngine'
if (-not (Test-Path $Root)) {
    Write-Verbose "No hay $Root. Unreal nunca corrio en este perfil; nada que hacer."
    exit 0
}

# Solo carpetas que parecen version (5.8, 6.0). Descarta Common, Editor, Intermediate y
# cualquier carpeta con nombre de proyecto.
$Installed = Get-ChildItem $Root -Directory -ErrorAction SilentlyContinue |
    Where-Object { $_.Name -match '^\d+\.\d+$' } |
    ForEach-Object { $_.Name }

$Targets = @($Installed) + @($FutureVersions) | Sort-Object -Unique
$Changed = @()

foreach ($Version in $Targets) {
    $SavedDir = Join-Path $Root (Join-Path $Version 'Saved')
    $File     = Join-Path $SavedDir 'AutoLoadProject.txt'
    $IsFuture = $Installed -notcontains $Version

    if ($Undo) {
        if (Test-Path $File) {
            Set-ItemProperty -Path $File -Name IsReadOnly -Value $false
            $Changed += "$Version -> read-only sacado (Unreal vuelve a manejarlo)"
        }
        continue
    }

    # Se crea el Saved si falta. Unreal lo crea igual en el primer arranque, y adelantarse es
    # justo lo que cubre el caso "instalo una version y la abro desde el Launcher antes de que
    # este script haya corrido para ella".
    if (-not (Test-Path $SavedDir)) {
        New-Item -ItemType Directory -Path $SavedDir -Force | Out-Null
    }

    $Existing = Get-Item $File -ErrorAction SilentlyContinue
    if ($Existing -and $Existing.Length -eq 0 -and $Existing.IsReadOnly) {
        continue   # ya esta desarmado
    }

    if ($Existing -and $Existing.IsReadOnly) {
        Set-ItemProperty -Path $File -Name IsReadOnly -Value $false
    }

    [System.IO.File]::WriteAllText($File, '')
    Set-ItemProperty -Path $File -Name IsReadOnly -Value $true

    $Was = if ($Existing) { "tenia $($Existing.Length) bytes" } else { 'no existia' }
    $Tag = if ($IsFuture) { ' (pre-desarmado, version no instalada)' } else { '' }
    $Changed += "$Version -> desarmado ($Was)$Tag"
}

if ($Changed.Count -gt 0) {
    Write-Output "[unreal] auto-load de proyecto desarmado -- ver design/architecture/09-abrir-el-proyecto.md"
    $Changed | ForEach-Object { Write-Output "[unreal]   $_" }
}

exit 0
