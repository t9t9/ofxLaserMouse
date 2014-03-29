#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1160,600,OF_WINDOW);			// <-------- setup the GL context

	ofRunApp(new testApp());

}
