#pragma once

#include "ofMain.h"
#include "ofxFft.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void plot(float* array, int length, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
	void keyPressed(int key);
	
	int plotHeight, bufferSize;
	
	ofxFft* fft;
	
	float* audioInput;
	float* fftOutput;
	float* eqFunction;
	float* eqOutput;
	float* ifftOutput;
	
	float* fftBarkScale;
	float fftBarkBands[];
	int* binToBark;

	int numBarkScaleValues;
	
	ofImage spectrogram;
	int spectrogramOffset;
	
	float appWidth;
	float appHeight;
	
	int mode;
};
