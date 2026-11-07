#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <files.h>
int parsing_http(char request[])
{
    /* 200 OK
    request: GET /index HTTP/1.1
    Host: localhost:8080
    Connection: keep-alive
    Cache-Control: max-age=0
    sec-ch-ua: "Not:A-Brand";v="8", "Chromium";v="156", "Brave";v="156"
    sec-ch-ua-mobile: ?0
    sec-ch-ua-platform: "Linux"
    Upgrade-Insecure-Requests: 1
    User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/156.0.0.0 Safari/537.36
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/jxl,image/avif,image/webp,image/apng,* /*;q=0.8
    Sec-GPC: 1
    Sec-Fetch-Site: none
    Sec-Fetch-Mode: navigate
    Sec-Fetch-User: ?1
    Sec-Fetch-Dest: document
    Accept-Encoding: gzip, deflate, br, zstd
    Accept-Language: en-US,en;q=0.5
    */

    char header[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n\r\n";

    char metodo[16];
    char ruta[256];
    char protocolo[16];

    int extraidos = sscanf(request, "%s %s %s", metodo, ruta, protocolo);
    if (extraidos == 3)
    {
        if (strcmp(metodo, "GET") != 0)
        {
            printf("405: Method not allowed (%s)\n", metodo);
            return -1;
        }
        if (strcmp(protocolo, "HTTP/1.1") != 0)
        {
            printf("400: Bad Request (%s)\n", protocolo);
            return -1;
        }
        printf("Solicitud HTTP válida: %s %s %s\n", metodo, ruta, protocolo);
        obtener_archivo(ruta);
    }
    else
    {
        printf("sscanf salió mal\n");
        return -1;
    }
    return 0;
}
