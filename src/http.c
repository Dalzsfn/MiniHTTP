#include <stdio.h>
#include <string.h>
#include <files.h>
#include <unistd.h>
#include <mime.h>
#include <errno.h>

int parsing_http(char request[], int client_fd)
{

    int fd = -1, nread, codigo_estado;
    char header[1024];
    const char *content_type;
    char metodo[16];
    char ruta[256];
    char protocolo[16];
    char mensaje_estado[32];
    char buff[1024];
    char *body;
    int extraidos = sscanf(request, "%s %s %s", metodo, ruta, protocolo);

    if (extraidos != 3 || strcmp(protocolo, "HTTP/1.1") != 0)
    {
        codigo_estado = 400;
    }
    else if (strcmp(metodo, "GET") != 0)
    {
        codigo_estado = 405;
    }
    else
    {
        fd = obtener_archivo(ruta, client_fd);
        if (fd == -1)
        {
            if (errno == ENOENT)
                codigo_estado = 404;
            else if (errno == EACCES || errno == EPERM)
                codigo_estado = 403;
            else
                codigo_estado = 500;
        }
        else
        {
            codigo_estado = 200;
        }
    }

    switch (codigo_estado)
    {
    case 400:
        strcpy(mensaje_estado, "Bad Request");
        body = "400 Bad Request";
        break;
    case 403:
        strcpy(mensaje_estado, "Forbidden");
        body = "403 Forbidden";
        break;
    case 404:
        strcpy(mensaje_estado, "Not Found");
        body = "404 Not Found";
        break;
    case 405:
        strcpy(mensaje_estado, "Method Not Allowed");
        body = "405 Method Not Allowed";
        break;
    case 500:
        strcpy(mensaje_estado, "Internal Server Error");
        body = "500 Internal Server Error";
        break;
    default:
        strcpy(mensaje_estado, "OK");
        body = "200 OK";
        break;
    }

    if (codigo_estado == 200)
    {
        content_type = obtener_content_type(ruta);
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nConnection: close\r\n\r\n", codigo_estado, mensaje_estado, content_type);
        write(client_fd, header, strlen(header));
        while ((nread = read(fd, buff, sizeof(buff))) > 0)
        {
            write(client_fd, buff, nread);
        }
        close(fd);
    }
    else
    {
        content_type = "text/html";
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n", codigo_estado, mensaje_estado, content_type, strlen(body));
        write(client_fd, header, strlen(header));
        write(client_fd, body, strlen(body));
    }

    printf("Código de estado: %d\n", codigo_estado);

    return 0;
}
