#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();

	ofSetWindowTitle("Cloud Modeling - Paint");
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

	// UIの配置はcloudModeling側で指定する。
	const float uiX = editor.paint.canvasWidth + 8.0f;
	const float uiWidth = 170.0f;

	paintcoreui::PaintEditorLayout layout;

	// カラーピッカーは従来と同じ位置。
	layout.colorPickerPosition = { uiX, 20.0f };

	// カラーピッカーとHueスライダーの下。
	layout.brushPanelPosition = { uiX, 324.0f };

	// Brushパネルの下。
	// 追加・削除ボタンと、その下の一覧をまとめて配置する。
	layout.layerAreaPosition = {
		uiX,
		500.0f
	};
	// 背景ピッカー展開時の下端を998pxに収める。
	layout.visibleLayerRows = 3;
	layout.backgroundWidth = uiWidth;

	// レイヤー操作ボタン68px + 一覧(1行24px)の下に16px空ける。
	layout.backgroundButtonPosition = {
		layout.layerAreaPosition.x,
		layout.layerAreaPosition.y
			+ 68.0f + 24.0f * layout.visibleLayerRows + 16.0f
	};

	editor.paint.setBackground(ofColor(50, 50, 50));
	editor.setup(layout);

	cloudStamp = std::make_shared<ofImage>();
	cloudStampLoaded = cloudStamp->load("cloudTexture1.png");

	if (!cloudStampLoaded) {
		ofLogError("cloudModeling")
			<< "Failed to load bin/data/cloudTexture1.png";
	}

	editor.paint.engine.brush.setImageStamp(cloudStamp);

	// ここまでがペイントソフト

	camera.setPosition(0, 0, 600);
	camera.setControlArea(ofRectangle(
		0, canvasSettings.height,
		canvasSettings.width, canvasSettings.height));
	light.setSpotlight();
	light.setPosition(-250, 300, 350);
	light.enable();
	plane.set(100, 100, 10, 10);
	plane.setPosition(0, 0, 0);
	mesh = plane.getMesh();

	ofFbo::Settings s3D;
	s3D.width = canvasSettings.width;
	s3D.height = canvasSettings.height;

	fbo3D.allocate(s3D);
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		glm::vec3 pos = mesh.getVertices()[i];
		pos.z = ofRandom(-100, 100);
		mesh.setVertex(i, pos);
	}
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

	// 以下3D

	fbo3D.begin();
	ofPushStyle();

	ofClear(0, 0, 0, 0);

	camera.begin();
	ofEnableDepthTest();
	ofEnableLighting();

	plane.draw();

	ofDisableLighting();
	ofDisableDepthTest();
	camera.end();

	ofPopStyle();
	fbo3D.end();


	ofPushStyle();
	ofSetColor(255);
	fbo3D.draw(0, canvasSettings.height, canvasSettings.width, canvasSettings.height);
	ofPopStyle();
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

	const bool insideCanvas = x >= 0
		&& y >= 0
		&& x < editor.paint.canvasWidth
		&& y < editor.paint.canvasHeight;

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
void ofApp::mouseScrolled(
	int x, int y, float scrollX, float scrollY) {
	editor.mouseScrolled(scrollY);
}
