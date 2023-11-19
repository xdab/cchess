#include "uci.h"
#include "board.h"
#include "board_fen.h"
#include "move.h"
#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_BUF_LENGTH 64
#define OPTIONS_BUF_LENGTH 128
#define INPUT_BUF_LENGTH (COMMAND_BUF_LENGTH + 1 + OPTIONS_BUF_LENGTH)
#define RESPONSE_BUF_LENGTH 128
#define MOVE_BUF_LENGTH 6

#define COMMAND_UCI "uci"
#define RESPONSE_UCI_ID_NAME "id name cchess"
#define RESPONSE_UCI_ID_AUTHOR "id author xdab"
#define RESPONSE_UCI_OPTION_HASH "option name Hash type spin default 64 min 4 max 128"
#define RESPONSE_UCI_UCIOK "uciok"

#define COMMAND_IS_READY "isready"
#define RESPONSE_IS_READY "readyok"

#define COMMAND_UCI_NEW_GAME "ucinewgame"

#define COMMAND_POSITION "position"
#define COMMAND_POSITION_FEN "fen"
#define COMMAND_POSITION_STARTPOS "startpos"
#define COMMAND_POSITION_MOVES "moves"

#define COMMAND_GO "go"
#define COMMAND_GO_NODES "nodes"
#define COMMAND_GO_DEPTH "depth"

#define COMMAND_STOP "stop"

#define COMMAND_QUIT "quit"
#define RESPONSE_QUIT "bye"

void reply(const char *response)
{
    fputs(response, stdout);
    fputc('\n', stdout);
    fflush(stdout);
#ifdef CCHESS_DEBUG
    fputs(">>> ", stderr);
    fputs(response, stderr);
    fputc('\n', stderr);
    fflush(stderr);
#endif
}

int uci_loop()
{
    board_t board;
    char input[INPUT_BUF_LENGTH];
    char command[COMMAND_BUF_LENGTH];
    char options[COMMAND_BUF_LENGTH];
    char response[RESPONSE_BUF_LENGTH];

    while (fgets(input, COMMAND_BUF_LENGTH, stdin) != NULL)
    {
        input[strlen(input) - 1] = '\0';

#ifdef CCHESS_DEBUG
        fputs("... ", stderr);
        fputs(input, stderr);
        fputc('\n', stderr);
        fflush(stderr);
#endif

        char *token = strtok(input, " ");
        int token_index = 0;
        while ((token != NULL) && (token_index < 2))
        {
            if (token_index == 0)
                strncpy(command, token, COMMAND_BUF_LENGTH);
            else if (token_index == 1)
                strncpy(options, token, OPTIONS_BUF_LENGTH);

            token = strtok(NULL, " ");
            token_index++;
        }

        if (strcmp(command, COMMAND_UCI) == 0)
        {
            reply(RESPONSE_UCI_ID_NAME);
            reply(RESPONSE_UCI_ID_AUTHOR);
            reply(RESPONSE_UCI_OPTION_HASH);
            reply(RESPONSE_UCI_UCIOK);
        }
        else if (strcmp(command, COMMAND_IS_READY) == 0)
        {
            reply(RESPONSE_IS_READY);
        }
        else if (strcmp(command, COMMAND_UCI_NEW_GAME) == 0)
        {
            board_init(&board);
        }
        else if (strcmp(command, COMMAND_POSITION) == 0)
        {
            char *pos = command + strlen(COMMAND_POSITION) + 1;
            if (strncmp(pos, COMMAND_POSITION_FEN, strlen(COMMAND_POSITION_FEN)) == 0)
            {
                pos += strlen(COMMAND_POSITION_FEN) + 1;
                board_set_fen(&board, pos);
            }
            else if (strncmp(pos, COMMAND_POSITION_STARTPOS, strlen(COMMAND_POSITION_STARTPOS)) == 0)
            {
                pos += strlen(COMMAND_POSITION_STARTPOS) + 1;
                board_init(&board);
            }
        }
        else if (strcmp(command, COMMAND_GO) == 0)
        {
            int options_given = 0;
            token = strtok(options, " ");
            while (token != NULL)
            {
                if (strcmp(token, COMMAND_GO_NODES) == 0)
                    options_given++;
                else if (strcmp(token, COMMAND_GO_DEPTH) == 0)
                    options_given++;
                token = strtok(NULL, " ");
            }

            score_t cp = evaluate(&board);
            snprintf(response, RESPONSE_BUF_LENGTH, "info depth 1 score cp %d time 1 nodes 1 nps 1 pv e2e4", cp);
            reply(response);

            if (options_given > 0)
            {
                reply("bestmove e2e4");
            }
        }
        else if (strcmp(command, COMMAND_STOP) == 0)
        {
            reply("bestmove e2e4");
        }
        else if (strcmp(command, COMMAND_QUIT) == 0)
        {
            return EXIT_SUCCESS;
        }

        memset(command, 0, COMMAND_BUF_LENGTH);
        memset(options, 0, OPTIONS_BUF_LENGTH);
    }

    return EXIT_FAILURE;
}