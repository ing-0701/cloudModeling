#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "PaintEngine.h"
#include "CanvasRenderer.h"
#include "ColorPicker.h"

#include <map>
#include <utility>

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

		ofRectangle canvasRect { 280, 20, 800, 700 };
		bool isDrawing = false;
		bool stampLoaded = false;

		paintcore::BrushSettings strokeSettings {};
		ofxPanel gui;
		ofParameter<float> brushWidth;
		ofParameter<float> brushOpacity;
		ofParameter<float> brushSpacing;

		glm::vec2 toCanvasPosition(int x, int y) const;
		void finishCurrentStroke();
		paintcoreui::ColorPicker colorPicker;

		// アドオンのピッカーは256×256固定。
		ofRectangle colorPickerRect { 10, 200, 256, 256 };
		glm::vec2 colorPickerPosition { 10.0f, 200.0f };
		bool isPickingColor = false;

		ofParameter<int> backgroundGray;

		// ストローク内で、同じ配置点を描き直すときの角度を保持する。
		std::map<std::pair<float, float>, float> stampAngles;
};
