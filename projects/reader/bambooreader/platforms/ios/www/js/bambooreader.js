/**
 * bambooreader.js - The book reader javascript
 *
 * @category  Mobile-Hybrid
 * @package   bamboo
 * @author    Hongwei Li<lihw81@gmail.org>
 * @copyright 2012-2014 Future Interface
 * @license   MIT License
 * @version   0.1
 * @link      N/A
 * 
 *
 */

var BambooReader = {
    // In which frame we are now, the local home, the store, the configuration or in the book.
    _whereabout: null,
    // Whether to turn on debug log.
    _debugLog : true,
    // To debug the front end while the backend is disabled.
    _debugFrontEnd: true,
    
    
    // -------------------------------------------------------------- 
    // Operations
    // -------------------------------------------------------------- 
    initialize: function() {
    // Initialize the file system
        //window.requestFileSystem = window.requestFileSystem || window.webkitRequestFileSystem;
        //if (window.File && window.FileReader && window.FileList && window.Blob) {
        //    window.requestFileSystem(window.PERSISTENT, bamboo._fileSystemQuota, bamboo.onFileSystemInitialized, bamboo.onFileSystemError);
        //} else {
        //    bamboo.error("Failed to load file system. Quitting.");
        //}
        BambooHome._whereabout = "local";

        // status
        BambooHome.initialize();
    },
    
    // -------------------------------------------------------------- 
    // Log
    // -------------------------------------------------------------- 
    info: function(message) {
        console.log(message);
    },
    error: function(message) {
        console.log('(error): ' + message, 'background: #ff0000; color: #ffffff');
    },
    debug: function(message) {
        if (BambooReader._debug) {
            console.log('(debug): ' + message);
        }
    },
};
