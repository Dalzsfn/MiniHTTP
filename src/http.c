#include <stdio.h>
#include <string.h>
#include <files.h>
#include <unistd.h>
#include <mime.h>

int parsing_http(char request[], int client_fd)
{

    int fd = -1, nread;
    char header[1024];
    const char *content_type;
    char metodo[16];
    char ruta[256];
    char protocolo[16];
    char mensaje_estado[32];
    char buff[1024];
    int codigo_estado;
    char *body;

    int extraidos = sscanf(request, "%s %s %s", metodo, ruta, protocolo);
    if (extraidos == 3)
    {
        codigo_estado = 200;
        printf("Solicitud HTTP: %s %s %s\n", metodo, ruta, protocolo);
        printf("Ruta: %s\n", ruta);
        if (strcmp(metodo, "GET") != 0)
        {
            codigo_estado = 405;
        }
        else if (strcmp(protocolo, "HTTP/1.1") != 0)
        {
            printf("400: Bad Request (%s)\n", protocolo);
            codigo_estado = 400;
        }
        else
        {
            fd = obtener_archivo(ruta, client_fd);
            if (fd == -1)
            {
                codigo_estado = 404;
            }
        }

        // faltan los otros códigos de estado
        switch (codigo_estado)
        {
        case 400:
            strcpy(mensaje_estado, "Bad Request");
            body = "400 Bad Request";
            break;
        case 404:
            strcpy(mensaje_estado, "Not Found");
            body = "404 Not Found";
            break;
        case 405:
            strcpy(mensaje_estado, "Method Not Allowed");
            body = "405 Method Not Allowed";
            break;
        default:
            strcpy(mensaje_estado, "OK");
            break;
        }

        if (codigo_estado == 200)
        {
            content_type = obtener_content_type(ruta);
            snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nConnection: close\r\n\r\n", codigo_estado, mensaje_estado, content_type);
        }
        else
        {
            content_type = "text/html";
            snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n", codigo_estado, mensaje_estado, content_type, strlen(body));
        }

        printf("Código de estado: %d\n", codigo_estado);
        write(client_fd, header, strlen(header));
        if (codigo_estado == 200)
        {
            while ((nread = read(fd, buff, sizeof(buff))) > 0)
            {
                write(client_fd, buff, nread);
            }
            close(fd);
        }
        else
        {
            write(client_fd, body, strlen(body));
        }
    }
    else
    {
        printf("sscanf salió mal\n");
        return -1;
    }
    return 0;
}
