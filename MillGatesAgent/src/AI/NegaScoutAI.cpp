/*
 * NegaScoutAI.cpp
 *
 *  Created on: May 11, 2018
 *      Author: Luca
 */

#include "NegaScoutAI.h"

NegaScoutAI::NegaScoutAI() {
	_table = new HashSet<entry>();
	_hasher = ZobristHashing::getInstance();
	_depth = MIN_SEARCH_DEPTH;
	_stopFlag = false;
	_history = new ExpVector<hashcode>();
	_heuristic = new PawnCountHeuristic();
}

eval_t NegaScoutAI::negaScout(State * state, hashcode quickhash, uint8 depth, eval_t alpha, eval_t beta, sint8 color) {

	entry * e;
	bool presentInTable;
	presentInTable = _table->get(quickhash, &e);
	if (presentInTable && e->depth >= depth) {
		if (e->entryFlag == EXACT)
			return color * e->eval;
		if (e->entryFlag == ALPHA_PRUNE && color * e->eval > alpha)
			alpha = e->eval * color;
		if (e->entryFlag == BETA_PRUNE && color * e->eval < beta)
			beta = e->eval * color;
		if (alpha >= beta)
			return alpha;
	}

	eval_t score = 0;
	bool terminal = state->isTerminal();
	bool loop = false;

	for (short i = _history->getLogicSize() - 1; i >= 0; i--)
		if (_history->get(i) == quickhash && _depth + 1 != depth) {
			loop = true;
			break;
		}

	if (depth == 0 || _stopFlag || loop || terminal) {
		score = _heuristic->evaluate(state, terminal, loop);
		if (!presentInTable)
			_table->add(quickhash, entry{depth, EXACT, score});
		else
			*e = {depth, EXACT, score};
		return color * score;
	}

	hashcode child_hash = 0;
	ExpVector<Action> * actions = state->getActions();

	//Ordering section ==> TODO put inside a fuction...
	ExpVector<State*> * states = new ExpVector<State*>(actions->getLogicSize());
	ExpVector<hashcode> * hashes = new ExpVector<hashcode>(actions->getLogicSize());
	ExpVector<eval_t> * values = new ExpVector<eval_t>(actions->getLogicSize());
	entry * e_tmp;

	bool child_loop, thereIs;
	for(eval_t i=0; i<actions->getLogicSize(); i++) {
		states->add(state->result(actions->get(i)));
		hashes->add(_hasher->quickHash(state, actions->get(i), quickhash));
		thereIs = _table->get(quickhash, &e_tmp);
		if (thereIs && e->depth >= depth-1)
			values->add(e_tmp->eval * -color);
		else {//Else I have to estimate the value using function
			for (short j = _history->getLogicSize() - 1; j >= 0; j--)
				if (_history->get(j) == child_hash && _depth + 1 != depth) {
					child_loop = true;
					break;
				}
			values->add(_heuristic->evaluate(states->get(i), states->get(i)->isTerminal(), child_loop) * -color);
		}
	}

	//	quickSort(state, states, hashes, values, actions, 0, actions->getLogicSize()-1, -color, quickhash);
	setMaxFirst(state, states, hashes, values, actions);

	//Negascout
	State * child = NULL;
	entryFlag_t flag = ALPHA_PRUNE;
	for (eval_t i = 0; i < actions->getLogicSize(); i++) {
		child = states->get(i);
		child_hash = hashes->get(i);
		if(i == 0)
			score = -negaScout(child, child_hash, depth - 1, -beta, -alpha, -color);
		else {
			score = -negaScout(child, child_hash, depth - 1, -alpha - 1, -alpha, -color);
			if (score > alpha && score < beta)
				score = -negaScout(child, child_hash, depth - 1, -beta, -score, -color);
		}
		delete child;
		states->set(i, NULL);

		if (score > alpha) {
			alpha = score;
			flag = EXACT;
		}

		if (score >= beta) {
			flag = BETA_PRUNE;
			alpha = beta;
			break;
		}
	}
	delete actions;
	delete hashes;
	for (uint8 i = 0; i < states->getLogicSize(); i++)
		if (states->get(i) != NULL)
			delete states->get(i);
	delete states;
	delete values;

	if (!presentInTable)
		_table->add(quickhash, entry{depth, flag, (eval_t)(alpha * color)});
	else
		*e = {depth, flag, (eval_t)(alpha * color)};

	return alpha;

}

uint8 NegaScoutAI::getDepth() {
	return _depth;
}

void NegaScoutAI::setDepth(uint8 depth) {
	_depth = depth;
}

void NegaScoutAI::clear() {
	_table->clear();
}

void NegaScoutAI::stop() {
	_stopFlag = true;
}

void NegaScoutAI::addHistory(State * state) {
	_history->add(_hasher->hash(state));
}

void NegaScoutAI::clearHistory() {
	delete _history;
	_history = new ExpVector<hashcode>();
}

Action NegaScoutAI::choose(State * state) {

	_stopFlag = false;
	//	if (state->getPawnsToPlay(state->getPlayer()) == 9)
	//		return Action(POS_NULL, Position{2,2,1), POS_NULL);

	ExpVector<Action> * actions = state->getActions();

	Action res;
	eval_t score;
	entry * tempscore;
	hashcode quickhash, hash;

	hash = _hasher->hash(state);

	if (state->getPlayer() == PAWN_WHITE) {
		negaScout(state, hash, _depth + 1, -MAX_EVAL_T, MAX_EVAL_T, 1);
		for (uint8 i = 0; i < actions->getLogicSize(); i++) {
			quickhash = _hasher->quickHash(state, actions->get(i), hash);
			_table->get(quickhash, &tempscore);
			if (tempscore != NULL) {
				if (i == 0 || (tempscore->eval > score && tempscore->depth >= _depth - 1)) {
					score = tempscore->eval;
					res = actions->get(i);
				}
			}
			else {
				std::cout << "NOT FOUND " << quickhash << " -> " << actions->get(i) << "\n";
				//res = actions->get(0);
			}
		}
	}
	else {
		negaScout(state, hash, _depth + 1, -MAX_EVAL_T, MAX_EVAL_T, -1);
		for (uint8 i = 0; i < actions->getLogicSize(); i++) {
			quickhash = _hasher->quickHash(state, actions->get(i), hash);
			_table->get(quickhash, &tempscore);
			if (tempscore != NULL) {
				if (i == 0 || (tempscore->eval < score && tempscore->depth >= _depth - 1)) {
					score = tempscore->eval;
					res = actions->get(i);
				}
			}
			else {
				std::cout << "NOT FOUND " << quickhash << " -> " << actions->get(i) << "\n";
				//res = actions->get(0);
			}
		}
	}

	delete actions;

	return res;
}

void NegaScoutAI::recurprint(State * state, int depth, int curdepth) {
	entry * val = NULL;
	ExpVector<Action> * actions = state->getActions();
	State * child = NULL;
	for (int i = 0; i < actions->getLogicSize(); i++) {
		child = state->result(actions->get(i));
		_table->get(_hasher->hash(child), &val);
		if (val == NULL) {
			for (int j = 0; j < curdepth; j++)
				std::cout << " | ";
			std::cout << actions->get(i) << " -> CUT \n";
		}
		else if (depth == curdepth) {
			for (int j = 0; j < curdepth; j++)
				std::cout << " | ";
			std::cout << actions->get(i) << " -> " << (int)val->eval << ", " << (int)val->entryFlag << ", " << (int)val->depth <<"\n";
		}
		else {
			for (int j = 0; j < curdepth; j++)
				std::cout << " | ";
			std::cout << actions->get(i) << " { \n";
			recurprint(child, depth, curdepth + 1);
			for (int j = 0; j < curdepth; j++)
				std::cout << " | ";
			std::cout << "} -> " << (int)val->eval << ", " << (int)val->entryFlag << ", " << (int)val->depth <<"\n";
		}
		delete child;
	}
}

void NegaScoutAI::print(State * state, int depth) {

	recurprint(state, depth, 0);

}

void NegaScoutAI::setMaxFirst(State * state, ExpVector<State*> * states, ExpVector<hashcode> * hashes, ExpVector<eval_t> * values, ExpVector<Action> * actions) {
	// Find max
	eval_t max = values->get(0);
	eval_t indexMax = 0;
	for(eval_t i=1; i<values->getLogicSize(); i++) {
		if(values->get(i) > max) {
			max = values->get(i);
			indexMax = i;
		}
	}
	if(indexMax!=0) {
		actions->swap(0, indexMax);
		states->swap(0, indexMax);
		hashes->swap(0, indexMax);
		values->swap(0, indexMax);
	}
}

void NegaScoutAI::quickSort(State*state, ExpVector<State*> * states, ExpVector<hashcode> * hashes, ExpVector<eval_t> * values, ExpVector<Action> * actions, eval_t lo, eval_t hi, sint8 color, hashcode quickhash) {
	if(lo < hi) {
		uint8 p = partition(state, states, hashes, values, actions, lo, hi, color, quickhash);
		quickSort(state, states, hashes, values, actions, lo, p-1, color, quickhash);
		quickSort(state, states, hashes, values, actions, p+1, hi, color, quickhash);
	}
}

uint8 NegaScoutAI::partition(State*state, ExpVector<State*> * states, ExpVector<hashcode> * hashes, ExpVector<eval_t> * values, ExpVector<Action> * actions, eval_t lo, eval_t hi, sint8 color, hashcode quickhash){
	//Find the pivot
	sint8 x = values->get(hi);
	eval_t i = lo-1;
	eval_t value;

	for(int j=lo; j<hi-1; j++)   {
		value = values->get(j);
		if (value > x){
			i++;
			actions->swap(i,j);
			//			std::cout << "SWAP: " << actions->get(i) << "(" << value << ") with " << actions->get(j) << "\n";
			states->swap(i,j);
			hashes->swap(i, j);
			values->swap(i,j);
		}
	}
	actions->swap(i+1, hi);
	states->swap(i+1, hi);
	hashes->swap(i+1, hi);
	values->swap(i+1, hi);
	return i+1;
}

NegaScoutAI::~NegaScoutAI() {
	delete _table;
	delete _history;
	delete _heuristic;
}