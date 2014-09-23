// File: BambooReaderPlugin.m
// Bamboo Reader Phonegap plugin for iOS
// Copyright Future Interface 2012-2014
// All rights reserved.
// Author: Hongwei Li <lihw81@gmail.com>


#import "BambooPlugin.h"

#import <Cordova/CDV.h>

//#include <Bamboo/bamboo.h>


// @interface BambooPlugin ()
// - (FILE*)obtainBookFileHandle:(NSString*)book forPage:(int)page;
// - (void)releaseBookFileHandle:(FILE*)handle;
// @end

@implementation BambooReaderPlugin

- (void)openBook:(CDVInvokedUrlCommand*)command 
{
}

- (void)closeBook:(CDVInvokedUrlCommand*)command 
{
}

- (void)openPage:(CDVInvokedUrlCommand*)command 
{
}

- (void)showPageGL:(CDVInvokedUrlCommand*)command 
{
}


/*
- (void)loadBookContent:(CDVInvokedUrlCommand*)command
{
    CDVPluginResult* pluginResult = nil;
    NSString* echo = [command.arguments objectAtIndex:0];
    
    if (echo != nil && [echo length] > 0) {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:echo];
    } else {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)loadBookPage:(CDVInvokedUrlCommand*)command
{
    CDVPluginResult* pluginResult = nil;
    
    NSString* book = [command.arguments objectAtIndex:0];
    int page = [[command.arguments objectAtIndex:1] intValue];
    
    NSLog(@"Load page %i of book %@", page, book);
    
    // TODO: put to a background thread.
    FILE *fp = [self obtainBookFileHandle:book forPage:page];
    if (fp != NULL) {
        char* pageHTML = bbReadPage(fp, page);
        if (pageHTML != NULL) {
            NSString* result = [NSString stringWithUTF8String:pageHTML];
            free(pageHTML);
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:result];
        }
    } else {
        NSLog(@"Failed to obtain file handle of book %@ at %i", book, page);
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    [self releaseBookFileHandle:fp];
    
    // Tell GL layer to load this page.
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:book, @"book", [NSNumber numberWithInt:page], @"page", nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"turntopage-gllayer" object:self userInfo:dict];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)loadBookAsset:(CDVInvokedUrlCommand*)command
{
    CDVPluginResult* pluginResult = nil;
    NSString* echo = [command.arguments objectAtIndex:0];
    
    if (echo != nil && [echo length] > 0) {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:echo];
    } else {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)hideGLLayer:(CDVInvokedUrlCommand*)command
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"hide-gllayer" object:self];
    
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)showGLLayer:(CDVInvokedUrlCommand*)command
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"show-gllayer" object:self];
    
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (FILE*)obtainBookFileHandle:(NSString*)book forPage:(int)page
{
    FILE* fp = NULL;
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    // Create a folder for book
    NSString *bookPath = [documentsDirectory stringByAppendingPathComponent:book];
    NSString *bookAssetPath = [[bookPath stringByAppendingPathComponent:book] stringByAppendingString:@".bbm"];
    
    if ([fileManager fileExistsAtPath:bookAssetPath] == YES) {
        fp = fopen([bookAssetPath UTF8String],"rb");
    }
    
    return fp;
}

- (void)releaseBookFileHandle:(FILE*)handle
{
    if (handle != NULL) {
        fclose(handle);
    }
}
*/

@end

