/**
 * bamboobook.js - The script for the book reader
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


var BambooBook = {
    // -------------------------------------------------------------- 
    // Variables
    // -------------------------------------------------------------- 
    
    // -------------------------------------------------------------- 
    // Operations
    // -------------------------------------------------------------- 
    // Initialize the reader
    initialize: function(bookTitle) {
    },
    // Uninitialize the reader and return to home.
    uninitialize: function() {
    },
    // Next page
    pageDown: function() {
    },
    // Previous page
    pageUp: function() {
    },
    // Go to certain page
    gotoPage: function(pageNumber) {
    },


    // -------------------------------------------------------------- 
    // Interface to Cordova
    // -------------------------------------------------------------- 
    // Open a book.
    openBook: function(bookTitle) {
    },
    // Close a book
    closeBook: function() {
    },
    // Load book assets and html text
    loadPage: function(pageNumber) {
    },
    // Discard html text and tell native engine to unload assets of this page.
    unloadPage: function(pageNumber) {
    },
    // Tell native engine to display the 3D content of this page
    openPage: function(pageNumber) {
    },
    // Tell native engine to hide the 3D content of this page.
    closePage: function(pageNumber) {
    },

    // -------------------------------------------------------------- 
    // Event callbacks
    // -------------------------------------------------------------- 
    onTransitionAnimationEnd: function(direction) {
    }
};
