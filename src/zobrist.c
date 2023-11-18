#include "zobrist.h"
#include "random.h"

#define Z_WHITE_PAWN (WHITE | PAWN)

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
	for (square_t square = SQUARE_MIN; square <= SQUARE_MAX; square++)
	{
		piece_t piece = board_get(board, square); // (65 - 160)
		int piece_int = (int)piece - Z_WHITE_PAWN;  // (0 - 95)

		int feature_key = 64 * piece_int + square; // feature index (0 - 6143)
		if (piece != PIECE_NONE)
			hash ^= zobrist_features[feature_key];
	}

	// Black to move (feature index 6144)
	if (board->side_to_move == BLACK)
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
	if (board->en_passant_square != SQUARE_NONE)
	{
		file_t file = SQUARE_FILE(board->en_passant_square);
		rank_t rank = SQUARE_RANK(board->en_passant_square);
		int square_int = rank * FILE_COUNT + file; // (0 - 63)
		int feature_key = EN_PASSANT_SQUARE_FEATURE_START + square_int;
		hash ^= zobrist_features[feature_key];
	}

	return hash;
}

zobrist_t zobrist_update_piece(const board_t *board, square_t square, piece_t old_piece, piece_t new_piece)
{
	zobrist_t hash = board->hash;
	int square_int = SQUARE_RANK(square) * FILE_COUNT + SQUARE_FILE(square); // (0 - 63)

	if (old_piece >= Z_WHITE_PAWN)
	{
		int old_piece_int = (int)old_piece - Z_WHITE_PAWN;	   // (0 - 95)
		int old_feature_key = 64 * old_piece_int + square_int; // (0 - 6143)
		hash ^= zobrist_features[old_feature_key];
	}

	if (new_piece >= Z_WHITE_PAWN)
	{
		int new_piece_int = (int)new_piece - Z_WHITE_PAWN;	   // (0 - 95)
		int new_feature_key = 64 * new_piece_int + square_int; // (0 - 6143)
		hash ^= zobrist_features[new_feature_key];
	}

	return hash;
}