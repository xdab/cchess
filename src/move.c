#include "move.h"
#include "board.h"
#include <string.h>

#define FROM_SQUARE_OFFSET 0
#define TO_SQUARE_OFFSET 8
#define PROMOTION_OFFSET 16

#define SQUARE_MASK 0xFF
#define PROMOTION_MASK 0xF

#define UCI_PROMOTION_QUEEN 'q'
#define UCI_PROMOTION_ROOK 'r'
#define UCI_PROMOTION_BISHOP 'b'
#define UCI_PROMOTION_KNIGHT 'n'

promotion_t _move_get_promotion(move_t move)
{
    return (move >> PROMOTION_OFFSET) & PROMOTION_MASK;
}

move_t move_regular(square_t from, square_t to)
{
    return (from << FROM_SQUARE_OFFSET) | (to << TO_SQUARE_OFFSET);
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
    if (move == MOVE_NULL)
    {
        strcpy(uci, "0000");
        return;
    }
    
    square_t from = move_get_from(move);
    square_t to = move_get_to(move);
    promotion_t promotion = _move_get_promotion(move);

    uci[0] = FILE_SYMBOL(square_file(from));
    uci[1] = RANK_SYMBOL(square_rank(from));
    uci[2] = FILE_SYMBOL(square_file(to));
    uci[3] = RANK_SYMBOL(square_rank(to));

    if (promotion != PROMOTION_NONE)
    {
        switch (promotion)
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

square_t move_get_from(move_t move)
{
    return (move >> FROM_SQUARE_OFFSET) & SQUARE_MASK;
}

square_t move_get_to(move_t move)
{
    return (move >> TO_SQUARE_OFFSET) & SQUARE_MASK;
}

piece_t move_get_promoted_piece(move_t move)
{
    promotion_t promotion_instruction = (move >> PROMOTION_OFFSET) & PROMOTION_MASK;
    switch (promotion_instruction)
    {
    case PROMOTION_QUEEN:
        return PIECE_QUEEN;
    case PROMOTION_ROOK:
        return PIECE_ROOK;
    case PROMOTION_BISHOP:
        return PIECE_BISHOP;
    case PROMOTION_KNIGHT:
        return PIECE_KNIGHT;
    default:
        return PIECE_NONE;
    }
}