#include "StdAfx.h"
#include "Player.h"

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <fstream>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Player::Player(void)
{
	this->bet = 0;
	this->insurance_bet = 0;
	this->funds = 0;						
	this->card_no = 0;
	this->sum = 0;
	this->finished_hand = false;
	this->surrender_available = true;
	this->double_d_available = true;
	this->insurance_available = false;
	this->split_available = false;
	this->ace_available = false;
	this->Has_Player_BJ = false;
	this->bust = false;
	this->free_bet = 0;
	this->exit_do_while = false;
	this->hand = new Card[11];
}


//------------------------------------------------------------- S E T T E R S  ----------------------------------------------------------------------//


void Player::Set_Bet(int bet)
{
	this->bet = bet;
}

void Player::Set_Insurance_Bet(int insurance_bet)
{
	this->insurance_bet = insurance_bet;
}

void Player::Set_Funds(int funds)
{
	this->funds = funds;
}

void Player::Set_Card_No(int card_no)
{
	this->card_no = card_no;
}

void Player::Set_Sum(int sum)
{
	this->sum = sum;
}

void Player::Set_Finished_Hand(bool finished_hand)
{
	this->finished_hand = finished_hand;
}

void Player::Set_Surrender_Available(bool surrender_available)
{
	this->surrender_available = surrender_available;
}

void Player::Set_Double_Down_Available(bool double_d_available)
{
	this->double_d_available = double_d_available;
}

void Player::Set_Insurance_Available(bool insurance_available)
{
	this->insurance_available = insurance_available;
}

void Player::Set_Split_Available()
{
	this->split_available = split_available;
}

void Player::Set_Ace_Available(bool ace_available)
{
	this->ace_available = ace_available;
}

void Player::Set_Has_Player_BJ(bool Has_Player_BJ)
{
	this->Has_Player_BJ = Has_Player_BJ;
}

void Player::Set_Bust(bool bust)
{
	this->bust = bust;
}

void Player::Set_Betting_Mode(bool free_bet)
{
	this->free_bet = free_bet;
}

void Player::Set_Card(int i, Card new_i)
{
	this->hand[i] = new_i;
}


//------------------------------------------------------------- G E T T E R S  -----------------------------------------------------------------------//


int Player::Get_Bet()
{
	return bet;
}

int Player::Get_Insurance_Bet()
{
	return insurance_bet;
}

int Player::Get_Funds()
{
	return funds;
}

int Player::Get_Card_No()
{
	return card_no;
}

int Player::Get_Sum()
{
	return sum;
}

bool Player::Get_Finished_Hand()
{
	return finished_hand;
}

bool Player::Get_Surrender_Available()
{
	return surrender_available;
}

bool Player::Get_Double_Down_Available()
{
	return double_d_available;
}

bool Player::Get_Insurance_Available()
{
	return insurance_available;
}

bool Player::Get_Split_Available()
{
	return split_available;
}

bool Player::Get_Ace_Available()
{
	return ace_available;
}

bool Player::Get_Has_Player_BJ()
{
	return Has_Player_BJ;
}

bool Player::Get_Bust()
{
	return bust;
}

int Player::Get_Betting_Mode()
{
	return free_bet;
}

Card& Player::Get_Card(int i)
{
	return hand[i];
}


//----------------------------------------------------- G E N E R A L   F U N C T I O N S  -----------------------------------------------------------//


int Player::Player_Sum(Player player)										//calculate the sum of card values from player's/dealer's hand function
{
	int sum = 0;

	for (int i = 0; hand[i].Get_Rank() != NULL; i++)
		sum = sum + hand[i].Get_Value();
	return sum;
}

void Player::Modify_Funds(int n)			//1 - BlackJack x1.5 bet
{											//2 - Win x1 bet
	if (n == 1)								//3 - Push x0 bet
	{										//4 - Surrender -0.5x bet
		funds = funds + 2 * bet + 0.5*bet;	//5 - Double Down -1x bet
	}										//6 - Insurance win 2x insurance_bet
	else if (n == 2)						
	{
		funds = funds + 2 * bet;
	}
	else if (n == 3)
	{
		funds = funds + bet;
	}
	else if (n == 4)
	{
		funds = funds + 0.5*bet;
	}
	else if (n == 5)
	{
		funds = funds - bet;
	}
	else if (n == 6)
	{
		funds = funds + 3 * insurance_bet;
	}
}

bool Player::Verify_Ace_Availability(Player player)
{
	bool has_ace = false;

	for (int i = 0; Get_Card(i).Get_Rank()!=NULL; i++)
	{
		if (Get_Card(i).Get_Value() == 1)
		{
			Set_Ace_Available(true);
			has_ace = true;
			break;
		}
	}

	return has_ace;
}


//-------------------------------------------------- T E X T   F I L E   R E A D / W R I T E --------------------------------------------------------//


int Player::Read_Funds(Player player)
{
	std::ifstream file;
	file.open("funds2.txt");
	file >> funds;
	file.close();

	return (int)funds;
}

void Player::Write_Funds(Player player)
{
	std::ofstream file2;
	file2.open("funds2.txt");
	file2 << funds;
	file2.close();
}

void Player::Reset_Funds(Player &player)
{
	std::ofstream file2;
	file2.open("funds2.txt");
	funds = 50;
	file2 << funds;
	file2.close();
}

int Player::Read_Betting_Mode(Player &player)
{
	std::ifstream file3;
	file3.open("betting_mode.txt");
	file3 >> free_bet;
	file3.close();

	return free_bet;
}

void Player::Write_Betting_Mode(Player &player)
{
	std::ofstream file3;
	file3.open("betting_mode.txt");
	file3 << free_bet;
	file3.close();
}


//------------------------------------------------------- P L A Y E R    A C T I O N S -------------------------------------------------------------//


int Player::Place_Bet(Player &player)																//place bet function
{
	int bet;
	int aux_funds;
	bool ok;

	if (funds % 2 == 1)
	{
		aux_funds = funds - 1;
	}
	else if (funds % 2 == 0)
	{
		aux_funds = funds;
	}

	if (funds < 2)
	{
		std::cout << "\n\nYou have insufficient funds. You will be given 10 points in order to play. \nPress any key to continue.";
		ok = false;
		funds = 10;
		Write_Funds(player);
		getch();
		system("CLS");
	}
	else
	{
		std::cout << "\n   Your funds are: " << funds << std::endl;
		std::cout << "\n   Place an even bet (2 - " << aux_funds << "): ";
		std::cin >> bet;

		ok = true;
	}

	if (bet < 2 || bet > aux_funds || bet % 2 == 1)
	{
		std::cout << "   Bet is irrelevant! Please place another bet." << std::endl;
		ok = false;
		fflush(stdin);
		Sleep(800);
		system("CLS");
	}
	
	if (ok == true)
	{
		funds = funds - bet;
		system("CLS");

		return bet;
	}
	else if (ok == false)
	{
		Place_Bet(player);
	}
}

int Player::Place_Chips_Bet(Player &player)
{
	static int pos = 1;
	char key = NULL;
	static int chips_memory[100];
	static int memory_index = 0;

	fflush(stdin);

	if (funds < 2)
	{
		std::cout << "\n\nYou have insufficient funds. You will be given 10 points in order to play. \nPress any key to continue.";
		funds = 10;
		Write_Funds(player);
		getch();
		system("CLS");
	}

	do
	{
		if (pos >= 1 && pos <= 5)
		{
			switch (key)
			{
			case 119: pos--;
				if (pos == 0)
					pos = 1;
				break;

			case 115: pos++;
				if (pos == 6)
					pos = 5;
				break;
			}
		}

		std::cout << "\n  Place your bet: \n\n\n";

		if (pos == 1)
		{
			std::cout << " " << char(16) << "  2  ";

			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 2)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		else
		{
			std::cout << "    2  ";
			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 2)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}

		if (pos == 2)
		{
			std::cout << " " << char(16) << "  5  ";

			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 5)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}

		else
		{
			std::cout << "    5  ";
			
			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 5)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		if (pos == 3)
		{
			std::cout << " " << char(16) << "  25  ";

			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 25)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		else
		{
			std::cout << "    25  ";
			
			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 25)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";

		}
		if (pos == 4)
		{
			std::cout << " " << char(16) << "  100  ";

			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 100)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		else
		{
			std::cout << "    100  ";

			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 100)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		if (pos == 5)
		{
			std::cout << " " << char(16) << "  500  ";
			
			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 500)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}

		else
		{
			std::cout << "    500  ";
			
			for (int i = 0; i < 100; i++)
			{
				if (chips_memory[i] == 500)
				{
					std::cout << char(15) << " ";
				}
			}
			std::cout << "\n\n";
		}
		std::cout << "  Bet: " << bet;

		std::cout << "\n\n  Funds: " << funds;

		std::cout << "\n\n\n\n  (W) - Up";

		std::cout << "\n\n  (S) - Down";

		std::cout << "\n\n  (+) - Add to bet";

		std::cout << "\n\n  (-) - Subtract from bet";

		std::cout << "\n\n  (Enter) - Confirm bet";

		std::cout << "\n\n  (Esc) - Exit to Main Menu";


		if (key == 43)					//add to bet with '+' sign
		{
			if (pos == 1)
			{
				if ((funds - bet) >= 2)
				{
					bet = bet + 2;
					system("CLS");

					memory_index++;
					chips_memory[memory_index] = 2;

					Place_Chips_Bet(player);
				}
			}
			else if (pos == 2)
			{
				if ((funds - bet) >= 5)
				{
					bet = bet + 5;
					system("CLS");

					memory_index++;
					chips_memory[memory_index] = 5;

					Place_Chips_Bet(player);
				}
			}
			else if (pos == 3)
			{
				if ((funds - bet) >= 25)
				{
					bet = bet + 25;
					system("CLS");

					memory_index++;
					chips_memory[memory_index] = 25;

					Place_Chips_Bet(player);
				}
			}
			else if (pos == 4)
			{
				if ((funds - bet) >= 100)
				{
					bet = bet + 100;
					system("CLS");

					memory_index++;
					chips_memory[memory_index] = 100;

					Place_Chips_Bet(player);
				}
			}
			else if (pos == 5)
			{
				if ((funds - bet) >= 500)
				{
					bet = bet + 500;
					system("CLS");

					memory_index++;
					chips_memory[memory_index] = 500;

					Place_Chips_Bet(player);
				}
			}
		}

		else if (key == 45)				//subtract from bet with '-' sign	
		{
			if (pos == 1)
			{
				for (int i = 0; i < 100; i++)
				{
					if (chips_memory[i] == 2)
					{
						if (bet - 2 >= 0)
						{
							bet = bet - 2;
							chips_memory[i] = NULL;
							system("CLS");
							Place_Chips_Bet(player);
							break;
						}
					}
				}
			}
			else if (pos == 2)
			{
				for (int i = 0; i < 100; i++)
				{
					if (chips_memory[i] == 5)
					{
						if (bet - 5 >= 0)
						{
							bet = bet - 5;
							chips_memory[i] = NULL;
							system("CLS");
							Place_Chips_Bet(player);
							break;
						}
					}
				}
			}
			else if (pos == 3)
			{
				for (int i = 0; i < 100; i++)
				{
					if (chips_memory[i] == 25)
					{
						if (bet - 25 >= 0)
						{
							bet = bet - 25;
							chips_memory[i] = NULL;
							system("CLS");
							Place_Chips_Bet(player);
							break;
						}
					}
				}
			}
			else if (pos == 4)
			{
				for (int i = 0; i < 100; i++)
				{
					if (chips_memory[i] == 100)
					{
						if (bet - 100 >= 0)
						{
							bet = bet - 100;
							chips_memory[i] = NULL;
							system("CLS");
							Place_Chips_Bet(player);
							break;
						}
					}
				}
			}
			else if (pos == 5)
			{
				for (int i = 0; i < 100; i++)
				{
					if (chips_memory[i] == 500)
					{
						if (bet - 500 >= 0)
						{
							bet = bet - 500;
							chips_memory[i] = NULL;
							system("CLS");
							Place_Chips_Bet(player);
							break;
						}
					}
				}
			}
		}
		else if (key == 13)						//confirm bet
		{
			if (bet != 0)
			{
				funds = funds - bet;
				exit_do_while = true;
			}
		}
		else if (key == 27)
		{
			for (int i = 0; i < 100; i++)
			{
				chips_memory[i] = NULL;
			}
			return 0;
		}

		if (exit_do_while == false)
			key = getch();

		system("CLS");

	} while (exit_do_while == false);

	for (int i = 0; i < 100; i++)
	{
		chips_memory[i] = NULL;
	}

	return bet;
}

int Player::Place_Insurance_Bet(Deck &deck, Player &player, Dealer &dealer, Stats &stats, Game &game)			//place bet function
{
	fflush(stdin);
	int insurance_bet = 0;

	std::cout << "\n\n   Place your insurance bet (1 - " << bet/2 << "): ";
	std::cin >> insurance_bet;

	if (insurance_bet<1 || insurance_bet > bet/2)
	{
		std::cout << "\n\n   Insurance bet is irrelevant! Please place another bet." << std::endl;
		fflush(stdin);
		Sleep(800);
		system("CLS");
		insurance_bet = 0;
		game.HUD(deck, player, dealer);
		Place_Insurance_Bet(deck, player, dealer, stats, game);														//if bet is less then 1 and larger than funds, place bet again
	}
	else
	{
		funds = funds - insurance_bet;
		system("CLS");

		return insurance_bet;
	}
}

void Player::Player_Action(Deck deck, Player &player, Dealer dealer, Stats &stats, Game &game)		//choose whether to Hit or Stand
{
	char key;

	sum = Player_Sum(player);
	if (funds < bet)
		double_d_available = false;

	if (dealer.Get_Hand(0).Get_Rank() == 1 && insurance_bet == 0 && deck.Get_Card_No() < 5)
		insurance_available = true;
	else insurance_available = false;

	if (hand[0].Get_Value() == hand[1].Get_Value())
		split_available = true;

	if (sum < 21)														//if current sum (of cards) does not exceed 21, choose whether to Hit or Stand
	{
		do
		{
			std::cout << "\n\n   (H) - Hit";
			std::cout << "\n\n   (S) - Stand";
			if (surrender_available == true)
				std::cout << "\n\n   (X) - Surrender";
			if (double_d_available == true)
				std::cout << "\n\n   (D) - Double Down";
			if (insurance_available == true)
				std::cout << "\n\n   (I) - Insurance";
			//if (split_available == true)
				//std::cout << "\n   (2) - Split Cards";

			key = getch();

			switch (key)
			{
			case 72:
			case 104: Player_Hit(deck, player, dealer, stats, game);
					  break;
			case 83:
			case 115: Player_Stand(deck, player, dealer, stats, game);
					  break;
			case 88:
			case 120: if (surrender_available == true)
						  Player_Surrender(deck, player, dealer, stats, game);
					  break;
			case 68:
			case 100: if (double_d_available == true)
						  Player_Double_Down(deck, player, dealer, stats, game);
					  break;

			case 73:
			case 105: if (insurance_available == true)
						  Player_Insurance(deck, player, dealer, stats, game);
					  break;

			//case 50: Player_Split(deck, player, dealer, stats, game);
				//	 break;

			default: std::cout << "\n   Invalid input."; break;
			}
		} while (key != 72 && key != 104 && key != 83 && key != 115);
	}
	else if (sum > 21)												//if current sum (of cards) exceeds 21, the player goes bust
	{
		finished_hand = true;
		bust = true;
		system("CLS");
		
		game.HUD(deck, player, dealer);
		game.Evaluate(deck, player, dealer, stats, game);
	}
	else if (sum == 21)
	{
		finished_hand = true;
		system("CLS");
		
		game.HUD(deck, player, dealer);
		game.Evaluate(deck, player, dealer, stats, game);
	}
}

void Player::Player_Hit(Deck deck, Player &player, Dealer dealer, Stats stats, Game game)			//if player chose hit function
{
	surrender_available = false;
	double_d_available = false;

	std::cout << "\n\n   You chose to Hit.";
	Sleep(800);
	system("CLS");

	hand[card_no] = deck.Get_Deck(deck.Get_Card_No());						//draw another card from the deck
	card_no++;
	deck.Set_Card_No(deck.Get_Card_No() + 1);

	sum = Player_Sum(player);

	if (sum<21)
		game.HUD(deck, player, dealer);

	Player_Action(deck, player, dealer, stats, game);
}

void Player::Player_Stand(Deck deck, Player &player, Dealer dealer, Stats &stats, Game &game)
{
	std::cout << "\n\n   You chose to Stand.";
	Sleep(800);
	finished_hand = true;
	system("CLS");

	game.HUD(deck, player, dealer);
	game.Evaluate(deck, player, dealer, stats, game);
}

void Player::Player_Surrender(Deck deck, Player &player, Dealer dealer, Stats &stats, Game &game)
{
	std::cout << "\n\n   You chose to Surrender.";
	Sleep(1000);
	finished_hand = true;
	system("CLS");

	Modify_Funds(4);
	stats.Modify_Stats(3);

	game.HUD(deck, player, dealer);
	game.Replay(deck, player, dealer, stats, game);
}

void Player::Player_Double_Down(Deck deck, Player &player, Dealer dealer, Stats &stats, Game &game)
{
	if (funds < bet)
	{
		std::cout << "\n\n   You cannot Double Down. Not enough funds.";
		Sleep(1000);
		system("CLS");
		game.HUD(deck, player, dealer);
		Player_Action(deck, player, dealer, stats, game);
	}
	else
	{
		std::cout << "\n\n   You chose to Double Down.";
		Sleep(1000);
		
		hand[card_no] = deck.Get_Deck(deck.Get_Card_No());						//draw another card from the deck
		card_no++;
		deck.Set_Card_No(deck.Get_Card_No() + 1);

		Modify_Funds(5);
		bet = bet * 2;

		sum = Player_Sum(player);
		if (sum > 21)												//if current sum (of cards) exceeds 21, the player goes bust
		{
			finished_hand = true;
			bust = true;
			system("CLS");

			game.HUD(deck, player, dealer);
			game.Evaluate(deck, player, dealer, stats, game);
		}
		else
		{
			finished_hand = true;
			system("CLS");

			game.HUD(deck, player, dealer);
			game.Evaluate(deck, player, dealer, stats, game);
		}
	}
}

void Player::Player_Insurance(Deck deck, Player &player, Dealer dealer, Stats &stats, Game &game)
{
	std::cout << "\n\n   You chose Insurance.";
	Sleep(800);
	system("CLS");
	game.HUD(deck, player, dealer);

	insurance_bet = Place_Insurance_Bet(deck,player,dealer,stats,game);
	insurance_available = false;
	system("CLS");
	game.HUD(deck, player, dealer);
	Player_Action(deck, player, dealer, stats, game);
}

/*
void Player::Player_Split(Deck &deck, Player &player, Dealer &dealer, Stats &stats, Game &game)
{
	hand2[0] = hand[1];
	hand[1].color = true;
	hand[1].rank = 0;
	hand[1].suit = 0;
	hand[1].Get_Value() = 0;
	
	std::cout << "\n\n   You chose to Split.";
	Sleep(800);
	system("CLS");
	
	game.HUD(deck, player, dealer);
	hand[card_no] = deck.deck[deck.card_no];						//draw another card from the deck
	card_no++;
	deck.card_no++;
	
	Sleep(800);
	system("CLS");
	game.HUD(deck, player, dealer);
	hand2[card_no] = deck.deck[deck.card_no];						//draw another card from the deck
	card_no++;
	deck.card_no++;
	
	Sleep(800);
	system("CLS");
	game.HUD(deck, player, dealer);

}
*/
//--------------------------------------------------------- D E S T R U C T O R -------------------------------------------------------------------//

Player::~Player(void)
{
}
