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
    // The opened/selected book
    _currentBookIndex : -1,
    // The position of current message box
    _messageBoxPosition : null,
    
    // -------------------------------------------------------------- 
    // Operations
    // -------------------------------------------------------------- 
    // Initialize the reader
    initialize: function() {
        // We are at home.
        $(this)._bookInformation = null;
        $(this)._books = null;

        this.browseBooks().then(function(books) {
                BambooLocal._books = JSON && JSON.parse(books) || $.parseJSON(books);
            var numBooks = BambooLocal._books.length;
        
            var wrapper = $('#wrapper');
        
            wrapper.append('<div class="local navbar"> \
                        <ul> \
                        <li class="current"><div>本地</div></li> \
                        <li><div>设置</div></li> \
                        <li><div>商店</div></li> \
                        </ul> \
                        </div>]'); 

                // Reset the style of navigation bar.
                $('.local.navbar').height(32);
            
            var h = wrapper.height();
            var w = wrapper.width();

            if (numBooks > 8) {
                // substract the width of scrollbar.
                w -= 20;
            }

            var thumbnailSize = Math.floor(w * 0.2);
            var margin = Math.floor((w - 4 * thumbnailSize) / 10);
                
            var htmlText = '<div class="local container">';
            for (var i = 0; i < numBooks; i++) {
                htmlText += '<div class="local thumbnail" book="' +
                BambooLocal._books[i].title + 
                '" index="'+i+'"><img src="' + 
                BambooLocal._books[i].cover +'"></div>\n';
            }

            htmlText += '</div>';
            wrapper.append(htmlText);

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

            $$('#wrapper').singleTap(function(e) { 
                if ($('.local.messagebox').length != 0) {
                    $('.local.messagebox').remove();
                }
            });
            $$('.local.thumbnail img').hold(function(e) { 
                if (e && e.currentTouch && e.currentTarget) {
                    var x = e.currentTouch.x;
                    var y = e.currentTouch.y;
                    BambooLocal._currentBookIndex = $(e.currentTarget).parent().attr('index');
                    var book = BambooLocal._books[BambooLocal._currentBookIndex];
                    var infoHtml = '<div class="local messagebox" style="left:'+x+'px;top:'+y+'px;">\n' +
                               '<ul>\n' + 
                               '<li>'+book.title+'</li>' + 
                               '<li>'+book.author+'</li>' + 
                               '<li>'+book.date+'</li>' + 
                               '<li>'+book.price+'</li>' + 
                               '<li>删除</li>' + 
                               '</ul>' +
                               '</div>';
                    $('#wrapper').append(infoHtml);
                }
            });

            //var longPressTimer;
            //$('.local.thumbnail').mouseup(function() {
            //    clearTimeout(longPressTimer);

	    //    if ($('.local.messagebox').length == 0) {
	    //        // Open the book at tap.
	    //	    alert('tap');
	    //    } else {
	    //        // Delete the messagebox when another click down
	    //        $('#wrapper').mousedown(function() {
	    //            if ($('.local.messagebox').length != 0) {
	    //                    $('.local.messagebox').remove();
	    //            }
	    //            $('#wrapper').mousedown(null);
	    //        });
	    //    }
            //}).mousedown(function(e) {
	    //    $('#wrapper').unbind('mousedown', BambooLocal.removeMessageBox);

            //    }, 500);
            //});
        }).always(function() {
                //BambooReader.info($('#wrapper').html());
        });
    },
    uninitialize: function() {
        $('#wrapper').empty();
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
    
        var retText = '[{"title":"untitled", "cover":"a.jpg"},\
                        {"title":"untitled", "cover":"b.jpg"},\
                        {"title":"untitled", "cover":"a.jpg"},\
                        {"title":"untitled", "cover":"b.jpg"},\
                        {"title":"untitled", "cover":"a.jpg"},\
                        {"title":"untitled", "cover":"b.jpg"},\
                        {"title":"untitled", "cover":"a.jpg"},\
                        {"title":"untitled", "cover":"b.jpg"},\
                        {"title":"untitled", "cover":"a.jpg"},\
                        {"title":"untitled", "cover":"b.jpg"},\
                        {"title":"untitled", "cover":"a.jpg"}\
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
