#include "move.h"
#include "board.h"
#include <string.h>

#define FROM_FILE_OFFSET 0
#define FROM_RANK_OFFSET 3
#define TO_FILE_OFFSET 6
#define TO_RANK_OFFSET 9
#define PROMOTION_OFFSET 12

#define FILE_MASK 0b111
#define RANK_MASK 0b111
#define PROMOTION_MASK 0b111

#define UCI_PROMOTION_QUEEN 'q'
#define UCI_PROMOTION_ROOK 'r'
#define UCI_PROMOTION_BISHOP 'b'
#define UCI_PROMOTION_KNIGHT 'n'

move_t move_regular(square_t from, square_t to)
{
    return (square_file(from) << FROM_FILE_OFFSET) | (square_rank(from) << FROM_RANK_OFFSET) | (square_file(to) << TO_FILE_OFFSET) | (square_rank(to) << TO_RANK_OFFSET);
}

move_t move_promotion(square_t from, square_t to, promotion_t promotion)
{
    return move_regular(from, to) | (promotion << PROMOTION_OFFSET);
}

move_t move_uci(const char *uci)
{
    square_t from = square_of(FILE(uci[0]), RANK(uci[1]));
    square_t to = square_of(FILE(uci[2]), RANK(uci[3]));

    if (uci[4] != '\0')
    {
        promotion_t promotion_piece = PROMOTION_NONE;
        switch (uci[4])
        {
        case UCI_PROMOTION_QUEEN:
            promotion_piece = PROMOTION_QUEEN;
            break;
        case UCI_PROMOTION_ROOK:
            promotion_piece = PROMOTION_ROOK;
            break;
        case UCI_PROMOTION_BISHOP:
            promotion_piece = PROMOTION_BISHOP;
            break;
        case UCI_PROMOTION_KNIGHT:
            promotion_piece = PROMOTION_KNIGHT;
            break;
        default:
            break;
        }

        return move_promotion(from, to, promotion_piece);
    }

    return move_regular(from, to);
}

void move_to_uci(move_t move, char *uci)
{
    int from_file = move_get_from_file(move);
    int from_rank = move_get_from_rank(move);
    int to_file = move_get_to_file(move);
    int to_rank = move_get_to_rank(move);
    int promotion_piece = move_get_promotion(move);

    uci[0] = FILE_SYMBOL(from_file);
    uci[1] = RANK_SYMBOL(from_rank);
    uci[2] = FILE_SYMBOL(to_file);
    uci[3] = RANK_SYMBOL(to_rank);

    if (promotion_piece != PROMOTION_NONE)
    {
        switch (promotion_piece)
        {
        case PROMOTION_QUEEN:
            uci[4] = UCI_PROMOTION_QUEEN;
            break;
        case PROMOTION_ROOK:
            uci[4] = UCI_PROMOTION_ROOK;
            break;
        case PROMOTION_BISHOP:
            uci[4] = UCI_PROMOTION_BISHOP;
            break;
        case PROMOTION_KNIGHT:
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

int move_get_from_file(move_t move)
{
    return (move >> FROM_FILE_OFFSET) & FILE_MASK;
}

int move_get_from_rank(move_t move)
{
    return (move >> FROM_RANK_OFFSET) & RANK_MASK;
}

int move_get_to_file(move_t move)
{
    return (move >> TO_FILE_OFFSET) & FILE_MASK;
}

int move_get_to_rank(move_t move)
{
    return (move >> TO_RANK_OFFSET) & RANK_MASK;
}

int move_get_promotion(move_t move)
{
    return (move >> PROMOTION_OFFSET) & PROMOTION_MASK;
}