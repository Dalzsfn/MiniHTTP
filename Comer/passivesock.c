#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int errexit(const char *format, ...);
unsigned short portbase = 10000;

int passivesock(const char *service, const char *transport, int qlen)
{
	struct addrinfo hints = {0};
	struct addrinfo *result, *rp;
	int s, sfd, type;

	if (strcmp(transport, "tcp") == 0)
		type = SOCK_STREAM;
	else if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		errexit("Unknown transport: %s\n", transport);

	hints.ai_family = AF_INET;
	hints.ai_socktype = type;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	s = getaddrinfo(NULL, service, &hints, &result);

	if (s != 0)
		errexit("getaddrinfo: %s\n", gai_strerror(s));

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family,
					 rp->ai_socktype,
					 rp->ai_protocol);

		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sfd);
	}

	freeaddrinfo(result);

	if (rp == NULL)
		errexit("Cannot bind to service %s\n", service);

	if (type == SOCK_STREAM)
	{
		if (listen(sfd, qlen) < 0)
			errexit("Cannot listen on port %s\n", service);
	}

	return sfd;
}
