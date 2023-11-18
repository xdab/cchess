#include "fen.h"
#include <ctype.h>

int _fen_get_piece_placement(const board_t *board, char *fen);
int _fen_get_side_to_move(const board_t *board, char *fen);
int _fen_get_castling_rights(const board_t *board, char *fen);
int _fen_get_en_passant_square(const board_t *board, char *fen);
int _fen_get_halfmove_clock(const board_t *board, char *fen);
int _fen_get_fullmove_number(const board_t *board, char *fen);

void fen_get(const board_t *board, char *fen)
{
    int i = 0;

    i += _fen_get_piece_placement(board, fen);
    fen[i++] = ' ';

    i += _fen_get_side_to_move(board, fen + i);
    fen[i++] = ' ';

    i += _fen_get_castling_rights(board, fen + i);
    fen[i++] = ' ';

    i += _fen_get_en_passant_square(board, fen + i);
    fen[i++] = ' ';

    i += _fen_get_halfmove_clock(board, fen + i);
    fen[i++] = ' ';

    i += _fen_get_fullmove_number(board, fen + i);
    fen[i++] = '\0';
}

int _fen_put_piece_placement(board_t *board, const char *fen);
int _fen_put_side_to_move(board_t *board, const char *fen);
int _fen_put_castling_rights(board_t *board, const char *fen);
int _fen_put_en_passant_square(board_t *board, const char *fen);
int _fen_put_halfmove_clock(board_t *board, const char *fen);
int _fen_put_fullmove_number(board_t *board, const char *fen);

void fen_put(board_t *board, const char *fen)
{
    int i = _fen_put_piece_placement(board, fen);
    i++;

    i += _fen_put_side_to_move(board, fen + i);
    i++;

    i += _fen_put_castling_rights(board, fen + i);
    i++;

    i += _fen_put_en_passant_square(board, fen + i);
    i++;

    i += _fen_put_halfmove_clock(board, fen + i);
    i++;

    i += _fen_put_fullmove_number(board, fen + i);
    i++;
}

int _fen_get_piece_placement(const board_t *board, char *fen)
{
    int i = 0;
    for (rank_t rank = RANK_8; rank >= RANK_1; rank--)
    {
        int empty_count = 0;
        for (file_t file = FILE_A; file <= FILE_H; file++)
        {
            square_t square = SQUARE_OF(file, rank);
            piece_t piece = board_get(board, square);
            if (piece == PIECE_NONE)
                empty_count++;
            else
            {
                if (empty_count > 0)
                {
                    fen[i++] = '0' + empty_count;
                    empty_count = 0;
                }

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

                fen[i++] = piece_char;
            }
        }

        if (empty_count > 0)
            fen[i++] = '0' + empty_count;

        if (rank > RANK_1)
            fen[i++] = '/';
    }

    return i;
}

int _fen_get_side_to_move(const board_t *board, char *fen)
{
    if (board->side_to_move == WHITE)
        fen[0] = 'w';
    else
        fen[0] = 'b';

    return 1;
}

int _fen_get_castling_rights(const board_t *board, char *fen)
{
    int i = 0;

    if (board->white_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        fen[i++] = 'K';

    if (board->white_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        fen[i++] = 'Q';

    if (board->black_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        fen[i++] = 'k';

    if (board->black_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        fen[i++] = 'q';

    if ((board->white_castling_rights == 0) && (board->black_castling_rights == 0))
        fen[i++] = '-';

    return i;
}

int _fen_get_en_passant_square(const board_t *board, char *fen)
{
    if (!SQUARE_VALID(board->en_passant_square))
    {
        fen[0] = '-';
        return 1;
    }

    fen[0] = FILE_SYMBOL(SQUARE_FILE(board->en_passant_square));
    fen[1] = RANK_SYMBOL(SQUARE_RANK(board->en_passant_square));
    return 2;
}

int _fen_get_halfmove_clock(const board_t *board, char *fen)
{
    int i = 0;
    int halfmove_clock = board->halfmove_clock;
    if (halfmove_clock == 0)
        fen[i++] = '0';
    else
    {
        char buf[FEN_MAX_HALFMOVE_CLOCK_LENGTH];
        int j = 0;
        while (halfmove_clock > 0)
        {
            buf[j++] = '0' + (halfmove_clock % 10);
            halfmove_clock /= 10;
        }

        while (j > 0)
            fen[i++] = buf[--j];
    }

    return i;
}

int _fen_get_fullmove_number(const board_t *board, char *fen)
{
    int i = 0;
    int fullmove_number = board->fullmove_number;
    if (fullmove_number == 0)
        fen[i++] = '0';
    else
    {
        char buf[FEN_MAX_FULLMOVE_NUMBER_LENGTH];
        int j = 0;
        while (fullmove_number > 0)
        {
            buf[j++] = '0' + (fullmove_number % 10);
            fullmove_number /= 10;
        }

        while (j > 0)
            fen[i++] = buf[--j];
    }

    return i;
}

int _fen_put_piece_placement(board_t *board, const char *fen)
{
    file_t file = FILE_A;
    rank_t rank = RANK_8;

    int i = 0;
    char c;

    // Piece placement
    while (((c = fen[i]) != '\0') && (c != ' '))
    {
        if (c == '/')
        {
            file = FILE_A;
            rank--;
        }

        else if (isdigit(c))
        {
            int empty_places = c - '0';
            for (int j = 0; j < empty_places; j++)
                board_set(board, SQUARE_OF(file++, rank), PIECE_NONE);
        }

        else if (isalpha(c))
        {
            piece_t piece = PIECE_NONE;
            char uc = toupper(c);
            if (uc == SYMBOL_PAWN)
                piece = PAWN;
            else if (uc == SYMBOL_KNIGHT)
                piece = KNIGHT;
            else if (uc == SYMBOL_BISHOP)
                piece = BISHOP;
            else if (uc == SYMBOL_ROOK)
                piece = ROOK;
            else if (uc == SYMBOL_QUEEN)
                piece = QUEEN;
            else if (uc == SYMBOL_KING)
                piece = KING;
            piece |= (isupper(c)) ? WHITE : BLACK;

            board_set(board, SQUARE_OF(file++, rank), piece);
        }

        i++;
    }

    return i;
}

int _fen_put_side_to_move(board_t *board, const char *fen)
{
    if (fen[0] == 'w')
        board->side_to_move = WHITE;
    else
        board->side_to_move = BLACK;

    return 1;
}

int _fen_put_castling_rights(board_t *board, const char *fen)
{
    int i = 0;
    board->white_castling_rights = 0;
    board->black_castling_rights = 0;
    while (((fen[i]) != '\0') && (fen[i] != ' '))
    {
        if (fen[i] == 'K')
            board->white_castling_rights |= CASTLING_RIGHTS_KINGSIDE;
        else if (fen[i] == 'Q')
            board->white_castling_rights |= CASTLING_RIGHTS_QUEENSIDE;
        else if (fen[i] == 'k')
            board->black_castling_rights |= CASTLING_RIGHTS_KINGSIDE;
        else if (fen[i] == 'q')
            board->black_castling_rights |= CASTLING_RIGHTS_QUEENSIDE;
        i++;
    }

    return i;
}

int _fen_put_en_passant_square(board_t *board, const char *fen)
{
    if (fen[0] == '-')
    {
        board->en_passant_square = SQUARE_NONE;
        return 1;
    }

    board->en_passant_square = SQUARE_OF(FILE(fen[0]), RANK(fen[1]));
    return 2;
}

int _fen_put_halfmove_clock(board_t *board, const char *fen)
{
    int i = 0;
    board->halfmove_clock = 0;
    while (((fen[i]) != '\0') && (fen[i] != ' '))
    {
        board->halfmove_clock *= 10;
        board->halfmove_clock += fen[i] - '0';
        i++;
    }

    return i;
}

int _fen_put_fullmove_number(board_t *board, const char *fen)
{
    int i = 0;
    board->fullmove_number = 0;
    while (((fen[i]) != '\0') && (fen[i] != ' '))
    {
        board->fullmove_number *= 10;
        board->fullmove_number += fen[i] - '0';
        i++;
    }

    return i;
}