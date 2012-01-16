//
//  ofxKulerLoader.cpp
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/15/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#include <iostream>

#include "ofxKulerLoader.h"


ofxKulerLoader::ofxKulerLoader()
:status( NO_STATUS )
,verbose(false)
{		
}

ofxKulerLoader::~ofxKulerLoader()
{
}

void ofxKulerLoader::loadFromURL(string _urlRequest)
{
	urlRequest		= _urlRequest;
	status			= LOAD_STARTED;
	
	startThread( true, verbose );		
}

std::vector<ofxKulerTheme>& ofxKulerLoader::getLoadedThemes()
{
	return loadedThemes;
}

void ofxKulerLoader::clearLoadedThemes()
{
    loadedThemes.clear();
}

const ofxKulerLoader::loaderStatus ofxKulerLoader::getStatus()
{
	return status;
}

void ofxKulerLoader::setVerbose(bool newVerboseVal)
{
    verbose = newVerboseVal;
}

void ofxKulerLoader::threadedFunction()
{
    if( lock() )
    {
        ofxXmlSettings xmlSettings;
        
        bool dataLoaded         = false;
                    
        ofHttpResponse response = ofLoadURL( urlRequest );
                
        if ( response.status == 200 )
        {
            dataLoaded          = true;
            xmlSettings.loadFromBuffer( stringReplace( response.data.getText(), ":", "." ) );
        } else {
            dataLoaded  = false;                
            ofLogError( "ofxKulerLoader", "An error accured while loading Kuler service." );
        }
        
        if ( dataLoaded )
        {
            try {
                if ( xmlSettings.pushTag( "rss" ) && xmlSettings.pushTag( "channel" ) )
                {
                    const int itemCount						= xmlSettings.getNumTags( "item" );
                    for ( int i = 0; i < itemCount; i++ )
                    {
                        xmlSettings.pushTag( "item", i );
                        
                        const int themeItemCount			= xmlSettings.getNumTags( "kuler.themeItem" );
                        for( int j = 0; j < themeItemCount; j++ )
                        {
                            xmlSettings.pushTag( "kuler.themeItem", j );
                            
                            loadedThemes.push_back( ofxKulerTheme() );
                            
                            const int themeSwatchesCount	= xmlSettings.getNumTags( "kuler.themeSwatches" );
                            for ( int k = 0; k < themeSwatchesCount; k++ )
                            {
                                xmlSettings.pushTag( "kuler.themeSwatches", k );
                                
                                const int swatchCount		= xmlSettings.getNumTags( "kuler.swatch" );
                                for ( int m = 0; m < swatchCount; m++ )
                                {
                                    xmlSettings.pushTag( "kuler.swatch", m );
                                    
                                    float channel1  = xmlSettings.getValue( "kuler.swatchChannel1" , 0.0f );
                                    float channel2  = xmlSettings.getValue( "kuler.swatchChannel2" , 0.0f );
                                    float channel3  = xmlSettings.getValue( "kuler.swatchChannel3" , 0.0f );
                                    
                                    string mode     = xmlSettings.getValue( "kuler.swatchColorMode", "rgb" );
                                                                        
                                    if ( mode == "rgb" )
                                    {
                                        loadedThemes.back().addSwatch( channel1, channel2, channel3, 1.0f );
                                    } else if( mode == "hsv" )
                                    {
                                        loadedThemes.back().addSwatchHSV( channel1 * 255.0f, channel2 * 255.0f, channel3 * 255.0f, 1.0f );
                                    }
                                    
                                    xmlSettings.popTag();
                                }
                                
                                if ( loadedThemes.back().getSwatchCount() == 0 )
                                {
                                    loadedThemes.erase( loadedThemes.end() - 1 );
                                }
                                                                
                                xmlSettings.popTag();
                            }
                            xmlSettings.popTag();
                        }
                        xmlSettings.popTag();
                    }
                }
                
                status      = LOAD_COMPLETED;
                
            } catch ( ... ) {
                
                status		= LOAD_FAILED;
                
                ofLogError( "ofxKulerLoader", "An error accured while parsing Kuler service." );
                
                return;
            }
            
        } else {
            status  = LOAD_FAILED;
        }
            
        unlock();
        stopThread();
    }
}

inline std::string ofxKulerLoader::stringReplace( const std::string &original, const std::string &searchString, const std::string &newtext )
{
    std::stringstream ss;           
    std::string::size_type last = 0;
    std::string::size_type it = original.find( searchString, last );
    
    while( it != original.npos )
    {
        if( it-last > 0 )
        {
            ss << original.substr( last, it - last );
            ss << newtext;
        }
        
        last = it + searchString.size( );                
        it = original.find( searchString, last );
    }
    
    return ss.str( );
}