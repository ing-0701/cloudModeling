#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();

	ofSetWindowTitle("Cloud Modeling - Paint");
	ofSetWindowShape(1360, 900);
	ofSetVerticalSync(true);

	// GUI作成前にブラシの初期値を設定する。
	auto settings = editor.paint.getBrushSettings();

	settings.size = 2.4f; // 直径120px
	settings.eraserSize = 2.4f; // 消しゴムも初期直径120px
	settings.color = ofColor(255, 255, 255, 128);
	settings.isPen = true;
	settings.eraserAlpha = 128.0f;
	settings.angleDegrees = 0.0f;
	settings.randomAngleDegrees = 180.0f;
	settings.spacingPx = 36.0f;

	editor.paint.setBrushSettings(settings);
	editor.setup();

	// 初期直径120px、最小5px、最大400px。
	editor.setBrushSizeRange(2.4f, 0.1f, 8.0f);

	cloudStampLoaded = cloudStamp.load("cloudTexture1.png");

	if (!cloudStampLoaded) {
		ofLogError("cloudModeling")
			<< "Failed to load bin/data/cloudTexture1.png";
	}

	editor.paint.engine.brush.setDetailedStamp(
		[this](const paintcore::StampInfo & stamp) {
			if (!cloudStampLoaded) {
				return;
			}

			const float width = stamp.radius * 2.0f;
			const float height = width * cloudStamp.getHeight() / cloudStamp.getWidth();

			ofPushMatrix();
			ofTranslate(stamp.position.x, stamp.position.y);
			ofRotateDeg(stamp.angleDegrees);

			cloudStamp.draw(
				-width * 0.5f,
				-height * 0.5f,
				width,
				height);

			ofPopMatrix();
		});
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(35);
	editor.draw();

	if (!cloudStampLoaded) {
		ofPushStyle();

		ofSetColor(255, 80, 80);
		ofDrawBitmapString(
			"Failed to load bin/data/cloudTexture1.png",
			10,
			editor.paint.canvasHeight + 30);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	editor.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button != OF_MOUSE_BUTTON_LEFT
		|| editor.paint.isDrawing()) {
		return;
	}

	const glm::vec2 canvasPosition = editor.view.toCanvas(glm::vec2(x, y));

	const bool insideCanvas = canvasPosition.x >= 0.0f
		&& canvasPosition.y >= 0.0f
		&& canvasPosition.x < editor.paint.canvasWidth
		&& canvasPosition.y < editor.paint.canvasHeight;

	if (insideCanvas && !cloudStampLoaded) {
		return;
	}

	editor.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	editor.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	if (x >= editor.paint.canvasWidth) {
		editor.mouseScrolled(scrollY);
	}
}
