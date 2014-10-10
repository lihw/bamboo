/*
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.
 */

//
//  MainViewController.h
//  BambooReader
//
//  Created by Hongwei Li (lihw81@gmail.com) on 2014/09/22.
//  Copyright Future Interface 2012-2014. All rights reserved.
//

#import "MainViewController.h"

#include <Bamboo/bcontext.h>
#include <Bamboo/bbook.h>


@interface MainViewController ()
- (void)openPage:(NSNotification *)note;
- (void)showPageGL:(NSNotification *)note;
- (void)hidePageGL:(NSNotification *)note;
@end


@implementation MainViewController

@synthesize bview;

- (id)initWithNibName:(NSString*)nibNameOrNil bundle:(NSBundle*)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Uncomment to override the CDVCommandDelegateImpl used
        // _commandDelegate = [[MainCommandDelegate alloc] initWithViewController:self];
        // Uncomment to override the CDVCommandQueue used
        // _commandQueue = [[MainCommandQueue alloc] initWithViewController:self];
    }
    return self;
}

- (BContext *)context
{
    return _bcontext;
}

- (id)init
{
    self = [super init];
    if (self) {
        // Uncomment to override the CDVCommandDelegateImpl used
        // _commandDelegate = [[MainCommandDelegate alloc] initWithViewController:self];
        // Uncomment to override the CDVCommandQueue used
        // _commandQueue = [[MainCommandQueue alloc] initWithViewController:self];
    }
    return self;
}

- (id)initWithContext:(BContext *)bcontext
{
    self = [super init];
    if (self) {
        // Uncomment to override the CDVCommandDelegateImpl used
        // _commandDelegate = [[MainCommandDelegate alloc] initWithViewController:self];
        // Uncomment to override the CDVCommandQueue used
        // _commandQueue = [[MainCommandQueue alloc] initWithViewController:self];
        
        _bcontext = bcontext;
        
        
    }
    return self;
}


- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

#pragma mark View lifecycle

- (void)viewWillAppear:(BOOL)animated
{
    // View defaults to full size.  If you want to customize the view's size, or its subviews (e.g. webView),
    // you can do so here.
    // Create the window and view.
    
    [super viewWillAppear:animated];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Do any additional setup after loading the view from its nib.
    
    //
    // Regiters notifications from BambooReader plugin.
    //
    /*
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(openPage:)
                                                 name:@"bamboo-openpage"
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(showPageGL:)
                                                 name:@"bamboo-showpagegl"
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(hidePageGL:)
                                                 name:@"bamboo-hidepagegl"
                                               object:nil];
    */
    
    //
    // Initialize Bamboo
    //
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED <= __IPHONE_7_1
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (orientation == UIInterfaceOrientationLandscapeRight)
    {
        pSwap(screenBounds.size.width, screenBounds.size.height);
    }
#endif
    
    _bview = [[BambooView alloc] initWithFrame:screenBounds TechContext:_bcontext];
    if (_bview)
    {
        [self.view addSubview:_bview];
    }
    
    // Activiate Paper3D
    if (_bcontext->state() == P_CONTEXT_STATE_UNINITIALIZED)
    {
        PLOG_DEBUG("Starting Bamboo main loop.");
        _bcontext->setState(P_CONTEXT_STATE_RUNNING);
    }
    
    // DEBUG
    // Copy a default book from Bundle to Documents
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    // Create a folder for book
    //NSString *folderPath = [documentsDirectory stringByAppendingPathComponent:@"/bambooguide"];
    
    //if (![[NSFileManager defaultManager] fileExistsAtPath:folderPath]) {
    //    [[NSFileManager defaultManager] createDirectoryAtPath:folderPath withIntermediateDirectories:NO attributes:nil error:&error];
    //}
    
    NSString *dstPath = [documentsDirectory stringByAppendingPathComponent:@"book.par"];
    
    if ([fileManager fileExistsAtPath:dstPath] == NO || YES) {
        NSString *srcPath = [[NSBundle mainBundle] pathForResource:@"book" ofType:@"par"];
        [fileManager copyItemAtPath:srcPath toPath:dstPath error:&error];
    }
}


- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
    // Uninitialize Bamboo
    if (_bcontext->state() != P_CONTEXT_STATE_UNINITIALIZED)
    {
        _bcontext->onDestroy();
    }
    _bcontext->destroy();
    PDELETE(_bcontext);
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationLandscapeRight) ? YES : NO;
}

/* Comment out the block below to over-ride */

/*
- (UIWebView*) newCordovaViewWithFrame:(CGRect)bounds
{
    return[super newCordovaViewWithFrame:bounds];
}
*/

- (void)showPageGL:(NSNotification *)note
{
    // FIXME: enable the updating of PContext
    [_bview setHidden:NO];
    
}

- (void)hidePageGL:(NSNotification *)note
{
    // FIXME: disable the updating of PContext
    [_bview setHidden:YES];
}

- (void)openPage:(NSNotification *)note
{
    int page = [[[note userInfo] objectForKey:@"page"] intValue];
    
    _bcontext->book()->openPage(page);
    
    NSLog(@"Open page %i succeeds", page);
}


#pragma mark UIWebDelegate implementation

- (void)webViewDidFinishLoad:(UIWebView*)theWebView
{
    // Black base color for background matches the native apps
    theWebView.backgroundColor = [UIColor blackColor];

    return [super webViewDidFinishLoad:theWebView];
}

/* Comment out the block below to over-ride */

/*

- (void) webViewDidStartLoad:(UIWebView*)theWebView
{
    return [super webViewDidStartLoad:theWebView];
}

- (void) webView:(UIWebView*)theWebView didFailLoadWithError:(NSError*)error
{
    return [super webView:theWebView didFailLoadWithError:error];
}

- (BOOL) webView:(UIWebView*)theWebView shouldStartLoadWithRequest:(NSURLRequest*)request navigationType:(UIWebViewNavigationType)navigationType
{
    return [super webView:theWebView shouldStartLoadWithRequest:request navigationType:navigationType];
}
*/

@end

@implementation MainCommandDelegate

/* To override the methods, uncomment the line in the init function(s)
   in MainViewController.m
 */

#pragma mark CDVCommandDelegate implementation

- (id)getCommandInstance:(NSString*)className
{
    return [super getCommandInstance:className];
}

/*
   NOTE: this will only inspect execute calls coming explicitly from native plugins,
   not the commandQueue (from JavaScript). To see execute calls from JavaScript, see
   MainCommandQueue below
*/
- (BOOL)execute:(CDVInvokedUrlCommand*)command
{
    return [super execute:command];
}

- (NSString*)pathForResource:(NSString*)resourcepath;
{
    return [super pathForResource:resourcepath];
}

@end

@implementation MainCommandQueue

/* To override, uncomment the line in the init function(s)
   in MainViewController.m
 */
- (BOOL)execute:(CDVInvokedUrlCommand*)command
{
    return [super execute:command];
}

@end
