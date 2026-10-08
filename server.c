#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
// furat de la SO
#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)

#define PORT 1337
int main(void)
{
    // In case it's not obvious: https://youtube.com/shorts/T6Ol6ua0FOU
    char mesaj[] = "Puteti sa-mi ziceti si mie ce-a zis nevasta-mea sa cumpar "
                   "da' la supermarket?\n";

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // NOLINTNEXTLINE
    DIE(fd < 0, "socket(...)");

    int can_restart = 1;
    int errc        = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &can_restart,
                                 sizeof(can_restart));
    // NOLINTNEXTLINE
    DIE(errc < 0, "setsockopt(...)");

    struct sockaddr_in sock_addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(PORT),
    };

    errc = bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
    // NOLINTNEXTLINE
    DIE(errc < 0, "bind(...)");

    errc = listen(fd, 1);
    // NOLINTNEXTLINE
    DIE(errc < 0, "listen(...)");
    // "Alo? Buna ziua, am sunat la SRI? / Da / ..."
    printf("Dumneavoastra imi ascultati convorbirile (pe portul %d), "
           "nu?\n",
           PORT);

    int c_fd = accept(fd, NULL, NULL);
    // NOLINTNEXTLINE
    DIE(c_fd < 0, "accept(...)");
    printf("Da!\n");

    ssize_t went = send(c_fd, mesaj, strlen(mesaj), 0); // ssize_t is this old?
    // NOLINTNEXTLINE
    DIE(went < 0, "send(...)");

    close(c_fd);
    close(fd);

    return 0;
}
