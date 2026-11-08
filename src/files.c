#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int obtener_archivo(char ruta[], int client_fd)
{
    int fd, nread;
    char buff[1024];
    char full_path[512];

    snprintf(full_path, sizeof(full_path), "www%s", ruta);
    // Abrir el archivo
    fd = open(full_path, O_RDONLY);
    printf("Abriendo archivo: %s\n", full_path);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
        return -1;
    }
    while ((nread = read(fd, buff, sizeof(buff))) > 0)
    {
        write(client_fd, buff, nread);
    }
    close(fd);
    return 0;
}