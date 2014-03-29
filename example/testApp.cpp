#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofSetFrameRate(30);

	camW = 640;
	camH = 480;

	laserMouse.setup(camW, camH);
}

//--------------------------------------------------------------
void testApp::update(){

	laserMouse.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	laserMouse.draw();

	ofSetColor(0,255,0);
	ofNoFill();
	ofBeginShape();
	for(int i = 0; i < line.size(); i++)
		ofVertex(line[i].x, line[i].y);
	ofEndShape();
	ofFill();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key){
	case 'f':
	case 'F':
		ofToggleFullscreen();
		break;
	case 'c':
	case 'C':
		line.clear();
		break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	line.push_back( ofVec2f(x ,y ));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	line.push_back( ofVec2f(x ,y ));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	line.push_back( ofVec2f(x ,y ));
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit(){
	laserMouse.save();
}