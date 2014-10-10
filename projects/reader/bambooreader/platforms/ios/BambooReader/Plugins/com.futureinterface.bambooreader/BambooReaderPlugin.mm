// File: BambooReaderPlugin.m
// Bamboo Reader Phonegap plugin for iOS
// Copyright Future Interface 2012-2014
// All rights reserved.
// Author: Hongwei Li <lihw81@gmail.com>


#import "BambooReaderPlugin.h"

#import <Cordova/CDV.h>

#include <Bamboo/bamboo.h>

#include <PFoundation/parchivefile.h>

#import "MainViewController.h"


// @interface BambooPlugin ()
// - (FILE*)obtainBookFileHandle:(NSString*)book forPage:(int)page;
// - (void)releaseBookFileHandle:(FILE*)handle;
// @end

@implementation BambooReaderPlugin

- (void)pluginInitialize
{
    BContext *context = [((MainViewController *)self.viewController) context];
    _book = context->book();
}

- (void)openBook:(CDVInvokedUrlCommand*)command
{
    // TODO: Check if this book has been compressed and compress it if not.
    
    
    // TODO: when disk space is not enough.
    
    
    // Decompress the multimedia files in book archive
    // to Documents directory.
    CDVPluginResult* pluginResult;
    NSString *bookTitle = [command.arguments objectAtIndex:0];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    NSString *bookArchivePath = [[documentsDirectory stringByAppendingPathComponent:bookTitle] stringByAppendingString:@".par"];
    
    const pchar *bookArchivePathUTF8 = [bookArchivePath UTF8String];
    const pchar *bookTitleUTF8 = [bookTitle UTF8String];
    
    NSString *bookFolder = [documentsDirectory stringByAppendingPathComponent:bookTitle];
    
    if ([fileManager fileExistsAtPath:bookArchivePath] == NO)
    {
        PLOG_ERROR("Could not find the book %s in %s.", bookTitleUTF8, bookArchivePathUTF8);
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
        goto end;
    }
    else
    {
        // Create a book assets folder under Documents.
        PArchiveFile archive;
        if (!archive.load(bookArchivePathUTF8))
        {
            PLOG_WARNING("Could not load %s.", bookTitleUTF8);
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
            goto end;
        }
        
        if (archive.numberOfFiles() == 0)
        {
            PLOG_WARNING("There is no multimedia files in the %s.", bookTitleUTF8);
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
            goto end;
        }
        else
        {
            if (![fileManager fileExistsAtPath:bookFolder])
            {
                // Create folders.
                NSString *imageFolder = [bookFolder stringByAppendingPathComponent:@"image"];
                NSString *videoFolder = [bookFolder stringByAppendingPathComponent:@"video"];
                NSString *soundFolder = [bookFolder stringByAppendingPathComponent:@"sound"];
                NSString *fontFolder  = [bookFolder stringByAppendingPathComponent:@"font"];
                
                if (![fileManager createDirectoryAtURL:[NSURL fileURLWithPath:bookFolder]
                           withIntermediateDirectories:NO
                                            attributes:nil
                                                 error:nil] ||
                    ![fileManager createDirectoryAtURL:[NSURL fileURLWithPath:imageFolder]
                           withIntermediateDirectories:NO
                                            attributes:nil
                                                error:nil] ||
                    ![fileManager createDirectoryAtURL:[NSURL fileURLWithPath:videoFolder]
                           withIntermediateDirectories:NO
                                            attributes:nil
                                                 error:nil] ||
                    ![fileManager createDirectoryAtURL:[NSURL fileURLWithPath:soundFolder]
                           withIntermediateDirectories:NO
                                            attributes:nil
                                                 error:nil] ||
                    ![fileManager createDirectoryAtURL:[NSURL fileURLWithPath:fontFolder]
                           withIntermediateDirectories:NO
                                            attributes:nil
                                                 error:nil])
                {
                    PLOG_ERROR("Could not create asset folders.");
                    pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
                    goto end;
                }
            }
            
            
            // TODO: see if there is enough space for the uncompressed assets.

            // Uncompress all files to the directory.
            PArray<PString> filenames;
            archive.filenames(filenames);
            for (puint32 i = 0; i < archive.numberOfFiles(); ++i)
            {
                if (pstrncmp(filenames[i].c_str(), "image", 5) == 0 ||
                    pstrncmp(filenames[i].c_str(), "video", 5) == 0 ||
                    pstrncmp(filenames[i].c_str(), "sound", 5) == 0 ||
                    pstrncmp(filenames[i].c_str(), "font", 4) == 0)
                {
                    NSString *dstPath = [bookFolder
                                          stringByAppendingPathComponent:[NSString stringWithUTF8String:filenames[i].c_str()]];
                    const pchar *dstPathUTF8 = [dstPath UTF8String];
                    //PLOG_INFO("uncompressed to %s.", dstPathUTF8);
                    if (![fileManager fileExistsAtPath:dstPath])
                    {
                        archive.uncompress(filenames[i].c_str(), dstPathUTF8);
                    }
                }
            }
        }
        
        PInputStream inputStream;
        if (!archive.createInputStream("book.bbi", &inputStream))
        {
            PLOG_ERROR("Could not locate bbi in book %s.", bookTitleUTF8);
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
            goto end;
        }
        
        // Read the book information JSON file.
        puint8 *bbiBuffer;
        inputStream.readAllBytes(bbiBuffer);
        NSString *bbiString = [NSString stringWithUTF8String:(pchar *)bbiBuffer];
        PDELETEARRAY(bbiBuffer);
        
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsArray:[NSArray arrayWithObjects:bookFolder, bbiString, nil]];
    }
 
end:
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)closeBook:(CDVInvokedUrlCommand*)command
{
}

- (void)loadPage:(CDVInvokedUrlCommand*)command
{
    CDVPluginResult* pluginResult;
    
    int pageNumber = [[command.arguments objectAtIndex:0] intValue];
    
    // Read the HTML text from the archive and send it to the webview.
    BPage *page = _book->page(pageNumber);
    if (page == P_NULL)
    {
        PLOG_ERROR("Could not open page %d.", pageNumber);
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    else
    {
        // Load the page html and asset if there is any.
        page->load();
    
        const pchar *htmlText = page->html();
    
        if (htmlText != P_NULL)
        {
            NSString* result = [NSString stringWithUTF8String:htmlText];
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:result];
        }
        else
        {
            NSLog(@"Failed to get HTML text at page %d.", pageNumber);
            pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
        }
    }
    
    // Send the notification to the view.
    //NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:pageNumber], @"page", nil];
    //[[NSNotificationCenter defaultCenter] postNotificationName:@"bamboo-openpage" object:self userInfo:dict];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)unloadPage:(CDVInvokedUrlCommand*)command
{
    CDVPluginResult* pluginResult;
    
    int pageNumber = [[command.arguments objectAtIndex:0] intValue];
    
    BPage *page = _book->page(pageNumber);
    if (page == P_NULL)
    {
        PLOG_ERROR("Could not open page %d.", pageNumber);
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    else
    {
        page->unload();

        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    }

    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)openPage:(CDVInvokedUrlCommand*)command
{
    //[[NSNotificationCenter defaultCenter] postNotificationName:@"bamboo-showpagegl" object:self];
    int pageNumber = [[command.arguments objectAtIndex:0] intValue];
    _book->openPage(pageNumber);
    
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)closePage:(CDVInvokedUrlCommand*)command
{
    //[[NSNotificationCenter defaultCenter] postNotificationName:@"bamboo-hidepagegl" object:self];
    
    _book->closePage();
    
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
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

