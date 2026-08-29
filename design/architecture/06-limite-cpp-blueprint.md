# ⚙️ El límite C++ / Blueprint

← [Índice](README.md)

**Decisión: arquitectura híbrida.** C++ para el comportamiento y los sistemas, Blueprint para el
contenido, el cableado y las perillas. Registrada como
[D-13](../gdd/06-decisiones/registro.md).

Y una aclaración que importa más que la decisión: **"todo en C++" no es la opción más
profesional en Unreal, es un error conocido.** Un proyecto de UE con una capa de C++ que los
Blueprints no pueden alcanzar es *menos* mantenible, no más: los artistas y diseñadores quedan
afuera, cada ajuste de balance pasa por una recompilación, y se pierde exactamente la ventaja
que hace que la gente elija Unreal.

La práctica profesional real —la de Epic y la de los juegos que salieron con este engine— es un
límite declarado. Este documento lo declara.

---

## El patrón central: base en C++, hijo en Blueprint

> **Toda clase de C++ que el contenido toca tiene una subclase Blueprint. El C++ tiene el
> comportamiento, el Blueprint tiene los datos y el cableado.**

`AProjectCCharacter` en C++ define qué es un personaje y cómo funciona; `BP_Character_01` a
`BP_Character_04` son hijos que traen su mesh, su `Ratchet Card`, sus valores iniciales y sus
referencias de arte. Nadie edita C++ para agregar un personaje.

De ahí sale la regla de exposición, que no es opcional:

- Todo lo que el contenido configure va con `UPROPERTY(EditAnywhere, BlueprintReadWrite)`.
- Todo lo que el Blueprint tenga que llamar va con `UFUNCTION(BlueprintCallable)`.
- Todo lo que el Blueprint tenga que escuchar va como delegate asignable
  (`BlueprintAssignable`), que es el Event Dispatcher visto desde C++.

Si una clase de C++ no expone nada, o es un servicio interno, o está mal diseñada.

## Dónde va cada cosa

| Va en **C++** | Por qué |
|---|---|
| Las clases base del framework: `GameInstance`, `GameMode`, `GameState`, `PlayerController`, `Character` | son el contrato del proyecto. Reparentar un Blueprint es doloroso; agregar un campo a una clase de C++ es una línea |
| **Subsystems** (servicios) | no hay alternativa: no se pueden crear en Blueprint. Verificado, ver [`02`](02-managers-y-subsystems.md) |
| `USTRUCT` de filas de Data Table y clases de Data Asset | un struct definido en Blueprint es **frágil**: cambiarlo puede resetear las filas de las tablas que lo usan. En C++ es refactor normal |
| Algoritmos: BFS del grafo, distancias, resolución de tiradas | están en un loop y tienen que ser testeables |
| El **ejecutor de efectos** de cartas | es un intérprete. Un intérprete en nodos es ilegible a la tercera carta |
| Interfaces (`UINTERFACE`) | así las implementan tanto C++ como Blueprint |
| Gameplay Tags nativos | declarados en código, autocompletan y no se escriben mal |
| La **máquina de resolución** (`UResolutionSubsystem`) y sus structs (`FResolutionStep`, `FPendingChoice`) | es el ejecutor común de cartas, dados y acciones, y tiene que ser **serializable** para que el save funcione a mitad de una tirada. Ver [`07`](07-resolucion-y-pausa.md) |
| La **evaluación** de los modificadores de reglas permanentes | un efecto permanente de stage no se ejecuta: **se consulta** al armar cada `FRollRequest`. Es donde los Gameplay Tags nativos empiezan a pagar |
| El reproductor de presentación que **emite los eventos tipados** | lee el feed de pasos y dispara `OnFigureMoved`, `OnDiceSettled`, `OnStageRevealed`. Ver [`08`](08-presentacion-y-reglas.md) |
| **Tests de automatización** | ver abajo. Es lo más grande que se gana |

| Va en **Blueprint** | Por qué |
|---|---|
| Subclases de contenido: `BP_Character_*`, `BP_Enemy_*`, `BP_Space`, `BP_Gate` | son datos y wiring, no comportamiento |
| Animation Blueprints, state machines, blendspaces | clase 6 del temario, y son Blueprint por naturaleza |
| Widgets UMG: layout y binding | sobre una base `UUserWidget` de C++ cuando tengan lógica |
| Behaviour Trees y EQS | son assets de autor. Las tareas y servicios *custom* sí van en C++ |
| Sequencer, Niagara, materiales, Landscape | no hay versión de código y no la queremos |
| Actores colocados en el nivel y sus perillas | `UPROPERTY(EditAnywhere)` y se tunea sin compilar |
| Las **2 acciones propias de cada misión** (`UMissionAction` hija) | el escenario las define. Si necesitaran C++, cada misión nueva sería una compilación y un programador |
| Qué **se ve** al recibir un evento de presentación: animación, VFX, sonido, escalonado | es ritmo visual. Es justo lo que un diseñador tiene que poder tocar sin compilar |
| **Prototipos** | se prototipa en Blueprint y **se baja a C++ cuando la forma se estabilizó**. Ese camino de ida es la práctica normal, no una derrota |

## Los tres límites que hay que declarar

Las tablas de arriba alcanzan para la mayoría de los casos. Estos tres no se deducen de ellas y
son los que se rompen solos si no están escritos.

### 1. Blueprint **compone** pasos; no **define** tipos de paso

`UMissionAction` es `Blueprintable` para que un escenario pueda traer sus 2 acciones propias sin
compilar. Las 4 base —`Move`, `Attack`, `Recover`, `Trade`— van en C++, porque son reglas
centrales y necesitan tests.

El límite que hace que eso no degenere:

> Un Blueprint arma su resolución llamando **constructores de paso** expuestos como funciones
> `BlueprintCallable` estáticas. Si una acción necesita algo que el vocabulario de pasos no
> expresa, **eso es un tipo de paso nuevo y va en C++**.

Sin esa frase, la primera acción de misión que necesite algo raro se lo escribe adentro del
grafo, y la mitad de las reglas termina en Blueprints que no se testean, no se mergean y no se
revisan. Es la misma disciplina que ya vale para el vocabulario de efectos, un nivel más abajo.

Cada acción expone tres cosas: **costo** (0 para libres, 1 para normales y especiales),
**condición de legalidad** y **resolución**. La legalidad va como `BlueprintNativeEvent` y la
consulta el HUD para apagar el botón — así la regla "`Recover` solo en espacio `Clear`" se
escribe una vez y no se desincroniza con la UI.

### 2. El vocabulario de efectos es **cerrado**, así que es dato y no clase

Los selectores, condiciones y acciones están catalogados desde evidencia de cartas reales en
[`vocabulario-de-efectos.md`](../gdd/03-resolucion/vocabulario-de-efectos.md). Un vocabulario
cerrado se modela como `USTRUCT` + enum en C++, con las cartas como **filas de Data Table**.

La alternativa —una clase Blueprint por tipo de efecto— solo gana si el vocabulario fuera
abierto, y no lo es. Y perdería lo que más rinde de todo el diseño: que una carta nueva sea una
fila y no un asset.

### 3. La presentación parte en el evento tipado

El reproductor en C++ lee el feed de pasos y **emite eventos tipados**; el Blueprint decide qué
se ve al recibirlos. Esa es la costura, y ponerla en otro lado tiene costos concretos: más
arriba, el ritmo visual necesita recompilar; más abajo, las reglas empiezan a conocer widgets.

## Las tres cosas que C++ habilita y Blueprint no puede

### 1. Los servicios reales

La capa de servicios deja de ser "hospedada" en el `GameState` y pasa a ser lo que
corresponde: `UGraphSubsystem` como `UWorldSubsystem`, `URandomSubsystem` y `USaveSubsystem`
como `UGameInstanceSubsystem`. Sin workaround y sin migración pendiente para la clase 14.
Detalle en [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md).

### 2. Tests automatizados de las reglas

**Un grafo de Blueprint no se puede testear.** Con C++ sí, con el framework de automatización
del propio engine, y esto tiene un valor concreto y medible en este proyecto: los documentos
de sistema del GDD ya traen **fórmulas** y **criterios de aceptación** escritos, y hoy no hay
forma de verificar ninguno.

Los cuatro primeros tests, que se escriben antes que la UI:

| Test | Verifica | Estado |
|---|---|---|
| `ProjectC.Rules.Ratchet.Thresholds` | los 6 umbrales en 4/8/12/15/18/19, el espaciado 4-4-4-3-3-1, que el último esté a 1 de la muerte, y que un umbral no se cruce dos veces | ✅ **pasa** |
| `ProjectC.Rules.Ratchet.AdvanceAndLoss` | el clamp al final del track, que la casilla 19 no mate y la 20 sí, y que el trinquete **solo suba** | ✅ **pasa** |
| Distribución del dado | `p(Hit)=0,50`, `p(Toll)=0,33`, y que el dado bonus **no** tenga `Toll` | pendiente, falta el sistema |
| Costo esperado de una tirada | `E[ΔRatchet] = 3 × 0,33 = 1,0` | pendiente |
| BFS con aristas bloqueadas | que romper una pared cambie las distancias | pendiente, falta `UGraphSubsystem` |

Los números salen de
[`perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md) y
[`trinquete.md`](../gdd/02-personaje/trinquete.md).

**Y los dos tests que existen se verificaron rompiéndolos a propósito.** Se cambió `IsLost` de
`Position >= TrackLength` a `>`, se recompiló, y el test falló con el mensaje y la línea exactos
(`Expected 'la casilla 20 mata' to be true`) y exit code 255; después se revirtió y volvió a
verde. Un test que pasa pero no puede fallar no prueba nada, así que la mutación es parte de
escribirlo, no un extra.

Eso convierte los criterios de aceptación del GDD de párrafos en un semáforo. Es la diferencia
más grande entre "un proyecto de materia" y "un proyecto profesional", y no cuesta arte ni
diseño.

### 3. Merges que funcionan

**El `.cpp` es texto: mergea.** El riesgo número uno registrado para el trabajo grupal es que
los `.uasset` son binarios y un conflicto borra el trabajo de alguien
([`course-alignment.md`](../course-alignment.md)).

Mover el comportamiento de los grafos al código no lo mitiga: lo **elimina** para todo lo que
se mueve. Dos personas tocando el mismo sistema en el mismo día pasan de "una pierde el
trabajo" a "git resuelve el merge". Y de paso habilita code review, que sobre un grafo de
Blueprint no existe.

Este es, en la práctica, el argumento más fuerte de los tres.

## Lo que cuesta

### El toolchain · **resuelto el 2026-08-20**

Faltaba el compilador: Visual Studio 2022 Community estaba instalado **sin** el workload de C++
—`VC/` solo tenía `Auxiliary` y `Redist`, y de Windows SDK solo estaba el 8.1. Se agregó el
workload *Game development with C++* con el instalador en modo comando:

```
setup.exe modify --installPath "...\2022\Community" \
  --add Microsoft.VisualStudio.Workload.NativeGame --includeRecommended --passive --norestart
```

Estado verificado después de instalar:

| Componente | Estado |
|---|---|
| Visual Studio 2022 Community | ✅ con workload `NativeGame` registrado |
| Compilador MSVC | ✅ **14.44.35207** — `cl.exe` reporta `19.44.35227 for x64` |
| Windows SDK | ✅ **10.0.26100.0** |
| Detección por `vswhere` | ✅ `VC.Tools.x86.x64` resuelve al install path |
| UnrealBuildTool | ✅ viene con el engine |
| `.gitignore` | ✅ ignora `Binaries/`, `Intermediate/`, `Build/`; versiona `Source/` |

**Probado de punta a punta**, no solo por presencia de archivos: se compiló y corrió un programa
mínimo que incluye `<windows.h>` y `<cstdio>` con `vcvars64` + `cl`. Compila, linkea y ejecuta.
O sea que están el compilador, los headers del SDK y el linker — que es exactamente lo que UBT
necesita.

**Cada máquina que abra el proyecto necesita lo mismo**, porque un proyecto con módulo C++
compila al abrirse. El comando de arriba es reproducible y no requiere clickear el instalador.

### El resto del costo

- **Tiempos de compilación.** El primer build de un módulo vacío son minutos; después, con Live
  Coding, un cambio en un `.cpp` son segundos.
- **La regla de Live Coding:** cambios en `.cpp`, en caliente. Cambios en headers, en
  `UPROPERTY`, o clases nuevas → cerrar el editor y compilar. Ignorar esto es la causa número
  uno de crashes "inexplicables" del editor.
- **Reparentar después es peor que empezar así.** Cambiarle el padre a un Blueprint que ya
  existe puede perder variables y referencias.

### Por eso el momento es ahora

`Content/` está vacío: **cero Blueprints que reparentar**. El costo de adoptar C++ es el mínimo
que va a tener en toda la vida del proyecto, y sube con cada Blueprint que se cree antes de que
el módulo exista.

## La estructura del módulo · **existe y compila**

```
Source/
  Project_C.Target.cs          TargetType.Game
  Project_CEditor.Target.cs    TargetType.Editor
  ProjectC/
    ProjectC.Build.cs
    Public/
      ProjectC.h
      Core/         ProjectCGameInstance, MissionGameMode, MissionGameState, MissionPlayerController
      Characters/   ProjectCCharacter, RatchetComponent
      Rules/        RatchetRules
    Private/
      ProjectC.cpp
      Core/ Characters/ Rules/
      Tests/        RatchetRulesTest.cpp
```

Nombre del módulo **`ProjectC`** sin guión bajo (macro `PROJECTC_API`), aunque el proyecto se
llame `Project_C`: los nombres de módulo de Epic son alfanuméricos y el guión bajo trae fricción
en las macros generadas. El `.uproject` ganó su array `Modules`.

### Por qué `Public/` y `Private/` y no todo plano

Esto no es preferencia estética, es un requisito, y lo aprendí rompiéndolo. El primer intento usó
el layout plano del template (`Source/ProjectC/Core/MissionGameMode.h`) y **falló a compilar**:

```
fatal error C1083: Cannot open include file: 'Core/MissionGameMode.h'
```

La causa, leída del response file que generó UBT y no adivinada: **el único include path del
módulo era el de UHT.**

```
/I "…/Intermediate/Build/Win64/UnrealEditor/Inc/ProjectC/UHT"
```

Con `bLegacyPublicIncludePaths = false` —el default desde `BuildSettingsVersion.V2`— UBT agrega
como include path **solo** `Public/`, `Internal/` y `Private/`. Con layout plano no agrega el
directorio del módulo, así que un `#include "Core/X.h"` no tiene desde dónde resolver.

Y explica por qué el template en blanco *parece* funcionar plano: sus `.cpp` incluyen headers que
están **al lado**, y un include con comillas resuelve primero relativo al archivo que incluye. En
cuanto aparecen subcarpetas, se cae.

Con `Public/` + `Private/`, `#include "Core/MissionGameMode.h"` resuelve desde `Public/`. Es
además el layout canónico de cualquier módulo de Epic con macro `_API`.

## Cómo se compila y cómo se corren los tests

Comandos verificados en esta máquina, no copiados de un tutorial.

**Compilar el editor:**

```
"…\UE_5.8\Engine\Build\BatchFiles\Build.bat" Project_CEditor Win64 Development ^
  -Project="…\Project_C.uproject" -WaitMutex
```

**Correr los tests, headless, sin abrir el editor:**

```
"…\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "…\Project_C.uproject" ^
  -ExecCmds="Automation RunTests ProjectC.Rules; Quit" ^
  -unattended -nopause -nosplash -NullRHI -stdout
```

Devuelve **exit code 0** si pasan y **255** si alguno falla, así que sirve tal cual para CI.

**Pedirle a UBT el modelo de proyecto para el IDE.** No hace falta a mano: Rider se lo pide solo
al abrir el `.uproject`. Sirve para verificar que el toolchain está sano.

```
"…\Engine\Binaries\ThirdParty\DotNet\10.0\win-x64\dotnet.exe" ^
  "…\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" ^
  -projectfiles -project="…\Project_C.uproject" -game -Rider -progress
```

> **Por qué el `dotnet.exe` del engine y no `UnrealBuildTool.exe` directo.** Llamar al `.exe`
> falla: pide el runtime de .NET 10 y en esta máquina hay 8.0.13 y 8.0.21. El engine **trae** su
> propio .NET 10 en `Engine/Binaries/ThirdParty/DotNet/10.0/win-x64/`, y es lo que usa `Build.bat`
> por dentro — por eso compilar funciona y llamar a UBT a mano no.

## El IDE: Rider, abriendo el `.uproject`

**Se abre `Project_C.uproject`. No la carpeta, no un `.sln`.** No es una preferencia de estilo:
es lo que decide si Rider carga un proyecto de Unreal o cualquier otra cosa.

Abriendo el `.uproject`, Rider le pide el modelo a UBT (generador `-Rider`, que escribe JSON en
`Intermediate/`) y lo mantiene al día solo. **No hay `.sln` que generar ni project files que
regenerar** al agregar un `.cpp` o tocar un `.Build.cs` — que era justamente el impuesto del
esquema anterior.

**El accessor del editor se llama `Rider Uproject`**, y el nombre no es adivinable. El plugin
`RiderSourceCodeAccess` —que viene con el engine, con `EnabledByDefault: true`, así que no hay que
tocar el `.uproject`— registra varios accessors con nombre generado en tiempo de arranque
(`RiderSourceCodeAccessorModule.cpp`):

| Accessor | `FName` |
|---|---|
| Agregado, modelo `.uproject` | `Rider Uproject` |
| Agregado, modelo `.sln` | `Rider` — y se autodesactiva si el `.sln` no existe |
| Uno por instalación encontrada | `Rider 2025.x (installed)`, `… (toolbox)` |

Y cuidado con **dónde** vive ese setting. `Config/DefaultEditorSettings.ini` es solo la semilla
para una máquina nueva; una vez que el editor arrancó manda el `EditorSettings.ini` de
`%LOCALAPPDATA%\UnrealEngine\5.8\Saved\Config\WindowsEditor\`, que es global a todos los proyectos
y es lo que edita Editor Preferences > Source Code.

**RiderLink** es la integración editor↔IDE: Blueprints navegables desde el IDE y el log del editor
dentro de Rider. Rider la ofrece instalar sola la primera vez. Conviene instalarla en el **Engine**
y no en el Game: en el Game aparece un `Plugins/RiderLink/` dentro del repo y hay que decidir si se
versiona.

### Lo que dejó CLion, para que no vuelva a pasar

El esquema anterior generaba un `CMakeLists.txt` en la raíz con `-CLion` (que es `-CMakefile`
disfrazado: `CLionGenerator` hereda de `CMakefileGenerator` y no agrega nada — el comentario de
Epic dice que existe *"only here for UBT to match against"*). Dos cosas de ahí valen como cicatriz:

- **Ese `CMakeLists.txt` es lo que rompió el cambio de IDE.** Mientras estuvo en la raíz, Rider
  abría la carpeta como proyecto **CMake**: sin targets de UBT, sin configuraciones de build, sin
  integración de Blueprints. El síntoma es "no me buildea la solución", y la causa es que no había
  solución — había un proyecto CMake.
- **El configure arrastraba el árbol de fuentes del engine entero.** `cmake-build-debug-visual-studio`
  llegó a **63 GB**, con un `build.ninja` de 31.9 GB en un solo archivo.

Los dos directorios y el `CMakeLists.txt` se borraron el 2026-08-29. El `.gitignore` los sigue
ignorando por si alguien corre un `-CMakefile` a mano.

## Lo que **no** adoptamos, y por qué

Ser profesional también es no traer framework que no hace falta.

| Tecnología | Veredicto | Razón |
|---|---|---|
| **GAS** (Gameplay Ability System) | **no** | Está construido alrededor de predicción de cliente y ejecución en tiempo real. Este juego es por turnos, determinista y single player: la mitad de GAS no aplica, y la otra mitad —modificadores de atributos— son 200 líneas propias contra un framework enorme y opaco de debuggear. Las cartas de este juego son un intérprete de dos ramas con "un paso que no se puede ejecutar se saltea": eso pelea con la composición de `GameplayEffect`, no la aprovecha |
| **Gameplay Tags** | **sí** | Ya viene con el engine, no es plugin, y es el vocabulario que hace el core agnóstico al tema |
| **StateTree** | **no por ahora** | Las fases del turno son una máquina de estados chica y explícita. StateTree paga cuando hay muchas ramas de autor |
| **MassEntity** | **no** | Es para miles de agentes. El pool de `Servant` tiene techo 10 |
| **CommonUI** | **a evaluar** | Aporta ruteo de input y estilos consistentes. Decidir al empezar la clase 7, con el HUD ya especificado |

La entrada de GAS es la más discutible de la tabla. Si aparece un argumento fuerte a favor,
vale reabrirla — pero la carga de la prueba está del lado de sumarlo.

### GAS, concepto por concepto · el mapeo

**No usarlo no significa no conocerlo.** GAS resuelve problemas reales y este juego tiene varios
de ellos — sólo que ya están resueltos a mano, y a propósito. El mapeo:

| Concepto de GAS | Lo que hay acá | Por qué la versión propia |
|---|---|---|
| `UAttributeSet` | `UWoundsComponent`, `UReserveComponent`, `URatchetComponent` | son tres números con clamp. Un `AttributeSet` traería el `AbilitySystemComponent` entero detrás |
| `GameplayEffect` infinito con modificadores | **`FRuleMod`** — los efectos permanentes de los stages ([D-19](../gdd/06-decisiones/registro.md)) | un tag + una operación + un valor. La consulta es una función, no una pila de agregación replicada |
| `GameplayAbility` con costo y condición | `UMissionAction` — costo en acciones, condición de legalidad, resolución | la legalidad la consulta el HUD para apagar el botón; en GAS eso es `CanActivateAbility` más plomería de UI |
| `GameplayTags` | **adoptados**, sin discusión | vienen con el engine y no arrastran el framework |
| `AbilityTask` — una habilidad que dura y espera | la **pila de resolución** ([`07`](07-resolucion-y-pausa.md)) | acá es la pieza central, no un caso especial |

**Dónde se rompería el mapeo si lo forzáramos.** El modelo de ejecución de GAS es
instant / duration / periodic sobre atributos. No expresa *"una carta que se resuelve paso a
paso, de arriba hacia abajo, donde un paso que no se puede ejecutar **se saltea** y la carta
sigue"* — que es la semántica que vale para todo el juego y que hay que implementar una sola vez
([`turno.md`](../gdd/03-resolucion/turno.md)). Esa regla pelea con la composición de
`GameplayEffect` en vez de aprovecharla.

Y la mitad del peso de GAS —predicción de cliente, replicación de atributos, reconciliación— es
maquinaria para un problema que este juego no tiene: es por turnos, determinista y single player.

## Qué cambia de lo ya escrito

| Documento | Qué cambió |
|---|---|
| [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md) | los servicios pasan a ser Subsystems reales; la sección de hospedaje queda como historia de por qué existe el límite |
| [`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md) | se agrega el paso del módulo, **antes** del esqueleto |
| [`04-mapa-de-clases.md`](04-mapa-de-clases.md) | cada fila dice si es clase de C++ o subclase de Blueprint |
| [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md) | C++ deja de ser el último tema opcional y pasa a ser el primero |
| [`../course-alignment.md`](../course-alignment.md) | cae la restricción "Blueprint-only" |

## Dependencias

- Consume: [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md),
  [`07-resolucion-y-pausa.md`](07-resolucion-y-pausa.md),
  [`08-presentacion-y-reglas.md`](08-presentacion-y-reglas.md),
  [`../gdd/07-balance/perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md)
- Alimenta: todos los documentos de esta carpeta
- Registro: [D-13](../gdd/06-decisiones/registro.md)
