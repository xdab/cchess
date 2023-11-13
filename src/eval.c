#include "eval.h"

#define VALUE_PAWN 100
#define VALUE_KNIGHT 300
#define VALUE_BISHOP 300
#define VALUE_ROOK 500
#define VALUE_QUEEN 900
#define VALUE_KING 20000

centipawns_t _eval_material(const board_t *board, side_t side);

centipawns_t eval(const board_t *board)
{
    centipawns_t score = 0;

    // Material
    score += _eval_material(board, SIDE_WHITE);
    score -= _eval_material(board, SIDE_BLACK);

    return score;
}

centipawns_t _eval_material(const board_t *board, side_t side)
{
    centipawns_t material_value = 0;

    for (int file = FILE_A; file <= FILE_H; file++)
        for (int rank = RANK_1; rank <= RANK_8; rank++)
        {
            square_t square = square_of(file, rank);
            piece_t piece = board_get(board, square);
            if (!(piece & side))
                continue;
            if (piece & PIECE_PAWN)
                material_value += VALUE_PAWN;
            else if (piece & PIECE_KNIGHT)
                material_value += VALUE_KNIGHT;
            else if (piece & PIECE_BISHOP)
                material_value += VALUE_BISHOP;
            else if (piece & PIECE_ROOK)
                material_value += VALUE_ROOK;
            else if (piece & PIECE_QUEEN)
                material_value += VALUE_QUEEN;
            else if (piece & PIECE_KING)
                material_value += VALUE_KING;
        }

    return material_value;
}