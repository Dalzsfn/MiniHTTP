#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int parsing_http(char request[])
{
    /* estructura de peticion:
    request: GET / HTTP/1.1
    Host: localhost:8080
    Connection: keep-alive
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
    regex_t regex;
    regmatch_t matches[2];
    const char *pattern = "^GET\\s";
    if (regcomp(&regex, pattern, REG_EXTENDED))
    {
        fprintf(stderr, "Error compilando regex\n");
        return EXIT_FAILURE;
    }

    int result = regexec(&regex, request, 2, matches, 0);
    if (!result)
    {
        printf("Request is a GET request\n");
    }
    else if (result == REG_NOMATCH)
    {
        printf("Request is not a GET request\n");
    }
    else
    {
        perror("Regex match failed\n");
        return EXIT_FAILURE;
    }

    regfree(&regex);
}
