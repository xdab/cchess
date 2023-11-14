#include "zobrist.h"
#include "random.h"

#define WHITE_PAWN (SIDE_WHITE | PIECE_PAWN)
#define BLACK_KING (SIDE_BLACK | PIECE_KING)

#define BLACK_TO_MOVE_FEATURE 6144
#define WHITE_KINGSIDE_CASTLING_RIGHTS_FEATURE 6145
#define WHITE_QUEENSIDE_CASTLING_RIGHTS_FEATURE 6146
#define BLACK_KINGSIDE_CASTLING_RIGHTS_FEATURE 6147
#define BLACK_QUEENSIDE_CASTLING_RIGHTS_FEATURE 6148
#define EN_PASSANT_SQUARE_FEATURE_START 6149

#define ZOBRIST_FEATURE_TABLE_SIZE 6213
static zobrist_t zobrist_features[ZOBRIST_FEATURE_TABLE_SIZE];

void zobrist_init()
{
	for (int i = 0; i < ZOBRIST_FEATURE_TABLE_SIZE; i++)
		zobrist_features[i] = random_next();
}

zobrist_t zobrist_hash(const board_t *board)
{
	zobrist_t hash = 0;

	// Specific pieces on specific squares (0x0000 - 0xFFFF)
	for (int rank = RANK_1; rank <= RANK_8; rank++)
		for (int file = FILE_A; file <= FILE_H; file++)
		{
			square_t square = square_of(file, rank);
			int square_int = rank * FILE_COUNT + file; // (0 - 63)

			piece_t piece = board_get(board, square); // (65 - 160)
			int piece_int = (int)piece - WHITE_PAWN;  // (0 - 95)

			int feature_key = 64 * piece_int + square_int; // feature index (0 - 6143)
			if (piece != PIECE_NONE)
				hash ^= zobrist_features[feature_key];
		}

	// Black to move (feature index 6144)
	if (board->side_to_move == SIDE_BLACK)
		hash ^= zobrist_features[BLACK_TO_MOVE_FEATURE];

	// Castling rights (feature index 6145 - 6148)
	if (board->white_castling_rights & CASTLING_RIGHTS_KINGSIDE)
		hash ^= zobrist_features[WHITE_KINGSIDE_CASTLING_RIGHTS_FEATURE];
	if (board->white_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
		hash ^= zobrist_features[WHITE_QUEENSIDE_CASTLING_RIGHTS_FEATURE];
	if (board->black_castling_rights & CASTLING_RIGHTS_KINGSIDE)
		hash ^= zobrist_features[BLACK_KINGSIDE_CASTLING_RIGHTS_FEATURE];
	if (board->black_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
		hash ^= zobrist_features[BLACK_QUEENSIDE_CASTLING_RIGHTS_FEATURE];

	// En passant square (feature index 6149 - 6212)
	if (board->en_passant_square != SQUARE_NULL)
	{
		int file = square_file(board->en_passant_square);
		int rank = square_rank(board->en_passant_square);
		int square_int = rank * FILE_COUNT + file; // (0 - 63)
		int feature_key = EN_PASSANT_SQUARE_FEATURE_START + square_int;
		hash ^= zobrist_features[feature_key];
	}

	return hash;
}

zobrist_t zobrist_update_piece(const board_t *board, square_t square, piece_t old_piece, piece_t new_piece)
{
	zobrist_t hash = board->hash;
	int square_int = square_rank(square) * FILE_COUNT + square_file(square); // (0 - 63)

	if (old_piece >= WHITE_PAWN)
	{
		int old_piece_int = (int)old_piece - WHITE_PAWN;	   // (0 - 95)
		int old_feature_key = 64 * old_piece_int + square_int; // (0 - 6143)
		hash ^= zobrist_features[old_feature_key];
	}

	if (new_piece >= WHITE_PAWN)
	{
		int new_piece_int = (int)new_piece - WHITE_PAWN;	   // (0 - 95)
		int new_feature_key = 64 * new_piece_int + square_int; // (0 - 6143)
		hash ^= zobrist_features[new_feature_key];
	}

	return hash;
}