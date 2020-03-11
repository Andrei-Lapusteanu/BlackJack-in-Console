#pragma once

class Game;

#include "Game.h"

class Dealer
{
public:

	//constructor
	Dealer(void);

	//Setters
	void Set_Card_No(int);
	void Set_Sum(int);
	void Set_Bust(bool);
	void Set_Has_Dealer_BJ(bool);
	void Set_Ace_Available(bool);
	void Set_Hand(int, Card);

	//Getters
	int Get_Card_No();
	int Get_Sum();
	bool Get_Bust();
	bool Get_Has_Dealer_BJ();
	bool Get_Ace_Available();
	Card& Get_Hand(int);

	//Dealer AI
	int Dealer_Sum(Dealer);
	void Dealer_AI(Deck, Player, Dealer&, Game);

	//General functions
	bool Verify_Ace_Availability(Dealer);

	//destructor
	~Dealer(void);

private:

	int card_no;
	int sum;
	bool bust;
	bool Has_Dealer_BJ;
	bool ace_available;
	Card *hand;

};

