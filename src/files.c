#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int obtener_archivo(char ruta[])
{
    int fd;
    char ruta_relativa[512];
    snprintf(ruta_relativa, sizeof(ruta_relativa), "www/%s", ruta);
    char *ruta_absoluta = realpath(ruta_relativa, NULL);
    if (ruta_absoluta == NULL)
    {
        perror("Error al obtener la ruta absoluta");
        return -1;
    }

    fd = open(ruta_absoluta, O_RDONLY);
    printf("Abriendo archivo: %s\n", ruta_absoluta);
    free(ruta_absoluta);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
    }
    return fd;
}