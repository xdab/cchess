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

void board_make_move(board_t *board, move_t move)
{
    square_t from = move_get_from(move);
    piece_t moved_piece = board_get(board, from);
    piece_t promoted_piece = move_get_promoted_piece(move);

    square_t to = move_get_to(move);
    piece_t captured_piece = board_get(board, to);

    // Update piece positions
    piecepos_update(&board->white_piece_positions, &board->black_piece_positions, from, to, moved_piece, promoted_piece, captured_piece);

    // Castling
    if (moved_piece & KING)
    {
        if (moved_piece & WHITE)
        {
            board->white_castling_rights = CASTLING_RIGHTS_NONE;
            if (from == E1)
            {
                if (to == G1)
                {
                    board_set(board, H1, PIECE_NONE);
                    board_set(board, F1, WHITE | ROOK);
                }
                else if (to == C1)
                {
                    board_set(board, A1, PIECE_NONE);
                    board_set(board, D1, WHITE | ROOK);
                }
            }
        }
        else if (moved_piece & BLACK)
        {
            board->black_castling_rights = CASTLING_RIGHTS_NONE;
            if (from == E8)
            {
                if (to == G8)
                {
                    board_set(board, H8, PIECE_NONE);
                    board_set(board, F8, BLACK | ROOK);
                }
                else if (to == C8)
                {
                    board_set(board, A8, PIECE_NONE);
                    board_set(board, D8, BLACK | ROOK);
                }
            }
        }
    }
    else if (moved_piece & ROOK)
    {
        if (moved_piece & WHITE)
        {
            if (from == A1)
                board->white_castling_rights &= ~CASTLING_RIGHTS_QUEENSIDE;
            else if (from == H1)
                board->white_castling_rights &= ~CASTLING_RIGHTS_KINGSIDE;
        }
        else if (moved_piece & BLACK)
        {
            if (from == A8)
                board->black_castling_rights &= ~CASTLING_RIGHTS_QUEENSIDE;
            else if (from == H8)
                board->black_castling_rights &= ~CASTLING_RIGHTS_KINGSIDE;
        }
    }

    board_set(board, from, PIECE_NONE);
    board_set(board, to, (promoted_piece != PIECE_NONE) ? (promoted_piece | (moved_piece & SIDE_MASK)) : moved_piece);

    // Store event in history
    if (board->history_size == BOARD_HISTORY_SIZE)
    {
        for (int i = 0; i < BOARD_HISTORY_SIZE - 1; i++)
            board->history[i] = board->history[i + 1];
        board->history_size--;
    }
    board->history[board->history_size].move = move;
    board->history[board->history_size].captured_piece = captured_piece;
    board->history[board->history_size].white_castling_rights = board->white_castling_rights;
    board->history[board->history_size].black_castling_rights = board->black_castling_rights;
    board->history[board->history_size].en_passant_square = board->en_passant_square;
    board->history[board->history_size].halfmove_clock = board->halfmove_clock;
    board->history_size++;

    board->side_to_move = (moved_piece & WHITE) ? BLACK : WHITE;
    board->halfmove_clock = ((captured_piece != PIECE_NONE) || (moved_piece & PAWN)) ? 0 : (board->halfmove_clock + 1);
    board->fullmove_number += (board->side_to_move == BLACK);
}

void board_unmake_move(board_t *board)
{
    if (board->history_size == 0)
        return;

    board->history_size--;
    move_t move = board->history[board->history_size].move;

    square_t from = move_get_from(move);
    square_t to = move_get_to(move);

    piece_t moved_piece = board_get(board, to);
    piece_t captured_piece = board->history[board->history_size].captured_piece;

    piece_t promoted_piece = move_get_promoted_piece(move);
    if (promoted_piece != PIECE_NONE)
        moved_piece = PAWN | (moved_piece & SIDE_MASK);

    // Castling
    if (moved_piece & KING)
    {
        if (moved_piece & WHITE)
        {
            if (from == E1)
            {
                if (to == G1)
                {
                    board_set(board, H1, WHITE | ROOK);
                    board_set(board, F1, PIECE_NONE);
                }
                else if (to == C1)
                {
                    board_set(board, A1, WHITE | ROOK);
                    board_set(board, D1, PIECE_NONE);
                }
            }
        }
        else if (moved_piece & BLACK)
        {
            if (from == E8)
            {
                if (to == G8)
                {
                    board_set(board, H8, BLACK | ROOK);
                    board_set(board, F8, PIECE_NONE);
                }
                else if (to == C8)
                {
                    board_set(board, A8, BLACK | ROOK);
                    board_set(board, D8, PIECE_NONE);
                }
            }
        }
    }

    board_set(board, from, moved_piece);
    board_set(board, to, captured_piece);

    board->side_to_move = (moved_piece & WHITE) ? WHITE : BLACK;
    board->white_castling_rights = board->history[board->history_size].white_castling_rights;
    board->black_castling_rights = board->history[board->history_size].black_castling_rights;
    board->en_passant_square = board->history[board->history_size].en_passant_square;
    board->halfmove_clock = board->history[board->history_size].halfmove_clock;
    board->fullmove_number -= (board->side_to_move == BLACK);
}
