/*
 *  LocaytaSearch.h
 *  LocaytaSearch
 *
 *  Created by Chris Miles on 20/04/10.
 *  Copyright 2010-2011 Locayta Limited. All rights reserved.
 *
 */

/*! \mainpage Locayta Search Mobile for iOS - API Documentation

	 Locayta Search Mobile for iOS is a framework for embedding a full text
	 search engine within Apple iOS (iPhone, iPod Touch and iPad) applications.

	 It allows you to index any number of segments of text or data ("field values") for
	 each item you wish to search ("document"), then search across all the
	 documents quickly.

	 The following simplified code will create and index a document (see \subpage search_updates for full example including error checking and memory management):

\code
	LSLocaytaSearchIndexer *searchIndexer = [[LSLocaytaSearchIndexer alloc] initWithDatabasePath:databasePath delegate:self];
	NSDictionary *searchSchema = [[NSDictionary alloc] initWithContentsOfFile:schemaFile];
	LSLocaytaSearchIndexableRecord *indexableRecord = [[LSLocaytaSearchIndexableRecord alloc] initWithSchema:searchSchema];
	
	[indexableRecord addValue:@"document01" forField:@"id" error:&error]
	[indexableRecord addValue:@"my document title" forField:@"title" error:&error]
	[indexableRecord addValue:@"the quick brown fox jumps over the lazy dog" forField:@"content" error:&error]
	[searchIndexer addOrReplaceRecord:indexableRecord]; // Asynchronous with callback
\endcode
	
	The following code will search the index for a user-provided query string (see \subpage search_query for full example):

\code
	NSString *searchDatabasePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"my_search_db"];
	LSLocaytaSearchRequest *searchRequest = [[LSLocaytaSearchRequest alloc] initWithDatabasePath:searchDatabasePath delegate:self];
	LSLocaytaSearchQuery *searchQuery = [LSLocaytaSearchQuery queryWithQueryString:@"quick brown fox"];
	[searchRequest searchWithQuery:searchQuery topDocIndex:0 docsPerPage:50]; // Asynchronous with callback
\endcode	

	 This manual is divided into the following sections:
	 - \subpage intro 
	 - \subpage install
	 - \subpage locindex
	 - \subpage search_updates
	 - \subpage schema
	 - \subpage search_query
	 - \subpage locthesaurus
	 - \subpage glossary
 

 \page intro Introduction

	 Locayta Search Mobile for iOS is a framework for embedding a full text
	 search engine within Apple iOS (iPhone, iPod Touch and iPad) applications.
	 
	 Locayta Search Mobile for iOS is provided as a static library wrapped
	 in a framework, making for easy integration with iOS applications.  An Objective-C
	 API provides high level access to the search library, the core of which is written
	 in C/C++.
	 
	 Locayta Search works by building an index of documents to be searched, stored in
	 a "search database". Physically the search database is a directory or one or more index files,
	 but is treated as a single entity.
	 
	 Search databases can be created and updated within an iOS application on the device
	 itself; or created in advance on a desktop computer using the supplied OS X
	 command-line utility, "locindex".
	 
	 Applications that want user created data to be searchable would create and update the
	 search database on the fly.  Some examples might be: note taking app; task management app;
	 CRM app; word processing app.
	 
	 Applications that come bundled with a resource of information may include the pre-built
	 search database along with the application. In this case the search database can be
	 created on a desktop computer and added to the application resources in XCode.  Some
	 examples might be: digital magazine; story book app; technical reference manual;
	 subscription-based reports access.
	 
	 This documentation provides details for using the Locayta Search Mobile framework in iOS projects.
	 The use of the latest public Apple iOS SDK is assumed.
 

 \page install Installation

	 Copy "LocaytaSearch.framework" to somewhere useful, such as the project directory or
	 ~/Library/Frameworks/ .
	 
	 Open XCode 4 for your iOS project and select your Project Target. In the Target view
	 select "Build Phases", expand "Link Binary With Libraries", and select "+" (Add Items).
	 Select "Add Other..." and browse to and select "LocaytaSearch.framework".
	 
	 Add other linker flags to the build configuration:
 
\verbatim
"Other Linker Flags": "-ObjC -lstdc++"
\endverbatim

	 Do this by selecting the Project (or Target) and selecting the "Build Settings" tab.
	 Find the "Other Linker Flags" setting.  Add both flags "-ObjC", "-lstdc++" to this setting.

	 In the source file(s) where you will implement the LocaytaSearch integration, import the
	 header:
 
\code
#import <LocaytaSearch/LSLocaytaSearch.h>
\endcode

 

 \page locindex Creating Search Databases With locindex
 
	 Search databases can be created on a desktop computer using the "locindex" command-line
	 utility for OS X.
	 
	 For example:
 
 \code
 $ locindex -v -o my_search_db my_search_schema.plist my_searchable_text.data
 Schema loaded from "my_search_schema.plist"
 Created search database at 'my_search_db'
 Added 322 documents
 \endcode

	 Which will create a directory called "my_search_db" containing index files.
	 
	 "my_search_schema.plist" is a Property List file specifying the indexing strategy
	 for each of the data fields.  Any fields that are to be indexed should be specified
	 in the schema.  See \subpage schema.  An example of the schema plist file is:
 
 \code
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>content</key>
	<dict>
		<key>field</key>
		<true/>
		<key>index</key>
		<true/>
		<key>spell</key>
		<true/>
	</dict>
	<key>title</key>
	<dict>
		<key>field</key>
		<true/>
		<key>weight</key>
		<integer>5</integer>
		<key>index</key>
		<true/>
		<key>spell</key>
		<true/>
	</dict>
	<key>filename</key>
	<dict>
		<key>field</key>
		<true/>
		<key>id</key>
		<true/>
	</dict>
</dict>
</plist>
 \endcode
 
	 which can be edited using Property List Editor for OS X or XCode.
	 
	 The input data file is a simple text file containing documents to be indexed separated by
	 blank lines.  Each document is specified by one or more "field=value" lines.  For example:
 
\verbatim
filename=document1.txt
title=The first document
content=This is an example of a document that will be searchable.

filename=document2.txt
title=The second document
content=Another document that you can search for.

filename=document3.txt
title=The third document
content=This is an example of a field that is
=spread over
=multiple
=lines.
\endverbatim
 
	 A field value can be spread over multiple lines by ensuring that "=" is the very first
	 character of each line after the first.
 
	 locindex can be run repeatedly on the same search database to incrementally add documents
	 to an index.
 
	 A search database created by locindex can be added to an app in development by adding
	 the search database directory to the XCode Resources for the project.  The pre-built
	 search database will then be bundled up with the application.
 
	 Search databases can also be created and updated within an app via API calls. See
	 \subpage search_updates.
 
 
 \page locthesaurus Managing Synonyms With locthesaurus
 
 Synonyms can be defined for search databases by using the "locthesaurus" command-line
 utility for OS X.  Any synonyms in a search database will be automatically expanded
 at query time.
 
 For example:
 
\code
$ cat synonyms.csv 
color,colour
$ locthesaurus -v --clear poems_db synonyms.csv 
Clearing synonyms
Inserting synonyms from synonyms.csv
$ locthesaurus --display poems_db
color, colour
\endcode

 For this example, any search query containing the term "color" would be automatically
 expanded to also search for the term "colour".
 
 Synonym expansion is one-way only. For example, if you want both directions you would
 need to specify "colour,color" and "color,colour".
 
 Synonyms can also be added and removed directly via the API. See LSLocaytaSearchThesaurus.

 
 \page search_updates Creating & Updating Search Databases via API

	Search database indexing is managed by the LSLocaytaSearchIndexer class.  It provides a simple API
	to add, update or delete records from the search database.  All indexing operations are performed
	asynchronously (in a background thread) and a delegate object (implementing the
	LSLocaytaSearchIndexerDelegate protocol) can be notified as operations complete (or fail).

	A record to be indexed (or deleted) is represented by an instance of the
	LSLocaytaSearchIndexableRecord class.  LSLocaytaSearchIndexableRecord objects are initialised
	with a schema (usually loaded from a Property List file) - see \subpage schema.
	LSLocaytaSearchIndexableRecord ensures that fields are valid with respect to the schema.

	A simple example of adding a record to a search database, followed by deleting another record
	from the search database, is shown below.
 
\code
- (void)searchDatabaseIndexingDemonstration {
	NSError *error = nil;
 
	LSLocaytaSearchIndexer *searchIndexer = [[LSLocaytaSearchIndexer alloc] initWithDatabasePath:databasePath delegate:self];
	NSString *schemaFile = @"my_search_schema.plist";
	NSDictionary *searchSchema = [[NSDictionary alloc] initWithContentsOfFile:schemaFile];

	// Add (or update) a record in the search database
	LSLocaytaSearchIndexableRecord *indexableRecord = [[LSLocaytaSearchIndexableRecord alloc] initWithSchema:searchSchema];
	NSString *uniqueID = @"a001";

	if (![indexableRecord addValue:uniqueID forField:@"id" error:&error]) {
		NSLog([error localizedDescription]);	// handle error
	}
	if (![indexableRecord addValue:someTitleString forField:@"title" error:&error]) {
		NSLog([error localizedDescription]);	// handle error
	}
	if (![indexableRecord addValue:someContentString forField:@"content" error:&error]) {
		NSLog([error localizedDescription]);	// handle error
	}

	[searchIndexer addOrReplaceRecord:indexableRecord];
	// returns immediately as operation is performed asynchronously. A delegate method will be called when completed (or failed).

	[indexableRecord release];


	// multiple records can be batch indexed by passing an NSArray of indexable records to addOrReplaceRecords
	[searchIndexer addOrReplaceRecords:arrayOfIndexableRecords];
	// returns immediately as operation is performed asynchronously. A delegate method will be called when completed (or failed).
 

	// Delete a record from the search database
	indexableRecord = [[LSLocaytaSearchIndexableRecord alloc] initWithSchema:searchSchema];
	uniqueID = @"123456";
	if (![indexableRecord addValue:uniqueID forField:@"id" error:&error]) {
		NSLog([error localizedDescription]);	// handle error
	}

	[searchIndexer deleteRecord:indexableRecord];
	// returns immediately as operation is performed asynchronously. A delegate method will be called when completed (or failed).

	[indexableRecord release];

	[searchSchema release];
	[searchIndexer release];
}

#pragma mark -
#pragma mark LSLocaytaSearchIndexerDelegate methods

- (void)locaytaSearchIndexer:(LSLocaytaSearchIndexer *)searchIndexer didUpdateWithIndexableRecords:(NSArray *)indexableRecords {
	DLog(@"successfully indexed records: %@", indexableRecords);
}

- (void)locaytaSearchIndexer:(LSLocaytaSearchIndexer *)searchIndexer didFailToUpdateWithIndexableRecords:(NSArray *)indexableRecords error:(NSError *)error {
	DLog(@"failed to indexed records: %@ : %@", indexableRecords, error);
}

\endcode


 \page schema Schema
 
	The schema instructs the indexer how to process each field of a document.  The indexing API expects
	a NSDictionary tree structure, but this can be more easily defined by a Property List (plist) file
	and loaded into an NSDictionary using:

\code
NSDictionary *searchSchema = [[NSDictionary alloc] initWithContentsOfFile:schemaPlistFile];
\endcode
 
	The "locindex" command-line indexing utility also reads the schema from a Property List file, specified
	as one of its arguments.  See \ref locindex.
 
 \section field_options Field Options
 
	A schema definition should contain an entry for each field present in the documents to be indexed.
	For each field, one or more options will be specified.  The currently available options are listed
	below. More options may be added over time as functionality is added to the search engine.
 
	 - id (bool) : field will be used as a unique identifer. An id field is not required but is usually recommended
	 so that indexed documents can be referenced for update or deletion.
	 
	 - index (bool) : the field contents will be indexed so that they are searchable. Hence, a field not
	 configured with 'index' is not searchable.  A field is indexed by splitting
	 its contents into terms - typically all the "words" in the text separated by white space.  Terms are
	 reduced to their stemmed form based on the currently active stemmer (English by default) and stored in
	 the search database so that they can be matched against search queries.
	 
	 - weight (integer) : configures the weight (importance) of an indexed field (only relevant if 'index' is also enabled).
	 By default fields are indexed with a relative weighting of 1.  Specify a higher weight value to
	 increase the "importance" of the field.
	 
	 - spell (bool) : enable indexing of spell correction information for the field. Only fields indexed with 'spell'
	 enabled will be available for spell correction at query time. A field enabled for 'spell' will have trigrams
	 generated of all its terms and stored in the search database.  At query time, spell correction is achieved by a
	 combination of trigram analysis and edit distance.
	 
	 - field (bool) : stores the field contents allowing for retrieval as part of search results. If you store the
	 contents of your documents outside of the search database (e.g. in Core Data or files on disk) then you
	 probably wouldn't enable this option.  Use this option if you need the original contents of the field returned
	 with search results.
	 
	 - boolean (bool or string) : this option enables the field value to be used for filtering results at
	 query time. It can be enabled in conjunction with any other options. If set to YES (as a bool value) the
	 filter key defaults to the field name. To customise the filter key, specify it as a string value. Filter
	 keys must be alphanumeric characters only, with a minimum of 2 characters. Fields can share the same
	 boolean filter key, if desired, to filter by values across multiple fields.

	 - textslot (integer) : slot number to store values in for text (alphanumeric, case-insensitive) sorting. See below for details about slot numbers.
 
	 - numericslot (integer) : slot number to store values in for numeric sorting. See below for details about slot numbers.
	
	Slot numbers will usually map one-to-one on field names, but occasionally there are circumstances where one field will need more than one slot. Slot numbers are shared between textslots and numericslots and using the same slot number for both will lead to undefined behaviour.

 \section example Example Schema
 
	An example of a schema property list file is shown below.
 
 \code
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>content</key>
	<dict>
		<key>field</key>
		<true/>
		<key>index</key>
		<true/>
		<key>spell</key>
		<true/>
	</dict>
	<key>title</key>
	<dict>
		<key>field</key>
		<true/>
		<key>weight</key>
		<integer>5</integer>
		<key>index</key>
		<true/>
		<key>spell</key>
		<true/>
		<key>textslot</key>
		<integer>1</integer>
	</dict>
	<key>filename</key>
	<dict>
		<key>field</key>
		<true/>
		<key>id</key>
		<true/>
	</dict>
	<key>category</key>
	<dict>
		<key>field</key>
		<true/>
		<key>boolean</key>
		<true/>
	</dict>
 	<key>tag1</key>
	<dict>
		<key>field</key>
		<true/>
		<key>boolean</key>
		<string>tag</string>
	</dict>
	<key>tag2</key>
	<dict>
		<key>field</key>
		<true/>
		<key>boolean</key>
		<string>tag</string>
	</dict>
</dict>
</plist>
 \endcode

  
 \page search_query Performing Search Queries
 
	A search query is made using an LSLocaytaSearchRequest object. This object is initialised with the
	full path to the search database and, optionally, a delegate implementing the LSLocaytaSearchRequestDelegate
	protocol.
 
	The spell correction method can be chosen by assigning the LSLocaytaSearchRequest.spellCorrectionMethod property.
	By default, no spell correction is performed.

	By default, results are returned in order of relevancy.  The sort order can be customised by assigning
	the LSLocaytaSearchRequest.sortOrder property.
 
	A search query is requested by calling one of the searchWithQuery methods, passing it a
	LSLocaytaSearchQuery object.  The call will return immediately and the actual search query is
	performed asynchronously (in a background thread).  When completed, one of the 
	LSLocaytaSearchRequestDelegate methods will be called on the delegate.
 
	A LSLocaytaSearchQuery object is initialised with a combination of a query string and boolean
	filters.  Either of the query string or boolean filters can be unspecified, but not both.
 
	Boolean filters are defined by a NSDictionary keyed by filter keys (matching the boolean filter keys defined
	in the \subpage schema) with string values (or arrays of string values) to filter on.
 
	Example query with filters:
 
\code
NSDictionary *searchFilters = [NSDictionary dictionaryWithObjectsAndKeys:
			@"popular", @"category",
			@"foo", @"tag",
			nil];
LSLocaytaSearchQuery *searchQueryWithFilter = [[LSLocaytaSearchQuery alloc] initWithQueryString:@"my search query" filters:searchFilters];
\endcode

 
	\section search_results Search Results
 
	Search results are delivered as a LSLocaytaSearchResult object. This object provides:
 
	- the total count of documents that matched the query (which could be zero) and whether the match
		count is exact or just an approximation (which is possible for larger match counts due to
		optimisations);
	- the count of documents actually returned (typically much lower than the total match count, just
		enough to display a "page" of results);
	- the query string used to perform the search, as well as an indication whether the query string
		was spell corrected (if requested) and what the spell corrected query was;
	- the actual results as a NSArray of NSDictionary objects, each representing a document matching
		the search query. By default, results are returned in order of relevance.
 
	Each result in the results array is a NSDictionary containing the following keys:

	- docid (NSNumber) : internal reference number of the document (this is not the user-defined ID value).
	- weight (NSNumber) : the relevance of the result with respect to the search query, between 0.0 and 1.0.
	- fields (NSDictionary) : a dictionary of fieldname/array pairs from the stored record. These are all the fields
		that were indexed with the "field" indexing option (see \subpage schema).  Each field value is an NSArray of one or more
		NSStrings.  Multiple strings are returned when a field is specified more than once in a record at
		index time.

	Example of a search result:

\code
searchResult.results: (
        {
        docid = 1;
        fields =         {
            "cost_dollar" = (
				"1200"
			);
            "country" = (
				"UK"
			);
            "id" = (
				"ABCDB20099-0"
			);
            "title" = (
				"Industry Report - UK - May 2008"
			);
        };
        weight = 0.3774427;
    }
)
\endcode

 
	\section example_search_query Search Query Example
 
\code
- (void)searchQueryDemonstration {
	NSString *queryText = @"my query string";

	NSString *searchDatabasePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"my_search_db"];
	LSLocaytaSearchRequest *searchRequest = [[LSLocaytaSearchRequest alloc] initWithDatabasePath:searchDatabasePath delegate:self];
	searchRequest.spellCorrectionMethod = LSLocaytaSearchRequestSpellCorrectionMethodAuto;

	LSLocaytaSearchQuery *searchQuery = [LSLocaytaSearchQuery queryWithQueryString:queryText];
	[searchRequest searchWithQuery:searchQuery topDocIndex:0 docsPerPage:50];
	// request will be performed asynchronously and a LSLocaytaSearchRequestDelegate method called when complete.

	[searchRequest release];
}


#pragma mark -
#pragma mark LSLocaytaSearchRequestDelegate methods

- (void)locaytaSearchRequest:(LSLocaytaSearchRequest *)searchRequest didCompleteWithResult:(LSLocaytaSearchResult *)searchResult {
	NSLog(@"locaytaSearchRequest: %@ didCompleteWithResult: %@ matches=%d", searchRequest, searchResult, searchResult.matchCount);
}

- (void)locaytaSearchRequest:(LSLocaytaSearchRequest *)searchRequest didFailWithError:(NSError *)error {
	NSLog(@"locaytaSearchRequest: %@ didFailWithError: %@", searchRequest, error);
}

\endcode

\page glossary Glossary

	This list provides definitions for common terms used in Locayta Search Mobile.

	 - <b>Boolean</b>
	    - A boolean term is text which is indexed specially to allow results to be
	      filtered by that term. For instance, if an index \subpage schema contains
	      a "colour" field, values for colour can be indexed as booleans to allow
	      results to be filtered to only show results with a certain colour.\n
	      \n
	      Boolean terms are not found by users entering those terms in the search
	      box. If this behaviour is desired, they should also be indexed normally -
	      ie, the Schema should also contain the "index" option for that field.
	      \n
	      Boolean terms are indexed by boolean keys (defined in the \subpage schema)
              which default to field names.
	 - <b>Document</b>
	    - A searchable entity in the index. This is whatever data type you wish
	      the search engine to present as each result. For example: products,
	      encyclopaedia entries, articles, paragraphs from a book, journal entries,
	      and so forth.
	 - <b>Field</b>
	    - Each document can contain a number of fields. The primary use for fields
	      in Locayta Search Mobile is to allow different indexing options to be
	      provided for each field. For example, a title field could be weighted
	      as more important than a description field, and a price field could be
	      assigned to a sortable slot number.
	 - <b>Index</b>
	    - The storage mechanism used by Locayta Search Mobile. Currently this
	      is a directory containing several files, but the API abstracts this
	      down to just requiring a path to the index.\n
	      \n
	      The index contains information linking search terms to the documents
	      they appear in, and other information such as spelling information
	      and sort keys.
	 - <b>Slot</b>
	    - An area of the index where data is stored, primarily for sorting results.\n
	      \n
	      Usually slot numbers will have a one-to-one mapping to field names,
	      although there might be times when more than one slot is wanted. Locayta
	      allows data to be inserted into slots in two formats: as numbers, to
	      allow numeric sorting; or as text, to allow alphanumeric sorting.
	 - <b>Stemming</b>
	    - Stemming is a process where each word in the indexed text is reduced
	      to its stemmed form. The stemmer used varies by which language is set,
	      but for example in English, 'fish', 'fishing', 'fishes', and 'fisher'
	      all stem to 'fish'. The most obvious benefit this gives to users of the
	      search engine is that it doesn't matter if the user searches for the
	      plural or singular version of a noun.
	 - <b>Trigrams</b>
	    - Spelling in Locayta Search Mobile is performed using a system of breaking
	      words into groups of three letters, which allows several similarly-spelled
	      words to be retrieved. The spelling correction algorithm then picks
	      the closest word by edit distance, with the number of times the word
	      appears in the index used as a tie-breaker.
	 - <b>Weight</b>
	    - If a field in the index \subpage schema contains a weighting higher than
	      1, the effect is that each term indexed for that field is considered that
	      many times more important to the searching algorithm.\n
	      \n
	      For example, if a title field was weighted higher than a description
	      field, and a search returned two documents that contained the user's
	      search word in each respective field, then the document which had the
	      user's word in its title would likely be returned first in the results.

 */

#import "LSLocaytaSearchError.h"

#import "LSLocaytaSearchQuery.h"
#import "LSLocaytaSearchRequest.h"
#import "LSLocaytaSearchResult.h"

#import "LSLocaytaSearchIndexableRecord.h"
#import "LSLocaytaSearchIndexer.h"

#import "LSLocaytaSearchThesaurus.h"
