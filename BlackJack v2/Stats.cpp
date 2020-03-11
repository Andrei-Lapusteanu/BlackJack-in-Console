#include "StdAfx.h"
#include "Stats.h"

#include <iostream>
#include <fstream>


//--------------------------------------------------------- C O N S T R U C T O R -------------------------------------------------------------------//


Stats::Stats(void)
{
	this->played = played;
	this->win = win;
	this->lost = lost;
	this->draw = draw;

	this->win_p = win_p;
	this->lost_p = lost_p;
	this->draw_p = draw_p;
}


//------------------------------------------------------------- S E T T E R S  ----------------------------------------------------------------------//

void Stats::Set_Played(int played)
{
	this->played = played;
}

void Stats::Set_Won(int win)
{
	this->win = win;
}

void Stats::Set_Lost(int lost)
{
	this->lost = lost;
}

void Stats::Set_Draw(int draw)
{
	this->draw = draw;
}

void Stats::Set_Won_Percentage(float win_p)
{
	this->win_p = win_p;
}

void Stats::Set_Lost_Percentage(float lost_p)
{
	this->lost_p = lost_p;
}

void Stats::Set_Draw_Percentage(float draw_p)
{
	this->draw_p = draw_p;
}

//------------------------------------------------------------- G E T T E R S  ----------------------------------------------------------------------//

int Stats::Get_Played()
{
	return played;
}

int Stats::Get_Won()
{
	return win;
}

int Stats::Get_Lost()
{
	return lost;
}

int Stats::Get_Draw()
{
	return draw;
}

float Stats::Get_Won_Percentage()
{
	return win_p;
}

float Stats::Get_Lost_Percentage()
{
	return lost_p;
}

float Stats::Get_Draw_Percentage()
{
	return draw_p;
}

//------------------------------------------------------ G E N E R A L   F U N C T I O N S -------------------------------------------------------------//


void Stats::Percentage()
{
	if (played == 0)
		win_p = lost_p = draw_p = 0;
	else
	{
		win_p = ((float)win / (float)played) * 100;
		lost_p = ((float)lost / (float)played) * 100;
		draw_p = ((float)draw / (float)played) * 100;
	}
}

void Stats::Modify_Stats(int n)				// 1 - win
{											// 2 - push
	if (n == 1)								// 3 - lose
	{
		played++;
		win++;
	}
	else if (n == 2)
	{
		played++;
		draw++;
	}
	else if (n == 3)
	{
		played++;
		lost++;
	}
	Write_To_Stats();
}

void Stats::Reset_Stats()
{
	played = 0;
	win = 0;
	lost = 0;
	draw = 0;
	Write_To_Stats();
}

//------------------------------------------------------- T E X T   F I L E   R E A D / W R I T E --------------------------------------------------------//


void Stats::Read_From_Stats()
{
	std::ifstream file;
	file.open("Stats.txt");
	file>>played>>win>>lost>>draw;
	file.close();
}

void Stats::Write_To_Stats()
{
	std::ofstream file2;
	file2.open("Stats.txt");
	file2 << played << std::endl << win << std::endl << lost << std::endl << draw;
	file2.close();
}


//----------------------------------------------------------------- D E S T R U C T O R ----------------------------------------------------------------------//


Stats::~Stats(void)
{
}
