#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "PaintEngine.h"
#include "CanvasRenderer.h"

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

	private:
		paintcore::PaintEngine engine;
		ofFbo canvas;
		ofImage cloudStamp;

		ofRectangle canvasRect { 260, 20, 800, 700 };
		bool isDrawing = false;
		bool stampLoaded = false;

		paintcore::BrushSettings strokeSettings {};
		float strokeAngle = 0.0f;

		ofxPanel gui;
		ofParameter<float> brushWidth;
		ofParameter<float> brushOpacity;
		ofParameter<float> brushSpacing;
		ofParameter<float> brushAngle;
		ofParameter<ofColor> brushColor;

		glm::vec2 toCanvasPosition(int x, int y) const;
		void finishCurrentStroke();
};
