/*
 * Test.cpp
 *
 *  Created on: 02 mag 2018
 *      Author: Lorenzo Rosa
 */


#include <iostream>
#include <string.h>

#include "../AI/DummyAI.h"
#include "../AI/Hash/ZobristHashing.h"
#include "../AI/IterativeDeepeningAI.h"
#include "../AI/MinMaxAI.h"
#include "../AI/NegaScoutAI.h"
#include "../Domain/CubeStateImpl.h"
#include "../Domain/State.h"
#include "../Utils/HashSet.h"

State * random_state() {
	State * result = new CubeStateImpl();
	uint8 x, y, z, pawn;
	uint8 n = rand() % 24;
	for(uint8 i = 0; i < n; i++) {
		x = rand() % 3;
		y = rand() % 3;
		z = rand() % 3;
		pawn = rand() % 3;
		if (pawn == 0)
			pawn = 'O';
		else if (pawn == 1)
			pawn = 'W';
		else
			pawn = 'B';
		result->setPawnAt(x, y, z, pawn);
	}
	return result;
}

void testZobristHashing(State * state) {

	//	srand(300);
	//
	ZobristHashing * hasher = ZobristHashing::getInstance();

	DummyAI ai;
	Action a;
	srand(7200);

	for(int i = 0; i < 20000000; i++) {
		State * father = random_state();
		a = ai.choose(father);
		State * child = father->result(a);
		if(hasher->hash(child) != hasher->quickHash(father, a, hasher->hash(father))) {
			std::cout << "Error" << i << "\n";
		}
		delete child;
		delete father;
		//std::cout << father->toString() << " " << child->toString() << "\n";

	}

	std::cout << "Fine";
}

void testMinMaxAI(State * state) {

	MinMaxAI ai;
	srand(400);

	state->setPawnAt2D('c', '5', PAWN_WHITE);
	state->setPawnAt2D('a', '1', PAWN_BLACK);
	state->setPawnAt2D('b', '6', PAWN_WHITE);
	state->setPawnAt2D('d', '2', PAWN_BLACK);
	state->setPlayer(PAWN_WHITE);
	state->setPawnsToPlay(PAWN_BLACK, 7);
	state->setPawnsToPlay(PAWN_WHITE, 7);
	state->setPawnsOnBoard(PAWN_WHITE, 2);
	state->setPawnsOnBoard(PAWN_BLACK, 2);

	std::cout << ai.choose(state);

}

void testNegaScoutAI(State * state) {
	NegaScoutAI ai;
	srand(400);

	//	state->setPawnAt2D('b', '4', PAWN_BLACK);
	//	state->setPawnAt2D('c', '4', PAWN_BLACK);
	//	state->setPawnAt2D('d', '5', PAWN_BLACK);
	//	state->setPawnAt2D('d', '6', PAWN_BLACK);
	//	state->setPawnAt2D('c', '3', PAWN_WHITE);
	//	state->setPawnAt2D('e', '3', PAWN_WHITE);
	//	state->setPawnAt2D('c', '5', PAWN_WHITE);
	//	state->setPawnAt2D('e', '5', PAWN_WHITE);
	//	state->setPlayer(PAWN_WHITE);
	//	state->setPawnsToPlay(PAWN_BLACK, 5);
	//	state->setPawnsToPlay(PAWN_WHITE, 5);
	//	state->setPawnsOnBoard(PAWN_WHITE, 4);
	//	state->setPawnsOnBoard(PAWN_BLACK, 4);

	state->setPawnAt2D('d', '1', PAWN_BLACK);
	state->setPawnAt2D('d', '2', PAWN_BLACK);
	state->setPawnAt2D('d', '3', PAWN_BLACK);
	state->setPawnAt2D('d', '5', PAWN_BLACK);
	state->setPawnAt2D('d', '6', PAWN_BLACK);
	state->setPawnAt2D('a', '7', PAWN_BLACK);
	state->setPawnAt2D('b', '4', PAWN_BLACK);
	state->setPawnAt2D('e', '3', PAWN_BLACK);
	state->setPawnAt2D('f', '4', PAWN_BLACK);

	state->setPawnAt2D('b', '6', PAWN_WHITE);
	state->setPawnAt2D('b', '2', PAWN_WHITE);
	state->setPawnAt2D('e', '4', PAWN_WHITE);
	state->setPawnAt2D('e', '5', PAWN_WHITE);
	state->setPawnAt2D('c', '4', PAWN_WHITE);

	state->setPlayer(PAWN_WHITE);
	state->setPawnsToPlay(PAWN_BLACK, 0);
	state->setPawnsToPlay(PAWN_WHITE, 0);
	state->setPawnsOnBoard(PAWN_WHITE, 5);
	state->setPawnsOnBoard(PAWN_BLACK, 9);

	ai.setDepth(6);
	std::cout << ai.choose(state) << "\n\n";

	std::cout << "DEPTH 0" << "\n\n";
	ai.print(state, 0);
	std::cout << "DEPTH 1" << "\n\n";
	ai.print(state, 1);
	std::cout << "DEPTH 2" << "\n\n";
	ai.print(state, 2);
	std::cout << "DEPTH 3" << "\n\n";
	ai.print(state, 3);
	std::cout << "DEPTH 4" << "\n\n";
	ai.print(state, 4);
	std::cout << "DEPTH 5" << "\n\n";
	ai.print(state, 5);
	std::cout << "DEPTH 6" << "\n\n";
	ai.print(state, 6);
}

void testIterativeDeepeningAI(State * state) {

	srand(8000);

	//	state->setPawnAt2D('a', '1', PAWN_BLACK);
	//	state->setPawnAt2D('g', '1', PAWN_BLACK);
	//	state->setPawnAt2D('b', '2', PAWN_BLACK);
	//	state->setPawnAt2D('d', '3', PAWN_BLACK);
	//	state->setPawnAt2D('e', '5', PAWN_BLACK);
	//	state->setPawnAt2D('d', '7', PAWN_BLACK);
	//	state->setPawnAt2D('g', '7', PAWN_BLACK);
	//
	//	state->setPawnAt2D('d', '1', PAWN_WHITE);
	//	state->setPawnAt2D('g', '4', PAWN_WHITE);
	//	state->setPawnAt2D('d', '5', PAWN_WHITE);
	//	state->setPawnAt2D('c', '5', PAWN_WHITE);
	//	state->setPawnAt2D('b', '6', PAWN_WHITE);
	//	state->setPawnAt2D('d', '6', PAWN_WHITE);
	//	state->setPawnAt2D('f', '6', PAWN_WHITE);
	//	state->setPawnAt2D('a', '7', PAWN_WHITE);
	//
	//	state->setPawnsToPlay(PAWN_WHITE, 1);
	//	state->setPawnsToPlay(PAWN_BLACK, 1);
	//	state->setPawnsOnBoard(PAWN_WHITE, 8);
	//	state->setPawnsToPlay(PAWN_BLACK, 7);

	state->setPlayer(PAWN_WHITE);

	IterativeDeepeningAI * ai = new IterativeDeepeningAI();
	ai->setAI(new NegaScoutAI());

	ai->choose(state);
	ai->choose(state);

}

void testMorrisCount(State * state) {

	state->setPawnAt2D('a', '1', PAWN_WHITE);
	state->setPawnAt2D('a', '4', PAWN_WHITE);
	state->setPawnAt2D('a', '7', PAWN_WHITE);
	state->setPawnAt2D('b', '2', PAWN_WHITE);
	state->setPawnAt2D('b', '4', PAWN_WHITE);
	state->setPawnAt2D('b', '6', PAWN_WHITE);
	state->setPawnAt2D('c', '3', PAWN_WHITE);
	state->setPawnAt2D('c', '4', PAWN_WHITE);
	state->setPawnAt2D('c', '5', PAWN_WHITE);

	std::cout << (int)state->morrisCount(PAWN_WHITE);
}

void testBlockedPawns(State * state) {

	state->setPawnAt2D('a', '1', PAWN_BLACK);
	state->setPawnAt2D('a', '7', PAWN_WHITE);
	state->setPawnAt2D('a', '4', PAWN_BLACK);
	state->setPawnAt2D('b', '4', PAWN_BLACK);
	state->setPawnAt2D('b', '2', PAWN_BLACK);
	state->setPawnAt2D('d', '2', PAWN_WHITE);
	state->setPawnAt2D('d', '1', PAWN_WHITE);

	std::cout << (int)state->blockedPawnCount(PAWN_BLACK);

}

void testPotentialMorrises(State * state) {

	state->setPawnAt2D('d', '1', PAWN_BLACK);
	state->setPawnAt2D('b', '2', PAWN_BLACK);
	state->setPawnAt2D('f', '2', PAWN_BLACK);
	state->setPawnAt2D('d', '3', PAWN_BLACK);

	std::cout << (int)state->potentialMorrisCount(PAWN_BLACK);

}

void testPotentialDoubleMorrises(State * state) {
	state->setPawnAt2D('a', '1', PAWN_BLACK);
	state->setPawnAt2D('a', '7', PAWN_BLACK);
	state->setPawnAt2D('d', '7', PAWN_BLACK);
	state->setPawnAt2D('d', '1', PAWN_BLACK);

	std::cout << (int)state->potentialDoubleMorrisCount(PAWN_BLACK);
}

void testDoubleMorrisesCount(State * state) {

	state->setPawnAt2D('a', '1', PAWN_BLACK);
	state->setPawnAt2D('a', '4', PAWN_BLACK);
	state->setPawnAt2D('a', '7', PAWN_BLACK);
	state->setPawnAt2D('d', '1', PAWN_BLACK);
	state->setPawnAt2D('g', '1', PAWN_BLACK);
	state->setPawnAt2D('g', '4', PAWN_BLACK);
	state->setPawnAt2D('g', '7', PAWN_BLACK);
	state->setPawnAt2D('d', '7', PAWN_BLACK);

	std::cout << (int)state->doubleMorrisCount(PAWN_BLACK) << "\n";

}

void testOrdering(State * state) {
	state->setPawnAt2D('a', '1', PAWN_WHITE);
	state->setPawnAt2D('a', '7', PAWN_WHITE);
	state->setPawnAt2D('d', '7', PAWN_BLACK);
	state->setPawnAt2D('d', '1', PAWN_BLACK);
	state->setPawnsOnBoard(PAWN_WHITE, 2);
	state->setPawnsOnBoard(PAWN_BLACK, 2);

	RomanianHeuristic h;
	ExpVector<Action> * actions = state->getActions();
	State * child = NULL;
	for(int i = 0; i < actions->getLogicSize(); i++){
		std::cout << actions->get(i) << "(";
		child = state->result(actions->get(i));
		std::cout << h.evaluate(child, false, false) << ") ";
	}
	std::cout << "\n";
	NegaScoutAI ai;
	ExpVector<State*> * states = new ExpVector<State*>(actions->getLogicSize());
	for(eval_t i = 0; i < actions->getLogicSize(); i++){
		states->add(state->result(actions->get(i)));
	}
	ExpVector<hashcode> * hashes = new ExpVector<hashcode>(actions->getLogicSize());
	for(eval_t i=0; i<actions->getLogicSize(); i++) {
		hashes->add(ZobristHashing::getInstance()->quickHash(state, actions->get(i), ZobristHashing::getInstance()->hash(state)));
	}
	ExpVector<eval_t> * values = new ExpVector<eval_t>(actions->getLogicSize());
	for(eval_t i=0; i<actions->getLogicSize(); i++) {
		values->add(h.evaluate(states->get(i), false, false) * 1);
	}
	ai.quickSort(state, states, hashes, values, actions, 0, actions->getLogicSize()-1, 1, ZobristHashing::getInstance()->hash(state));
	std::cout << "\n" << "NEW:\n";
	for(int i = 0; i < actions->getLogicSize(); i++){
		std::cout << actions->get(i) << "(";
		child = state->result(actions->get(i));
		std::cout << h.evaluate(child, false, false) << ") ";
	}
	std::cout << "\nSize: " << states->getLogicSize()<<"\n";
	State * ch = NULL;
	for(int i = 0; i < states->getLogicSize(); i++){
		ch = states->get(i);
		std::cout << ch->toString() << " ";
	}
	delete actions;
	for(int i = 0; i < states->getLogicSize(); i++)
		delete states->get(i);
	delete states;
	delete hashes;


}

void testHashTable() {

	HashSet<int> * hashTable = new HashSet<int>;

	int * val = new int();
	*val = 10;
	int * val2 = new int();
	*val2 = 30;

	hashTable->add(100, 223);
	hashTable->add(167772214, 222);

	hashTable->get(100, &val);
	hashTable->get(167772214, &val2);

	std::cout << *val << "\n";
	std::cout << *val2 << "\n";
}

#if defined(DEBUG)
int main(void) {

	State * state = new CubeStateImpl();

	//testZobristHashing(state);
	testNegaScoutAI(state);
	//testIterativeDeepeningAI(state);
	//testMorrisCount(state);
	//testBlockedPawns(state);
	//testPotentialMorrises(state);
	//testPotentialDoubleMorrises(state);
	//testDoubleMorrisesCount(state);
	//testOrdering(state);
	//testHashTable();


	delete state;

}
#endif
