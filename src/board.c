#include "board.h"
#include <stdio.h>
#include <ctype.h>

#define CCHESS_UNICODE

#define SPACE ' '
#define NEWLINE '\n'
#define ONE '1'
#define FILE_LEGEND "    a b c d e f g h\n"
#define NO_EN_PASSANT -1

#ifdef CCHESS_UNICODE
#define VERTICAL "│"
#define UPPER_EDGE "  ┌─────────────────┐\n"
#define LOWER_EDGE "  └─────────────────┘\n"
#define PUT_VERTICAL(stream) fputs(VERTICAL, stream)
#define PUT_UPPER_EDGE(stream) fputs(UPPER_EDGE, stream)
#define PUT_LOWER_EDGE(stream) fputs(LOWER_EDGE, stream)
#else
#define VERTICAL '|'
#define HORIZONTAL_EDGE "  +-----------------+\n"
#define PUT_VERTICAL(stream) fputc(VERTICAL, stream)
#define PUT_UPPER_EDGE(stream) fputs(HORIZONTAL_EDGE, stream)
#define PUT_LOWER_EDGE(stream) fputs(HORIZONTAL_EDGE, stream)
#endif

void board_init(board_t *board)
{
    board->pieces[FILE_A][RANK_1] = PIECE_ROOK | SIDE_WHITE;
    board->pieces[FILE_B][RANK_1] = PIECE_KNIGHT | SIDE_WHITE;
    board->pieces[FILE_C][RANK_1] = PIECE_BISHOP | SIDE_WHITE;
    board->pieces[FILE_D][RANK_1] = PIECE_QUEEN | SIDE_WHITE;
    board->pieces[FILE_E][RANK_1] = PIECE_KING | SIDE_WHITE;
    board->pieces[FILE_F][RANK_1] = PIECE_BISHOP | SIDE_WHITE;
    board->pieces[FILE_G][RANK_1] = PIECE_KNIGHT | SIDE_WHITE;
    board->pieces[FILE_H][RANK_1] = PIECE_ROOK | SIDE_WHITE;

    for (int file = FILE_A; file <= FILE_H; file++)
        board->pieces[file][RANK_2] = PIECE_PAWN | SIDE_WHITE;

    board->pieces[FILE_A][RANK_8] = PIECE_ROOK | SIDE_BLACK;
    board->pieces[FILE_B][RANK_8] = PIECE_KNIGHT | SIDE_BLACK;
    board->pieces[FILE_C][RANK_8] = PIECE_BISHOP | SIDE_BLACK;
    board->pieces[FILE_D][RANK_8] = PIECE_QUEEN | SIDE_BLACK;
    board->pieces[FILE_E][RANK_8] = PIECE_KING | SIDE_BLACK;
    board->pieces[FILE_F][RANK_8] = PIECE_BISHOP | SIDE_BLACK;
    board->pieces[FILE_G][RANK_8] = PIECE_KNIGHT | SIDE_BLACK;
    board->pieces[FILE_H][RANK_8] = PIECE_ROOK | SIDE_BLACK;

    for (int file = FILE_A; file <= FILE_H; file++)
        board->pieces[file][RANK_7] = PIECE_PAWN | SIDE_BLACK;

    for (int file = FILE_A; file <= FILE_H; file++)
        for (int rank = RANK_3; rank <= RANK_6; rank++)
            board->pieces[file][rank] = PIECE_NONE;

    board->side_to_move = SIDE_WHITE;
    board->white_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->black_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->en_passant_file = NO_EN_PASSANT;
    board->en_passant_rank = NO_EN_PASSANT;
    board->halfmove_clock = 0;
    board->fullmove_number = 1;
}

void board_print(board_t *board, FILE *stream)
{
    PUT_UPPER_EDGE(stream);

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
        fputc(ONE + rank, stream);
        fputc(SPACE, stream);
        PUT_VERTICAL(stream);
        fputc(SPACE, stream);

        for (int file = FILE_A; file <= FILE_H; file++)
        {
            piece_t piece = board->pieces[file][rank];

#ifndef CCHESS_UNICODE
            char piece_char = SYMBOL_NONE;
            if (piece & PIECE_PAWN)
                piece_char = SYMBOL_PAWN;
            else if (piece & PIECE_KNIGHT)
                piece_char = SYMBOL_KNIGHT;
            else if (piece & PIECE_BISHOP)
                piece_char = SYMBOL_BISHOP;
            else if (piece & PIECE_ROOK)
                piece_char = SYMBOL_ROOK;
            else if (piece & PIECE_QUEEN)
                piece_char = SYMBOL_QUEEN;
            else if (piece & PIECE_KING)
                piece_char = SYMBOL_KING;
            if (piece & SIDE_BLACK)
                piece_char = tolower(piece_char);
            fputc(piece_char, stream);
#else
            if (piece & SIDE_WHITE)
            {
                if (piece & PIECE_PAWN)
                    fputs("♙", stream);
                else if (piece & PIECE_KNIGHT)
                    fputs("♘", stream);
                else if (piece & PIECE_BISHOP)
                    fputs("♗", stream);
                else if (piece & PIECE_ROOK)
                    fputs("♖", stream);
                else if (piece & PIECE_QUEEN)
                    fputs("♕", stream);
                else if (piece & PIECE_KING)
                    fputs("♔", stream);
            }
            else if (piece & SIDE_BLACK)
            {
                if (piece & PIECE_PAWN)
                    fputs("♟", stream);
                else if (piece & PIECE_KNIGHT)
                    fputs("♞", stream);
                else if (piece & PIECE_BISHOP)
                    fputs("♝", stream);
                else if (piece & PIECE_ROOK)
                    fputs("♜", stream);
                else if (piece & PIECE_QUEEN)
                    fputs("♛", stream);
                else if (piece & PIECE_KING)
                    fputs("♚", stream);
            }
            else
                fputc(SYMBOL_NONE, stream);
#endif
            fputc(SPACE, stream);
        }

        PUT_VERTICAL(stream);
        fputc(NEWLINE, stream);
    }

    PUT_LOWER_EDGE(stream);
    fputs(FILE_LEGEND, stream);
}

piece_t board_get(const board_t *board, int file, int rank)
{
    return board->pieces[file][rank];
}

void board_set(board_t *board, int file, int rank, piece_t piece)
{
    board->pieces[file][rank] = piece;
}

void board_make_move(board_t *board, move_t move)
{
    int from_file = move_get_from_file(move);
    int from_rank = move_get_from_rank(move);
    int to_file = move_get_to_file(move);
    int to_rank = move_get_to_rank(move);

    piece_t piece_to_move = board->pieces[from_file][from_rank];
    piece_t piece_on_target_square = board->pieces[to_file][to_rank];

    board->pieces[from_file][from_rank] = PIECE_NONE;
    board->pieces[to_file][to_rank] = piece_to_move;

    board->side_to_move = (piece_to_move & SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;
}