//
//  ofxKuler.h
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/15/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#pragma once

#include <vector>

#include "ofMain.h"
#include "ofEvents.h"

#include "ofxKulerTheme.h"
#include "ofxKulerLoader.h"

class ofxKuler {
	
public:
	
	ofxKuler();
	~ofxKuler();
	
	void setAPIKey( const string& _apiKey );
	const string getAPIKey();
	
	void addListingRating		( const int itemCount, const int startIndex = 0, const int timeSpan = 0 );
	void addListingRandom		( const int itemCount, const int startIndex = 0, const int timeSpan = 0 );
	void addListingRecents		( const int itemCount, const int startIndex = 0, const int timeSpan = 0 );
	void addListingPopulars		( const int itemCount, const int startIndex = 0, const int timeSpan = 0 );
	
	void addSearching			( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingThemeID	( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingUserID		( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingEmail		( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingTag		( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingHex		( const string& query, const int itemCount, const int startIndex = 0 );
	void addSearchingTitle		( const string& query, const int itemCount, const int startIndex = 0 );
		
	void clearThemes();
	
	bool getIsLoading();
	
	const std::vector<ofxKulerTheme*>& getThemes(); 
	
protected:
	
	static string API_LIST_PATH;
	
	static string ARGUMENT_LIST_TYPE;
	static string ARGUMENT_SEARCH_TYPE;
	static string ARGUMENT_START_INDEX;
	static string ARGUMENT_TIME_SPAN;
	static string ARGUMENT_ITEMS_PER_PAGE;
	static string ARGUMENT_API_KEY;
	
	static string LISTING_MODE_RATING;
	static string LISTING_MODE_RANDOM;
	static string LISTING_MODE_RECENTS;
	static string LISTING_MODE_POPULARS;
	
	static string SEARCH_MODE_THEME_ID;
	static string SEARCH_MODE_USER_ID;
	static string SEARCH_MODE_EMAIL;
	static string SEARCH_MODE_TAG;
	static string SEARCH_MODE_HEX;
	static string SEARCH_MODE_TITLE;
		
	void startLoadWithURLAndListingMode(const string listingMode, const int itemCount, const int startIndex, const int timeSpan);
	void startLoadWithURLAndSearchMode(const string searchMode, const string query, const int itemCount, const int startIndex);
	
	void startNewLoadWithURL( string url );
    
    void loadNextRequestOnQueue();
    
    void handleOfUpdateEvent( ofEventArgs &args );
	void checkLoaderStatus();
	
	std::vector<ofxKulerTheme*>		loadedThemes;
    
    std::vector<string>             urlLoadQueue;
    
	ofxKulerLoader					currentLoader;
    
	string							apiKey;
    
};