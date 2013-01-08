//
//  LocaytaSearchQuery.h
//  LocaytaSearch
//
//  Created by Chris Miles on 19/04/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "LSLocaytaSearchError.h"

/**
 * \brief	The default operator to use when constructing a search query.
 *
 * Default is to find documents matching any search term in the query string
 * (e.g. "term1 OR term2 OR term3").
 */
typedef enum {
    LSLocaytaSearchQueryOperatorOr = 0,		// Find results containing any search terms
    LSLocaytaSearchQueryOperatorAnd,		// Find results containing all search terms
    LSLocaytaSearchQueryOperatorPhrase		// Find results containing all search terms in sequential order
} LSLocaytaSearchQueryOperator;


@class LSDatabaseReader;
@class LSQuery;


/**
 * \brief	A class representing a text search query with optional filters.
 *
 * Parses a query string into searchable terms, optionally applying any boolean
 * filters.  Instances of LSLocaytaSearchQuery are passed to LSLocaytaSearchRequest
 * to perform the actual search request.
 */ 
@interface LSLocaytaSearchQuery : NSObject {
    
    LSLocaytaSearchQueryOperator defaultOperator;
    NSDictionary *filters;
    NSString *queryString;
    
@private
    LSQuery *_query;
    NSString *_stemmingLanguage;
    LSDatabaseReader *_synonymsDatabaseReader;
    LSQuery *_text_query;
    NSDictionary *_unstemmedTerms;
    BOOL _query_is_boolean;
}

/**
 * \brief	The default operator to use when constructing a search query.
 *
 * Choices available are:
 *
 * \code
 * searchQuery.defaultOperator = LSLocaytaSearchQueryOperatorOr;
 * \endcode
 * Match documents containing any of the search terms (default).
 *
 * \code
 * searchQuery.defaultOperator = LSLocaytaSearchQueryOperatorAnd;
 * \endcode
 * Match only documents that contain all of the search terms.
 *
 * \code
 * searchQuery.defaultOperator = LSLocaytaSearchQueryOperatorPhrase;
 * \endcode
 * Match only documents that contain all of the search terms in sequential order.
 *
 */
@property (nonatomic, assign) LSLocaytaSearchQueryOperator defaultOperator;

/**
 * \brief	The query string to search for.
 *
 * The query string will be parsed into terms, which will be used to perform the
 * actual matching.  Matching documents are returned in order of relevance to the
 * terms of the query string, based on the probabalistic model.
 *
 * Terms will be reduced to their stemmed form (based on the English stemmer).
 */
@property (nonatomic, copy) NSString *queryString;


/**
 * \brief	Set the search filters.
 *
 * \a filters is a NSDictionary of boolean key/value pairs to filter the query result
 * by.  Filter keys must match a boolean setting specified at indexing time.
 *
 * Example:
 * \code
 * NSDictionary *searchFilters = [NSDictionary dictionaryWithObjectsAndKeys:
 *				[NSArray arrayWithObject:@"foo"], @"XXAA",
 *				[NSArray arrayWithObject:@"bar"], @"XXBB",
 *				nil];
 * searchQuery.filters = searchFilters;
 * \endcode
 */
@property (nonatomic, retain) NSDictionary *filters;


/**
 * \brief	Returns an autoreleased instance of LSLocaytaSearchQuery for the given \a queryString.
 *
 * This is a convenience method for [[[LSLocaytaSearchQuery alloc] initWithQueryString:queryString filters:nil] autorelease];
 */
+ (id)queryWithQueryString:(NSString *)aQueryString;

/**
 * \brief	Returns a new instance of LSLocaytaSearchQuery for the given \a queryString.
 *
 * This is a convenience method for
 * \code
 * -[LSLocaytaSearchQuery initWithQueryString:queryString filters:nil]
 * \endcode
 *
 * See initWithQueryString:filters:
 */
- (id)initWithQueryString:(NSString *)aQueryString;

/**
 * \brief	Returns a new instance of LSLocaytaSearchQuery for the given \a queryString and \a searchFilters.
 *
 * \a searchFilters is a NSDictionary of boolean key/value pairs to filter the query result
 * by.  Filter keys must match a boolean setting specified at indexing time.
 *
 * The \a queryString and \a searchFilters arguments cannot both be nil (or empty).
 *
 * Example:
 * \code
 * NSDictionary *searchFilters = [NSDictionary dictionaryWithObjectsAndKeys:
 *				[NSArray arrayWithObject:@"foo"], @"XXAA",
 *				[NSArray arrayWithObject:@"bar"], @"XXBB",
 *				nil];
 * LSLocaytaSearchQuery *searchQueryWithFilter = [[LSLocaytaSearchQuery alloc] initWithQueryString:@"my search query" filters:searchFilters];
 * \endcode
 */
- (id)initWithQueryString:(NSString *)aQueryString filters:(NSDictionary *)searchFilters;


/**
 * \brief	Returns a description of the parsed query.
 *
 */
- (NSString *)queryDescription;

@end
