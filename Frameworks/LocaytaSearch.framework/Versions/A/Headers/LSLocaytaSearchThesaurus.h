//
//  LSLocaytaSearchThesaurus.h
//  LocaytaSearch
//
//  Created by Chris Miles on 24/08/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 \brief Class for managing synonyms.

 A synonym is a term that will be expanded to itself plus all its synonym
 terms at query time.  For example, a synonym of "color"="colour" will cause
 any search queries containing the term "color" to be expanded to also search
 for "colour".
 
 Synonym expansion happens automatically at query time if any query terms
 match a synonym key.

 Example:
\code
LSLocaytaSearchThesaurus *thesaurus = [[LSLocaytaSearchThesaurus alloc] initWithDatabasePath:databasePath];
NSDictionary *synonyms = [NSDictionary dictionaryWithObjectsAndKeys:
		[NSArray arrayWithObjects:@"term2", @"term3", nil],		@"term1",
		[NSArray arrayWithObjects:@"bar", nil],				@"foo",
		[NSArray arrayWithObjects:@"color", nil],			@"colour",
		nil];
[thesaurus addSynonyms:synonyms];
[thesaurus release];
\endcode

 */ 
@interface LSLocaytaSearchThesaurus : NSObject {
    NSString *databasePath;
}

/**
 * \brief	The full path to the search database directory.
 *
 */
@property (nonatomic, copy) NSString *databasePath;


/**
 * \brief	Initialise with the full path to an existing search database.
 *
 *	\a aDatabasePath must point to an existing search database directory.
 */
- (id)initWithDatabasePath:(NSString *)aDatabasePath;

/**
 * \brief	Add all synonym definitions from the provided NSDictionary.
 *
 *	\a synonyms an NSDictionary keyed by term (NSString) with an NSArray
 *	of synonym terms for each.
 *
 *	The search database is updated synchronously.
 */
- (void)addSynonyms:(NSDictionary *)synonyms;

/**
 * \brief	Add all synonym definitions from the provided NSArray.
 *
 *	\a synonymsArray an NSArray of NSArrays of the form
 *	("term", "syn1", "syn2", ...).
 *
 *	Performs the same function as -addSynonyms: with the convenience of
 *	defining synonyms by NSArray.
 *
 *	The search database is updated synchronously.
 */
- (void)addSynonymsFromArray:(NSArray *)synonymsArray;

/**
 * \brief	Return all synonyms defined in a search database.
 *
 */
- (NSDictionary *)getSynonyms;

/**
 * \brief	Clears all synonyms from a search database.
 *
 */
- (void)clearSynonyms;

@end
