// BlackJack v2.cpp : Defines the entry point for the console application.


//#include <opencv2\opencv.hpp>
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
//#include <opencv2/core/core.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>

//using namespace cv;

//const int h = 800;
//const int w= 600;

#include <cstdio>


#include "stdafx.h"
#include "Game.h"

Game game;

int main()
{
	/*
	char text[10] = "testtt"; 

	cv::Mat board(w, h, CV_8UC3, cv::Scalar(50, 150, 50));
	cv::Mat img = imread("D:/Facultate/Anul I/Semestrul II/PCLP II/Visual Studio 2013/BlackJack V8/card1.JPG", CV_LOAD_IMAGE_UNCHANGED);
	cv::Mat img2 = imread("D:/Facultate/Anul I/Semestrul II/PCLP II/Visual Studio 2013/BlackJack V8/KS.PNG", CV_LOAD_IMAGE_UNCHANGED);

	cv::Point center(board.cols / 2, board.rows / 2);

	cv::rectangle(board, cv::Rect(center, cv::Size(50, 50)), cv::Scalar(100, 0, 0), CV_FILLED);
	cv::rectangle(board, cv::Rect(cv::Point(100, 100), cv::Point(150, 200)), cv::Scalar(0, 0, 0), 1);

	cv::line(board, cv::Point(50, 50), cv::Point(25, 25), cv::Scalar(0, 0, 0), 5 );

	//sprintf_s(text, 50, "test");
	cv::putText(board, std::string(text), cv::Point(10, 100), cv::FONT_HERSHEY_COMPLEX, 1, CV_RGB(100, 20 ,200), 1.5);


	namedWindow("poza", CV_WINDOW_AUTOSIZE);
	cv::imshow("board", board);
	
	cv::imshow("poza", img);

	waitKey(0);
	*/

	//cv::Mat M(5, 5, CV_8UC3, Scalar(0, 0, 255));

	//std::cout << M << std::endl;

	game.Start(game);

	return 0;
}


