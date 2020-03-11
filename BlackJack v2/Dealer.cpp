#include "StdAfx.h"
#include "Dealer.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Dealer::Dealer(void)
{
	this->card_no = 0;
	this->sum = 0;
	this->bust = false;
	this->Has_Dealer_BJ = false;
	this->ace_available = false;
	this->hand = new Card[11];
}


//------------------------------------------------------------- S E T T E R S  ----------------------------------------------------------------------//


void Dealer::Set_Card_No(int card_no)
{
	this->card_no = card_no;
}

void Dealer::Set_Sum(int sum)
{
	this->sum = sum;
}
void Dealer::Set_Bust(bool bust)
{
	this->bust = bust;
}

void Dealer::Set_Has_Dealer_BJ(bool Has_Dealer_BJ)
{
	this->Has_Dealer_BJ = Has_Dealer_BJ;
}

void Dealer::Set_Ace_Available(bool ace_available)
{
	this->ace_available = ace_available;
}

void Dealer::Set_Hand(int i, Card new_i)
{
	this->hand[i] = new_i;
}


//------------------------------------------------------------- G E T T E R S  ----------------------------------------------------------------------//


int Dealer::Get_Card_No()
{
	return card_no;
}

int Dealer::Get_Sum()
{
	return sum;
}
bool Dealer::Get_Bust()
{
	return bust;
}

bool Dealer::Get_Has_Dealer_BJ()
{
	return Has_Dealer_BJ;
}

bool Dealer::Get_Ace_Available()
{
	return ace_available;
}

Card& Dealer::Get_Hand(int i)
{
	return hand[i];
}

//---------------------------------------------------------- D E A L E R    A I ----------------------------------------------------------------------//

int Dealer::Dealer_Sum(Dealer dealer)										//calculate the sum of card values from player's/dealer's hand function
{
	int sum = 0;

	for (int i = 0; hand[i].Get_Rank() != NULL; i++)
		sum = sum + hand[i].Get_Value();
	return sum;
}

void Dealer::Dealer_AI(Deck deck, Player player, Dealer &dealer, Game game)			//function which determines dealer's action according to his sum of cards
{
	bool stand = true;

	sum = Dealer_Sum(dealer);

	if (ace_available == true)
	{
		if ((sum + 10) <= 21)
		{
			sum = sum + 10;
		}
	}

	if (sum < 17)									//dealer hits
	{
		std::cout << "\n\n   Dealer Hits.";
		hand[card_no] = deck.Get_Deck(deck.Get_Card_No());
		card_no++;
		deck.Set_Card_No(deck.Get_Card_No() + 1);
		stand = false;
		Sleep(1500);
		system("CLS");
		game.HUD(deck, player, dealer);
	}
	else if (sum >= 17)							//dealer stands
	{
		std::cout << "\n\n   Dealer Stands.";
		stand = true;
		Sleep(1500);
		system("CLS");
		game.HUD(deck, player, dealer);
	}

	if (sum > 21)
		bust = true;

	if (stand == false && bust == false)
		Dealer_AI(deck, player, dealer, game);
}


//----------------------------------------------------- G E N E R A L   F U N C T I O N S  -----------------------------------------------------------//


bool Dealer::Verify_Ace_Availability(Dealer dealer)
{
	bool has_ace = false;

	for (int i = 0; Get_Hand(i).Get_Rank() != NULL; i++)
	{
		if (Get_Hand(i).Get_Value() == 1)
		{
			Set_Ace_Available(true);
			has_ace = true;
			break;
		}
	}

	return has_ace;
}


//----------------------------------------------------------------- D E S T R U C T O R ----------------------------------------------------------------------//


Dealer::~Dealer(void)
{
}
