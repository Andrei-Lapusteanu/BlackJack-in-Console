#pragma once

#include "Card.h"
#include "Game.h"

class Player
{
public:
	
	//Constructor
	Player(void);

	//Setters
	void Set_Bet(int);
	void Set_Insurance_Bet(int);
	void Set_Funds(int);
	void Set_Card_No(int);
	void Set_Sum(int);
	void Set_Finished_Hand(bool);
	void Set_Surrender_Available(bool);
	void Set_Double_Down_Available(bool);
	void Set_Insurance_Available(bool);
	void Set_Split_Available();
	void Set_Ace_Available(bool);
	void Set_Has_Player_BJ(bool);
	void Set_Bust(bool);
	void Set_Card(int, Card);
	void Set_Betting_Mode(bool);

	//Getters
	int Get_Bet();
	int Get_Insurance_Bet();
	int Get_Funds();
	int Get_Card_No();
	int Get_Sum();
	bool Get_Finished_Hand();
	bool Get_Surrender_Available();
	bool Get_Double_Down_Available();
	bool Get_Insurance_Available();
	bool Get_Split_Available();
	bool Get_Ace_Available();
	bool Get_Has_Player_BJ();
	bool Get_Bust();
	int Get_Betting_Mode();
	Card& Get_Card(int i);

	//General functions
	int Player_Sum(Player);
	void Modify_Funds(int);
	bool Verify_Ace_Availability(Player);

	//Text file read/write
	int Read_Funds(Player);
	void Write_Funds(Player);
	void Reset_Funds(Player&);
	int Read_Betting_Mode(Player&);
	void Write_Betting_Mode(Player&);

	//Player actions
	int Place_Bet(Player&);
	int Place_Chips_Bet(Player&);  
	int Place_Insurance_Bet(Deck&, Player&, Dealer&, Stats&, Game&);
	void Player_Action(Deck, Player&, Dealer, Stats&, Game&);
	void Player_Hit(Deck, Player&, Dealer, Stats, Game);
	void Player_Stand(Deck, Player&, Dealer, Stats&, Game&);
	void Player_Surrender(Deck, Player&, Dealer, Stats&, Game&);
	void Player_Double_Down(Deck, Player&, Dealer, Stats&, Game&);
	void Player_Insurance(Deck, Player&, Dealer, Stats&, Game&);
	void Player_Split(Deck&, Player&, Dealer&, Stats&, Game&);
	
	//Destructor
	~Player(void);

private:

	int bet;
	int insurance_bet;
	int funds;
	int card_no;
	int sum;
	bool finished_hand;
	bool surrender_available;
	bool double_d_available;
	bool insurance_available;
	bool split_available;
	bool ace_available;
	bool Has_Player_BJ;
	bool bust;
	int free_bet;
	bool exit_do_while;
	Card *hand;

};

