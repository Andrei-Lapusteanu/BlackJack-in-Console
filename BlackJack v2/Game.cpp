#include "stdafx.h"
#include "Game.h"

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Game::Game()
{
	this->replay = false;
}


//------------------------------------------------------- M A I N   F U N C T I O N ----------------------------------------------------------------//

void Game::Start(Game game)
{
	Player player;
	Dealer dealer;
	Deck deck;
	Stats stats;

	if (replay == false)
		Main_Menu(player, stats);

	Generate_and_Assign_Cards(deck, player, dealer);

	player.Read_Funds(player);

	player.Read_Betting_Mode(player);

	stats.Read_From_Stats();

	if (player.Read_Betting_Mode(player) == 0)
	{
		player.Set_Bet(player.Place_Chips_Bet(player));

		if (player.Get_Bet() == 0)
		{
			system("CLS");
			Start(game);
		}
	}
	else if (player.Read_Betting_Mode(player) == 1)
	{
		player.Set_Bet(player.Place_Bet(player));

		if (player.Get_Bet() == 0)
		{
			system("CLS");
			Start(game);
		}

	}

	//Show_All_Cards(deck);

	HUD(deck, player, dealer);

	player.Set_Has_Player_BJ(Is_BlackJack(player,player.Get_Sum()));		//verify if player has BlackJack
	//dealer.Set_Has_Dealer_BJ(Is_BlackJack(dealer.Get_Sum()));		//verify if dealer has BlackJack

	if (player.Get_Has_Player_BJ() == true)
	{
		Evaluate_BlackJack(deck, player, dealer, stats, game);
	}
	else if (player.Get_Has_Player_BJ() == false)
	{
		player.Player_Action(deck, player, dealer, stats, game);
	}
}


//---------------------------------------------------- G E N E R A L   F U N C T I O N S --------------------------------------------------//


void Game::Generate_and_Assign_Cards(Deck &deck, Player &player, Dealer &dealer)
{
	deck.Generate_Deck();				//generate deck only once

	deck.Generate_Cards();					//generate cards

	deck.Shuffle_Cards();				//shuffle cards

	for (int i = 0; i<52; i++)
		deck.Get_Deck(i).Card_Value();		//assign a value to each card

	deck.Initial_Deal(deck, player, dealer);		//deal initial cards
}


void Game::Reinitialize(Deck deck, Player player, Dealer dealer)		//after game finished, delete/reinitialize all used variables
{
	/*
	delete[] deck;
	delete[] player.hand;
	delete[] dealer.hand;
	*/

	deck.Set_Card_No(0);
	
	player.Set_Bet(0);
	player.Set_Card_No(0);
	player.Set_Sum(0);
	player.Set_Finished_Hand(false);
	player.Set_Double_Down_Available(true);
	player.Set_Insurance_Available(false);
	player.Set_Surrender_Available(true);
	player.Set_Ace_Available(false);
	player.Set_Betting_Mode(false);
	
	dealer.Set_Card_No(0);
	dealer.Set_Sum(0);
	dealer.Set_Bust(false);
	dealer.Set_Has_Dealer_BJ(false);
	dealer.Set_Ace_Available(false);
}

void Game::Replay(Deck &deck, Player &player, Dealer &dealer, Stats &stats, Game &game)		//function that asks the player which action he wants after the game has finished
{
	char key;

	if (player.Get_Insurance_Bet() > 0)
		Evaluate_Insurance(deck, player, dealer, stats, game);
	
	player.Write_Funds(player);

	do
	{
		std::cout << "\n\n   Do you want to (P)lay again or (E)xit ?";
		key = getch();

		switch (key)
		{
		case 80:
		case 112: Reinitialize(deck, player, dealer);
				  replay = true;
				  system("CLS");
				  Start(game);
				  break;

		case 96:
		case 101: Reinitialize(deck, player, dealer);
				  replay = false;
				  system("CLS");
				  Start(game);
				  break;

		default: std::cout << "\nInvalid input."; break;
		}
	} while (key != 80 && key != 112 && key != 96 && key != 101);
}


//-------------------------------------------------- E V A L U A T O R S --------------------------------------------------------//


bool Game::Is_BlackJack(Player player, int sum)														//verify if player/dealer have BlackJack function
{	
	bool has_player_ace;

	has_player_ace = player.Verify_Ace_Availability(player);

	if (has_player_ace == true)
	{
		if (sum == 11)
		{
			return true;
		}
		else return false;
	}
	else if (has_player_ace == false)
	{
		return false;
	}
}

void Game::Evaluate_BlackJack(Deck deck, Player player, Dealer dealer, Stats stats, Game game)		//if player has BlackJack, compare his sum to the dealer's sum
{
	player.Set_Sum(player.Get_Sum() + 10);

	if (dealer.Get_Ace_Available() == true)
		{
			dealer.Set_Sum(dealer.Get_Sum() + 10);
		}

	if (player.Get_Sum() == dealer.Get_Sum())
	{
		std::cout << "\n\n   PUSH. You and the dealer have same value cards.";
		player.Set_Finished_Hand(true);
		player.Modify_Funds(3);
		stats.Modify_Stats(3);
		Replay(deck, player, dealer, stats, game);
	}
	else if (player.Get_Sum() > dealer.Get_Sum())
	{
		std::cout << "\n\n   YOU WIN with a BlackJack!";
		player.Set_Finished_Hand(true);
		player.Modify_Funds(1);
		stats.Modify_Stats(1);
		Replay(deck, player, dealer, stats, game);
	}

}

void Game::Evaluate(Deck deck, Player player, Dealer dealer, Stats stats, Game game)		//if the player finished his hand and did not go bust, compare his hand to the dealer's hand
{
	if (player.Get_Bust() == true)
	{
		std::cout << "\n\n   You went BUST! Dealer wins.";
		stats.Modify_Stats(3);
		Replay(deck, player, dealer, stats, game);
	}
	else
	{
		dealer.Dealer_AI(deck, player, dealer, game);

		player.Set_Sum(player.Player_Sum(player));
		dealer.Set_Sum(dealer.Dealer_Sum(dealer));

		if (player.Get_Ace_Available() == true)
		{
			if ((player.Get_Sum() + 10) <= 21)
			{
				player.Set_Sum(player.Get_Sum() + 10);
			}
		}

		if (dealer.Get_Ace_Available() == true)
		{
			if ((dealer.Get_Sum() + 10) <= 21)
			{
				dealer.Set_Sum(dealer.Get_Sum() + 10);
			}
		}

		if (dealer.Get_Bust() == true)
		{
			std::cout << "\n\n   YOU WIN! The dealer went bust.";
			player.Modify_Funds(2);
			stats.Modify_Stats(1);
			Replay(deck, player, dealer, stats, game);
		}
		else if (dealer.Get_Bust() == false)
		{
			if (player.Get_Sum() > dealer.Get_Sum())
			{
				std::cout << "\n\n   YOU WIN! Dealer loses.";
				player.Modify_Funds(2);
				stats.Modify_Stats(1);
				Replay(deck, player, dealer, stats, game);
			}
			else if (player.Get_Sum() == dealer.Get_Sum())
			{
				std::cout << "\n\n   PUSH. You and the dealer have same value cards.";
				player.Modify_Funds(3);
				stats.Modify_Stats(2);
				Replay(deck, player, dealer, stats, game);
			}
			else if ((player.Get_Sum() < dealer.Get_Sum()) && (dealer.Get_Sum() == 21))
			{
				std::cout << "\n\n   YOU LOSE! Dealer wins with a BlackJack.";
				stats.Modify_Stats(3);
				Replay(deck, player, dealer, stats, game);
			}
			else if (player.Get_Sum() < dealer.Get_Sum())
			{
				std::cout << "\n\n   YOU LOSE! Dealer wins.";
				stats.Modify_Stats(3);
				Replay(deck, player, dealer, stats, game);
			}
		}
	}

}

void Game::Evaluate_Insurance(Deck deck, Player player, Dealer dealer, Stats stats, Game game)
{
	dealer.Set_Sum(dealer.Dealer_Sum(dealer));

	if (dealer.Get_Sum() == 21)
	{
		player.Modify_Funds(6);
		std::cout << "\n\n   You won your Insurance!";
	}
	else 
	{
		std::cout << "\n\n   You lost your Insurance.";
	}
}


//------------------------------------------- D I S P L A Y   F U N C T I O N S -------------------------------------------------//


void Game::HUD(Deck &deck, Player &player, Dealer &dealer)			//function than converts card rank and suit according to ascii characters
{																	//for a more convenient display
	std::string pl_r, pl_s, pl_r_2, pl_s_2, d_r, d_s;
	
	std::cout <<"\n   Dealer:\n\n  ";

	std::cout << char(218);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(194);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(191);
	
	dealer.Set_Sum(dealer.Dealer_Sum(dealer));
	dealer.Set_Ace_Available(dealer.Verify_Ace_Availability(dealer));

	if (player.Get_Finished_Hand() == false)
		std::cout << "\n  " << char(179) << "Sum of cards " << char(179) << "     N/A" << "\t\t  " << char(179) << "\n  ";
	
	else if (player.Get_Finished_Hand() == true && dealer.Get_Ace_Available() == false)
		std::cout << "\n  " << char(179) << "Sum of cards " << char(179) << "    " << dealer.Get_Sum() << "\t\t\t  " << char(179) << "\n  ";
	
	else if (player.Get_Finished_Hand() == true && dealer.Get_Ace_Available() == true)
		std::cout << "\n  " << char(179) << "Sum of cards " << char(179) << "    " << dealer.Get_Sum() << "/" << dealer.Get_Sum() + 10 << "\t\t  " << char(179) << "\n  ";

	std::cout << char(192);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(193);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(217);
	

	std::cout << "\n\n   Dealer's cards:     ";

	for (int i = 0; dealer.Get_Hand(i).Get_Value() != NULL; i++)
	{
		if (dealer.Get_Hand(i).Get_Rank() == 1)
			d_r = "A";
		else if (dealer.Get_Hand(i).Get_Rank() == 2)
			d_r = "2";
		else if (dealer.Get_Hand(i).Get_Rank() == 3)
			d_r = "3";
		else if (dealer.Get_Hand(i).Get_Rank() == 4)
			d_r = "4";
		else if (dealer.Get_Hand(i).Get_Rank() == 5)
			d_r = "5";
		else if (dealer.Get_Hand(i).Get_Rank() == 6)
			d_r = "6";
		else if (dealer.Get_Hand(i).Get_Rank() == 7)
			d_r = "7";
		else if (dealer.Get_Hand(i).Get_Rank() == 8)
			d_r = "8";
		else if (dealer.Get_Hand(i).Get_Rank() == 9)
			d_r = "9";
		else if (dealer.Get_Hand(i).Get_Rank() == 10)
			d_r = "10";
		else if (dealer.Get_Hand(i).Get_Rank() == 11)
			d_r = "J";
		else if (dealer.Get_Hand(i).Get_Rank() == 12)
			d_r = "Q";
		else if (dealer.Get_Hand(i).Get_Rank() == 13)
			d_r = "K";

		if (dealer.Get_Hand(i).Get_Suit() == 0)
			d_s = 3;
		else if (dealer.Get_Hand(i).Get_Suit() == 1)
			d_s = 4;
		else if (dealer.Get_Hand(i).Get_Suit() == 2)
			d_s = 5;
		else if (dealer.Get_Hand(i).Get_Suit() == 3)
			d_s = 6;

		if (i == 0 && player.Get_Finished_Hand() == false)
			std::cout << "" << d_r << "" << d_s << "  " << char(178) << char(178);
		else if (player.Get_Finished_Hand() == true)
			std::cout << "" << d_r << "" << d_s << "  ";
	}
	
	std::cout << "\n\n\n\n";
	std::cout << "\n\n\n\n";

	std::cout << "   Player:\n\n";

		std::cout << "   Your cards:     ";

		for (int i = 0; player.Get_Card(i).Get_Value() != NULL; i++)
		{
			if (player.Get_Card(i).Get_Rank() == 1)
				pl_r = "A";
			else if (player.Get_Card(i).Get_Rank() == 2)
				pl_r = "2";
			else if (player.Get_Card(i).Get_Rank() == 3)
				pl_r = "3";
			else if (player.Get_Card(i).Get_Rank() == 4)
				pl_r = "4";
			else if (player.Get_Card(i).Get_Rank() == 5)
				pl_r = "5";
			else if (player.Get_Card(i).Get_Rank() == 6)
				pl_r = "6";
			else if (player.Get_Card(i).Get_Rank() == 7)
				pl_r = "7";
			else if (player.Get_Card(i).Get_Rank() == 8)
				pl_r = "8";
			else if (player.Get_Card(i).Get_Rank() == 9)
				pl_r = "9";
			else if (player.Get_Card(i).Get_Rank() == 10)
				pl_r = "10";
			else if (player.Get_Card(i).Get_Rank() == 11)
				pl_r = "J";
			else if (player.Get_Card(i).Get_Rank() == 12)
				pl_r = "Q";
			else if (player.Get_Card(i).Get_Rank() == 13)
				pl_r = "K";

			if (player.Get_Card(i).Get_Suit() == 0)
				pl_s = 3;
			else if (player.Get_Card(i).Get_Suit() == 1)
				pl_s = 4;
			else if (player.Get_Card(i).Get_Suit() == 2)
				pl_s = 5;
			else if (player.Get_Card(i).Get_Suit() == 3)
				pl_s = 6;

			std::cout << "" << pl_r << "" << pl_s << "  ";
		}

	std::cout << "\n\n  ";

	std::cout << char(218);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(194);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(191) << std::endl;

	player.Set_Sum(player.Player_Sum(player));
	player.Set_Ace_Available(player.Verify_Ace_Availability(player));
	/*for (int i = 0; player.Get_Card(i).Get_Rank() != NULL; i++)
	{
		if (player.Get_Card(i).Get_Value() == 1)
			player.Set_Ace_Available(true);
	}
	*/
	if (player.Get_Ace_Available() == false)
		std::cout << "  " << char(179) << "Sum of cards " << char(179) << "\t" << player.Get_Sum() << "\t\t  " << char(179) <<"\n  ";
	else if (player.Get_Ace_Available() == true)
		std::cout << "  " << char(179) << "Sum of cards " << char(179) << "\t" << player.Get_Sum() << "/" << player.Get_Sum() + 10 << "\t\t  " << char(179) << "\n  ";

	std::cout << char(195);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(197);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(180) << std::endl;

	std::cout << "  " << char(179) << "Your bet     " << char(179) << "\t" << player.Get_Bet() << "\t\t  " << char(179) << std::endl<< "  ";

	std::cout << char(195);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(197);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(180) << std::endl;
	
	if (player.Get_Insurance_Bet() >0)
	{
		std::cout << "  " << char(179) << "Insurance    " << char(179) << "\t" << player.Get_Insurance_Bet() << "\t\t  " << char(179) << std::endl << "  ";

		std::cout << char(195);
		for (int k = 0; k < 13; k++)
			std::cout << char(196);
		std::cout << char(197);
		for (int k = 0; k < 25; k++)
			std::cout << char(196);
		std::cout << char(180) << std::endl;
	}
	
	std::cout << "  " << char(179) << "Your funds   " << char(179) << "\t" << player.Get_Funds()<< "\t\t  " << char(179) << std::endl <<"  ";
	

	std::cout << char(192);
	for (int k = 0; k < 13; k++)
		std::cout << char(196);
	std::cout << char(193);
	for (int k = 0; k < 25; k++)
		std::cout << char(196);
	std::cout << char(217) << std::endl;

	//std::cout << "   Cards remaining in deck: " << 52 - deck.Get_Card_No() << "\n";

}

void Game::Main_Menu(Player &player, Stats &stats)
{
	fflush(stdin);
	char key = NULL;

	std::string betting_mode;

	player.Read_Betting_Mode(player);

	if (player.Get_Betting_Mode() == 1)
	{
		betting_mode = "Free bet";
	}

	else if (player.Get_Betting_Mode() == 0)
	{
		betting_mode = "Casino bet";
	}

	player.Set_Funds(player.Read_Funds(player));
	stats.Read_From_Stats();

	std::cout << (char)201;
	for (int i = 1; i<21; i++)
		std::cout << (char)205;

	printf("BlackJack");
	for (int i = 1; i<21; i++)
		std::cout << (char)205;

	std::cout << (char)187 << std::endl;

	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t   Select an option from below:\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        1. Play BlackJack\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        2. Options\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        3. Exit Game\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;

	if (player.Get_Funds() < 10)
		std::cout << (char)186 << " Your funds: " << player.Get_Funds() << "\t\t\t\t\t  " << char(186) << std::endl;
	else std::cout << (char)186 << " Your funds: " << player.Get_Funds() << "\t\t\t\t  " << char(186) << std::endl;
	
	std::cout << (char)186 << " Betting mode: " << betting_mode << "\t\t\t  " << char(186) << std::endl;

	std::cout << (char)200;
	for (int i = 1; i<50; i++)
		std::cout << (char)205;
	std::cout << (char)188;


	std::cout << "\n\nChoise: ";
	key = std::cin.get();
	fflush(stdin);

	switch (key)
	{
	case 49: system("CLS"); 
			 break;

	case 50: system("CLS");
			 Options_Menu(player, stats);
			 break;

	case 51: exit(0);

	default: std::cout << "Invalid input";
			 Sleep(500);
			 fflush(stdin);
			 char key = NULL;
			 system("CLS");
			 Main_Menu(player, stats);
			 break;
	}
}

void Game::Options_Menu(Player &player, Stats &stats)
{
	char key = NULL;

	std::cout << (char)201;
	for (int i = 1; i<22; i++)
		std::cout << (char)205;

	printf("Options");
	for (int i = 1; i<22; i++)
		std::cout << (char)205;

	std::cout << (char)187 << std::endl;

	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t   Select an option from below:\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        1. View statistics\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        2. Reset statistics\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        3. Reset funds to 50\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t      (only if funds exceed 50)\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        4. View game documentation\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        5. Betting options\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        6. Return\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	/*
	if (player.Get_Funds() < 10)
		std::cout << (char)186 << " your funds: " << player.Get_Funds() << "\t\t\t\t\t  " << char(186) << std::endl;
	else std::cout << (char)186 << " your funds: " << player.Get_Funds() << "\t\t\t\t  " << char(186) << std::endl;
	*/
	std::cout << (char)200;
	for (int i = 1; i<50; i++)
		std::cout << (char)205;
	std::cout << (char)188;

	std::cout << "\n\nChoise: ";
	key = std::cin.get();
	fflush(stdin);

	switch (key)
	{
	case 49: system("CLS");
		     Stats_Menu(stats, player);
		     break;

	case 50: stats.Reset_Stats();
			 stats.Read_From_Stats();
			 stats.Percentage();
			 system("CLS");
			 Options_Menu(player, stats);
			 break;

	case 51: if (player.Get_Funds() >= 50)
			{
				 player.Reset_Funds(player);
				 std::cout << "\n\nFunds have been set to 50";
				 Sleep(500);
				 system("CLS");
				 Options_Menu(player, stats);
				 break;
			}
			 else std::cout << "Your funds do not exceed 50";
			 
			 Sleep(700);
			 system("CLS");
			 fflush(stdin);
			 Options_Menu(player, stats);
			 break;

	case 52: system("CLS");
			 Game_Documentation();
			 system("CLS");
			 Options_Menu(player, stats);
			 break;
	
	case 53: system("CLS");
			 Betting_Menu(player, stats);
		 	break;

	case 54: system("CLS");
		     Main_Menu(player, stats);
			 break;

	default: std::cout << "\nInvalid input";
			 Sleep(500);
			 system("CLS");
			 fflush(stdin);
			 Options_Menu(player, stats);
			 break;
	}
}

void Game::Stats_Menu(Stats &stats, Player player)
{
	stats.Read_From_Stats();
	stats.Percentage();

	std::cout << (char)201;
	for (int i = 1; i<21; i++)
		std::cout << (char)205;

	printf("Statistics");
	for (int i = 1; i<20; i++)
		std::cout << (char)205;

	std::cout << (char)187 << std::endl;

	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;

	if (stats.Get_Played() < 10)
		std::cout << (char)186 << "\t        Games played: " << stats.Get_Played() << "\t\t\t  " << char(186) << std::endl;
	else 	std::cout << (char)186 << "\t        Games played: " << stats.Get_Played() << "\t\t  " << char(186) << std::endl;

	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Games won: " << stats.Get_Won() << "\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Games lost: " << stats.Get_Lost() << "\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Games drawn: " << stats.Get_Draw() << "\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Win percentage: " << std::setprecision(3) << stats.Get_Won_Percentage() << "%\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Lose percentage: " << std::setprecision(3) << stats.Get_Lost_Percentage() << "%\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t        Draw percentage: " << std::setprecision(3) << stats.Get_Draw_Percentage() << "%\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;

	if (player.Get_Funds() < 999)
		std::cout << (char)186 << "\t\tYour funds: " << player.Get_Funds() << "\t\t\t  " << char(186) << std::endl;
	else std::cout << (char)186 << "\t\tYour funds: " << player.Get_Funds() << "\t\t  " << char(186) << std::endl;
	
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;

	std::cout << (char)200;
	for (int i = 1; i<50; i++)
		std::cout << (char)205;
	std::cout << (char)188;

	std::cout << "\n\nPress any key to return";

	getch();
	system("CLS");
	Options_Menu(player, stats);
}

void Game::Betting_Menu(Player &player, Stats &stats)
{
	char key = NULL;
	std::string current_mode;

	player.Read_Betting_Mode(player);

	if (player.Get_Betting_Mode() == 0)
	{
		current_mode = "Casino bet";
	}
	else current_mode = "Free bet   ";

	std::cout << (char)201;
	for (int i = 1; i<20; i++)
		std::cout << (char)205;

	printf("Betting Menu");
	for (int i = 1; i<19; i++)
		std::cout << (char)205;

	std::cout << (char)187 << std::endl;

	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t     1. Free bet " << "\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t     2. Casino bet " << "\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t     3. Return " << "\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t         Current mode: " << current_mode << "\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;
	std::cout << (char)186 << "\t\t\t\t\t\t  " << char(186) << std::endl;

	std::cout << (char)200;
	for (int i = 1; i<50; i++)
		std::cout << (char)205;
	std::cout << (char)188;

	std::cout << "\n\nChoise: ";

	do
	{
		key = std::cin.get();
		switch (key)
		{
		case 49: player.Set_Betting_Mode(1);
				 player.Write_Betting_Mode(player);
				 system("CLS");
				 Betting_Menu(player, stats);
				 break;

		case 50: player.Set_Betting_Mode(0);
				 player.Write_Betting_Mode(player);
				 system("CLS");
				 Betting_Menu(player, stats);
				 break;

		case 51: system("CLS");
				 Options_Menu(player, stats);
				 break;
		}
	} while (key != 49 && key != 50 && key != 51);
}

void Game::Game_Documentation()
{
	std::cout << 
		
		"\n BlackJack Documentation"

		
		"\n\n\n All variables and functions are briefly described below. For further documentation, study \n the Game Rules page."
		"\n\n Contructor, destructor, setter and getter functions are not listed."
	
		
		"\n\n\n BlackJack.cpp"

		
		"\n\n\n Contains the Main function of the program. From here, the Start() method is called \n from the Game class."

		
		"\n\n\n Class GAME :"

		"\n\n Variables :"


		"\n\n\n bool replay - true or false depending on game status. If the game has finished the variable \n becomes true."


		"\n\n\n Methods:"

		
		"\n\n void Start() - The second most important function after Main(). Calls most methods needed to \n start a game of BlackJack."

		"\n\n void Generate_and_Assign Card(Deck&, Player&, Dealer&) - Contains calls to several functions \n which generate cards, a deck, shuffles it and assingns 2 cards to each player."

		"\n\n void Reinitialize(Deck, Player, Dealer) - After the game has finished and the player has chosen \n whether to replay or to exit, all variables are reset to their original state and decks are deleted."

		"\n\n void Replay(Deck&, Player&, Dealer&, Stats&, Game&) - After the games has finished, the Player \n has the option to choose whether to play another game or to exit to the Main Menu."

		"\n\n bool Is_BlackJack(int) - Function which based on the passed amount, return true is sum of cards \n is 21 or false if it differs."

		"\n\n void Evaluate_BlackJack(Deck, Player, Dealer, Stats, Game) - If the Player has a BlackJack, the \n Dealer's hand is analyzed in order to generate an outcome (Win or Push)."

		"\n\n void Evaluate(Deck, Player, Dealer, Stats, Game) - After both the Player and the Dealer finished \n their hand(even if the Player went bust), their hands are analyzed and an outcome is \n generated(Win, Push, Lost)."

		"\n\n void Evaluate_Insurance(Deck, Player, Dealer, Stats, Game) - If the player chose to Insure, based \n on the outcome of the Dealer's hand, the Player either Wins the insurance bet or Loses it."

		"\n\n void HUD(Deck&, Player&, Dealer&) - The main board of the game. The function displays cards, card \n sums, bet, funds and tables."

		"\n\n void Main_Menu(Player&, Stats&) - The Main Menu of the game (mostly display).The Player gets to \n choose an option."

		"\n\n void Options_Menu(Player&, Stats&) - The Options Menu of the game (mostly display).The Player \n gets to choose an option."

		"\n\n void Stats_Menu(Stats&, Player) - The Statistics Menu of the game (display only)."

		"\n\n void Show_All_Cards(Deck deck) - Funtion which isn't used in the game by default. It displays the\n  entire shuffled deck. For testing purposes only."


		"\n\n\n\n CLASS PLAYER :"

		"\n\n Variables :"

		
		"\n\n\n int bet - Variable which stores the wager typed by the Player at the beggining of a game."

		"\n\n int insurance_bet - Variable which stores the wager typed by the Player when prompted."

		"\n\n int funds - Variable which stores the Player's funds."

		"\n\n int card_no - Variable which stores the amount of cards the Player has (Player's hand is made up of \n 3 cards = > card_no = 3)."

		"\n\n int sum - Variable which stores the sum of cards from the Player's hand."

		"\n\n bool finished_hand - If the Player chose to Stand or if he went Bust, variable is true. False \n otherwise."

		"\n\n bool surrender_available - True after the initial deal, false otherwise. Gives the Player a chance \n to surrener if he considers his hand is weak."

		"\n\n bool double_d_available - True after the initial deal, if the amount of bet is at least equal to \n remaining funds and if hand isn't a BlackJack. False otherwise."

		"\n\n bool insurance_available - If the Dealer's face up card is an Ace, it is the initial deal and the \n Player's hand isn't a BlackJack, the variable is true. False otherwise."

		"\n\n bool split_available - Unimplemented."

		"\n\n bool ace_available - If the Player has an Ace, the variable is true. False otherwise. The Ace card will \n value either 1 or 11, depending on the Player's preference."

		"\n\n bool Has_Player_BJ - If the Player has a BlackJack, the variable is true. False otherwise."

		"\n\n bool bust - If the sum of cards from the Player's hand exceeds 21, the variable is true. False otherwise."

		"\n\n Card *hand - Array of elements type 'Card'. It represents the Player's hand."

		
		"\n\n\n Methods:"

		
		"\n\n\n int Player_Sum(Player) - Function which based on the value of each card from the Player's hand, \n determines their sum."

		"\n\n void Modify_Funds(int) - Function which based on the value passed, modifies the Player's funds \n accordingly. Possible values are listed below:"
		
		"\n\n 1 - BlackJack - Player gets x1.5 bet"
		"\n 2 - Win - Player gets x1 bet"
		"\n 3 - Push - Player gets x0 bet"
		"\n 4 - Surrender - Player gets 0.5x bet"
		"\n 5 - Double Down -Player's funds are reduced by 1x bet"
		"\n 6 - Insurance - Player wins 2x insurance_bet"

		"\n\n int Read_Funds(Player) - Player's funds are being read from an external .txt file and saved \n in 'int funds'."

		"\n\n void Write_Funds(Player) - Player's funds are stores in an external.txt, overwriting the old \n value."

		"\n\n void Reset_Funds(Player&) - From the Options Menu, the Player has the choise to reset his \n funds, but only if they exceed 50."

		"\n\n int Place_Bet(Player&) - Function which allows the player to place a wager depending on his \n available funds. If Player has 0 funds, he will be given 10 in order to start playing (and winning)."

		"\n\n int Place_Insurance_Bet(Deck&, Player&, Dealer&, Stats&, Game&) - If 'bool inurance_available = true', \n the Player has the option to place an Insurance Bet."

		"\n\n void Player_Action(Deck, Player&, Dealer, Stats&, Game&) - The Player gets to choose from a \n total of 5 different options, listed below."

		"\n\n void Player_Hit(Deck, Player&, Dealer, Stats, Game) - Function available when Player's sum of \n cards doesn't equal or exceed 21, Player's hand isn't a BlackJack he and did not choose \n to Stand previously. Player is dealt another card."

		"\n\n void Player_Stand(Deck, Player&, Dealer, Stats&, Game&) - Function available if Player's hand \n isn't a BlackJack and if his sum of cards doesn't equal or exceed 21. The Player's hand is finished \n and it's the Dealer's turn to play. No further actions are permitted."

		"\n\n void Player_Surrender(Deck, Player&, Dealer, Stats&, Game&) - After the initial deal, if the \n Player's hand isn't a BlackJack, he is given the option to Surrender. It is basically a Stand but \n half of the Player's bet goes to the house, half is returned. No further actions are permitted."

		"\n\n void Player_Double_Down(Deck, Player&, Dealer, Stats&, Game&) - After the initial deal, if the \n Player's hand isn't a BlackJack, and his funds are at least equal to the current bet, he is \n given the option to Double Down. Player's bet is doubled and he is given one more card. \n No further actions are permitted."

		"\n\n void Player_Insurance(Deck, Player&, Dealer, Stats&, Game&) - If available, and Player chose Insurance,\n  he will be prompted to enter an Insurance wager. Afterwards, the game continues as usual."

		"\n\n void Player_Split(Deck&, Player&, Dealer&, Stats&, Game&) - Unimplemented."

	 	
		"\n\n\n\n CLASS DEALER "

		"\n\n Variables :"

		
		"\n\n\n int card_no - Variable which stores the amount of cards the Dealer has (Dealer's hand is made \n up of 3 cards = > card_no = 3)."

		"\n\n int sum - Variable which stores the sum of cards from the Delaer's hand."

		"\n\n bool bust - If the sum of cards from the Dealer's hand exceed 21, the variable is true. False \n otherwise."

		"\n\n bool Has_Dealer_BJ - If the Dealer has a BlackJack, the variable is true. False otherwise."

		"\n\n bool ace_available - If the Dealer has an Ace, the variable is true. False otherwise. The Ace\n  card will value either 1 or 11, depending on the Dealer's AI."

		"\n\n Card *hand - Array of elements type 'Card'. It represents the Dealer's hand."

		
		"\n\n\n Methods:"

		
		"\n\n\n int Dealer_Sum(Dealer) - Function which based on the value of each card from the Dealer's \n hand, determines their sum."

		"\n\n void Dealer_AI(Deck, Player, Dealer&, Game) - Function which executes after the Player has \n finished his hand. Based on a set of rules, the Dealer chooses whether to Hit or Stand,\n  without any other options. Detailed in the Rules page."

		
		"\n\n\n\n CLASS CARD :"

		"\n\n Variables :"

		
		"\n\n\n int rank - Variable which stores the face of each card. 1 - Ace, 2 - 2, ..., 11 - Jack, \n 12 - King, 13 - Queen."

		"\n\n bool color - Variable which stores each card's colour. 'True = red', 'false = black'."

		"\n\n int suit - Variable which stores each card's suit as follows : 0 - hearts, 1 - diamonds, \n 2 - clubs, 3 - spades."

		"\n\n int value - Variable which stores each card's value in BlackJack, as follows : \n 2 - 9 (intrensic rank), J - K - Q(rank 10), A(1 / 11)."

		
		"\n\n\n Methods :"

		
		"\n\n\n void Card_Value() - Function which assigns a value to each card according to game rules."

		
		"\n\n\n\n CLASS DECK :"

		"\n\n Variables :"

		
		"\n\n\n int card_no - Variable which stores the card number of the deck (if 5 cards have been \n dealt, card_no = 5)."

		"\n\n Card *deck - Array of elements type 'Card'.It represents the deck of cards."

		
		"\n\n\n Methods:"

		
		"\n\n\n void Generate_Deck() - Function which allocates memory for the deck of cards and creates \n an array of 52 elements type 'Card'."
		 
		"\n\n void Generate_Cards() - Function which assingns to each element of the array of cards a \n rank, suit, color and value."

		"\n\n void Shuffle_Cards() - Function which copies the generated array into another one (for safety \n purposes) and shuffles it randomly."

		"\n\n void Initial_Deal(Deck&, Player&, Dealer&) - Function which deals the initial cards."

		
		"\n\n\n CLASS STATS :"

		"\n\n Variables :"

		
		"\n\n\n int played - Variable which stores the number of games played."

		"\n\n int win - Variable which stores the number of games won."

	    "\n\n int lost - Variable which stores the number of games lost."

		"\n\n int draw - Variable which stores the number of games drawn."

		"\n\n float win_p - Variable which stores the percentage of games won."

		"\n\n float lost_p - Variable which stores the percentage of games lost."

		"\n\n float draw_p - Variable which stores the percentage of games drawn."

		
		"\n\n\n Methods:"

		
		"\n\n\n void Read_From_Stats() - Function which reads from an external .txt the statictics, \n and stores them in the variables listed above."

		"\n\n void Write_To_Stats() - Function which writes in an external.txt the statistics, \n overwriting them after the game has finished."

		"\n\n void Percentage() - Function which calculates the win, lose and draw percentage, taking \n into account the number of games played."

		"\n\n void Modify_Stats(int) - Function which based on the value passed, modifies the Player's \n statistics accordingly. Possible values are listed below :"

		"\n\n 1 - Win"
		"\n 2 - Push"
		"\n 3 - Lose"

		"\n\n void Reset_Stats() - Function which resets the statistics to 0."

		"\n\n\n\nPress any key to return";

		getch();
}


//----------------------------------------------------------- A U X I L I A R Y -----------------------------------------------//


void Game::Show_All_Cards(Deck deck)
{
	std::string d_r, d_s;
	for (int i = 0; i < 52; i++)
	{
		if (deck.Get_Deck(i).Get_Rank() == 1)
			d_r = "A";
		else if (deck.Get_Deck(i).Get_Rank() == 2)
			d_r = "2";
		else if (deck.Get_Deck(i).Get_Rank() == 3)
			d_r = "3";
		else if (deck.Get_Deck(i).Get_Rank() == 4)
			d_r = "4";
		else if (deck.Get_Deck(i).Get_Rank() == 5)
			d_r = "5";
		else if (deck.Get_Deck(i).Get_Rank() == 6)
			d_r = "6";
		else if (deck.Get_Deck(i).Get_Rank() == 7)
			d_r = "7";
		else if (deck.Get_Deck(i).Get_Rank() == 8)
			d_r = "8";
		else if (deck.Get_Deck(i).Get_Rank() == 9)
			d_r = "9";
		else if (deck.Get_Deck(i).Get_Rank() == 10)
			d_r = "10";
		else if (deck.Get_Deck(i).Get_Rank() == 11)
			d_r = "J";
		else if (deck.Get_Deck(i).Get_Rank() == 12)
			d_r = "Q";
		else if (deck.Get_Deck(i).Get_Rank() == 13)
			d_r = "K";

		if (deck.Get_Deck(i).Get_Suit() == 0)
			d_s = 3;
		else if (deck.Get_Deck(i).Get_Suit() == 1)
			d_s = 4;
		else if (deck.Get_Deck(i).Get_Suit() == 2)
			d_s = 5;
		else if (deck.Get_Deck(i).Get_Suit() == 3)
			d_s = 6;

		std::cout << "" << d_r << "" << d_s << " - " << deck.Get_Deck(i).Get_Value() << "\n";
	}
}


//--------------------------------------------------------- D E S T R U C T O R -------------------------------------------------------------------//

Game::~Game()
{
}
