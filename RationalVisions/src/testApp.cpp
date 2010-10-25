#include "testApp.h"

void testApp::setup() {
	plotHeight = 128;
	bufferSize = 512;
	
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT);
	// To use with FFTW, try:
	// fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT, OF_FFT_FFTW);
	
	audioInput = new float[bufferSize];
	fftOutput = new float[fft->getBinSize()];
	eqFunction = new float[fft->getBinSize()];
	eqOutput = new float[fft->getBinSize()];
	ifftOutput = new float[bufferSize];
	
	//bins are 0-nyquist - ie 0Hz - 22,500Hz
	//we want to clump according bark scale - freq bands are:
	//20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500.
	//http://en.wikipedia.org/wiki/Bark_scale
	
	numBarkScaleValues = 25;
	fftBarkScale = new float[numBarkScaleValues];
	float fftBarkBands[] = 
	{
		20, 100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500
	};
	
	binToBark = new int[fft->getBinSize()];
	int bark = 0;
	for(int i=0; i<fft->getBinSize(); ++i)
	{
		float f = (float)i / (float)fft->getBinSize();
		float freq = 22500.0f * f;
		if(freq > fftBarkBands[bark] && bark <= numBarkScaleValues-1)
		{
			++bark;
		}
		binToBark[i] = bark;
	}
	
	spectrogram.allocate(bufferSize, numBarkScaleValues*4, OF_IMAGE_GRAYSCALE);
	float w = spectrogram.getWidth();
	memset(spectrogram.getPixels(), 0, (int) (spectrogram.getWidth() * spectrogram.getHeight()) );
	spectrogramOffset = 0;
	
	
	// 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// [bins] samples per buffer
	// 4 num buffers (latency)
	
	// this describes a linear low pass filter
	for(int i = 0; i < fft->getBinSize(); i++)
		eqFunction[i] = (float) (fft->getBinSize() - i) / (float) fft->getBinSize();
	
	appWidth = ofGetWidth();
	appHeight = ofGetHeight();
	
	ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
	
	
	
	ofBackground(0, 0, 0);
}

void testApp::draw() {
	ofSetColor(0xffffff);
	ofPushMatrix();
	
	glTranslatef(16, 16, 0);
	ofDrawBitmapString("Audio Input", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0);
	
	glTranslatef(0, plotHeight + 16, 0);
	//ofDrawBitmapString("FFT Output", 0, 0);
	//plot(fftOutput, fft->getBinSize(), -plotHeight, plotHeight / 2);
	
	ofPushMatrix();
	glTranslatef(fft->getBinSize(), 0, 0);
	ofDrawBitmapString("EQd FFT Output", 0, 0);
	plot(fftBarkScale, numBarkScaleValues, -plotHeight, plotHeight / 2);
	ofPopMatrix();
	
	glTranslatef(0, plotHeight + 16, 0);
	ofDrawBitmapString("IFFT Output", 0, 0);
	plot(ifftOutput, fft->getSignalSize(), plotHeight / 2, 0);
	
	glTranslatef(0, plotHeight + 16, 0);
	spectrogram.update();
	spectrogram.draw(0, 0);
	
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);
}

void testApp::plot(float* array, int length, float scale, float offset) {
	ofNoFill();
	ofRect(0, 0, length, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < length; i++)
		ofVertex(i, array[i] * scale);
	ofEndShape();
	glPopMatrix();
}


void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
	// store input in audioInput buffer
	memcpy(audioInput, input, sizeof(float) * bufferSize);
	
	fft->setSignal(audioInput);
	memcpy(fftOutput, fft->getAmplitude(), sizeof(float) * fft->getBinSize());
	
	for(int i = 0; i < fft->getBinSize(); i++)
		eqOutput[i] = fftOutput[i] * eqFunction[i];
	
	fft->setPolar(eqOutput, fft->getPhase());
	
	fft->clampSignal();
	memcpy(ifftOutput, fft->getSignal(), sizeof(float) * fft->getSignalSize());
	
	for(int i=0; i<numBarkScaleValues; i++)
	{
		//cout << fftBarkScale[i] << ",";
		fftBarkScale[i] = 0;
	}
	//cout << "\n";

	
	for(int i = 0; i < fft->getBinSize(); i++)
	{
		int bark = binToBark[i];
		fftBarkScale[bark] += fftOutput[i];
	}
	
	int spectrogramWidth = (int) spectrogram.getWidth();
	unsigned char* pixels = spectrogram.getPixels();
	for(int i = 0; i < numBarkScaleValues; i++)
	{	
		pixels[i * spectrogramWidth + spectrogramOffset] = (unsigned char) (255. * fftBarkScale[i]) + 32;
	}
	spectrogramOffset = (spectrogramOffset + 1) % spectrogramWidth;
}

void testApp::keyPressed(int key) {
	switch (key) {
		default:
			break;
	}
}
