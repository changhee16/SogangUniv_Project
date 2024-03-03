#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); 
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0; //draw, color flag 초기화
	color_flag = 0;
    
}

//--------------------------------------------------------------
void ofApp::update() {


}


//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0, 0, 0); 

	//피아노 그리기
	ofDrawRectangle(0, 0, 1024, 200); 
	ofSetLineWidth(2);
	for (int k = 0; k < 5; k++) {
		ofDrawLine(204.8*(k + 1), 200, 204.8*(k + 1), ofGetWidth());
		if (k != 2) {
			ofDrawRectangle(204.8*(k + 1) - 50, 200, 100, 300);
		}
	}


	ofSetLineWidth(5);
	if (draw_flag) { //key에 의해 피아노가 선택되었을때, 색 변환

		ofSetColor(230, 230, 230); //회색 지정 
		ofDrawRectangle(205.8*location, 200, 204.8, ofGetWidth());
		if ((location == 0) || (location == 3)) { //건반 위치에 따른 다른 색 변환
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

		int r = ofRandom(100, 255); //랜덤 색 정의
		int g = ofRandom(100, 255);
		int b = ofRandom(100, 255);


		ofSetColor(r, g, b);
		for (int k = 0; k < 5; k++) { //건반 중앙에 line 위치
			ofDrawLine((204.8*(location)+204.8*(location + 1)) / 2 - 20+(k*10), 200, (204.8*(location)+204.8*(location + 1)) / 2 - 20 + (k * 10), line);
		} //line 표현 
		if (turnflag == 1) {
			updateFall();
		}
		else
			updateFall2();

	}
}
	

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ //key 동작에 따른 변화

    if (key == 'q'|| key == 'Q'){ //종료 
 
		location = 0;
		line = 0; 
		sheet.clear(); //할당된 메모리 초기화
		cout << "Good bye!" << endl;
        _Exit(0); //종료
    }
	if (key == 'r' || key == 'R') {
		print_flag = 1; //기록 시작
		cout << "recording start" << endl;
	}

	if (key == 'x' || key == 'X') { //기록을 print하고 sheet.txt 파일에 저장
		ofstream fout("sheet.txt");
		sheet.push_back('\n');
		for (int i = 0; i < sheet.size(); i++) {
			cout << sheet[i] << " ";
			fout.put(sheet[i]);
		}
		cout << endl << "recording finish : sheet.txt" << endl;
		fout.close();
	}

    if (key == 'a'|| key =='A'){  //피아노 건반 '도(C)' 를 나타내는 key
		mySound.load("C.mp3"); //C.mp3 음악 파일 불러오기
		mySound.play(); //재생
		location = 0; //각 변수 초기화
		draw_flag = 1;
		color_flag = 1;
		turnflag = 1;
		line = 200;
		if (print_flag) {
			sheet.push_back('C');
		}

    }
	if (key == 's' || key == 'S') {  //피아노 건반 '레(D)' 를 나타내는 key
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
	if (key == 'd' || key == 'D') {  //피아노 건반 '미(E)' 를 나타내는 key
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
	if (key == 'f' || key == 'F') {  //피아노 건반 '파(F)' 를 나타내는 key
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
	if (key == 'g' || key == 'G') {  //피아노 건반 '솔(G)' 을 나타내는 key
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

