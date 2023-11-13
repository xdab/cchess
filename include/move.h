#ifndef MOVE_H
#define MOVE_H

#define PROMOTION_NONE 0
#define PROMOTION_QUEEN 1
#define PROMOTION_ROOK 2
#define PROMOTION_BISHOP 3
#define PROMOTION_KNIGHT 4

typedef unsigned short move_t;

move_t move_regular(int from_file, int from_rank, int to_file, int to_rank);
move_t move_promotion(int from_file, int from_rank, int to_file, int to_rank, int promotion_piece);

move_t move_uci(const char *uci);
void move_to_uci(move_t move, char *uci);

int move_get_from_file(move_t move);
int move_get_from_rank(move_t move);
int move_get_to_file(move_t move);
int move_get_to_rank(move_t move);
int move_get_promotion(move_t move);

#endif