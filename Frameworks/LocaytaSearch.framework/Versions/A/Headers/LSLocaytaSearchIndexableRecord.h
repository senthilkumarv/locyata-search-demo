//
//  LSLocaytaSearchIndexableRecord.h
//  LocaytaSearch
//
//  Created by Chris Miles on 27/05/10.
//  Copyright 2010-2011 Locayta Limited. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 * \brief	Represents a record to be added, updated or deleted from a search database.
 *
 *	An indexable record is made up of one or more fields.  The indexing strategy of fields
 *	is specified by the search schema.  The search schema must contain an entry for every
 *	field that will be indexed by a particular LSLocaytaSearchIndexableRecord.
 *
 *	A LSLocaytaSearchIndexableRecord object representing a record to be deleted only
 *	needs to have the "ID" field set.  Any other fields are ignored.
 */
@interface LSLocaytaSearchIndexableRecord : NSObject <NSCopying> {
    
@private
    BOOL isDeleting;
    NSDictionary *searchSchema;
    NSMutableDictionary *validatedFields;
}

/**
 * \brief	Will be set to YES by the database update operation if the object represented
 *	a record that was deleted.
 */
@property (nonatomic, assign, readonly)	BOOL wasDeleted;

/**
 * \brief	A NSDictionary describing all the fields to be indexed.
 *
 *	The simplest way to create and manage a schema is to define it as a Property List (plist)
 *	file.  For example, given this Property List file (which could be created by hand or using XCode
 *	or the Property List Editor) "notes_search_schema.plist" (in XML format):
 * \code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
 * <plist version="1.0">
 * <dict>
 * 	<key>content</key>
 * 	<dict>
 * 		<key>field</key>
 * 		<true/>
 * 		<key>index</key>
 * 		<true/>
 * 		<key>spell</key>
 * 		<true/>
 * 	</dict>
 * 	<key>title</key>
 * 	<dict>
 * 		<key>field</key>
 * 		<true/>
 * 		<key>weight</key>
 * 		<integer>5</integer>
 * 		<key>index</key>
 * 		<true/>
 * 		<key>spell</key>
 * 		<true/>
 * 	</dict>
 * 	<key>id</key>
 * 	<dict>
 * 		<key>field</key>
 * 		<true/>
 * 		<key>id</key>
 * 		<true/>
 * 	</dict>
 * </dict>
 * </plist>
 * \endcode
 *
 *	Then the schema file can be loaded into a NSDictionary using:
 * \code
 * NSString *schemaFile = [documentPath stringByAppendingPathComponent:@"notes_search_schema.plist"];
 * NSDictionary *searchSchema = [[NSDictionary alloc] initWithContentsOfFile:schemaFile];
 * \endcode
 */
@property (nonatomic, retain, readonly)	NSDictionary *searchSchema;

/**
 * \brief	Return all values for the specified field name.
 *
 *	A field can be assigned multiple values and values are returned as a NSArray object.
 */
- (NSArray *)valuesForField:(NSString *)fieldName;

/**
 * \brief	Return an array of all field names.
 *
 */
- (NSArray *)fieldNames;

/**
 * \brief	Add one or more values for a specified field name.
 *
 *	Add a value as an NSString for the specified field name, or add multiple
 *	values at once using an NSArray of NSString objects.
 *
 *	Each field can be assigned one or more values.  Multiple calls for the same
 *	fieldName are allowed, all values will be indexed for the field.
 *
 *	If a value cannot be added for a field the method will return NO.  Pass a
 *	reference to an NSError object as "anError" to get back information about the
 *	error, or NULL if an NSError object is not required.
 */
- (BOOL)addValue:(id)value forField:(NSString *)fieldName error:(NSError **)anError;

/**
 * \brief	Initialise with a search schema represented by a NSDictionary object.
 *
 *	The search schema defines the indexing strategy of all fields in the search database.
 *	Field cannot be added to search database if they are not specified in the schema.
 */
- (id)initWithSchema:(NSDictionary *)aSearchSchema;

@end
