---
name: clickup
description: "ClickUp integration: create/update/search tasks, docs, time entries and spaces via MCP tools."
category: "integrations"
argument-hint: "[task|doc|time] <action> <args>"
user-invocable: true
allowed-tools: Read, Glob, Grep, Bash, Write, Edit
---
# ClickUp Manager

Skill que usa el MCP nativo de ClickUp para gestión completa de tasks, docs y tiempo.

## Tools MCP Disponibles

El server es el conector de claude.ai, y **las tools vienen diferidas**: hay que cargarlas
con **una sola** llamada a `ToolSearch` antes de usarlas. Ejemplo:

```
ToolSearch query "select:mcp__claude_ai_ClickUp__clickup_search,mcp__claude_ai_ClickUp__clickup_create_task"
```

Todas llevan el prefijo `mcp__claude_ai_ClickUp__`:

| Tool | Para qué |
|---|---|
| `clickup_get_workspace_hierarchy` | spaces, folders y lists del workspace |
| `clickup_search` | búsqueda por texto en todo el workspace (paginada, seguir `next_cursor`) |
| `clickup_filter_tasks` | filtrar tasks por status, priority, tags o fechas |
| `clickup_get_task` | detalle completo de una task |
| `clickup_create_task` | crear (requiere `list_id`) |
| `clickup_update_task` | status, campos, assignees |
| `clickup_create_comment` | comentar |
| `clickup_get_task_comments` | leer comentarios |
| `clickup_create_document` | crear doc (`parent.type`: `4`=space, `5`=folder, `6`=list) |
| `clickup_create_document_page` | crear página (`content_format: "text/md"`) |
| `clickup_list_document_pages` | estructura del doc, sin contenido |
| `clickup_get_document_pages` | contenido de las páginas |
| `clickup_update_document_page` | actualizar una página |
| `clickup_get_folder`, `clickup_get_list` | resolver nombres a IDs |
| `clickup_create_list`, `clickup_create_list_in_folder` | crear listas |
| `clickup_start_time_tracking`, `clickup_stop_time_tracking`, `clickup_get_time_entries` | time tracking |
| `clickup_resolve_assignees`, `clickup_get_workspace_members` | resolver personas a user IDs |

## Spaces conocidos

| Space | ID |
|-------|-----|
| Project_C | 1000270000004350 |
| Project_T | 90138713959 |

### Project_C — estructura

| Contenedor | ID |
|---|---|
| 🎮 Development (folder) | 1000270000007217 |
| ↳ Fase 1 — 1er Parcial (24/9) | 1000270000008736 |
| ↳ Fase 2 — 2do Parcial (12/11) | 1000270000008737 |
| 🎨 Art (folder) | 1000270000007218 |
| 📚 Narrative (folder) | 1000270000007219 |
| 🔬 QA / Testing (folder) | 1000270000007220 |
| Doc `GDD — Project_C` | 2ky4j7vt-8099 |

> El GDD de ClickUp tiene que quedar **1:1 con `design/gdd/gdd.md` del repo**. El del repo
> es la fuente de verdad; el de ClickUp es la versión legible para el equipo. Si cambia uno,
> cambia el otro en el mismo turno.

## Comandos

### Crear task
```
/clickup create [space] "título de la task"
```
Preguntá:
- Lista destino (si hay múltiples en el space)
- Tipo (feature/bug/task)
- Assignee
- Priority
- Due date
- Descripción (markdown soportado)

### Buscar tasks
```
/clickup search [query]
```
Filtros posibles:
- por space, list, assignee, status, priority
- por texto en title/description
- por date range (created/updated/due)

Devolver tabla:
| ID | Title | Status | Assignee | Due |

### Actualizar task
```
/clickup update <task_id>
```
Cambios comunes:
- Status (Open, In Progress, Review, Done, Archived)
- Assignee (uno o múltiples)
- Priority (Urgent/High/Normal/Low)
- Due date
- Custom fields

### Comentar
```
/clickup comment <task_id> "texto"
```

### Docs
```
/clickup doc create [space] "título"
/clickup doc read <doc_id>
/clickup doc update <doc_id>
```

### Time tracking
```
/clickup time start <task_id>
/clickup time stop <task_id>
/clickup time report [period]
```

## Flujos Típicos

### Daily triage
1. Listar tasks asignadas a mí en estado "Open" o "In Progress"
2. Agrupá por priority
3. Sugerí reordenamiento si hay misalignment

### Crear feature con contexto completo
1. User describe feature
2. Detectá space target (por keywords o preguntá)
3. Buscá tasks similares para evitar duplicados
4. Generá descripción estructurada:
   ```markdown
   ## Contexto
   [por qué se necesita]

   ## Descripción
   [qué hace]

   ## Acceptance criteria
   - [ ] Criterio 1
   - [ ] Criterio 2

   ## Referencias
   - [links a docs, designs, tickets relacionados]
   ```
5. Pedí confirmación antes de crear
6. Crear task, devolver URL

### Bulk update
Para actualizar múltiples tasks:
1. Listá las targeteadas (con filtros)
2. Mostrá preview del cambio
3. Pedí confirmación
4. Ejecutá uno por uno con rate limiting

## Reglas

- **SIEMPRE** confirmar antes de crear/actualizar/eliminar
- **NO** crear duplicados — buscar primero
- **NO** cambiar assignees sin saber quién está en el space
- **Rate limit**: max 10 operaciones consecutivas, después pausar
- Descripción siempre en markdown estructurado
- En español
