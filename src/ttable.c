#include "ttable.h"

#include <stdlib.h>
#include <string.h>

static ttable_entry_t *ttable;
static long ttable_size;

void ttable_init(long size)
{
    ttable = calloc(size, sizeof(ttable_entry_t));
    ttable_size = size;
}

void ttable_clear()
{
    free(ttable);
    ttable = NULL;
    ttable_size = 0;
}

void ttable_store(const ttable_entry_t *entry)
{
    ttable_entry_t *table_entry = &ttable[entry->hash % ttable_size];
    memcpy(table_entry, entry, sizeof(ttable_entry_t));
}

void ttable_retrieve(zobrist_t hash, ttable_entry_t *out_entry)
{
    ttable_entry_t *table_entry = &ttable[hash % ttable_size];
    memcpy(out_entry, table_entry, sizeof(ttable_entry_t));
}