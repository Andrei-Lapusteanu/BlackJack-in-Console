#pragma once

#include "Player.h"
#include "Dealer.h"

class Deck
{
public:
	
	//Constructor
	Deck(void);
	
	//Setters
	void Set_Card_No(int);
	void Set_Deck(int,Card*);

	//Getters
	int Get_Card_No();

	Card& Get_Deck(int);

	//Create Cards
	void Generate_Deck();
	void Generate_Cards();
	void Shuffle_Cards();
	
	//General functions
	void Initial_Deal(Deck&, Player&, Dealer&);

	//Desctructor
	~Deck(void);

private:

	int card_no;
	Card *deck;

};

