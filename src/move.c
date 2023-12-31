#include "move.h"
#include "board.h"
#include <string.h>

#define UCI_PROMOTION_QUEEN 'q'
#define UCI_PROMOTION_ROOK 'r'
#define UCI_PROMOTION_BISHOP 'b'
#define UCI_PROMOTION_KNIGHT 'n'

move_t move_uci(const char *uci)
{
    square_t from = SQUARE_OF(FILE(uci[0]), RANK(uci[1]));
    square_t to = SQUARE_OF(FILE(uci[2]), RANK(uci[3]));

    piece_t promote_to = PIECE_NONE;
    switch (uci[4])
    {
    case UCI_PROMOTION_QUEEN:
        promote_to = QUEEN;
        break;
    case UCI_PROMOTION_ROOK:
        promote_to = ROOK;
        break;
    case UCI_PROMOTION_BISHOP:
        promote_to = BISHOP;
        break;
    case UCI_PROMOTION_KNIGHT:
        promote_to = KNIGHT;
        break;
    default:
        break;
    }

    return move_promotion(from, to, promote_to);
}

void move_to_uci(move_t move, char *uci)
{
    if (move == MOVE_NULL)
    {
        strcpy(uci, "0000");
        return;
    }

    square_t from = move_get_from(move);
    square_t to = move_get_to(move);
    piece_t promote_to = move_get_promoted_piece(move);

    uci[0] = FILE_SYMBOL(SQUARE_FILE(from));
    uci[1] = RANK_SYMBOL(SQUARE_RANK(from));
    uci[2] = FILE_SYMBOL(SQUARE_FILE(to));
    uci[3] = RANK_SYMBOL(SQUARE_RANK(to));

    if (promote_to != PIECE_NONE)
    {
        switch (promote_to)
        {
        case QUEEN:
            uci[4] = UCI_PROMOTION_QUEEN;
            break;
        case ROOK:
            uci[4] = UCI_PROMOTION_ROOK;
            break;
        case BISHOP:
            uci[4] = UCI_PROMOTION_BISHOP;
            break;
        case KNIGHT:
            uci[4] = UCI_PROMOTION_KNIGHT;
            break;
        default:
            break;
        }

        uci[5] = '\0';
    }
    else
    {
        uci[4] = '\0';
    }
}
