#pragma once
using namespace std;
#include "ofMain.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


    
	//이번 프로젝트에서 새롭게 선언한 변수
	ofSoundPlayer   mySound; //음악을 load하고 재생하기 위한 class 정의
	int location; //입력에 따른 위치를 표시하기 위한 변수
	int line; //움직이는 line을 위한 변수
	int turnflag; //updateFall()에서 updateFall2로 변환해주는 변수
	int color_flag; //line출력을 위한 변수
	int draw_flag; //입력에 따른 피아노의 변화를 출력하기 위한 변수
	vector<char> sheet;  //입력할 때마다 기록을 위한 벡터 선언
	int print_flag; //기록 시작을 결정하는 flag



	void updateFall() { 
		if (line >= 150-(location*30)) {
			line-= 20;
		}
		else
		{
			turnflag = 0;

		}
	}
	void updateFall2() { 
		if (line <= 195)
		{
			line += 20;
		}
		else {
			color_flag = 0;
		}
	}
		
};
