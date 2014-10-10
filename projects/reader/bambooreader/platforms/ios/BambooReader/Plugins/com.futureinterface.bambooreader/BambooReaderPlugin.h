// File: BambooReaderPlugin.h
// Bamboo Reader Phonegap plugin for iOS
// Copyright Future Interface 2012-2014
// All rights reserved.
// Author: Hongwei Li <lihw81@gmail.com>


#import <Cordova/CDV.h>

class BBook;

@interface BambooReaderPlugin: CDVPlugin {
    BBook *_book;
}

// I/O 
- (void)openBook:(CDVInvokedUrlCommand*)command;
- (void)closeBook:(CDVInvokedUrlCommand*)command;
// Load page's assets and HTML text.
- (void)loadPage:(CDVInvokedUrlCommand*)command;
// Discard page's assets and HTML text.
- (void)unloadPage:(CDVInvokedUrlCommand*)command;
// Set the current page.
- (void)openPage:(CDVInvokedUrlCommand*)command;
// Close the current page.
- (void)closePage:(CDVInvokedUrlCommand*)command;

@end
