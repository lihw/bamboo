/**
 * bamboobook.js - Read a book
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

    // The information of curently opened book.
    _information: null,
    // The current page number. The page number starts from 0 to _totalPageNumber - 1.
    _currentPageNumber : 0,
    // The current page object
    _currentPage : null,
    // The book file path
    _fileURL: null,
        
    
    // -------------------------------------------------------------- 
    // Operations
	// -------------------------------------------------------------- 
    
    intialize: function(title) {
        this.openBook(title).then(function(informationText) {
            // Load the book basic information.
            BambooBook._information = JSON && JSON.parse(informationText[1]) || $.parseJSON(informationText[1]);
            BambooBook._fileURL = "file://" + informationText[0] + "/";
            BambooBook.info("book " + BambooBook._information.title + " has " + BambooBook._information.page_number + " pages");
                                      
            // Load the first 3 pages of the book.
            BambooBook.loadPage(1).then(function(htmlText) {
                $('#wrapper').append('<div page="1" class="page center left">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());
                return BambooBook.loadPage(2);
            }).then(function(htmlText) {
                $('#wrapper').append('<div page="2" class="page right">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());
                return BambooBook.loadPage(3);
            }).done(function(htmlText) {
                $('#wrapper').append('<div page="3" class="page right">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());
                BambooBook.debug("after added, structure:" + $('#wrapper').html());
            });
            
            BambooBook._currentPageNumber = 1;

            // Open the first page.
            BambooBook.openPage(1);
                    
            //BambooBook.debug("after added, structure:" + $('#wrapper').html());
        }, function(error) {
            BambooReader.error('could not open book ' + title + '!');
            BambooBook._currentPageNumber = 1;
        });
    },

    uninitialize: function() {
    },

    pageDown
};

