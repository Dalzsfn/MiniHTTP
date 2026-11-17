# MiniHTTP

Servidor HTTP mínimo en C para la materia de Computación Distribuida. Atiende varias conexiones a la vez con `epoll` y responde únicamente a peticiones `GET` para devolver una página HTML.

## Qué hace

- Acepta solo peticiones `GET` con protocolo `HTTP/1.1`.
- Busca el archivo pedido dentro de `www/` (por ejemplo, `GET /index.html` abre `www/index.html`).
- Devuelve el tipo de contenido según la extensión: `.html`, `.css`, `.js`, `.png` y `.jpg`.
- Responde con `200`, `400`, `403`, `404`, `405` o `500` según el caso.

## Estructura

```
src/        lógica del servidor (bucle epoll, parseo HTTP, archivos y MIME)
include/    cabeceras del proyecto
Comer/      socket pasivo TCP (passiveTCP / passivesock)
www/        sitio estático que se sirve
```

## Requisitos

- Linux (usa `epoll`)
- `gcc` y `make`

## Uso

Hay que ejecutarlo desde la raíz del proyecto, porque los archivos se buscan en `www/` relativo al directorio actual.

```bash
make          # compila y genera ./minihttp
make run      # compila (si hace falta) y arranca el servidor
make clean    # borra el binario
```

Con el servidor en marcha, abre en el navegador:

```
http://localhost:8080/index.html
```

Para detenerlo, usa `Ctrl+C`.
