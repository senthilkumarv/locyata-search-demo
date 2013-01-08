//
//  LocaytaSearchRequest.h
//  LocaytaSearch
//
//  Created by Chris Miles on 19/04/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * \brief	Search request spelling correction methods.
 */
typedef enum {
    LSLocaytaSearchRequestSpellCorrectionMethodNone = 0,	// No spell correction (default)
    LSLocaytaSearchRequestSpellCorrectionMethodSuggest,		// Suggest spell corrections with result
    LSLocaytaSearchRequestSpellCorrectionMethodAuto		// Automatically spell correct unknown terms
} LSLocaytaSearchRequestSpellCorrectionMethod;


@protocol LSLocaytaSearchRequestDelegate;
@class LSLocaytaSearchQuery;
@class LSLocaytaSearchResult;
@class LSSearchRequestHandler;


#pragma mark -
#pragma mark LSLocaytaSearchRequest

/**
 * \brief Class for performing search query requests.
 * 
 * A search request is performed by passing an instance of LSLocaytaSearchQuery to it,
 * with some optional parameters.  Search requests are performed asynchronously (in a
 * background thread) with the delegate notified when the request has completed via
 * the LSLocaytaSearchRequestDelegate protocol.
 *
 * Example:
 * \code
 * LSLocaytaSearchQuery *searchQuery = [LSLocaytaSearchQuery queryWithQueryString:@"test search"];
 *
 * LSLocaytaSearchRequest *searchRequest = [[LSLocaytaSearchRequest alloc] initWithDatabasePath:databasePath delegate:self];
 * searchRequest.spellCorrectionMethod = LSLocaytaSearchRequestSpellCorrectionMethodAuto;
 *
 * [searchRequest searchWithQuery:searchQuery topDocIndex:0 docsPerPage:20];
 * // query will be performed asynchronously on background thread and call delegate method when complete
 *
 * [searchRequest release];
 * // No need to keep reference to LSLocaytaSearchRequest object, it will be retained internally
 * // until the search request has completed and delegate called.
 *
 * \endcode
 */

@interface LSLocaytaSearchRequest : NSObject {
    NSString *databasePath;
    id<LSLocaytaSearchRequestDelegate> delegate;
    BOOL searchRequestInProgress;
    NSArray *sortOrder;
    LSLocaytaSearchRequestSpellCorrectionMethod	spellCorrectionMethod;
    NSString * stemmingLanguage;
    
@private
    BOOL cancelled;
}

/** \brief	The absolute path to the search database directory.
 * 
 */
@property (nonatomic, copy) NSString *databasePath;

/** \brief	The delegate object that should implement LSLocaytaSearchRequestDelegate protocol methods.
 * 
 */
@property (nonatomic, assign) id<LSLocaytaSearchRequestDelegate> delegate;

/** \brief	Returns YES if a search request is currently in progress.
 * 
 */
@property (nonatomic, readonly) BOOL searchRequestInProgress;

/** \brief	Set the method to use for spell correction.
 *
 * Enable spell correction by setting the method to use. By default, no spell correction is performed.
 *
 * Spell correction is a mechanism for finding the closest matching term to any term in the
 * search query that dooes not exist in a search database.
 *
 * For example, a search for "watar": if the term "watar" does not exist in a search database
 * but "water" does, then spell correction would replace "watar" with "water" to guarantee
 * some search results.
 *
 * Possible methods are:
 *
 * \code
 * searchRequest.spellCorrectionMethod = LSLocaytaSearchRequestSpellCorrectionMethodNone
 * \endcode
 * No spell correction (default).
 *
 * \code
 * searchRequest.spellCorrectionMethod = LSLocaytaSearchRequestSpellCorrectionMethodSuggest
 * \endcode
 * Suggest spelling corrections. The search is performed using the given query string. If
 * any spell corrections are available they are returned along with the search results.  Use this
 * method to suggest better search terms to users without modifying their original query.
 * For example: "Your search for 'watar bottles' returned 2 results. Did you mean 'water bottles'?".
 *
 * \code
 * searchRequest.spellCorrectionMethod = LSLocaytaSearchRequestSpellCorrectionMethodAuto
 * \endcode
 * Spelling corrections are applied automatically if any are available. This means that the
 * actual search terms used for the query may be different to those requested. The spell
 * corrected query will be returned as part of the result set. For example, a search query for
 * "watar bottles" would be automatically changed to "water bottles" if the term "watar" did not
 * exist in the search database but "water" did.
 */
@property (nonatomic, assign) LSLocaytaSearchRequestSpellCorrectionMethod spellCorrectionMethod;

/** \brief	Customise the sort order of results returned.
 *
 * By default, results are returned sorted by relevancy to the search query, with most
 * relevant results first.  The sort order can be customised to sort results by text or
 * numeric fields in either ascending or descending order.  To sort by a field the field
 * values must be stored in either a "text slot" (for alphanumeric, case-insensitive sorting) or
 * a "numeric slot" (for numeric sorting) by specifying either "textslot" or "numericslot"
 * in the indexing schema for the field.  Each slot is assigned a positive integer number (1+)
 * which is referred to at query time.
 *
 * To customise the results sort order, specify an NSArray of one or more slot numbers.
 * Each slot number is specified by an NSString, prefixed with either "-" for descending
 * or "+" for ascending.  Also, NSNull is used to specify sorting by relevance.  Results
 * will be sorted by the first slot specified in the array.  If there are any ties, then
 * the second slot in the array (if specified) is used and so on.
 *
 * Example - sort by values in slot #1 in ascending order:
 * \code
 * NSArray *sortOrder = [NSArray arrayWithObject:@"+1"];
 * searchRequest.sortOrder = sortOrder;
 * \endcode
 *
 * Example - sort by values in slot #1 in ascending order, followed by slot #5 in
 * descending order, followed by relevancy:
 * \code
 * NSArray *sortOrder = [NSArray arrayWithObjects:@"+1", @"-5", [NSNull null], nil];
 * searchRequest.sortOrder = sortOrder;
 * \endcode
 *
 * Example - default sort order (by relevancy):
 * \code
 * searchRequest.sortOrder = nil;
 * \endcode
 */
@property (nonatomic, retain) NSArray *sortOrder;

/**
 * \brief	The language to use for stemming words.
 *
 *	Word stemming is language dependent. Specify one of the languages below to choose
 *	a stemmer or set to nil to disable stemmed term generation. Stemming defaults to
 *	"english". The stemmer should match the language used at index time.
 *
 *	    - danish
 *	    - dutch
 *	    - english
 *	    - finnish
 *	    - french
 *	    - german
 *	    - hungarian
 *	    - italian
 *	    - norwegian
 *	    - portuguese
 *	    - russian
 *	    - spanish
 *	    - swedish
 */
@property (nonatomic, copy) NSString *stemmingLanguage;

/** \brief	Initialise with path to a search database and delegate object.
 *
 * A database path should be the full directory path of an existing search database.
 *
 * The delegate is optional and will be called with the result of search query requests.
 */
- (id)initWithDatabasePath:(NSString *)aDatabasePath delegate:(id<LSLocaytaSearchRequestDelegate>)delegate;

/**
 * \brief Performs a search request asynchronously, requesting the top 10 results.
 * 
 * On Completion calls one of the LocaytaSearchRequestDelegate methods on the delegate.
 *
 * This is a convenience method for \code [searchRequest searchWithQuery:searchQuery topDocIndex:0 docsPerPage:10] \endcode
 */
- (void)searchWithQuery:(LSLocaytaSearchQuery *)query;

/**
 * \brief Performs a search request asynchronously, requesting a specified range of results.
 * 
 * A search request is performed with up to docsPerPage results returned beginning from topDocIndex offset.
 * On Completion calls one of the LSLocaytaSearchRequestDelegate methods on the delegate.
 *
 * For example:
 * \code
 *  
 *  LSLocaytaSearchRequest *searchRequest = [[LSLocaytaSearchRequest alloc] initWithDatabasePath:databasePath delegate:self];
 *  LSLocaytaSearchQuery *searchQuery = [LSLocaytaSearchQuery queryWithQueryString:@"search query"];
 *  
 *  // Fetch results 41-60 (i.e. third "page" of results)
 *  [searchRequest searchWithQuery:searchQuery topDocIndex:40 docsPerPage:20];
 *  [searchRequest release];
 * 
 * \endcode
 */
- (void)searchWithQuery:(LSLocaytaSearchQuery *)query topDocIndex:(NSInteger)topDocIndex docsPerPage:(NSInteger)docsPerPage;

/** \brief	Returns the total count of documents in the search database.
 *
 */
- (unsigned long long int)documentCount;

/**
 * \brief	Cancel a search request.
 *
 * If a search request is currently being performed it will be terminated. No delegate methods
 * will be called.
 */
- (void)cancel;

/**
 * \brief	Fetch character ranges of query terms within the specified document field.
 *
 * The character ranges of any query terms used to produce searchResult that matched
 * terms within the specified document field are returned.  This includes the ranges
 * of terms that were matched by stemming or symonym expansion, that may not otherwise
 * be obvious.
 *
 * An optional callback object+selector can be provided which can be used to cancel
 * the request. The callback method is expected to take no arguments and should return
 * a BOOL.  Return NO if you want the fetch char ranges request to immediately abort,
 * other return YES to allow it to continue.  The callback is called periodically
 * during a fetch char ranges request.
 *
 * Note: this is a synchronous request. It will be slower for larger field values
 * and/or larger query term sets.
 */
- (NSDictionary *)fetchCharRangesForQueryTermsInSearchResult:(LSLocaytaSearchResult *)searchResult inField:(NSString *)fieldName docID:(long long int)docID callbackObject:(id)callbackObject callbackSelector:(SEL)callbackSelector;

/**
 * \brief	Return words prefixed by the specified word fragment.
 *
 * This method suggests the best word completions for the given fragment,
 * based on the frequency of existing terms in the search index.
 *
 * For example, a request with fragment "for" might return suggested words:
 * "for", "forced", "form", "forecast", "foresee", "forgets", "forcing".
 *
 * Note: this is a synchronous request.
 */
- (NSArray *)suggestionsForWordFragment:(NSString *)fragment maxSuggestions:(int)maxSuggestions;

@end


#pragma mark -
#pragma mark LocaytaSearchRequestDelegate

/**
 * \brief Protocol for the LSLocaytaSearchRequest delegate object.
 *
 * This protocol defines the methods that may be called by a LSLocaytaSearchQuery search request.
 */

@protocol LSLocaytaSearchRequestDelegate<NSObject>

/**
 * \brief	Method that will be called when a search request completes without error.
 *
 * A search query that returns zero results (i.e. no documents found) is not considered an error
 * and will result in this method being called.
 *
 * The LSLocaytaSearchRequest object performing the request is passed as the \a searchRequest argument.
 *
 * The result of the query is represented by a LSLocaytaSearchResult object passed as the \a searchResult argument.
 */
- (void)locaytaSearchRequest:(LSLocaytaSearchRequest *)searchRequest didCompleteWithResult:(LSLocaytaSearchResult *)searchResult;

/**
 * \brief	Method that will be called when a search request fails for any reason.
 *
 * This method will only be called if the search request could be performed.  A search query that
 * results in no matches is not considered an error.
 *
 * The LSLocaytaSearchRequest object performing the request is passed as the \a searchRequest argument.
 *
 * Details of the failure will be passed as the \a error argument.
 */
- (void)locaytaSearchRequest:(LSLocaytaSearchRequest *)searchRequest didFailWithError:(NSError *)error;

@end
