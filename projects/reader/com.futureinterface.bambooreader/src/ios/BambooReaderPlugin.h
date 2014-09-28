// File: BambooReaderPlugin.h
// Bamboo Reader Phonegap plugin for iOS
// Copyright Future Interface 2012-2014
// All rights reserved.
// Author: Hongwei Li <lihw81@gmail.com>


#import <Cordova/CDV.h>

@interface BambooReaderPlugin: CDVPlugin {
    BBookArchive *_archive;
}

// I/O 

- (void)openBook:(CDVInvokedUrlCommand*)command;
- (void)closeBook:(CDVInvokedUrlCommand*)command;
- (void)openPage:(CDVInvokedUrlCommand*)command;
- (void)showPageGL:(CDVInvokedUrlCommand*)command;
- (void)hidePageGL:(CDVInvokedUrlCommand*)command;

@end
