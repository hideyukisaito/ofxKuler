//
//  ofxKulerTheme.h
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/14/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#pragma once

#include <vector>

#include "ofMain.h"

class ofxKulerTheme {

public:
	
	ofxKulerTheme();
    ofxKulerTheme( const ofxKulerTheme &c );
	~ofxKulerTheme();
	
    void addSwatch( const int hex, const float a );
    void addSwatch( const ofFloatColor &color );
	void addSwatch( const ofColor& color );
	void addSwatch( const float r, const float g, const float b, const float a );
	void addSwatchHSV( const float h, const float s, const float v, const float a );
    
	const int getSwatchCount() const;
    
    const std::vector<ofFloatColor>& getSwatches() const;
	
	ofFloatColor& getSwatch( const int index );
	ofFloatColor  getSwatchInterpolated( const float delta );
	ofFloatColor  getRandomSwatch() const;
	ofFloatColor  getRandomSwatchInterpolated();
	
	void calculateInterpolatedSwatch( ofFloatColor& result, const float delta ) const;
	
protected:
	
	std::vector<ofFloatColor>	swatches;
	ofFloatColor				tempColor;
	
};
