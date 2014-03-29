//
//  LaserMouse
//
//  Created by T9T9.com 2014/03/29
//
//

#include "ofxLaserMouse.h"

#define PREVIEW_W	320
#define PREVIEW_H	240

ofxLaserMouse::ofxLaserMouse(){
	x = y = 0;
	leftClick = rightClick = false;
}

void ofxLaserMouse::setup(int camW, int camH){

	w = camW;
	h = camH;

	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(w,h);

	colorImg.allocate(w,h);
	grayImg.allocate(w,h);
	warpImg.allocate(w,h);
	binaryImg.allocate(w,h);

	drawGui = true;

	gui.setup("LaserMouse"); // most of the time you don't need a name
	gui.add(guiHelp.setup("[H] Key ", "Hide Menu"));
	gui.add(imgThreshold.setup("Threshold", 30, 0, 255));
	gui.add(minArea.setup("Min Area", 20, 0, 100));
	gui.add(maxArea.setup("Max Area", 900, 0, 600*440));
	gui.add(hMirror.setup("H Mirror", false));
	gui.add(vMirror.setup("V Mirror", false));
	gui.add(bgBlank.setup("Black Screen", false));
	gui.add(debugView.setup("Debug View", true));
	gui.add(mouseMode.setup("Mouse Mode", false));
	gui.add(LT.setup("LeftTop",ofVec2f(0,0),ofVec2f(0,0),ofVec2f(camW,camH)));
	gui.add(RT.setup("RightTop",ofVec2f(camW,0),ofVec2f(0,0),ofVec2f(camW,camH)));
	gui.add(RB.setup("RightBottom",ofVec2f(camW,camH),ofVec2f(0,0),ofVec2f(camW,camH)));
	gui.add(LB.setup("LeftBottom",ofVec2f(0,camH),ofVec2f(0,0),ofVec2f(camW,camH)));
	gui.add(mouseStatus.setup("Mouse ", ""));
	gui.loadFromFile("settings.xml");
	gui.setPosition( 0, 0 );

	toPoints[0]=ofVec2f(0,0);
	toPoints[1]=ofVec2f(camW,0);
	toPoints[2]=ofVec2f(camW,camH);
	toPoints[3]=ofVec2f(0,camH);

	ofAddListener(ofEvents().keyPressed, this, &ofxLaserMouse::_keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxLaserMouse::_keyReleased);
}

void ofxLaserMouse::update(){

	fromPoints[0] = (ofVec2f)LT;
	fromPoints[1] = (ofVec2f)RT;
	fromPoints[2] = (ofVec2f)RB;
	fromPoints[3] = (ofVec2f)LB;

	polyWarp.clear();
	for (int i = 0; i < 4; i++)
		polyWarp.addVertex( fromPoints[i] );
	polyWarp.addVertex( fromPoints[0] );
	
	vidGrabber.update();

	if (vidGrabber.isFrameNew()){
		colorImg.setFromPixels(vidGrabber.getPixels(), w, h);
		colorImg.mirror( vMirror, hMirror );

		grayImg = colorImg;		
		warpImg.warpIntoMe( grayImg, fromPoints, toPoints);

		binaryImg = warpImg;
		binaryImg.threshold(imgThreshold);

		contourFinder.findContours(binaryImg, minArea, maxArea, 1, false);
	}

	mouseStatus = ofToString(x) + "," + ofToString(y);
	if (lClick)
		mouseStatus = ofToString(x) + "," + ofToString(y) + " : Left";
	else if (rClick)
		mouseStatus = ofToString(x) + "," + ofToString(y) + " : Right";

	if (mouseMode){
		SetCursorPos( x * (float)ofGetScreenWidth() / w, y * (float)ofGetScreenHeight() / h);
	}
	
}

void ofxLaserMouse::draw(){

	if (!bgBlank){
		ofBackgroundGradient(ofColor::gray, ofColor::black);
	}

	ofSetColor(255);

	if (debugView){
		ofPushMatrix();
		ofTranslate( 200, 0 );
		colorImg.draw( 0, 0, PREVIEW_W, PREVIEW_H);
		grayImg.draw( PREVIEW_W, 0, PREVIEW_W, PREVIEW_H);
		warpImg.draw( PREVIEW_W*2, 0, PREVIEW_W, PREVIEW_H);
		binaryImg.draw( 0, PREVIEW_H, PREVIEW_W, PREVIEW_H);

		ofPushStyle();
		ofFill();
		ofSetColor(30);
		ofRect(PREVIEW_W, PREVIEW_H, PREVIEW_W, PREVIEW_H);

		ofNoFill();
		ofSetColor(255,0,0);
		ofPushMatrix();
		// Warp Points
		ofTranslate( PREVIEW_W, 0 );
		ofScale((float)PREVIEW_W / w,(float)PREVIEW_H / h);
		polyWarp.draw();

		for (int i = 0; i < 4; i++)
			ofCircle(polyWarp[i].x, polyWarp[i].y,12);
		ofPopMatrix();

		ofPushMatrix();
		// Contour Finder
		ofTranslate( 0, PREVIEW_H );
		ofScale((float)PREVIEW_W / w,(float)PREVIEW_H / h);
		stringstream str;
		for (int i = 0; i < contourFinder.nBlobs; i++){

			contourFinder.blobs[i].draw(0, 0);

			x = (contourFinder.blobs[i].centroid.x);
			y = (contourFinder.blobs[i].centroid.y);

			ofSetColor(ofColor::red);

			str << std::fixed << setprecision(0) << x << "," << setprecision(0) << y;
			ofDrawBitmapString(str.str(), x, y);
		}
		ofPopMatrix();

		if (contourFinder.nBlobs > 0){
			ofPushMatrix();
			ofTranslate( PREVIEW_W, PREVIEW_H );
			ofScale((float)PREVIEW_W / w,(float)PREVIEW_H / h);
			ofSetColor(ofColor::green);
			ofFill();
			ofCircle(x,y,14);
			ofPopMatrix();
		}
		ofPopStyle();
		ofPopMatrix();
	}

	if (drawGui){
		ofEnableAlphaBlending();
		gui.draw();
		ofDisableAlphaBlending();
	}

}

void ofxLaserMouse::_keyPressed(ofKeyEventArgs &e){
	switch (e.key){
	case 'h':
	case 'H':
		drawGui = !drawGui;
		break;
	case 'm':
	case 'M':
		mouseMode = !mouseMode;
		break;
	case OF_KEY_PAGE_UP:
		lClick = true;
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,GetMessageExtraInfo());
		break;
	case OF_KEY_PAGE_DOWN:
		rClick = true;
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,GetMessageExtraInfo());
		break;
	}
}

void ofxLaserMouse::_keyReleased(ofKeyEventArgs &e){
	switch (e.key){
	case OF_KEY_PAGE_UP:
		lClick = false;
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());
		break;
	case OF_KEY_PAGE_DOWN:
		rClick = false;
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,GetMessageExtraInfo());
		break;

	}
}

void ofxLaserMouse::save(){
	gui.saveToFile("settings.xml");
}