/**
 * bamboo_local.js - The script for the local book browser
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

var BambooLocal= {
    // -------------------------------------------------------------- 
    // Variables
    // -------------------------------------------------------------- 
    // The information of books.
    _books : null,
    // The system path of book storage.
    _booksPath : null,
    // The current page number in the home page.
    _currentPageNumber: 1,
    // The number of pages in the home page.
    _numPages : 0,
    // The current page object.
    _currentPage : null,
    
    // -------------------------------------------------------------- 
    // Operations
    // -------------------------------------------------------------- 
    // Initialize the reader
    initialize: function() {
	    // We are at home.
	    $(this)._bookInformation = null;
	    $(this)._books = null;

	    // Set up the page layout of the home.
	    var wrapper = $('#wrapper');
	    
	    var h = wrapper.height();
	    var w = wrapper.width();

	    wrapper.append('<div class="local navbar"> \
	    	        <ul> \
	    	        <li class="current"><div>本地</div></li> \
	    	        <li><div>设置</div></li> \
	    	        <li><div>商店</div></li> \
	    	        </ul> \
	    	        </div>]'); 

	    // Reset the style of navigation bar.
	    $('.local.navbar').height(32);

	    var thumbnailSize = Math.floor(w * 0.2);
	    var margin = Math.floor((w - 4 * thumbnailSize) / 10);


	    this.browseBooks().then(function(books) {
                BambooLocal._books = JSON && JSON.parse(books) || $.parseJSON(books);
	        var numBooks = BambooLocal._books.length;
	    
	        var wrapper = $('#wrapper');
	        
	        var numPages = Math.floor(numBooks / 8);
	        for (var i = 0; i < numPages; i++) {
	        	wrapper.append('<div class="local container ' + (i == 0? 'center':'right') + '">\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 0].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 1].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 2].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 3].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 4].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 5].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 6].cover +'"></div>\n' +
	    	   '<div class="local thumbnail"><img src="'+ BambooLocal._books[i * 8 + 7].cover +'"></div>\n' +
	    	'</div>\n');
	        }

	        var restNumPages = numBooks - numPages * 8;
	        if (restNumPages > 0) {
	        	var htmlText = '<div class="local container right">';
	        	for (var i = 0; i < restNumPages; i++) {
	    	    var j = numPages * 8 + i;
	    	    htmlText += '<div class="local thumbnail"><img src="'+ BambooLocal._books[j].cover +'"></div>\n';
	    	}
	    	htmlText += '</div>';
	        	wrapper.append(htmlText);
	        }

	        $('.local.container').css({
	        	'padding-left': margin,
	        	'padding-right': margin
	        });

	        $('.local.thumbnail').css({
	        	'margin': margin,
	        	'width': thumbnailSize,
	        	'height': thumbnailSize,
	        	'line-height': thumbnailSize
	        });
	        
	        $('.local.thumbnail img').css({
	    	        'max-width': thumbnailSize,
	    	        'max-height': thumbnailSize,
	        });

	        BambooLocal._numPages = numPages + 1;

	    }).always(function() {
            $('#wrapper').append('<div class="local footer"><p>��1ҳ/��'+BambooLocal._numPages+'ҳ</p></div>');
            BambooReader.info($('#wrapper').html());
	    });
    },
    uninitialize: function() {
	$('#wrapper').empty();
    },
    
    pageDown: function() {
    	// We are at the last page of the book, no way to page down.
    	if (BambooLocal._currentPageNumber >= BambooLocal._numPages) {
    	    return ;
    	}

    	BambooLocal._currentPageNumber++;

    	BambooLocal._currentPage = $('#wrapper').find('.local.container.center');
    	
    	// Next page.
    	var nextPage = BambooLocal._currentPage.next().first();
    	nextPage.attr('class', "local current right" );
    	nextPage.attr('class', "local current transition center");
    	BambooLocal._currentPage.attr('class', "local transition left");
        
        BambooLocal._currentPage.unbind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd");
        
        BambooLocal._currentPage = nextPage;
        BambooLocal._currentPageNumber = parseInt($(this._currentPage).attr('page'));
        
        BambooLocal._currentPage.bind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd", 
            function() { BambooLocal.onPageAnimationEnd(direction); });
    },

    pageUp: function() {
    	// We are at the last page of the book, no way to page down.
    	if (BambooLocal._currentPageNumber <= 1) {
    	    return ;
    	}

    	BambooLocal._currentPageNumber--;

    	BambooLocal._currentPage = $('#wrapper').find('.local.container.center');
    	
    	// Next page.
    	var nextPage = BambooLocal._currentPage.next().first();
    	nextPage.attr('class', "local current left" );
    	nextPage.attr('class', "local current transition center");
    	BambooLocal._currentPage.attr('class', "local transition right");
	
        BambooLocal._currentPage.unbind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd");
        
        BambooLocal._currentPage = nextPage;
        BambooLocal._currentPageNumber = parseInt($(this._currentPage).attr('page'));
        
        BambooLocal._currentPage.bind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd", 
            function() { BambooLocal.onPageAnimationEnd(direction); });
    },
    
    // -------------------------------------------------------------- 
    // Interface to the BambooLocal cordova plugin
    // -------------------------------------------------------------- 
    browseBooks : function() {
        var deferred = new $.Deferred();

        //cordova.exec(function(bookInformationText) {
        //                deferred.resolve(bookInformationText);
        //             },
        //            function() {
        //                BambooLocal.error("(openBook): could not open book " + BambooLocal._bookTitle);
        //            },
        //            "BambooLocalPlugin", "openBook", [BambooLocal._bookTitle]);
        
        setTimeout(function(){
	
	    var retText = '[{"cover":"a.jpg"},\
	                    {"cover":"b.jpg"},\
	                    {"cover":"a.jpg"},\
	                    {"cover":"b.jpg"},\
	                    {"cover":"a.jpg"},\
	                    {"cover":"b.jpg"},\
	                    {"cover":"a.jpg"},\
	                    {"cover":"b.jpg"},\
	                    {"cover":"a.jpg"},\
	                    {"cover":"b.jpg"},\
	                    {"cover":"a.jpg"}\
	                   ]'; 

            deferred.resolve(retText);
        }, 10);
        
        return deferred.promise();
    },
    
    // -------------------------------------------------------------- 
    // Events
    // -------------------------------------------------------------- 
    onPageAnimationEnd: function() {
    },
};
