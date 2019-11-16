#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"

class LissajousApp : public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    ofxAudioUnitNetReceive mAudioUnitNetReceive;
    ofxAudioUnitTap mAudioUnitTap;
    ofxAudioUnitOutput mAudioUnitOutput;

    vector<float> mLeftSamples;
    vector<float> mRightSamples;

    ofPolyline mPolyline;
    vector<ofVec2f> mVertices;

    ofEasyCam mCamera;
    ofShader mShader;
};

void LissajousApp::setup()
{
    mAudioUnitNetReceive.connectToHost("127.0.0.1");
    mAudioUnitNetReceive.connectTo(mAudioUnitTap).connectTo(mAudioUnitOutput);

    mAudioUnitTap.setBufferLength(512);

    mAudioUnitOutput.start();

    ofBackground(0, 0, 0);
    ofEnableDepthTest();
    ofEnableSmoothing();

    mShader.load("", "shader.flag");
}

void LissajousApp::update()
{
    mAudioUnitTap.getLeftSamples(mLeftSamples);
    mAudioUnitTap.getRightSamples(mRightSamples);

    mPolyline.clear();
    for (auto i = 0; i < mLeftSamples.size(); ++i)
    {
        mPolyline.addVertex(ofVec3f(mLeftSamples[i], mRightSamples[i], mLeftSamples[i] * mRightSamples[i]) * 200);
    }

    mVertices.clear();
    for (const auto &world : mPolyline)
    {
        auto screen = mCamera.worldToScreen(world);
        mVertices.emplace_back(screen.x, ofMap(screen.y, 0, ofGetHeight(), ofGetHeight(), 0));
    }
}

void LissajousApp::draw()
{
    ofSetColor(255);
    ofSetLineWidth(2);

    mCamera.begin();
    mPolyline.draw();
    mCamera.end();

    mShader.begin();
    mShader.setUniform2fv("vertices", &mVertices[0][0], mVertices.size());
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    mShader.end();
}
