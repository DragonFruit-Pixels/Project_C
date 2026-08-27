# Port map — MCPs locales

Convencion de puertos de esta maquina. Leerla **antes** de registrar un MCP nuevo o de cambiar el puerto de uno existente, para no re-descubrir el mapa cada sesion.

Regla general: **si un MCP puede correr por stdio, corre por stdio.** Un puerto solo se justifica cuando el server tiene que vivir fuera del ciclo de vida del cliente (varias instancias de Claude Code contra el mismo proceso, acceso desde otra maquina, o un editor que hostea el server el mismo, como Unreal y Unity).

## Asignacion

| Puerto | Server | Transport | Estado |
|---|---|---|---|
| `8000` | `unreal-mcp` — editor de Project_C | http, `/mcp` | **asignado** |
| `8001`–`8009` | instancias adicionales del editor UE | http, `/mcp` | reservado |
| `8010` | `meshy` **si alguna vez pasa a HTTP** | http, `/mcp` | reservado, sin usar |
| `8080` | `mcp-for-unity` — proyecto `Unity/Tesis-UADE-2026` | http | ocupado, ajeno a este repo |
| `8090` | reservado por el usuario | — | **no usar** |
| `9010`, `9012`, `9013` | procesos ajenos | — | ocupados |

Evitar el rango dinamico de Windows (`49152`+, 16384 puertos) y las exclusiones de Hyper-V/WSL. Chequear ambas cosas antes de elegir un puerto nuevo:

```powershell
netsh int ipv4 show dynamicport tcp
netsh int ipv4 show excludedportrange protocol=tcp
Get-NetTCPConnection -State Listen | Select-Object -Expand LocalPort | Sort-Object -Unique
```

Un puerto excluido da "address in use" **sin que nada aparezca escuchando** — es el falso positivo clasico en Windows.

## meshy — stdio, sin puerto (decidido)

`@meshy-ai/meshy-mcp-server` corre por stdio y esa es la configuracion vigente en `.mcp.json`. Claude Code spawnea y mata el proceso solo; no abre socket ni compite con nadie.

El paquete **si** soporta HTTP, pero no se usa. Si en algun momento hiciera falta (compartir un solo server entre clientes, o llegar desde otra maquina):

- Se activa con las env vars `TRANSPORT=http` y `PORT=<puerto>`. No hay flags de CLI.
- Su default es `3000` — pisado por cualquier dev server de Node. Usar `8010` (ver tabla).
- Expone `POST /mcp` y `GET /health`. El cliente pasa a `{"type":"http","url":"http://127.0.0.1:8010/mcp"}` y el proceso hay que levantarlo a mano **antes** de abrir Claude Code.
- **Riesgo real:** el `app.listen(port)` del paquete bindea a `0.0.0.0`, no a loopback, y `/mcp` no tiene auth. Del otro lado hay una API key con credito pagado — un `text_to_3d` ajeno cuesta 30 creditos reales. Si se activa HTTP: regla de firewall inbound para ese puerto, o un proxy que bindee a `127.0.0.1`.

## unreal-mcp — un puerto por instancia de editor

El server vive dentro del editor, asi que dos instancias de UE abiertas pelean por `8000` y la segunda falla con `Failed to listen on port`. Asignar un puerto del bloque reservado por proyecto:

```ini
; <Project>/Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini
[/Script/ModelContextProtocolEngine.ModelContextProtocolSettings]
bAutoStartServer=True
ServerPortNumber=8001
ServerUrlPath=/mcp
```

Despues, `ModelContextProtocol.GenerateClientConfig ClaudeCode` en la consola del editor: mergea sobre el `.mcp.json` existente sin pisar las otras entradas. El `.ini` es per-user y no va a source control, asi que cada maquina mantiene su propio mapa.

Para una prueba puntual alcanza `-ModelContextProtocolPort=<port>` en el launch, pero no persiste.

## Al agregar un MCP nuevo

1. Preguntarse si necesita puerto. Casi siempre la respuesta es no → stdio.
2. Si lo necesita, elegir del bloque `80xx`, verificar con los tres comandos de arriba, y **anotarlo en la tabla de este archivo en el mismo turno**.
3. Nunca reusar `8080` ni `8090`.
