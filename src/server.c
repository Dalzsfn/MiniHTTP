#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <passiveTCP.h>
#include <fcntl.h>
#include <unistd.h>

int service()
{
#define MAX_EVENTS 10
    struct epoll_event ev, events[MAX_EVENTS];
    int sd, nsd, fd, nread, epollfd, nfds, n;
    char buffer[1024];

    // instancia de epoll
    epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        perror("Error de creacion de epoll");
        exit(EXIT_FAILURE);
    }

    // configuracion del evento para el socket pasivo
    ev.events = EPOLLIN;

    // creacion del socket pasivo
    sd = passiveTCP("8080", 10);

    // guardar el socket pasivo en el evento
    ev.data.fd = sd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sd, &ev) == -1)
    {
        perror("Error en epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // espera de eventos
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("Error en epoll_wait");
            exit(EXIT_FAILURE);
        }
        // itera únicamente sobre los eventos listos
        for (n = 0; n < nfds; ++n)
        {
            // si el evento es del socket pasivo, se acepta la conexión entrante
            if (events[n].data.fd == sd)
            {
                nsd = accept(sd, NULL, NULL);
                if (nsd < 0)
                {
                    perror("Error en accept");
                    exit(EXIT_FAILURE);
                }

                // procesaminento del socket de conexión
                n = read(nsd, buffer, sizeof(buffer));
                if (n < 0)
                {
                    perror("Error en read");
                    exit(EXIT_FAILURE);
                }
                buffer[n] = '\0';

                // seccion de parsing para extraer el nombre del archivo
            }
            else
            // seccion para el procesamiento de sockets conectados
            {
            }
        }

        /*fd = open(filename, O_RDONLY);
        if (fd < 0)
        {
            perror("Error en open");
            exit(EXIT_FAILURE);
        }
        while (nread = read(fd, buffer, sizeof(buffer)))
        {
            write(nsd, buffer, nread);
        }

        close(fd);
        close(nsd);*/
    }
    close(nsd);
}