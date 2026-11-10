#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int obtener_archivo(char ruta[], int client_fd)
{
    int fd;
    char full_path[512];

    snprintf(full_path, sizeof(full_path), "www%s", ruta);
    fd = open(full_path, O_RDONLY);
    printf("Abriendo archivo: %s\n", full_path);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
        return -1;
    }
    return fd;
}