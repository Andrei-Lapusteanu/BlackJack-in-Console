#include "StdAfx.h"
#include "Card.h"

#include <iostream>
#include <string>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Card::Card(void)
{
	this->rank=0;
	this->color=true;
	this->suit=0;
	this->value=0;
}


//------------------------------------------------------------- S E T T E R S  ----------------------------------------------------------------------//


void Card::Set_Rank(int rank)
{
	this->rank = rank;
}

void Card::Set_Color(int color)
{
	this->color = color;
}

void Card::Set_Suit(int suit)
{
	this->suit = suit;
}

void Card::Set_Value(int value)
{
	this->value = value;
}


//------------------------------------------------------------- G E T T E R S  -----------------------------------------------------------------------//


int Card::Get_Rank()
{
	return rank;
}

int Card::Get_Color()
{
	return color;
}

int Card::Get_Suit()
{
	return suit;
}

int Card::Get_Value()
{
	return value;
}


//----------------------------------------------- A S S I G N   V A L U E S   T O   C A R D S  -------------------------------------------------------//


void Card::Card_Value()		//assign a value to each card (2-9 have intrisec value, J-K-Q have value 10, A has value 1 or 11)
{
	if(rank == 1)
		value = 1;
	else
	if(rank == 2)
		value = 2;
	else
	if(rank == 3)
		value = 3;
	else
	if(rank == 4)
		value = 4;
	else
	if(rank == 5)
		value = 5;
	else
	if(rank == 6)
		value = 6;
	else
	if(rank == 7)
		value = 7;
	else
	if(rank == 8)
		value = 8;
	else
	if(rank == 9)
		value = 9;
	else
	if(rank == 10)
		value = 10;
	else
	if(rank == 11)
		value = 10;
	else
	if(rank == 12)
		value = 10;
	else
	if(rank == 13)
		value = 10;
}


//----------------------------------------------------------------- D E S T R U C T O R ----------------------------------------------------------------------//


Card::~Card(void)
{
}
