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


    
	//�̹� ������Ʈ���� ���Ӱ� ������ ����
	ofSoundPlayer   mySound; //������ load�ϰ� ����ϱ� ���� class ����
	int location; //�Է¿� ���� ��ġ�� ǥ���ϱ� ���� ����
	int line; //�����̴� line�� ���� ����
	int turnflag; //updateFall()���� updateFall2�� ��ȯ���ִ� ����
	int color_flag; //line����� ���� ����
	int draw_flag; //�Է¿� ���� �ǾƳ��� ��ȭ�� ����ϱ� ���� ����
	vector<char> sheet;  //�Է��� ������ ����� ���� ���� ����
	int print_flag; //��� ������ �����ϴ� flag



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
