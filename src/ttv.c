#include "game.h"

struct ttv_data
{
    char* channel;
    TCPsocket socket;
    int connected;
};


int ttv_globalInit();
int ttv_globalDeInit();
int ttv_init(struct context2ds* c2d, struct gstates* gs, char* username, char* password, char* channel);
int ttv_deinit(struct context2ds* c2d, struct gstates* gs);
int ttv_read(struct context2ds* c2d, struct gstates* gs);

int ttv_test(struct context2ds* c2d, struct gstates* gs)
{
    int i = 0;
    printf("ttv_test\n");

    ttv_globalInit();
    ttv_init(c2d, gs, "justinfan121345", "justinfan121345", "saruei");
    while(i==0)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                i=1;
                break;
            }
        }

        ttv_read(c2d, gs);
        usleep(1000);
    }
    ttv_deinit(c2d, gs);
    ttv_globalDeInit();

    return 0;
}

int ttv_globalInit()
{
    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 1;
    }
    return 0;
}

int ttv_globalDeInit()
{
    SDLNet_Quit();
    return 0;
}

int ttv_init(struct context2ds* c2d, struct gstates* gs, char* username, char* password, char* channel)
{
    gs->datas[GSTATES_DATAS_TTV] = malloc(sizeof(struct ttv_data));
    struct ttv_data* ttv = (struct ttv_data*)gs->datas[GSTATES_DATAS_TTV];
    memset(ttv, 0, sizeof(struct ttv_data));

    // Create the IP address structure
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "irc.chat.twitch.tv", 6667) < 0) {
        printf("SDLNet_ResolveHost failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 1;
    }

    // Open a connection to the server
    ttv->socket = SDLNet_TCP_Open(&ip);
    if (!ttv->socket) {
        printf("SDLNet_TCP_Open failed: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        printf("-"); fflush(stdout);
        return 1;
    }

    // Send the login information
    char loginMessage[512];
    snprintf(loginMessage, sizeof(loginMessage), "PASS oauth:%s\r\nNICK %s\r\n", password, username);
    if (SDLNet_TCP_Send(ttv->socket, loginMessage, strlen(loginMessage)) < strlen(loginMessage)) {
        printf("SDLNet_TCP_Send failed: %s\n", SDLNet_GetError());
        ttv_deinit(c2d, gs);
        printf("-"); fflush(stdout);
        return 1;
    }

    // Receive and print server response
    char response[512];
    if (SDLNet_TCP_Recv(ttv->socket, response, sizeof(response)) <= 0) {
        printf("SDLNet_TCP_Recv failed: %s\n", SDLNet_GetError());
        ttv_deinit(c2d, gs);
        printf("-"); fflush(stdout);
        return 1;
    }
    printf("%s", response);

    // Join a channel
    char join[512];
    snprintf(join, sizeof(join), "JOIN #%s\r\n", channel);
    if (SDLNet_TCP_Send(ttv->socket, join, strlen(join)) < strlen(join)) {
        printf("SDLNet_TCP_Send failed: %s\n", SDLNet_GetError());
        ttv_deinit(c2d, gs);
        printf("-"); fflush(stdout);
        return 1;
    }

    // Receive and print server response
    if (SDLNet_TCP_Recv(ttv->socket, response, sizeof(response)) <= 0) {
        printf("SDLNet_TCP_Recv failed: %s\n", SDLNet_GetError());
        ttv_deinit(c2d, gs);
        printf("-"); fflush(stdout);
        return 1;
    }
    printf("%s", response);

    // // enable more info, like sub stauts
    // const char* capMessage = "CAP REQ :twitch.tv/tags\r\n";
    // if (SDLNet_TCP_Send(ttv->socket, capMessage, strlen(capMessage)) < strlen(capMessage)) {
    //     printf("SDLNet_TCP_Send failed: %s\n", SDLNet_GetError());
    //     ttv_deinit(c2d, gs);
    //     printf("-"); fflush(stdout);
    //     return 1;
    // }

    // // Receive and print server response
    // if (SDLNet_TCP_Recv(ttv->socket, response, sizeof(response)) <= 0) {
    //     printf("SDLNet_TCP_Recv failed: %s\n", SDLNet_GetError());
    //     ttv_deinit(c2d, gs);
    //     printf("-"); fflush(stdout);
    //     return 1;
    // }

    return 0;
}

int ttv_deinit(struct context2ds* c2d, struct gstates* gs)
{
    struct ttv_data* ttv = (struct ttv_data*)gs->datas[GSTATES_DATAS_TTV];
    SDLNet_TCP_Close(ttv->socket);
    free(ttv);
    return 0;
}

int ttv_read(struct context2ds* c2d, struct gstates* gs)
{
    struct ttv_data* ttv = (struct ttv_data*)gs->datas[GSTATES_DATAS_TTV];
    char buffer[4096] = { 0 };
    if (SDLNet_TCP_Recv(ttv->socket, buffer, sizeof(buffer)) <= 0) {
        printf("SDLNet_TCP_Recv failed: %s\n", SDLNet_GetError());
        SDLNet_TCP_Close(ttv->socket);
        SDLNet_Quit();
        gs->datas[GSTATES_DATAS_TTV] = NULL;
        return 1;
    }
    printf("%s", buffer);
    return 0;
}

/*
use anonymous endpoint to get the data, 
the anoinimous enpoint is moslty inteded for testing, 
but it work so we can use it for now,
if the anonymous endpoint stop working then we can switch to the irc endpoint with oauth
if the irc endpoint stop working then we can switch to the websocket endpoint
*/