/**
 * bamboohome.js - The script for the book browser
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

var BambooHome= {
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
        $(this)._books = null;

        this.browseBooks().then(function(books) {
            BambooHome._books = JSON && JSON.parse(books) || $.parseJSON(books);
            var numBooks = BambooHome._books.length;
        
            var wrapper = $('#wrapper');
        
            wrapper.append('<div id="tabs">\n '+
                           '	<ul>\n '+
                           '		<li><a href="#local">本地</a></li> \n '+
                           '		<li><a href="#conf">设置</a></li> \n '+
                           '		<li><a href="#store">商店</a></li> \n '+
                           '	</ul> \n '+
                           '	<div id="local"></div> \n '+
                           '	<div id="conf">Under construction.</div> \n '+
                           '	<div id="store">Under construction.</div> \n '+
                           '</div>');

            $("#tabs").tabs();

            var h = wrapper.height();
            var w = wrapper.width();

            if (numBooks > 8) {
                // substract the width of scrollbar.
                w -= 20;
            }

            var thumbnailSize = Math.floor(w * 0.2);
            var margin = Math.floor((w - 4 * thumbnailSize) / 10);
                
            var htmlText = '<ul id="bookcover" class="ui-widget ui-helper-clearfix">';
            for (var i = 0; i < numBooks; i++) {
                htmlText += '<li id="bookcover" index="'+i+'" class="ui-corner-all">' +
                '<img src="' + BambooHome._books[i].cover +'" /></div>\n' +'</li>';
            }
            htmlText += '</ul>';

            $('#local').append(htmlText);

            $('#local').append('<div id="bookinfo" title="信息">' +
                               '<ul>' +
                               '<li id="localbooktitle"></li>' +
                               '<li id="localbookauthor"></li>' +
                               '<li id="localbookdate"></li>' +
                               '<li id="localbookprice"></li>' +
                               '<li id="localbookdesc"></li>' +
                               '<li>删除</li>' +
                               '</ul>' +
                               '</div>');
            

            $('#bookinfo').dialog({
                autoOpen: false,
                draggable: false,
                resizable: false,
                width: 400,
            });

            // FIXME: quoJS's longpress (hold) is buggy. It doesn't always
            // provide a valid target value or current position. Should consider
            // another gesture library.
            $$('#local').singleTap(function(e) { 
                if ($('#bookinfo').dialog("isOpen")) {
                    $('#bookinfo').dialog("close");
                } 
            });
            $$('#bookcover img').singleTap(function(e) { 
                if ($('#bookinfo').dialog("isOpen")) {
                    $('#bookinfo').dialog("close");
                } else {
                    BambooHome._currentBookIndex = $(e.currentTarget).parent().attr('index');
                    var book = BambooHome._books[BambooHome._currentBookIndex];
                    if (book) {
                        BambooHome.uninitialize();
                        BambooReader._whereabout = "book";

                        // TODO: an animation to open the book.
                        BambooBook.initialize(book.title);
                    }
                }
            });
            $$('#bookcover img').hold(function(e) { 
                if (e && e.currentTouch && e.currentTarget) {
                    var x = e.currentTouch.x;
                    var y = e.currentTouch.y;
                    BambooHome._currentBookIndex = $(e.currentTarget).parent().attr('index');
                    var book = BambooHome._books[BambooHome._currentBookIndex];

                    $('#localbooktitle').html("书名: "+book.title);
                    $('#localbookauthor').html("作者: "+book.author);
                    $('#localbookdate').html("出版日期:"+book.date);
                    $('#localbookprice').html("定价:"+(parseInt(book.price) == 0? "免费" : book.price+"元"));
                    $('#localbookdesc').html("简介:<br>"+book.desc);

                    $('#bookinfo').dialog("option", "position",  
                        { my: "left top", at: "center", of: e.currentTarget});
                    $("#bookinfo").dialog("open");
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
	    //    $('#wrapper').unbind('mousedown', BambooHome.removeMessageBox);

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
    // Interface to the BambooHome cordova plugin
    // -------------------------------------------------------------- 
    browseBooks : function() {
        var deferred = new $.Deferred();

        if (BambooReader._debugFrontEnd) {
        //cordova.exec(function(bookInformationText) {
        //                deferred.resolve(bookInformationText);
        //             },
        //            function() {
        //                BambooHome.error("(openBook): could not open book " + BambooHome._bookTitle);
        //            },
        //            "BambooHomePlugin", "openBook", [BambooHome._bookTitle]);
        
            setTimeout(function(){
                var retText = '[{"title":"星际探索", \
                    "author": "未知", \
                    "affliation":"未知",\
                    "date":"2014-10-14",\
                    "price":0,\
                    "cover":"star.jpg",\
                    "desc":"介绍了从地球到火星的探险故事"},\
                {"title":"上古青铜器", \
                    "author": "未知", \
                    "affliation":"未知",\
                    "date":"2014-10-14",\
                    "price":0,\
                    "cover":"antique.jpg",\
                    "desc":"介绍了中国古代青铜器"}\
                    ]'; 

            deferred.resolve(retText);
            }, 10);
        } else {
        }
        
        return deferred.promise();
    },
    
    // -------------------------------------------------------------- 
    // Events
    // -------------------------------------------------------------- 
};
