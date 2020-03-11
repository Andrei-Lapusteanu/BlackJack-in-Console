#pragma once

class Dealer;
class Deck;
class Player;
class Stats;
class Game;

#include "Deck.h"
#include "Stats.h"

class Game
{
public:

	//Constructor
	Game();

	//Main function
	void Start(Game);
	
	//General functions
	void Generate_and_Assign_Cards(Deck&, Player&, Dealer&);
	void Reinitialize(Deck, Player, Dealer);	
	void Replay(Deck&, Player&, Dealer&, Stats&, Game&);

	//Evaluators
	bool Is_BlackJack(Player,int);
	void Evaluate_BlackJack(Deck, Player, Dealer, Stats, Game);
	void Evaluate(Deck, Player, Dealer, Stats, Game);
	void Evaluate_Insurance(Deck, Player, Dealer, Stats, Game);

	//Display functions
	void HUD(Deck&, Player&, Dealer&);
	void Main_Menu(Player&, Stats&);
	void Options_Menu(Player&, Stats&);
	void Stats_Menu(Stats&, Player); 
	void Betting_Menu(Player&, Stats&);
	void Game_Documentation();

	//Auxiliary
	void Show_All_Cards(Deck deck);

	//Destructor
	~Game();


private:

	bool replay;		//no need for setters and getters

};

