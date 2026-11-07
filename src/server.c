#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <passiveTCP.h>
#include <fcntl.h>
#include <unistd.h>
#include "http.h"
#include <netinet/in.h>

int service()
{
#define MAX_EVENTS 10
    struct epoll_event ev, events[MAX_EVENTS];
    int sd, nsd, fd, nread, epollfd, nfds, n, flags;
    char buffer[1024];
    socklen_t l;
    struct sockaddr_in cltAddr;

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
        for (int i = 0; i < nfds; ++i)
        {
            // si el evento es del socket pasivo, se acepta la conexión entrante
            if (events[i].data.fd == sd)
            {
                l = sizeof(cltAddr);
                nsd = accept(sd, (struct sockaddr *)&cltAddr, &l);
                if (nsd < 0)
                {
                    perror("Error en accept");
                    exit(EXIT_FAILURE);
                }

                // configuracion del socket no bloqueante
                flags = fcntl(nsd, F_GETFL, 0);
                fcntl(nsd, F_SETFL, flags | O_NONBLOCK);

                // configuracion del evento para el socket de conexión
                ev.events = EPOLLIN;
                ev.data.fd = nsd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, nsd, &ev) == -1)
                {
                    perror("Error en epoll_ctl");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // seccion para el procesamiento de sockets conectados
                int client_fd = events[i].data.fd;
                n = read(client_fd, buffer, sizeof(buffer));
                if (n < 0)
                {
                    perror("Error en read");
                    exit(EXIT_FAILURE);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                }

                else
                {
                    buffer[n] = '\0';

                    // seccion de parsing para extraer el nombre del archivo
                    parsing_http(buffer);

                    epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                }
            }
        }
    }
}
