//
//  AppDelegate_Pad.m
//  LocNotes
//
//  Created by Chris Miles on 24/05/10.
//
//  Copyright (c) Locayta Limited 2010-2011.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//


#import "AppDelegate_Pad.h"

#import "EditNoteViewController.h"
#import "Note+Management.h"
#import "NotesBrowserTableViewController.h"

@implementation AppDelegate_Pad

@synthesize editNoteViewController;
@synthesize mainSplitViewController;
@synthesize notesBrowserTableViewController;

#pragma mark -
#pragma mark Application delegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    [self setUpSearchDatabase];

    self.enableAutoSpellCorrection = YES;

    NSFetchRequest *request = [[NSFetchRequest alloc] initWithEntityName:@"Note"];
    NSError *error;
    NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
    NSLog(@"ArrCOunt: %d", [array count]);
    if ([array count] == 0) {
        [self readAndIndex];
    }

    // Select the first available note by default. Or create a new note if none exist.
    NSFetchedResultsController *noteFetchedResultsController = [self.notesBrowserTableViewController fetchedResultsControllerForNoteWithDelegate:nil];
	id <NSFetchedResultsSectionInfo> sectionInfo = [[noteFetchedResultsController sections] objectAtIndex:0];
    if ([sectionInfo numberOfObjects] > 0) {
		NSIndexPath *firstNoteIndexPath = [NSIndexPath indexPathForRow:0 inSection:0];
		Note *note = [noteFetchedResultsController objectAtIndexPath:firstNoteIndexPath];
		if (note) {
			self.editNoteViewController.note = note;
		}
	}
	
	[window addSubview:self.mainSplitViewController.view];
	
    [window makeKeyAndVisible];

	return YES;
}


/**
 Superclass implementation saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[super applicationWillTerminate:application];
}


#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[editNoteViewController release];
	[mainSplitViewController release];
	[notesBrowserTableViewController release];
	
	[super dealloc];
}


@end

