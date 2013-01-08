//
//  LSLocaytaSearchIndexer.h
//  LocaytaSearch
//
//  Created by Chris Miles on 26/05/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LocaytaSearch/LSLocaytaSearchIndexableRecord.h>

@protocol LSLocaytaSearchIndexerDelegate;


/**
 * \brief	Provides an interface for creating and updating search databases.
 *
 *	Search database updates are performed asynchronously in a background thread.
 *	The delegate is notified when updates have completed (or failed).
 */
@interface LSLocaytaSearchIndexer : NSObject {
    NSString *databasePath;
    id<LSLocaytaSearchIndexerDelegate> delegate;
    NSString *stemmingLanguage;
    
@private
    NSOperationQueue *indexerOperationQueue;
}

/**
 * \brief	The full path to the search database directory.
 *
 */
@property (nonatomic, copy) NSString *databasePath;

/**
 * \brief	A delegate implementing the LSLocaytaSearchIndexerDelegate protocol.
 *
 *	The delegate will receive calls to the LSLocaytaSearchIndexerDelegate methods when
 *	search database updates have completed (or failed).
 */
@property (nonatomic, assign) id<LSLocaytaSearchIndexerDelegate> delegate;

/**
 * \brief	The number of indexing requests that are still queued (including in progress).
 *
 *	Note: by the time you use the returned value, the actual number of operations may be different.
 */
@property (nonatomic, readonly) NSUInteger queuedOperationCount;

/**
 * \brief	The language to use for stemming words.
 *
 *	Word stemming is language dependent. Specify one of the languages below to choose
 *	a stemmer or set to nil to disable stemmed term generation. Stemming defaults to
 *	"english". Search queries should use the same stemmer as at index time.
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

/**
 * \brief	Checks if a valid search database exists at the given directory path.
 *
 *	Returns YES if a valid search database exists at the path and NO otherwise.
 *
 *	If the path exists but is not a valid search database then NO is returned.
 */
+ (BOOL)databaseExistsAtPath:(NSString *)aDatabasePath;


/**
 * \brief	Create search database at given path, but only if path does not already exist.
 *
 *	Returns YES if database creation succeeded.
 *
 *	Returns NO if database creation failed, anError will contain
 *	an error if it was not passed in as nil.
 */
+ (BOOL)createDatabaseAtPath:(NSString *)aDatabasePath error:(NSError **)anError;

/**
 * \brief	Initialise with the full path to an existing search database and (optional) delegate object.
 *
 *	\a aDatabasePath must point to an existing search database directory.
 *
 *	\a aDelegate must be an object implementing the LSLocaytaSearchIndexerDelegate protocol.
 *	It can be nil if no callbacks are required.
 */
- (id)initWithDatabasePath:(NSString *)aDatabasePath delegate:(id<LSLocaytaSearchIndexerDelegate>)aDelegate;

/**
 * \brief	Delete a record from the search database.
 *
 *	The \a indexableRecord object needs to only contain the "id" field of the record to delete.
 *
 *	The operation will be performed asynchronously. The delegate will be notified when complete.
 */
- (void)deleteRecord:(LSLocaytaSearchIndexableRecord *)indexableRecord;

/**
 * \brief	Update the search database with the array of records.
 *
 *	\a indexableRecords is expected to be an array of LSLocaytaSearchIndexableRecord objects.
 *
 *	The operation will be performed asynchronously. The delegate will be notified when complete.
 */
- (void)addOrReplaceRecords:(NSArray *)indexableRecords;

/**
 * \brief	Update the search database with LSLocaytaSearchIndexableRecord object.
 *
 *	The operation will be performed asynchronously. The delegate will be notified when complete.
 */
- (void)addOrReplaceRecord:(LSLocaytaSearchIndexableRecord *)indexableRecord;

/**
 * \brief	Request all unfinished indexing requests to be aborted as soon as possible.
 *
 *	Cancels any queued and in progress (unfinished) indexing operations.  The delegate will
 *	receive a failure callback for all indexing requests that were cancelled.
 */
- (void)cancelIndexing;

/**
 * \brief       Block until all current asynchronous operations have completed. 
 */
- (void)waitUntilIndexingIsFinished;

@end




#pragma mark -
#pragma mark LSLocaytaSearchIndexerDelegate

/**
 * \brief	Protocol for the LSLocaytaSearchIndexer delegate object.
 */

@protocol LSLocaytaSearchIndexerDelegate<NSObject>

/**
 * \brief	Called when the search database was successfully updated with one or more records.
 *
 */
- (void)locaytaSearchIndexer:(LSLocaytaSearchIndexer *)searchIndexer didUpdateWithIndexableRecords:(NSArray *)indexableRecords;

/**
 * \brief	Called when an update fails for any reason.
 *
 */
- (void)locaytaSearchIndexer:(LSLocaytaSearchIndexer *)searchIndexer didFailToUpdateWithIndexableRecords:(NSArray *)indexableRecords error:(NSError *)error;

@end
