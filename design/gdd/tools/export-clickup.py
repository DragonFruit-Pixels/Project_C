# Genera el export del GDD para ClickUp, y el plan de llamadas a la API.
#
# Existe porque el GDD del repo es la fuente de verdad y el doc de ClickUp tiene que quedar
# 1:1. Cada vez que el GDD cambia, se vuelve a correr esto en vez de rearmar el mapeo a mano.
#
#   python design/gdd/tools/export-clickup.py
#
# Produce, en design/gdd/tools/_export/:
#   clickup-pages.md   -> las 19 paginas, con separadores, listo para copiar y pegar
#   plan-api.md        -> que llamada exacta hace falta por pagina (update vs create)
#
# Transformaciones aplicadas (el contenido queda 1:1, la navegacion no):
#   - se saca la linea de navegacion "<- Indice", que en ClickUp no lleva a ningun lado
#   - los links relativos .md pasan a texto en negrita: en ClickUp darian 404
#   - los links http se dejan intactos
import io, os, re, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
GDD = os.path.join(ROOT, 'design', 'gdd')
OUT = os.path.join(GDD, 'tools', '_export')

DOC_ID = '2ky4j7vt-8099'

# (numero, nombre de pagina en ClickUp, archivo fuente, page_id a reusar o None para crear)
PAGES = [
    ('01', 'Indice',                   'README.md',                                  None),
    ('02', 'Concepto',                 '00-vision/concepto.md',                      '2ky4j7vt-9119'),
    ('03', 'Alcance',                  '00-vision/alcance.md',                       None),
    ('04', 'Vocabulario neutro',       '01-fundamentos/vocabulario.md',              '2ky4j7vt-9219'),
    ('05', 'Mapa y espacios',          '01-fundamentos/mapa-y-espacios.md',          '2ky4j7vt-9279'),
    ('06', 'Barras y recursos',        '02-personaje/barras-y-recursos.md',          '2ky4j7vt-9079'),
    ('07', 'Trinquete',                '02-personaje/trinquete.md',                  '2ky4j7vt-9199'),
    ('08', 'Skills',                   '02-personaje/skills.md',                     '2ky4j7vt-9039'),
    ('09', 'Dados',                    '03-resolucion/dados.md',                     '2ky4j7vt-9259'),
    ('10', 'Turno',                    '03-resolucion/turno.md',                     '2ky4j7vt-9099'),
    ('11', 'Vocabulario de efectos',   '03-resolucion/vocabulario-de-efectos.md',    None),
    ('12', 'Enemigos',                 '04-oposicion/enemigos.md',                   None),
    ('13', 'Reloj y avance',           '04-oposicion/reloj-y-avance.md',             '2ky4j7vt-9159'),
    ('14', 'Objetivo y adversario',    '04-oposicion/objetivo-y-adversario.md',      '2ky4j7vt-9179'),
    ('15', 'Victoria y derrota',       '05-partida/victoria-y-derrota.md',           '2ky4j7vt-9239'),
    ('16', 'Contenido de una mision',  '05-partida/contenido-de-una-mision.md',      '2ky4j7vt-9059'),
    ('17', 'Perillas y constantes',    '07-balance/perillas-y-constantes.md',        None),
    ('18', 'Registro de decisiones',   '06-decisiones/registro.md',                  None),
    ('19', 'Decisiones abiertas',      '06-decisiones/abiertas.md',                  '2ky4j7vt-9139'),
]

NAV = re.compile(r'^\s*←\s*\[.*?\]\(.*?\)\s*$')
REL = re.compile(r'\[([^\]]+)\]\((?!https?://)[^)]*\.md[^)]*\)')


def transform(text):
    lines = [l for l in text.split('\n') if not NAV.match(l)]
    out = REL.sub(lambda m: '**' + m.group(1).strip('`') + '**', '\n'.join(lines))
    return re.sub(r'\n{3,}', '\n\n', out).strip() + '\n'


def main():
    if not os.path.isdir(GDD):
        print('no encuentro', GDD); return 1
    os.makedirs(OUT, exist_ok=True)

    pages, missing = [], []
    for num, name, src, pid in PAGES:
        p = os.path.join(GDD, src)
        if not os.path.exists(p):
            missing.append(src); continue
        pages.append((num, name, src, pid, transform(io.open(p, encoding='utf-8').read())))

    if missing:
        print('FALTAN archivos fuente:', missing)
        print('El mapeo de PAGES quedo desactualizado respecto del GDD. Corregilo antes de exportar.')
        return 1

    # comprobacion: todo .md del GDD tiene que estar mapeado, si no el export queda incompleto
    on_disk = set()
    for dp, _, fs in os.walk(GDD):
        if 'tools' in dp.replace(GDD, ''):
            continue
        for f in fs:
            if f.endswith('.md'):
                on_disk.add(os.path.relpath(os.path.join(dp, f), GDD).replace(os.sep, '/'))
    mapped = set(s for _, _, s, _, _ in pages)
    huerfanos = sorted(on_disk - mapped)

    with io.open(os.path.join(OUT, 'clickup-pages.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('# Export del GDD para ClickUp\n\n')
        f.write('Generado por `design/gdd/tools/export-clickup.py`. **No editar a mano**: se\n')
        f.write('regenera. La fuente de verdad es `design/gdd/`.\n\n')
        f.write('%d paginas. Cada bloque separado por una linea de guiones es una pagina;\n' % len(pages))
        f.write('el titulo del bloque es el nombre exacto que va en ClickUp.\n')
        if huerfanos:
            f.write('\n> **Ojo:** hay archivos del GDD sin mapear y por lo tanto sin exportar: %s\n' % ', '.join(huerfanos))
        for num, name, src, pid, body in pages:
            f.write('\n\n' + '=' * 78 + '\n')
            f.write('PAGINA %s -- %s\n' % (num, name))
            f.write('fuente: design/gdd/%s\n' % src)
            f.write('=' * 78 + '\n\n')
            f.write(body)

    upd = [p for p in pages if p[3]]
    crt = [p for p in pages if not p[3]]
    with io.open(os.path.join(OUT, 'plan-api.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('# Plan de llamadas a la API de ClickUp\n\n')
        f.write('Generado por `export-clickup.py`. Objetivo: dejar el doc `%s` 1:1 con el GDD\n' % DOC_ID)
        f.write('del repo, **con la menor cantidad de llamadas posible**.\n\n')
        f.write('| Total de llamadas de escritura | %d |\n|---|---|\n' % len(pages))
        f.write('| Updates (reusan pagina existente) | %d |\n' % len(upd))
        f.write('| Creates (paginas nuevas) | %d |\n\n' % len(crt))
        f.write('Es el minimo: una llamada por pagina. Reusar las 13 paginas existentes en vez de\n')
        f.write('crear un doc nuevo cuesta lo mismo y no deja el viejo tirado — no hay tool para\n')
        f.write('borrar docs ni paginas.\n\n')
        f.write('**Correrlas en tandas de 4 con pausa entre tandas.** El rate limit ya se agoto una\n')
        f.write('vez con 77 llamadas en 6 minutos, y el error que devuelve viene roto ("wait NaN\n')
        f.write('minutes"), asi que no informa cuanto falta.\n\n')
        f.write('## Updates — `clickup_update_document_page`\n\n')
        f.write('`document_id` = `%s`, `content_format` = `text/md`, `content_edit_mode` = `replace`\n\n' % DOC_ID)
        f.write('| # | page_id | nombre nuevo | nombre viejo | fuente |\n|---|---|---|---|---|\n')
        viejos = {'2ky4j7vt-9119': '01. Que es el juego', '2ky4j7vt-9219': '02. Vocabulario neutro',
                  '2ky4j7vt-9279': '03. El mapa es un grafo', '2ky4j7vt-9079': '04. El personaje',
                  '2ky4j7vt-9199': '05. El trinquete', '2ky4j7vt-9259': '06. Dados',
                  '2ky4j7vt-9099': '07. Secuencia de turno', '2ky4j7vt-9159': '08. El reloj',
                  '2ky4j7vt-9179': '09. Objetivo y adversario', '2ky4j7vt-9239': '10. Victoria y derrota',
                  '2ky4j7vt-9039': '11. Skills', '2ky4j7vt-9059': '12. Contenido de una mision',
                  '2ky4j7vt-9139': '13. Decisiones abiertas'}
        for num, name, src, pid, _ in upd:
            f.write('| %s | `%s` | %s. %s | %s | `%s` |\n' % (num, pid, num, name, viejos.get(pid, '?'), src))
        f.write('\n## Creates — `clickup_create_document_page`\n\n')
        f.write('`document_id` = `%s`, `content_format` = `text/md`\n\n' % DOC_ID)
        f.write('| # | nombre | fuente |\n|---|---|---|\n')
        for num, name, src, _, _ in crt:
            f.write('| %s | %s. %s | `%s` |\n' % (num, num, name, src))
        f.write('\n## Por que los nombres van numerados\n\n')
        f.write('ClickUp muestra las paginas en orden de creacion y no hay tool para reordenarlas.\n')
        f.write('Como 13 paginas se reusan en su orden viejo y 6 se crean al final, el orden\n')
        f.write('visual no va a coincidir con el logico. El numero en el nombre es lo que deja al\n')
        f.write('lector seguir la secuencia igual.\n')
        if huerfanos:
            f.write('\n## Sin mapear\n\nEstos archivos del GDD no estan en el export: %s\n' % ', '.join(huerfanos))

    print('%d paginas exportadas -> %s' % (len(pages), os.path.relpath(OUT, ROOT)))
    print('  updates: %d   creates: %d' % (len(upd), len(crt)))
    print('  huerfanos (sin mapear): %s' % (', '.join(huerfanos) if huerfanos else 'ninguno'))
    return 0


if __name__ == '__main__':
    sys.exit(main())
