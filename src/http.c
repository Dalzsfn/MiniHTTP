#include <stdio.h>
#include <string.h>
#include <files.h>
#include <unistd.h>
#include <mime.h>

int parsing_http(char request[], int client_fd)
{
    // ejemplo de solicitud HTTP
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
    int fd, nread;
    char header[256];
    const char *content_type;
    char metodo[16];
    char ruta[256];
    char protocolo[16];
    char mensaje_estado[32];
    char buff[1024];
    int codigo_estado;

    int extraidos = sscanf(request, "%s %s %s", metodo, ruta, protocolo);
    if (extraidos == 3)
    {
        printf("Solicitud HTTP: %s %s %s\n", metodo, ruta, protocolo);
        printf("Ruta: %s\n", ruta);
        if (strcmp(metodo, "GET") != 0)
        {
            codigo_estado = 405;
            return -1;
        }
        if (strcmp(protocolo, "HTTP/1.1") != 0)
        {
            printf("400: Bad Request (%s)\n", protocolo);
            codigo_estado = 400;
            return -1;
        }
        fd = obtener_archivo(ruta, client_fd);
        if (fd == -1)
        {
            codigo_estado = 404;
            return -1;
        }

        // faltan los otros códigos de estado
        switch (codigo_estado)
        {
        case 400:
            strcpy(mensaje_estado, "Bad Request");
            return -1;
        case 404:
            strcpy(mensaje_estado, "Not Found");
            return -1;
        case 405:
            strcpy(mensaje_estado, "Method Not Allowed");
            return -1;
        default:
            strcpy(mensaje_estado, "OK");
            break;
        }

        content_type = obtener_content_type(ruta);

        // falta header para error
        snprintf(header, sizeof(header), "HTTP/1.1 %s %s\r\nContent-Type: %s\r\nConnection: close\r\n\r\n", codigo_estado, mensaje_estado, content_type);
        write(client_fd, header, strlen(header));
        while ((nread = read(fd, buff, sizeof(buff))) > 0)
        {
            write(client_fd, buff, nread);
        }
        close(fd);
    }
    else
    {
        printf("sscanf salió mal\n");
        return -1;
    }
    return 0;
}
