/*
 * State.cpp
 *
 *  Created on: 26 apr 2018
 *      Author: Lorenzo Rosa (lorenzo.rosa.bo@gmail.com)
 */

#include "State.h"

#include <string>
#include <cstdlib>
#include "stdio.h"

/*
7 O--------O--------O
6 |--O-----O-----O--|
5 |--|--O--O--O--|--|
4 O--O--O     O--O--O
3 |--|--O--O--O--|--|
2 |--O-----O-----O--|
1 O--------O--------O
  a  b  c  d  e  f  g
 */

bool State::setPawnAt2D(int8 x, int8 y, pawn value){
	int i, j, k;
	i = j = k = -1;

	//Convert from char to int
	y= y - '1' + 1;

	if ( x == 'd') { // 'd'
		//Coordinate 'x'
		j = 1;
		//Coordinate 'y'
		if ( y > 0 && y < 4) {
			i = 2;
			k =  3 - y;
		}
		else if ( y == 4) {
			i = 1;
			k = 0;
		}
		else if (y > 4 && y < 8) {
			i = 0;
			k = y % 5;
		}
		else return false;
	}
	else {
		//Coordinate 'x'
		if (x >= 'a' && (x/100) == 0) { // 'a','b','c'
			j = 0;
			k = 2 - (x % 'a');
		}
		else if (x <= 'g' && (x/100) == 1) { // 'e','f','g'
			j = 2;
			k = x % 'e';
		} else return false;

		//Coordinate 'y'
		if ( y > 0 && y < 4 )
			i = 2;
		else if ( y == 4)
			i = 1;
		else if (y > 4 && y < 8)
			i = 0;
		else return false;
	}

	if (i<0 || j<0 || k<0)
		return false;

	setPawnAt(i,j,k, value);
	return true;
}

// If coordinate is not valid, returns -1;
pawn State::getPawnAt2D(int8 x, int8 y) const{
	int i, j, k;
	i = j = k = -1;

	//Convert from char to int
	y= y - '1' + 1;

	if ( x == 'd') { // 'd'
		//Coordinate 'x'
		j = 1;
		//Coordinate 'y'
		if ( y > 0 && y < 4) {
			i = 2;
			k =  3 - y;
		}
		else if ( y == 4) {
			i = 1;
			k = 1;
		}
		else if (y > 4 && y < 8) {
			i = 0;
			k = y % 5;
		}
		else return -1;
	}
	else {
		//Coordinate 'x'
		if (x >= 'a' && (x/100) == 0) { // 'a','b','c'
			j = 0;
			k = 2 - (x % 'a');
		}
		else if (x <= 'g' && (x/100) == 1) { // 'e','f','g'
			j = 2;
			k = x % 'e';
		} else return -1;

		//Coordinate 'y'
		if ( y > 0 && y < 4 )
			i = 2;
		else if ( y == 4)
			i = 1;
		else if (y > 4 && y < 8)
			i = 0;
		else return -1;
	}

	if (i<0 || j<0 || k<0)
		return -1;

	return getPawnAt(i,j,k);
}


void State::setWhitePawnsOnBoardStr(std::string number){
	setPawnAt(1,1,2,atoi(number.c_str()));
}
int8 State::getWhitePawnsOnBoardStr() const {
	return getPawnAt(1,1,2) + '1' - 1;
}

void State::setBlackPawnsOnBoardStr(std::string  number) {
	setPawnAt(1,1,1,atoi(number.c_str()));
}
int8 State::getBlackPawnsOnBoardStr() const {
	return getPawnAt(1,1,1) + '1' - 1;
}

void State::setPawnsOnBoard(pawn pawn, int8 count) {
	if (pawn == PAWN_WHITE)
		setPawnAt(1,1,2,count);
	else if (pawn == PAWN_BLACK)
		setPawnAt(1,1,1, count);
}
int8 State::getPawnsOnBoard(pawn pawn) const {
	if (pawn == PAWN_WHITE)
		return getPawnAt(1,1,2);
	else if (pawn == PAWN_BLACK)
		return getPawnAt(1,1,1);
	return 0;
}

std::string State::toString() const {

	char res[CUBE_SIZE_X*CUBE_SIZE_Y*CUBE_SIZE_Z + 1];

	int l = 0;
	for (int8 i = 0; i < CUBE_SIZE_X; i++)
		for (int8 j = 0; j < CUBE_SIZE_Y; j++)
			for (int8 k = 0; k < CUBE_SIZE_Z; k++){
				if (i==1 && j==1 && k==1 && getPawnAt(i,j,k) != PAWN_NONE)
					res[l] = getPawnAt(i,j,k) < 10 ? (getPawnAt(1,1,1) + '0') : 'A' + (getPawnAt(i,j,k)%10);
				else if (i==1 && j==1 && k==2 && getPawnAt(i,j,k) != PAWN_NONE)
					res[l] = getPawnAt(i,j,k) < 10 ? (getPawnAt(1,1,2) + '0') : 'A' + (getPawnAt(i,j,k)%10);
				else res[l] = getPawnAt(i,j,k);
				l++;
			}
	res[l] = '\0';
	return std::string(res) + '('+ (char)getPhase()+')';

}

int State::hash() {
	//TODO
	return 0;
}

State::~State() {}

std::ostream& operator<<(std::ostream &strm, const State &s){
	return strm << s.toString();
}

void State::toStringToSend() const{
	char ret[25]; //TODO migliorare per inserire la casella al centro
	//24 + \0

	//Insert left face
	int count = 0;
	int i, j=0, k;

	for(k=2; k>=0; k--)
		for(i=2; i>=0; i--){
			ret[count++] = getPawnAt(i,j,k);
			printf("%c", ret[count - 1]);
		}

	//Insert middle line
	j=1;
	i=2;
	for(k=2; k>=0; k--){
		ret[count++] = getPawnAt(i,j,k);
		printf("%c", ret[count - 1]);
	}
	i=0;
	for(k=0; k<=2; k++){
		ret[count++] = getPawnAt(i,j,k);
		printf("%c", ret[count - 1]);
	}

	//Insert right face
	j=2;
	for(k=0; k<=2; k++)
		for(i=2; i>=0; i--){
			ret[count++] = getPawnAt(i,j,k);
			printf("%c", ret[count - 1]);
		}


	ret[count] = '\0';
}

bool State::isInMorris(int8 pos) const {

	bool morrisX = true;
	bool morrisY = true;
	bool morrisZ = true;
	pawn selected = getPawnAt(GETX(pos), GETY(pos), GETZ(pos));

	if (selected == PAWN_NONE)
		return false;

	for (int x = 0; x < CUBE_SIZE_X; x++)
		if (getPawnAt(x, GETY(pos), GETZ(pos)) != selected)
			morrisX = false;

	for (int y = 0; y < CUBE_SIZE_Y; y++)
		if (getPawnAt(GETX(pos), y, GETZ(pos)) != selected)
			morrisY = false;

#if defined(DIAGONALS) && defined(PERPENDICULARS)
	for (int z = 0; z < CUBE_SIZE_Z; z++)
		if (getPawnAt(GETX(pos), GETY(pos), z) != selected)
			morrisZ = false;
#endif

#if !defined(DIAGONALS) && defined(PERPENDICULARS)
	if (ON_PERPENDICULAR(pos)) {
		for (int z = 0; z < CUBE_SIZE_Z; z++)
			if (getPawnAt(GETX(pos), GETY(pos), z) != selected)
				morrisZ = false;
	}
	else
		morrisZ = false;

#endif

#if defined(DIAGONALS) && !defined(PERPENDICULARS)
	if (ON_DIAGONAL(pos)) {
		for (int z = 0; z < CUBE_SIZE_Z; z++)
			if (getPawnAt(GETX(pos), GETY(pos), z) != selected)
				morrisZ = false;
	}
	else
		morrisZ = false;
#endif

	return morrisX || morrisY || morrisZ;

}

bool State::isInMorris(int8 pos, int8 axis) const {

	pawn selected = getPawnAt(GETX(pos), GETY(pos), GETZ(pos));

	if (selected == PAWN_NONE)
		return false;

	switch (axis) {

	case X_AXIS :

		for (int x = 0; x < CUBE_SIZE_X; x++)
			if (getPawnAt(x, GETY(pos), GETZ(pos)) != selected)
				return false;

		break;

	case Y_AXIS :

		for (int y = 0; y < CUBE_SIZE_Y; y++)
			if (getPawnAt(GETX(pos), y, GETZ(pos)) != selected)
				return false;
		break;

	case Z_AXIS :
		for (int z = 0; z < CUBE_SIZE_Z; z++)
			if (getPawnAt(GETX(pos), GETY(pos), z) != selected)
				return false;

		break;
	}

	return true;

}

//Pls specify in a comment what do parameters mean!
bool State::willBeInMorris(int8 src, int8 dest, pawn pawn) const {

	bool morrisX = true;
	bool morrisY = true;
	bool morrisZ = true;

	if (pawn == PAWN_NONE)
		return false;

	if (getPawnAt(GETX(dest), GETY(dest), GETZ(dest)) != PAWN_NONE)
		return false;

	for (int x = 0; x < CUBE_SIZE_X; x++) {
		if (src == NEW_POS(x,GETY(dest), GETZ(dest)))
			morrisX = false;
		if (x != GETX(dest)) {
			if (getPawnAt(x, GETY(dest), GETZ(dest)) != pawn)
				morrisX = false;
		}
	}

	for (int y = 0; y < CUBE_SIZE_Y; y++) {
		if (src == NEW_POS(GETX(dest), y, GETZ(dest)))
			morrisY = false;
		if (y != GETY(dest)) {
			if (getPawnAt(GETX(dest), y, GETZ(dest)) != pawn)
				morrisY = false;
		}
	}

#if defined(DIAGONALS) && defined(PERPENDICULARS)
	for (int z = 0; z < CUBE_SIZE_Z; z++) {
		if (src == NEW_POS(GETX(dest), GETY(dest), z))
			morrisZ = false;
		if (z != GETZ(dest)) {
			if (getPawnAt(GETX(dest), GETY(dest), z) != pawn)
				morrisZ = false;
		}
	}
#endif

#if !defined(DIAGONALS) && defined(PERPENDICULARS)
	if (ON_PERPENDICULAR(dest))
		for (int z = 0; z < CUBE_SIZE_Z; z++) {
			if (src == NEW_POS(GETX(dest), GETY(dest), z))
				morrisZ = false;
			if (z != GETZ(dest)) {
				if (getPawnAt(GETX(dest), GETY(dest), z) != pawn)
					morrisZ = false;
			}
		}
	else
		morrisZ = false;
#endif

#if defined(DIAGONALS) && !defined(PERPENDICULARS)
	if (ON_DIAGONAL(dest))
		for (int z = 0; z < CUBE_SIZE_Z; z++) {
			if (src == NEW_POS(GETX(dest), GETY(dest), z))
				morrisZ = false;
			if (z != GETZ(dest)) {
				if (getPawnAt(GETX(dest), GETY(dest), z) != pawn)
					morrisZ = false;
			}
		}
	else
		morrisZ = false;
#endif

	return morrisX || morrisY || morrisZ;
}

ExpVector<int8> State::getAllPositions(pawn pawn) const {

	ExpVector<int8> result(AVERAGE_PAWNS_ON_BOARD);

	for (int8 x = 0; x < CUBE_SIZE_X; x++)
		for (int8 y = 0; y < CUBE_SIZE_Y; y++)
			for (int8 z = 0; z < CUBE_SIZE_Z; z++)
				if (getPawnAt(x, y, z) == pawn && POS_ENABLED(NEW_POS(x,y,z))) {
					result.add(NEW_POS(x,y,z));
				}

	return result;

}

ExpVector<int8> State::getAvailablePositions(int8 pos) const {

	pawn myPawn = getPawnAt(GETX(pos), GETY(pos), GETZ(pos));

	if (getPhase() == PHASE_1 || getPawnsOnBoard(myPawn) == '3')
		return getAllPositions(PAWN_NONE);

	ExpVector<int8> result(MAX_MOVES_PHASE_2);
	int8 p;

	p = FWX(pos);
	if (GETX(p) != 0 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);

	p = BWX(pos);
	if (GETX(p) != 2 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);

	p = FWY(pos);
	if (GETY(p) != 0 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);

	p = BWY(pos);
	if (GETY(p) != 2 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);

#if defined(DIAGONALS) && defined(PERPENDICULARS)
	p = FWZ(pos);
	if (GETZ(p) != 0 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);

	p = BWZ(pos);
	if (GETZ(p) != 2 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
		result.add(p);
#endif

#if !defined(DIAGONALS) && defined(PERPENDICULARS)
	p = FWZ(pos);
	if (ON_PERPENDICULAR(p)) {
		if (GETZ(p) != 0 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
			result.add(p);

		p = BWZ(pos);
		if (GETZ(p) != 2 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
			result.add(p);
	}
#endif

#if defined(DIAGONALS) && !defined(PERPENDICULARS)
	p = FWZ(pos);
	if (ON_DIAGONAL(p)) {
		if (GETZ(p) != 0 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
			result.add(p);

		p = BWZ(pos);
		if (GETZ(p) != 2 && getPawnAt(GETX(p), GETY(p), GETZ(p)) == PAWN_NONE && POS_ENABLED(p))
			result.add(p);
	}
#endif

	return result;

}
