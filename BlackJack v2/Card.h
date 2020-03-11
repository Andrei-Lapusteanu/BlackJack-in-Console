#pragma once

class Card
{
public:

	//Constructor
	Card();
	
	//Setters
	void Set_Rank(int);
	void Set_Color(int);
	void Set_Suit(int);
	void Set_Value(int);

	//Getters
	int Get_Rank();
	int Get_Color();
	int Get_Suit();
	int Get_Value();

	//Assign values to cards
	void Card_Value();
	
	//Destrcutor
	~Card();

private:

	int rank;	//1-13
	bool color;	//true = red, false = black;
	int suit;	//0 -hearts, 1 - diamonds, 2 - clubs, 3 - spades
	int value;  //2-9 (intrensic rank), J-K-Q (rank 10), A(1/11)

};

