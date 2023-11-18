#ifndef TTABLE_H
#define TTABLE_H

#include "zobrist.h"
#include "move.h"
#include "eval.h"

typedef enum ttable_entry_type
{
    TTABLE_ENTRY_EXACT,
    TTABLE_ENTRY_ALPHA,
    TTABLE_ENTRY_BETA
} ttable_entry_type_t;

typedef struct ttable_entry
{
    zobrist_t hash;
    ttable_entry_type_t type;
    move_t best_move;
    score_t score;
    int depth;
} ttable_entry_t;

/**
 * Initializes the transposition table.
 *
 * @param size The size of the transposition table in entries.
 */
void ttable_init(long size);

/**
 * Clears the transposition table.
 */
void ttable_clear();

/**
 * Stores a transposition table entry.
 *
 * @param entry The entry to store.
 */
void ttable_store(const ttable_entry_t *entry);

/**
 * Retrieves a transposition table entry.
 *
 * @param hash The hash of the entry to retrieve.
 * @param out_entry Pointer where to store the retrieved entry.
 */
void ttable_retrieve(zobrist_t hash, ttable_entry_t *out_entry);

#endif