# 09 — Abrir el proyecto

Este documento existe porque el proyecto se rompio al abrirse tres veces seguidas, con tres
mensajes distintos, y las tres veces la causa fue la misma. Queda escrito para no volver a
diagnosticarlo.

## La regla

**El editor tiene que arrancar con `Project_C.uproject` en la linea de comandos. Siempre.**

Un proyecto con modulo C++ que arranca sin esa ruta no encuentra su propio modulo y muere.

## Rutas validas

| Como abrir | Pasa la ruta? | Estado |
|---|---|---|
| Acceso directo `Project_C (UE 5.8)` en el escritorio | si | recomendado |
| Doble click en `Project_C.uproject` | si, via `UnrealVersionSelector` | ok |
| `Abrir_Project_C.bat` | si | ok |
| Project Browser -> elegir Project_C | si, el editor se relanza con la ruta | ok |
| **Boton `Launch` del Epic Games Launcher** | **no** | **rompia** |

## Por que rompia

El boton `Launch` del Epic Games Launcher corre `UnrealEditor.exe` sin ningun `.uproject`. El
engine entonces intenta reabrir el ultimo proyecto solo, leyendolo de:

```
%LOCALAPPDATA%\UnrealEngine\5.8\Saved\AutoLoadProject.txt
```

Ese camino esta roto para proyectos con modulo C++, y el orden en
`Engine/Source/Runtime/Launch/Private/LaunchEngineLoop.cpp` explica por que:

| Linea | Que pasa |
|---|---|
| 1838 | `LaunchSetGameName` falla: no hay proyecto en la linea de comandos |
| ~2000-2400 | el `ModuleManager` arma su cache de rutas de modulos, **sin** el directorio de binarios del proyecto |
| 2444 | recien aca `LaunchUpdateMostRecentProjectFile()` setea la ruta del proyecto |
| 6672 | `CheckModuleCompatibility` busca `ProjectC` en la cache vieja y no lo encuentra |

La cache se arma antes de que el proyecto exista y **nunca se invalida**. Comprobado: falla igual
con ruta relativa y con ruta absoluta en `AutoLoadProject.txt`. No hay configuracion del proyecto
que lo arregle, porque el problema es de orden dentro del engine.

Los tres mensajes que tira son el mismo bug en distintos puntos de la recuperacion:

- `The following modules are missing or built with a different engine version: ProjectC`
- `Project_C could not be compiled. Try rebuilding from source manually.`
- `The game module 'ProjectC' could not be found.`

## Por que reaparecia despues de arreglarlo

UE se autoprotege: si el auto-load falla, deja un archivo `AutoLoadProject.txt.InProgress` y no
vuelve a intentar. Pero `UEditorEngine::InitEditor` **borra ese guard y reescribe el `.txt`** cada
vez que un proyecto abre bien.

Es decir: cada compilacion y cada corrida de tests headless rearmaba la trampa. Por eso el sintoma
era "se rompe cada vez que trabajamos".

## El fix

`AutoLoadProject.txt` queda **vacio y de solo lectura** en todas las versiones de UE del perfil.

Vacio, el engine saltea el auto-load (`if (RecentProjectFileContents.Len())`). De solo lectura,
`InitEditor` no lo puede reescribir, asi que el fix no se desarma solo.

Consecuencia: el boton `Launch` del Launcher ahora abre el **Project Browser** en vez de intentar
cargar el ultimo proyecto. Elegir Project_C desde ahi relanza el editor con la ruta y funciona.
Es el mismo comportamiento que el Hub de Unity.

### Que lo mantiene aplicado

| Pieza | Que hace |
|---|---|
| `tools/Fix-UnrealAutoLoad.ps1` | desarma el archivo en **todas** las versiones del perfil, y pre-desarma 5.9/6.0/6.1/6.2 antes de que existan |
| `.claude/hooks/fix-unreal-autoload.sh` | lo corre en cada `SessionStart`, enganchado en `.claude/settings.json` |

El script es idempotente: si ya esta desarmado no dice nada. Solo habla cuando cambio algo, y
dice que encontro.

El pre-desarme de versiones futuras es lo que cubre el caso feo: instalar UE 5.9 y abrirlo desde
el Launcher **antes** de que el hook haya corrido alguna vez para esa version.

### Correr a mano

```powershell
powershell -NoProfile -File tools\Fix-UnrealAutoLoad.ps1
powershell -NoProfile -File tools\Fix-UnrealAutoLoad.ps1 -Undo   # revertir
```

### El hueco que queda

El hook corre cuando se abre una sesion de Claude Code en este repo. Si instalas una version de UE
que no este en la lista de pre-desarme **y** la abris desde el Launcher sin haber abierto Claude
antes, ese primer arranque puede fallar. Se arregla corriendo el script y volviendo a abrir.

Para cerrar ese hueco del todo haria falta una tarea programada de Windows al iniciar sesion.
Se evaluo y se descarto: no valia un cambio a nivel maquina por ese caso.

## Verificado

| Chequeo | Resultado |
|---|---|
| `UnrealEditor.exe` sin argumentos | abre Project Browser, 0 warnings de modulo |
| `UnrealEditor.exe "...\Project_C.uproject"` | editor arriba en 24 s, 0 warnings |
| `UnrealEditor-ProjectC.dll` | cargado desde `Project_C/Binaries/Win64/` |
| `AutoLoadProject.txt` tras abrir bien | sigue en 0 bytes y read-only |
| script sobre las 7 versiones del perfil | 5.1, 5.5, 5.8, 5.9, 6.0, 6.1, 6.2 en 0 bytes y read-only |
| segunda corrida del script | silenciosa (idempotente) |
| trampa rearmada a mano y hook corrido | la detecto y la desarmo, reportando `5.8 -> desarmado (tenia 73 bytes)` |
| `.claude/settings.json` | parsea, y el hook aparece 5to en `SessionStart` |

## Registro del engine

Aparte, `HKLM\SOFTWARE\EpicGames\Unreal Engine` tenia solo una entrada muerta `4.0` apuntando a una
carpeta inexistente, y ninguna `5.8`. Por eso el doble click sobre el `.uproject` fallaba: el
`UnrealVersionSelector` no podia resolver `"EngineAssociation": "5.8"`. Quedo registrado:

```
5.8 -> C:\Program Files\Epic Games\UE_5.8
```

## Si vuelve a pasar

1. Mirar `%LOCALAPPDATA%\UnrealEngine\5.8\Saved\Logs\` — si hay un log ahi, el editor arranco **sin**
   proyecto y ese es el problema. Los arranques sanos loguean en `Project_C\Saved\Logs\`.
2. Buscar `Loading recent project file` en ese log. Si aparece, el auto-load se rearmo:
   `powershell -NoProfile -File tools\Fix-UnrealAutoLoad.ps1` y volver a abrir.
3. Nunca es el codigo del juego. Antes de tocar `Source/`, descartar esto.
