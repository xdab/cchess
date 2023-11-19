#include "board.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define CCHESS_UNICODE

#define SPACE ' '
#define NEWLINE '\n'
#define ONE '1'
#define FILE_LEGEND "    a b c d e f g h\n"

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

const square_t INITIAL_SQUARE_CONTENTS[SQUARE_COUNT] = {
    BLACK | ROOK,
    BLACK | KNIGHT,
    BLACK | BISHOP,
    BLACK | QUEEN,
    BLACK | KING,
    BLACK | BISHOP,
    BLACK | KNIGHT,
    BLACK | ROOK,

    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,
    BLACK | PAWN,

    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,

    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,

    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,

    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,
    PIECE_NONE,

    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,
    WHITE | PAWN,

    WHITE | ROOK,
    WHITE | KNIGHT,
    WHITE | BISHOP,
    WHITE | QUEEN,
    WHITE | KING,
    WHITE | BISHOP,
    WHITE | KNIGHT,
    WHITE | ROOK,
};

void board_init(board_t *board)
{
    memcpy(board->squares, INITIAL_SQUARE_CONTENTS, SQUARE_COUNT);
    piecepos_init_white(&board->white_piece_positions);
    piecepos_init_black(&board->black_piece_positions);

    board->side_to_move = WHITE;
    board->white_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->black_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->en_passant_square = SQUARE_NONE;

    board->halfmove_clock = 0;
    board->fullmove_number = 1;
    board->history_size = 0;
    board->hash = zobrist_hash(board);
}

void board_clone(const board_t *board, board_t *clone)
{
    memcpy(clone->squares, board->squares, SQUARE_COUNT);
    memcpy(&clone->white_piece_positions, &board->white_piece_positions, sizeof(piecepos_t));
    memcpy(&clone->black_piece_positions, &board->black_piece_positions, sizeof(piecepos_t));

    clone->side_to_move = board->side_to_move;
    clone->white_castling_rights = board->white_castling_rights;
    clone->black_castling_rights = board->black_castling_rights;
    clone->en_passant_square = board->en_passant_square;
    clone->halfmove_clock = board->halfmove_clock;
    clone->fullmove_number = board->fullmove_number;
    clone->history_size = board->history_size;
    clone->hash = board->hash;
}

void board_print(const board_t *board, FILE *stream)
{
    PUT_UPPER_EDGE(stream);

    for (rank_t rank = RANK_8; rank >= RANK_1; rank--)
    {
        fputc(ONE + rank, stream);
        fputc(SPACE, stream);
        PUT_VERTICAL(stream);
        fputc(SPACE, stream);

        for (file_t file = FILE_A; file <= FILE_H; file++)
        {
            piece_t piece = board_get(board, SQUARE_OF(file, rank));

#ifndef CCHESS_UNICODE
            char piece_char = SYMBOL_NONE;
            if (piece & PAWN)
                piece_char = SYMBOL_PAWN;
            else if (piece & KNIGHT)
                piece_char = SYMBOL_KNIGHT;
            else if (piece & BISHOP)
                piece_char = SYMBOL_BISHOP;
            else if (piece & ROOK)
                piece_char = SYMBOL_ROOK;
            else if (piece & QUEEN)
                piece_char = SYMBOL_QUEEN;
            else if (piece & KING)
                piece_char = SYMBOL_KING;
            if (piece & BLACK)
                piece_char = tolower(piece_char);
            fputc(piece_char, stream);
#else
            if (piece & WHITE)
            {
                if (piece & PAWN)
                    fputs("♙", stream);
                else if (piece & KNIGHT)
                    fputs("♘", stream);
                else if (piece & BISHOP)
                    fputs("♗", stream);
                else if (piece & ROOK)
                    fputs("♖", stream);
                else if (piece & QUEEN)
                    fputs("♕", stream);
                else if (piece & KING)
                    fputs("♔", stream);
            }
            else if (piece & BLACK)
            {
                if (piece & PAWN)
                    fputs("♟", stream);
                else if (piece & KNIGHT)
                    fputs("♞", stream);
                else if (piece & BISHOP)
                    fputs("♝", stream);
                else if (piece & ROOK)
                    fputs("♜", stream);
                else if (piece & QUEEN)
                    fputs("♛", stream);
                else if (piece & KING)
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

piece_t board_get(const board_t *board, square_t square)
{
    return board->squares[square];
}

void board_set(board_t *board, square_t square, piece_t piece)
{
    piece_t old_piece = board_get(board, square);
    board->hash = zobrist_update_piece(board, square, old_piece, piece);
    board->squares[square] = piece;
}

