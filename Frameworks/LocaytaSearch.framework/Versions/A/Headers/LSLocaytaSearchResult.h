//
//  LocaytaSearchResult.h
//  LocaytaSearch
//
//  Created by Chris Miles on 19/04/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 * \brief	Represents the result of a successful search query.
 *
 * Contains metadata about the query, such as how many documents were matched,
 * how many were returned, whether the search query was spell corrected, etc.
 *
 * The actual search results are available in the \a results array.
 */
@interface LSLocaytaSearchResult : NSObject {
    NSString *requestedQueryString;
    NSString *correctedQueryString;
    NSString *suggestedQueryString;
    NSSet *queryTerms;
    BOOL wasAutoSpellCorrected;
    
    NSInteger itemCount;
    NSInteger matchCount;
    BOOL matchCountExact;
    NSArray *results;
    
    NSSet *_rawQueryTerms;
    NSDictionary *_unstemMapping;
}

/**
 * \brief	The query string given to the search request.
 */
@property (nonatomic, copy, readonly) NSString *requestedQueryString;

/**
 * \brief	The query string actually used if the requested query was automatically spell corrected.
 *
 * \a wasAutoSpellCorrected will be YES if this property contains a useful value.
 */ 
@property (nonatomic, copy, readonly) NSString *correctedQueryString;

/**
 * \brief	Suggested query spelling if spell suggestions was requested.
 *
 */ 
@property (nonatomic, copy, readonly) NSString *suggestedQueryString;

/**
 * \brief	Set of all terms used to perform the search query.
 *
 */ 
@property (nonatomic, retain, readonly) NSSet *queryTerms;

/**
 * \brief	Only YES iff auto spell correction was requested and the requested query was corrected.
 *
 * The actual query used will be available in the \a correctedQueryString property.
 */ 
@property (nonatomic, assign, readonly) BOOL wasAutoSpellCorrected;

/**
 * \brief	The number of items returned in this result set.
 *
 */
@property (nonatomic, assign, readonly) NSInteger itemCount;

/**
 * \brief	The number of records matched by the search query.
 *
 * May be an estimation, see \a matchCountExact.
 */ 
@property (nonatomic, assign, readonly) NSInteger matchCount;

/**
 * \brief	If YES then \a matchCount is exact, otherwise \a matchCount is only an estimation (for performance reasons).
 *
 */ 
@property (nonatomic, assign, readonly) BOOL matchCountExact;

/**
 * \brief	An array of search results matching the search query.
 */
@property (nonatomic, retain, readonly) NSArray *results;

@end
