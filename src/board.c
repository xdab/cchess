#include "board.h"
#include <stdio.h>
#include <ctype.h>

#define SPACE ' '
#define VERTICAL '|'
#define NEWLINE '\n'
#define ONE '1'

#define HORIZONTAL_EDGE "  +-----------------+\n"
#define FILE_LEGEND "    a b c d e f g h\n"

#define NO_EN_PASSANT -1

void board_init(board_t *board)
{
    board->board[FILE_A][RANK_1] = PIECE_ROOK | SIDE_WHITE;
    board->board[FILE_B][RANK_1] = PIECE_KNIGHT | SIDE_WHITE;
    board->board[FILE_C][RANK_1] = PIECE_BISHOP | SIDE_WHITE;
    board->board[FILE_D][RANK_1] = PIECE_QUEEN | SIDE_WHITE;
    board->board[FILE_E][RANK_1] = PIECE_KING | SIDE_WHITE;
    board->board[FILE_F][RANK_1] = PIECE_BISHOP | SIDE_WHITE;
    board->board[FILE_G][RANK_1] = PIECE_KNIGHT | SIDE_WHITE;
    board->board[FILE_H][RANK_1] = PIECE_ROOK | SIDE_WHITE;

    for (int file = FILE_A; file <= FILE_H; file++)
        board->board[file][RANK_2] = PIECE_PAWN | SIDE_WHITE;

    board->board[FILE_A][RANK_8] = PIECE_ROOK | SIDE_BLACK;
    board->board[FILE_B][RANK_8] = PIECE_KNIGHT | SIDE_BLACK;
    board->board[FILE_C][RANK_8] = PIECE_BISHOP | SIDE_BLACK;
    board->board[FILE_D][RANK_8] = PIECE_QUEEN | SIDE_BLACK;
    board->board[FILE_E][RANK_8] = PIECE_KING | SIDE_BLACK;
    board->board[FILE_F][RANK_8] = PIECE_BISHOP | SIDE_BLACK;
    board->board[FILE_G][RANK_8] = PIECE_KNIGHT | SIDE_BLACK;
    board->board[FILE_H][RANK_8] = PIECE_ROOK | SIDE_BLACK;

    for (int file = FILE_A; file <= FILE_H; file++)
        board->board[file][RANK_7] = PIECE_PAWN | SIDE_BLACK;

    for (int file = FILE_A; file <= FILE_H; file++)
        for (int rank = RANK_3; rank <= RANK_6; rank++)
            board->board[file][rank] = PIECE_NONE;

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
    fputs(HORIZONTAL_EDGE, stream);

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
        fputc(ONE + rank, stream);
        fputc(SPACE, stream);
        fputc(VERTICAL, stream);
        fputc(SPACE, stream);

        for (int file = FILE_A; file <= FILE_H; file++)
        {
            piece_t piece = board->board[file][rank];
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
            fputc(SPACE, stream);
        }

        fputc(VERTICAL, stream);
        fputc(NEWLINE, stream);
    }

    fputs(HORIZONTAL_EDGE, stream);
    fputs(FILE_LEGEND, stream);
}

void board_make_move(board_t *board, move_t move)
{
    int from_file = move_get_from_file(move);
    int from_rank = move_get_from_rank(move);
    int to_file = move_get_to_file(move);
    int to_rank = move_get_to_rank(move);

    piece_t piece_to_move = board->board[from_file][from_rank];
    piece_t piece_on_target_square = board->board[to_file][to_rank];
    
    board->board[from_file][from_rank] = PIECE_NONE;
    board->board[to_file][to_rank] = piece_to_move;
}