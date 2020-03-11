#include "StdAfx.h"
#include "Deck.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Deck::Deck(void)
{
	this->card_no = 0;
}


//------------------------------------------------------------- S E T T E R S  ----------------------------------------------------------------------//


void Deck::Set_Card_No(int card_no)
{
	this->card_no = card_no;
}

void Deck::Set_Deck(int i, Card *new_i)
{
	this->deck = new_i;
}

//------------------------------------------------------------- G E T T E R S  -----------------------------------------------------------------------//


int Deck::Get_Card_No()
{
	return card_no;
}

Card& Deck::Get_Deck(int i)
{
	return deck[i];
}
//-------------------------------------------------------- C R E A T E   C A R D S -------------------------------------------------------------------//


void Deck::Generate_Deck()
{
	this->deck = new Card [52];
}

void Deck::Generate_Cards()
{
	int card_no=0;

	for(int i=1;i<14;i++)
	{
		for(int j=1;j<5;j++)		//generate rank and suit
		{
			deck[card_no].Set_Rank(i);
			deck[card_no].Set_Suit(j-1);
			card_no++;
		}
	}

		card_no = 0;

	for(int i=1;i<27;i++)
	{
		for(int j=0;j<2;j++)		//generate color
		{
			if(i%2==0)
			{
				deck[card_no].Set_Color(0);
				card_no++;
			}
			else if(i%2 == 1)
			{
				deck[card_no].Set_Color(1);
				card_no++;
			}
		}
	}
}

void Deck::Shuffle_Cards()
{
	std::srand(time(NULL));
	std::random_shuffle(deck,deck+52);
	std::cout<<"\n";
}


//------------------------------------------------------ G E N E R A L   F U N C T I O N S -------------------------------------------------------------//


void Deck::Initial_Deal(Deck &deck, Player &player, Dealer &dealer)		//deal initial cards function
{
	player.Set_Card(0, deck.deck[0]);
	dealer.Set_Hand(0, deck.deck[1]);
	player.Set_Card(1, deck.deck[2]);
	dealer.Set_Hand(1, deck.deck[3]);

	player.Set_Card_No(player.Get_Card_No() + 2);
	dealer.Set_Card_No(dealer.Get_Card_No() + 2);
	card_no = card_no + 4;
}


//----------------------------------------------------------------- D E S T R U C T O R ----------------------------------------------------------------------//

Deck::~Deck(void)
{
}
