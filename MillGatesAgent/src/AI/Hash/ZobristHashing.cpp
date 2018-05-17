/*
 * ZobristHashing.cpp
 *
 *  Created on: May 8, 2018
 *      Author: Luca
 */

#include "ZobristHashing.h"

#include <stdlib.h>

ZobristHashing* ZobristHashing::instance = NULL;

ZobristHashing* ZobristHashing::getInstance() {
	if(instance == NULL)
		instance = new ZobristHashing();
	return instance;
}

ZobristHashing::ZobristHashing() {

	unsigned long long h = 0;

	for (uint8 i = 0; i < HASH_TABLE_WIDTH; i++)
		for (uint8 j = 0; j < HASH_TABLE_HEIGHT; j++) {
			h = rand();
			h = (h << 32) | rand();
			_table[i][j] = (hashcode) h;
		}

}

hashcode ZobristHashing::hash(State * state) const {

	hashcode hash = 0;
	uint8 j = 0;
	uint8 i = 0;

	for (sint8 x = 0; x < CUBE_SIZE_X; x++)
		for (sint8 y = 0; y < CUBE_SIZE_Y; y++)
			for (sint8 z = 0; z < CUBE_SIZE_Z; z++) {
				if (POS_ENABLED(x,y)) {
					if ((j = state->getPawnAt(x, y, z)) != PAWN_NONE) {
						j = (j == PAWN_WHITE) ? HASH_PAWN_WHITE : HASH_PAWN_BLACK;
						hash ^= _table[i][j];
					}
				}
				i++;
			}
	return hash;
}

hashcode ZobristHashing::quickHash(State * state, Action action, hashcode previousHash) const {

	Position pos;
	uint8 j;
	uint8 i;
	hashcode h = previousHash;
	pos = action.getSrc();
	if (IS_VALID(pos)) {
		i = pos.x * CUBE_SIZE_Y * CUBE_SIZE_Z + pos.y * CUBE_SIZE_Z + pos.z;
		j = (state->getPlayer() == PAWN_WHITE ? HASH_PAWN_WHITE : HASH_PAWN_BLACK);
		h ^= _table[i][j];
	}
	pos = action.getDest();
	if (IS_VALID(pos)) {
		i = pos.x * CUBE_SIZE_Y * CUBE_SIZE_Z + pos.y * CUBE_SIZE_Z + pos.z;
		j = (state->getPlayer() == PAWN_WHITE ? HASH_PAWN_WHITE : HASH_PAWN_BLACK);
		h ^= _table[i][j];
	}
	pos = action.getRemovedPawn();
	if (IS_VALID(pos)) {
		i = pos.x * CUBE_SIZE_Y * CUBE_SIZE_Z + pos.y * CUBE_SIZE_Z + pos.z;
		j = (OPP(state->getPlayer()) == PAWN_WHITE ? HASH_PAWN_WHITE : HASH_PAWN_BLACK);
		h ^= _table[i][j];
	}

	return h;

//	State * child = state->result(action);
//	int res = hash(child);
//	delete child;
//	return res;
}

ZobristHashing::~ZobristHashing() {
	// TODO Auto-generated destructor stub
}

