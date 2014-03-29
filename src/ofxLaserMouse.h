//
//  LaserMouse
//
//  Created by T9T9.com 2014/03/29
//

#include "ofxOpenCv.h"
#include "ofxGui.h"

class ofxLaserMouse{

public:
	ofxLaserMouse();

	void			setup(int camW=640, int camH=480);
	void			update();
	void			draw();
	void			_keyPressed(ofKeyEventArgs &e);
	void			_keyReleased(ofKeyEventArgs &e);
	void			save();

	int				w, h;
	int				x, y;
	bool			leftClick;
	bool			rightClick;
	bool			lClick;
	bool			rClick;

	ofPolyline		polyWarp;

	ofVideoGrabber 	vidGrabber;

	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage		grayImg;
	ofxCvGrayscaleImage 	warpImg;
	ofxCvGrayscaleImage 	binaryImg;

	ofxCvContourFinder		contourFinder;

	// GUI group
	bool			drawGui;

	ofxGuiGroup		gui;
	ofxLabel		guiHelp;

	ofxVec2Slider	LT;	// Left Top
	ofxVec2Slider	LB;	// Left Bottom
	ofxVec2Slider	RB; // Right Bottom
	ofxVec2Slider	RT; // Right Top

	ofxIntSlider	imgThreshold;
	ofxIntSlider	minArea;
	ofxIntSlider	maxArea;

	ofxToggle		vMirror;
	ofxToggle		hMirror;
	ofxToggle		bgBlank;
	ofxToggle		debugView;
	ofxToggle		mouseMode;

	ofxLabel		mouseStatus;

	ofPoint         fromPoints[4];
	ofPoint         toPoints[4];
};
