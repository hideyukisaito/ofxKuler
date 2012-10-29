//
//  ofxKuler.cpp
//  ofxKuler
//
//  Created by Ali Nakipoğlu on 1/15/12.
//  Copyright (c) 2012 Ali Nakipoglu. All rights reserved.
//

#include <iostream>

#include "ofxKuler.h"

string ofxKuler::API_LIST_PATH			= "http://kuler-api.adobe.com/feeds/rss/get.cfm?";

string ofxKuler::ARGUMENT_LIST_TYPE		= "listType";
string ofxKuler::ARGUMENT_SEARCH_TYPE	= "searchQuery";
string ofxKuler::ARGUMENT_START_INDEX	= "startIndex";
string ofxKuler::ARGUMENT_TIME_SPAN		= "timeSpan";
string ofxKuler::ARGUMENT_ITEMS_PER_PAGE= "itemsPerPage";
string ofxKuler::ARGUMENT_API_KEY		= "key";

string ofxKuler::LISTING_MODE_RATING	= "rating";
string ofxKuler::LISTING_MODE_RANDOM	= "random";
string ofxKuler::LISTING_MODE_RECENTS	= "recent";
string ofxKuler::LISTING_MODE_POPULARS	= "popular";

string ofxKuler::SEARCH_MODE_THEME_ID	= "themeID";
string ofxKuler::SEARCH_MODE_USER_ID	= "userID";
string ofxKuler::SEARCH_MODE_EMAIL		= "email";
string ofxKuler::SEARCH_MODE_TAG		= "tag";
string ofxKuler::SEARCH_MODE_HEX		= "hex";
string ofxKuler::SEARCH_MODE_TITLE		= "title";

ofxKuler::ofxKuler()
{
}

ofxKuler::~ofxKuler()
{
    for( unsigned int i = 0; i < loadedThemes.size(); i++ )
    {
        delete loadedThemes[i];
    }
    
    loadedThemes.clear();
}

void ofxKuler::setAPIKey( const string& _apiKey )
{
	apiKey	= _apiKey;
}

const string ofxKuler::getAPIKey()
{
	return apiKey;
}

void ofxKuler::addListingRating( const int itemCount, const int startIndex, const int timeSpan )
{
	startLoadWithURLAndListingMode( LISTING_MODE_RATING, itemCount, startIndex, timeSpan );
}

void ofxKuler::addListingRandom( const int itemCount, const int startIndex, const int timeSpan )
{
	startLoadWithURLAndListingMode( LISTING_MODE_RANDOM, itemCount, startIndex, timeSpan );
}

void ofxKuler::addListingRecents( const int itemCount, const int startIndex, const int timeSpan )
{
	startLoadWithURLAndListingMode( LISTING_MODE_RECENTS, itemCount, startIndex, timeSpan );
}

void ofxKuler::addListingPopulars( const int itemCount, const int startIndex, const int timeSpan )
{
	startLoadWithURLAndListingMode( LISTING_MODE_POPULARS, itemCount, startIndex, timeSpan );
}

void ofxKuler::addSearching( const string& query, const int itemCount, const int startIndex)
{
	startLoadWithURLAndSearchMode( "", query, itemCount, startIndex );
}

void ofxKuler::addSearchingThemeID	( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_THEME_ID, query, itemCount, startIndex );
}

void ofxKuler::addSearchingUserID( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_USER_ID, query, itemCount, startIndex );
}

void ofxKuler::addSearchingEmail( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_EMAIL, query, itemCount, startIndex );
}

void ofxKuler::addSearchingTag( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_TAG, query, itemCount, startIndex );
}

void ofxKuler::addSearchingHex( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_HEX, query, itemCount, startIndex );
}

void ofxKuler::addSearchingTitle( const string& query, const int itemCount, const int startIndex )
{
	startLoadWithURLAndSearchMode( SEARCH_MODE_TITLE, query, itemCount, startIndex );
}

void ofxKuler::clearThemes()
{
	for ( std::vector<ofxKulerTheme*>::iterator it = loadedThemes.begin(); it != loadedThemes.end(); ++it )
	{
		delete *it;
	}
	
	loadedThemes.clear();
}

const std::vector<ofxKulerTheme*>& ofxKuler::getThemes()
{
	return loadedThemes;
}

bool ofxKuler::getIsLoading()
{
	return currentLoader.getStatus() == ofxKulerLoader::LOAD_STARTED;;
}

void ofxKuler::startLoadWithURLAndListingMode(const string listingMode, const int itemCount, const int startIndex, const int timeSpan)
{
	startNewLoadWithURL(	API_LIST_PATH
						  + ARGUMENT_LIST_TYPE		+ "=" + listingMode					+ "&"
						  + ARGUMENT_START_INDEX	+ "=" + ofToString( startIndex )	+ "&"
						  + ARGUMENT_TIME_SPAN		+ "=" + ofToString( timeSpan )		+ "&"
						  + ARGUMENT_ITEMS_PER_PAGE	+ "=" + ofToString( itemCount )		+ "&"
						  + ARGUMENT_API_KEY		+ "=" + apiKey
						  );

}

void ofxKuler::startLoadWithURLAndSearchMode(const string searchMode, const string query, const int itemCount, const int startIndex)
{
	string filteredSearchModeAndQuery	= searchMode + ":" + query;
	
	if ( searchMode.length() == 0 )
	{
		filteredSearchModeAndQuery		= query;
	}
	
	startNewLoadWithURL(	API_LIST_PATH
						  + ARGUMENT_SEARCH_TYPE	+ "=" + filteredSearchModeAndQuery	+ "&"
						  + ARGUMENT_START_INDEX	+ "=" + ofToString( startIndex )	+ "&"
						  + ARGUMENT_ITEMS_PER_PAGE	+ "=" + ofToString( itemCount )		+ "&"
						  + ARGUMENT_API_KEY		+ "=" + apiKey
						  );
}

void ofxKuler::startNewLoadWithURL( string url )
{
	if ( currentLoader.getStatus() != ofxKulerLoader::LOAD_STARTED )
	{				
        ofAddListener( ofEvents().update, this, &ofxKuler::handleOfUpdateEvent );
        
		currentLoader.loadFromURL( url );	
		checkLoaderStatus();
                
        ofLogVerbose( "ofKuler" , "Start To Load From Kuler Service" );
		ofLogVerbose( "ofKuler" , "Service URI: " + url  );
        
	} else {
        urlLoadQueue.push_back( url );
    }
}

void ofxKuler::loadNextRequestOnQueue()
{
    if ( urlLoadQueue.size() > 0 )
    {   
        startNewLoadWithURL( urlLoadQueue.back() );
        
        urlLoadQueue.erase( urlLoadQueue.end() - 1 );
    }
}

void ofxKuler::handleOfUpdateEvent( ofEventArgs &args )
{
    checkLoaderStatus();
}

void ofxKuler::checkLoaderStatus()
{    
    if ( currentLoader.getStatus() == ofxKulerLoader::LOAD_COMPLETED )
    {
        for (   std::vector<ofxKulerTheme>::const_iterator it = currentLoader.getLoadedThemes().begin();
             it != currentLoader.getLoadedThemes().end();
             ++ it )
        {
            loadedThemes.push_back( new ofxKulerTheme( *it ) );            
        }
        
		ofLogVerbose( "ofKuler" , "Service Load Completed" );
    }
    
    if( currentLoader.getStatus() == ofxKulerLoader::LOAD_FAILED )
    {        
        ofLogVerbose( "ofKuler" , "Service Load Failed" );
    }
    
    if( currentLoader.getStatus() != ofxKulerLoader::LOAD_STARTED )
    {
        currentLoader.clearLoadedThemes();
        ofRemoveListener( ofEvents().update, this, &ofxKuler::handleOfUpdateEvent );
        
        loadNextRequestOnQueue();
    }
}

