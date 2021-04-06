#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->fbo1.allocate(ofGetWidth(), ofGetHeight());
	this->fbo2.allocate(ofGetWidth(), ofGetHeight());
	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	this->fbo1.begin();
	ofClear(0);
	
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofRotateX(ofGetFrameNum() * 0.37);
	ofRotateY(ofGetFrameNum() * 0.72);

	ofMesh face, frame;
	frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	float phi_deg_step = 5;
	float theta_deg_step = 3;
	float radius = 250;

	for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

		for (float theta_deg = 0; theta_deg < 180; theta_deg += theta_deg_step) {

			auto noise_value = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.005);
			if (noise_value < 0.45) { continue; }

			auto noise_1 = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.005);
			auto noise_2 = ofNoise(radius * cos((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02, radius * sin((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.005);
			auto noise_3 = ofNoise(radius * cos((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02, radius * sin((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.005);
			auto noise_4 = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.005);

			auto index = face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
			vertices.push_back(glm::vec3(
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
				radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

			face.addVertices(vertices);

			face.addIndex(index + 0); face.addIndex(index + 1); face.addIndex(index + 3);
			face.addIndex(index + 0); face.addIndex(index + 3); face.addIndex(index + 2);

			if (noise_1 < 0.45) {

				frame.addVertex(vertices[0]);
				frame.addVertex(vertices[1]);

				frame.addIndex(frame.getNumVertices() - 1);
				frame.addIndex(frame.getNumVertices() - 2);
			}

			if (noise_2 < 0.45) {

				frame.addVertex(vertices[0]);
				frame.addVertex(vertices[2]);

				frame.addIndex(frame.getNumVertices() - 1);
				frame.addIndex(frame.getNumVertices() - 2);
			}

			if (noise_3 < 0.45) {

				frame.addVertex(vertices[1]);
				frame.addVertex(vertices[3]);

				frame.addIndex(frame.getNumVertices() - 1);
				frame.addIndex(frame.getNumVertices() - 2);
			}

			if (noise_4 < 0.45) {

				frame.addVertex(vertices[2]);
				frame.addVertex(vertices[3]);

				frame.addIndex(frame.getNumVertices() - 1);
				frame.addIndex(frame.getNumVertices() - 2);

			}
		}
	}

	ofSetColor(0);
	face.draw();

	ofSetColor(255);
	frame.drawWireframe();
		
	this->fbo1.end();

	this->fbo2.begin();
	ofClear(0);
	ofSetColor(0);

	int span = 180;
	auto flag = true;
	for (int x = 0; x < ofGetWidth(); x += span) {

		for (int y = 0; y < ofGetHeight(); y += span) {

			if (flag) { ofDrawRectangle(x, y, span, span); }
			flag = !flag;
		}
		flag = !flag;
	}

	this->fbo2.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniformTexture("tex1", this->fbo1.getTexture(), 1);
	this->shader.setUniformTexture("tex2", this->fbo2.getTexture(), 2);

	ofDrawRectangle(glm::vec2(0, 0), ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}