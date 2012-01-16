//
//  ofxKulerTheme.cpp
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/14/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#include <iostream>

#include "ofxKulerTheme.h"

ofxKulerTheme::ofxKulerTheme()
{	
}

ofxKulerTheme::ofxKulerTheme( const ofxKulerTheme &c )
{
    for ( std::vector<ofFloatColor>::const_iterator it = c.getSwatches().begin(); it != c.getSwatches().end(); ++it )
    {        
        addSwatch( *it );
    }
}

ofxKulerTheme::~ofxKulerTheme()
{	
}

void ofxKulerTheme::addSwatch( const int hex, const float a )
{
    ofFloatColor color;
    
    color.setHex( hex, a );
    
    swatches.push_back( color );
}

void ofxKulerTheme::addSwatch( const ofFloatColor &color )
{
    swatches.push_back( ofFloatColor( color.r, color.g, color.b, color.a )  );
}

void ofxKulerTheme::addSwatch(const ofColor &color)
{
	swatches.push_back( ofFloatColor( color.r, color.g, color.b, color.a ) );
}

void ofxKulerTheme::addSwatch(const float r, const float g, const float b, const float a)
{
	swatches.push_back( ofFloatColor( r, g, b, a ) );
}

void ofxKulerTheme::addSwatchHSV( const float h, const float s, const float v, const float a )
{
    ofColor color;
    
    color.setHsb( h, s, v, a);
    
    swatches.push_back( ofFloatColor( color.r, color.g, color.b, color.a ) );
}

const int ofxKulerTheme::getSwatchCount() const
{
	return (int)swatches.size();
}

const std::vector<ofFloatColor>& ofxKulerTheme::getSwatches() const
{
    return swatches;
}

ofFloatColor& ofxKulerTheme::getSwatch(const int index)
{
	if( index < getSwatchCount() )
	{
		return swatches[index];
	}
}

ofFloatColor ofxKulerTheme::getSwatchInterpolated(const float delta)
{
	if ( getSwatchCount() > 0 )
    {
        calculateInterpolatedSwatch( tempColor, delta );
        
        return tempColor;
    }
}

ofFloatColor ofxKulerTheme::getRandomSwatch() const
{	
    if ( getSwatchCount() > 0 )
    {
        return swatches[ (int)ofRandom( 0, getSwatchCount() - 1 ) ];
    }
}

ofFloatColor ofxKulerTheme::getRandomSwatchInterpolated()
{
    if ( getSwatchCount() > 0 )
    {
        return getSwatchInterpolated( ofRandom(0.0f, 1.0f) );
    }
}

void ofxKulerTheme::calculateInterpolatedSwatch( ofFloatColor& result, const float delta ) const
{
    const int swatchCount   = getSwatchCount();
    
    if ( swatchCount > 0 )
    {
        const float newInter	= fmodf( delta, 1.0f );
        const float t			= 1.0f / (float)swatchCount;
        const int prevIndex		= (int)floorf( newInter / t );
        const float f			= ( newInter - ( (float)prevIndex * t ) ) / t;
		
        if ( f == 0.0f )
        {
            result.set( swatches[prevIndex] );
        } else {
            
            int nextIndex		= prevIndex + 1;
            
            if ( prevIndex == getSwatchCount() - 1 )
            {
                nextIndex		= 0;
            }
            
            result.set( swatches[prevIndex] );
            result.lerp( swatches[nextIndex] , f );
        }
    }
}