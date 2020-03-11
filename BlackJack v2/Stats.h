#pragma once

class Stats
{
public:
	
	//Constructor
	Stats(void);
	
	//Setters
	void Set_Played(int);
	void Set_Won(int);
	void Set_Lost(int);
	void Set_Draw(int);
	void Set_Won_Percentage(float);
	void Set_Lost_Percentage(float);
	void Set_Draw_Percentage(float);
	
	//Getters
	int Get_Played();
	int Get_Won();
	int Get_Lost();
	int Get_Draw();
	float Get_Won_Percentage();
	float Get_Lost_Percentage();
	float Get_Draw_Percentage();

	//Text file read/write
	void Read_From_Stats();
	void Write_To_Stats();
	
	//General functions
	void Percentage();
	void Modify_Stats(int);
	void Reset_Stats();

	//Desctructor
	~Stats(void);

private:

	int played;
	int win;
	int lost;
	int draw;

	float win_p;
	float lost_p;
	float draw_p;

};

