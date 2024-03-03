#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); 
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0; //draw, color flag �ʱ�ȭ
	color_flag = 0;
    
}

//--------------------------------------------------------------
void ofApp::update() {


}


//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0, 0, 0); 

	//�ǾƳ� �׸���
	ofDrawRectangle(0, 0, 1024, 200); 
	ofSetLineWidth(2);
	for (int k = 0; k < 5; k++) {
		ofDrawLine(204.8*(k + 1), 200, 204.8*(k + 1), ofGetWidth());
		if (k != 2) {
			ofDrawRectangle(204.8*(k + 1) - 50, 200, 100, 300);
		}
	}


	ofSetLineWidth(5);
	if (draw_flag) { //key�� ���� �ǾƳ밡 ���õǾ�����, �� ��ȯ

		ofSetColor(230, 230, 230); //ȸ�� ���� 
		ofDrawRectangle(205.8*location, 200, 204.8, ofGetWidth());
		if ((location == 0) || (location == 3)) { //�ǹ� ��ġ�� ���� �ٸ� �� ��ȯ
			ofSetColor(0, 0, 0);
			ofDrawRectangle(204.8*(location + 1) - 50, 200, 100, 300);
		}
		if ((location == 1) || (location == 4)) {
			ofSetColor(0, 0, 0);
			ofDrawRectangle(204.8*(location + 1) - 50, 200, 100, 300);
			ofDrawRectangle(204.8*location - 50, 200, 100, 300);
		}
		if (location == 2) {
			ofSetColor(0, 0, 0);
			ofDrawRectangle(204.8*location - 50, 200, 100, 300);
		}
		draw_flag -= 0.5;
	}

	if (color_flag) {

		int r = ofRandom(100, 255); //���� �� ����
		int g = ofRandom(100, 255);
		int b = ofRandom(100, 255);


		ofSetColor(r, g, b);
		for (int k = 0; k < 5; k++) { //�ǹ� �߾ӿ� line ��ġ
			ofDrawLine((204.8*(location)+204.8*(location + 1)) / 2 - 20+(k*10), 200, (204.8*(location)+204.8*(location + 1)) / 2 - 20 + (k * 10), line);
		} //line ǥ�� 
		if (turnflag == 1) {
			updateFall();
		}
		else
			updateFall2();

	}
}
	

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ //key ���ۿ� ���� ��ȭ

    if (key == 'q'|| key == 'Q'){ //���� 
 
		location = 0;
		line = 0; 
		sheet.clear(); //�Ҵ�� �޸� �ʱ�ȭ
		cout << "Good bye!" << endl;
        _Exit(0); //����
    }
	if (key == 'r' || key == 'R') {
		print_flag = 1; //��� ����
		cout << "recording start" << endl;
	}

	if (key == 'x' || key == 'X') { //����� print�ϰ� sheet.txt ���Ͽ� ����
		ofstream fout("sheet.txt");
		sheet.push_back('\n');
		for (int i = 0; i < sheet.size(); i++) {
			cout << sheet[i] << " ";
			fout.put(sheet[i]);
		}
		cout << endl << "recording finish : sheet.txt" << endl;
		fout.close();
	}

    if (key == 'a'|| key =='A'){  //�ǾƳ� �ǹ� '��(C)' �� ��Ÿ���� key
		mySound.load("C.mp3"); //C.mp3 ���� ���� �ҷ�����
		mySound.play(); //���
		location = 0; //�� ���� �ʱ�ȭ
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('C');
		}

    }
	if (key == 's' || key == 'S') {  //�ǾƳ� �ǹ� '��(D)' �� ��Ÿ���� key
		mySound.load("D.mp3");
		mySound.play();
		location = 1;
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('D');
		}
	}
	if (key == 'd' || key == 'D') {  //�ǾƳ� �ǹ� '��(E)' �� ��Ÿ���� key
		mySound.load("E.mp3");
		mySound.play();
		location = 2;
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('E');
		}

	}
	if (key == 'f' || key == 'F') {  //�ǾƳ� �ǹ� '��(F)' �� ��Ÿ���� key
		mySound.load("F.mp3");
		mySound.play();
		location = 3;
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('F');
		}
	}
	if (key == 'g' || key == 'G') {  //�ǾƳ� �ǹ� '��(G)' �� ��Ÿ���� key
		mySound.load("G.mp3");
		mySound.play();
		location = 4;
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('G');
		}

	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

