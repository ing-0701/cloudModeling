#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(1100, 740);
	ofSetVerticalSync(true);

	ofFbo::Settings settings;
	settings.width = 800;
	settings.height = 700;
	settings.internalformat = GL_RGBA;
	settings.useDepth = false;
	settings.useStencil = false;
	settings.numSamples = 0;

	canvas.allocate(settings);
	canvas.begin();
	ofClear(0, 0, 0, 0);
	canvas.end();

	engine.allocateStrokeBuffers(settings);
	gui.setup("Cloud Brush");
	gui.setPosition(10, 20);

	gui.add(brushWidth.set("Width(px)", 120.0f, 10.0f, 400.0f));
	gui.add(brushOpacity.set("Opacity", 0.5f, 0.0f, 1.0f));
	gui.add(brushSpacing.set("Spacing / width", 0.3f, 0.05f, 2.0f));
	gui.add(backgroundGray.set("Background", 100, 0, 255));

	gui.setWidthElements(256);
	colorPicker.setup(colorPickerRect.x, colorPickerRect.y);

	stampLoaded = cloudStamp.load("cloudTexture1.png");
	if (!stampLoaded) {
		ofLogError("cloudModeling") << "Failed to load bin/data/cloudTexture1.png";
		return;
	}

	cloudStamp.setImageType(OF_IMAGE_COLOR_ALPHA);
	auto & pixels = cloudStamp.getPixels();

	for (int y = 0; y < pixels.getHeight(); y++) {
		for (int x = 0; x < pixels.getWidth(); x++) {
			const auto alpha = pixels.getColor(x, y).a;
			pixels.setColor(x, y, ofColor(255, 255, 255, alpha));
		}
	}

	cloudStamp.update();

	// 雲ブラシの関数実装
	engine.brush.setStamp(
		[this](const glm::vec2 & center, float radius) {
			const auto key = std::make_pair(center.x, center.y);
			auto angleIt = stampAngles.find(key);

			if (angleIt == stampAngles.end()) {
				angleIt = stampAngles.emplace(key, ofRandom(0.0f, 360.0f)).first;
			}

			const float angle = angleIt->second;
			const float width = radius * 2.0f;
			const float height = width * cloudStamp.getHeight() / cloudStamp.getWidth();

			ofPushMatrix();
			ofTranslate(center.x, center.y);
			ofRotateDeg(angle);

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

	ofPushStyle();
	ofFill();

	// この背景色は画面表示用
	ofSetColor(backgroundGray.get());
	ofDrawRectangle(canvasRect);

	ofPushMatrix();
	ofTranslate(canvasRect.x, canvasRect.y);

	if (isDrawing) {
		paintcore::CanvasRenderer::drawNormalImage(
			engine.getStrokePreview());
	} else {
		paintcore::CanvasRenderer::drawNormalImage(canvas);
	}

	ofPopMatrix();
	ofPopStyle();

	// CanvasRendererが設定した合成方式をGUI用に戻す
	ofEnableAlphaBlending();
	ofSetColor(255);
	gui.draw();

	colorPicker.drawPicker();

	// pick()には、色の取得と選択位置の円を描く処理がある。
	// 毎フレーム呼ぶことで、選択位置の表示と色相変更を反映する。
	colorPicker.pick(
		colorPickerPosition.x,
		colorPickerPosition.y);

	colorPicker.drawHueSlider();

	if (!stampLoaded) {
		ofSetColor(255);
		ofDrawBitmapString(
			"Failed to load bin/data/cloudTexture1.png",
			canvasRect.x + 20,
			canvasRect.y + 30);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (button != OF_MOUSE_BUTTON_LEFT) {
		return;
	}

	if (isPickingColor) {
		colorPickerPosition = glm::vec2(x, y);
		return;
	}

	if (!isDrawing) {
		return;
	}

	ofPushStyle();
	engine.addStrokePoint(toCanvasPosition(x, y));
	engine.drawStroke(strokeSettings);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button != OF_MOUSE_BUTTON_LEFT || isDrawing) {
		return;
	}

	if (colorPickerRect.inside(x, y)) {
		isPickingColor = true;
		colorPickerPosition = glm::vec2(x, y);
		return;
	}

	if (!stampLoaded
		|| gui.getShape().inside(x, y)
		|| !canvasRect.inside(x, y)) {
		return;
	}

	strokeSettings.size = brushWidth.get() / 50.0f;
	strokeSettings.color = colorPicker.getColor();
	strokeSettings.color.a = static_cast<unsigned char>(
		brushOpacity.get() * 255.0f + 0.5f);
	strokeSettings.isPen = true;
	strokeSettings.eraserAlpha = 255.0f;

	// ストロークを開始するたびに角度を決め直す。
	stampAngles.clear();

	const float spacingPx = brushWidth.get() * brushSpacing.get();

	ofPushStyle();

	engine.beginStroke(
		toCanvasPosition(x, y),
		canvas,
		spacingPx);
	engine.drawStroke(strokeSettings);
	ofPopStyle();

	isDrawing = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (button != OF_MOUSE_BUTTON_LEFT) {
		return;
	}

	if (isPickingColor) {
		colorPickerPosition = glm::vec2(x, y);
		isPickingColor = false;
		return;
	}

	if (isDrawing) {
		finishCurrentStroke();
	}
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
glm::vec2 ofApp::toCanvasPosition(int x, int y) const {
	return glm::vec2(
		static_cast<float>(x) - canvasRect.x,
		static_cast<float>(y) - canvasRect.y);
}

//--------------------------------------------------------------
void ofApp::finishCurrentStroke() {
	if (!isDrawing) {
		return;
	}

	ofPushStyle();

	engine.drawStroke(strokeSettings);
	engine.commitStroke(canvas);

	ofPopStyle();

	isDrawing = false;
}
