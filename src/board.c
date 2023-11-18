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

void board_init(board_t *board)
{
    memset(board->squares, PIECE_NONE, SQUARE_COUNT);
    board->hash = 0;
    
    board_set(board, A1, PIECE_ROOK | SIDE_WHITE);
    board_set(board, B1, PIECE_KNIGHT | SIDE_WHITE);
    board_set(board, C1, PIECE_BISHOP | SIDE_WHITE);
    board_set(board, D1, PIECE_QUEEN | SIDE_WHITE);
    board_set(board, E1, PIECE_KING | SIDE_WHITE);
    board_set(board, F1, PIECE_BISHOP | SIDE_WHITE);
    board_set(board, G1, PIECE_KNIGHT | SIDE_WHITE);
    board_set(board, H1, PIECE_ROOK | SIDE_WHITE);

    for (file_t file = FILE_A; file <= FILE_H; file++)
        board_set(board, SQUARE_OF(file, RANK_2), PIECE_PAWN | SIDE_WHITE);

    board_set(board, A8, PIECE_ROOK | SIDE_BLACK);
    board_set(board, B8, PIECE_KNIGHT | SIDE_BLACK);
    board_set(board, C8, PIECE_BISHOP | SIDE_BLACK);
    board_set(board, D8, PIECE_QUEEN | SIDE_BLACK);
    board_set(board, E8, PIECE_KING | SIDE_BLACK);
    board_set(board, F8, PIECE_BISHOP | SIDE_BLACK);
    board_set(board, G8, PIECE_KNIGHT | SIDE_BLACK);
    board_set(board, H8, PIECE_ROOK | SIDE_BLACK);

    for (file_t file = FILE_A; file <= FILE_H; file++)
        board_set(board, SQUARE_OF(file, RANK_7), PIECE_PAWN | SIDE_BLACK);

    for (file_t file = FILE_A; file <= FILE_H; file++)
        for (rank_t rank = RANK_3; rank <= RANK_6; rank++)
            board_set(board, SQUARE_OF(file, rank), PIECE_NONE);

    board->side_to_move = SIDE_WHITE;
    board->white_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->black_castling_rights = CASTLING_RIGHTS_KINGSIDE | CASTLING_RIGHTS_QUEENSIDE;
    board->en_passant_square = SQUARE_NONE;

    board->halfmove_clock = 0;
    board->fullmove_number = 1;
    board->history_size = 0;
}

void board_clone(const board_t *board, board_t *clone)
{
    memcpy(clone->squares, board->squares, SQUARE_COUNT);

    clone->side_to_move = board->side_to_move;
    clone->white_castling_rights = board->white_castling_rights;
    clone->black_castling_rights = board->black_castling_rights;
    clone->en_passant_square = board->en_passant_square;
    clone->halfmove_clock = board->halfmove_clock;
    clone->fullmove_number = board->fullmove_number;
    clone->history_size = board->history_size;
    clone->hash = board->hash;
}

void board_print(board_t *board, FILE *stream)
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

    square_t to = move_get_to(move);
    piece_t captured_piece = board_get(board, to);

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

    piece_t promoted_piece = move_get_promoted_piece(move);
    if (promoted_piece != PIECE_NONE)
        moved_piece = promoted_piece | (moved_piece & SIDE_MASK);

    board_set(board, from, PIECE_NONE);
    board_set(board, to, moved_piece);
    board->side_to_move = (moved_piece & SIDE_WHITE) ? SIDE_BLACK : SIDE_WHITE;
    board->halfmove_clock = ((captured_piece != PIECE_NONE) || (moved_piece & PIECE_PAWN)) ? 0 : (board->halfmove_clock + 1);
    board->fullmove_number += (board->side_to_move == SIDE_BLACK);
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
        moved_piece = PIECE_PAWN | (moved_piece & SIDE_MASK);

    board_set(board, from, moved_piece);
    board_set(board, to, captured_piece);
    board->side_to_move = (moved_piece & SIDE_WHITE) ? SIDE_WHITE : SIDE_BLACK;
    board->white_castling_rights = board->history[board->history_size].white_castling_rights;
    board->black_castling_rights = board->history[board->history_size].black_castling_rights;
    board->en_passant_square = board->history[board->history_size].en_passant_square;
    board->halfmove_clock = board->history[board->history_size].halfmove_clock;
    board->fullmove_number -= (board->side_to_move == SIDE_BLACK);
}