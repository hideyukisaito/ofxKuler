//
//  ofxKulerLoader.h
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/15/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#pragma once

#include <vector>

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "ofxKulerTheme.h"

class ofxKulerLoader: public ofThread {
	
public:
	
	ofxKulerLoader();
	~ofxKulerLoader();
	
	void loadFromURL( string _urlRequest );
	
    void setVerbose( bool newVerboseVal );
    
	std::vector<ofxKulerTheme>& getLoadedThemes();
    
    void clearLoadedThemes();
	
	typedef enum _loaderStatus
	{
		NO_STATUS,
		LOAD_STARTED,
		LOAD_COMPLETED,
		LOAD_FAILED,
	} loaderStatus;
	
	const loaderStatus getStatus();
	
protected:
	
	virtual void threadedFunction();
	
    static std::string stringReplace( const std::string &original, const std::string &searchString, const std::string &newtext );
    
    string						urlRequest;
	std::vector<ofxKulerTheme>	loadedThemes;
	
	loaderStatus				status;
    
    bool                        verbose;
	
};