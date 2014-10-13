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

    // The information of curently opened book.
    _bookInformation : null,
    // The current page number. The page number starts from 0 to _totalPageNumber - 1.
    _currentPageNumber : 0,
    // The current page object
    _currentPage : null,
    // In which frame we are now, the local home, the store, the configuration or in the book.
    _whereabout: null,
    // Whether to turn on debug log.
    _debug : false,
    
    //
    // Operations.
    //
    initialize: function() {
        // Bind the swiping gesture.
        $$('#wrapper').swipeLeft(BambooReader.pageDown);
        $$('#wrapper').swipeRight(BambooReader.pageUp);

        // status
        BambooHome.initialize();

        BambooHome._whereabout = "local";
    },
    
    pageDown: function() {
        if (BambooReader._whereabout == "local") {
            BambooHome.pageDown();
        } else if (BambooReader._whereabout == "store") {
        } else if (BambooReader._whereabout == "configuration") {
        } else if (BambooReader._whereabout == "book") {
            BambooBook.pageDown();
        } else {
            BambooReader.error("Unknown place!" + BambooReader._whereabout);
        }
    },

    /*
    pageDown: function() {
            // We are at the last page of the book, no way to page down.
            if (BambooReader._currentPageNumber >= BambooReader._bookInformation.page_number) {
                return ;
            }

            BambooReader._currentPageNumber++;

            BambooReader._currentPage = $('#wrapper').find('.page.center');
            
            // Next page.
            var nextPage = BambooReader._currentPage.next().first();

            BambooReader.closePage().then(function() {
                BambooReader.animatePage(nextPage, "right");

                // Check if the we are now reach the last visible page. If so
                // Load a few pages up.
                nextPage = BambooReader._currentPage.next().first();
                if (nextPage.length == 0) {
                    // It is last visible page and we need to load 3 more new pages.
                    var nextPageNumber = BambooReader._currentPageNumber + 1;

                    BambooReader.loadPage(nextPageNumber).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                        
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());

                        return BambooReader.loadPage(nextPageNumber + 1);
                    }).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber + 1;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                        
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());

                        return BambooReader.loadPage(nextPageNumber + 2)
                    }).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber + 2;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                            
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());
                    }).always (function() {
                        // Discard 3 heading pages.
                        var wrapper = BambooReader._currentPage.parent();
                        // The page number of the first visible page
                        var firstPageNumber = wrapper.children().first().attr('page'); 
                        for (var i = firstPageNumber; i < BambooReader._currentPageNumber - 1; ++i) {
                            BambooReader.debug("page delete " + i);
                            wrapper.children().first().remove();
                            BambooReader.unloadPage(i);
                        }
                        
                        BambooReader.debug("after deleted, structure:" + $('#wrapper').html());
                    });
                }
            });
    },
    */


        
        // Initialize the file system
        //window.requestFileSystem = window.requestFileSystem || window.webkitRequestFileSystem;
        //if (window.File && window.FileReader && window.FileList && window.Blob) {
        //    window.requestFileSystem(window.PERSISTENT, bamboo._fileSystemQuota, bamboo.onFileSystemInitialized, bamboo.onFileSystemError);
        //} else {
        //    bamboo.error("Failed to load file system. Quitting.");
        //}
    initializeBook : function() {
        // Load the first few pages.
        var bookTitle = "book";
        this.openBook(bookTitle).then(function(bookInformation) {
            // Parse the content
            BambooReader._bookInformation = JSON && JSON.parse(bookInformation[1]) || $.parseJSON(bookInformation[1]);
                                      
            BambooReader.info("book " + BambooReader._bookInformation.title + " has " + BambooReader._bookInformation.page_number + " pages");
                                      
            BambooReader._bookPath = "file://" + bookInformation[0] + "/";
                                      
            // Load the first 3 pages.
            BambooReader.loadPage(1).then(function(htmlText) {
                $('#wrapper').append('<div page="1" class="page center left">'+htmlText+'</div>');
                BambooReader.replaceAssetURLs($('#wrapper').children().last());
                                          
                BambooReader.debug("after added, structure:" + $('#wrapper').html());
                                          
                return BambooReader.loadPage(2);
            }).then(function(htmlText) {
                $('#wrapper').append('<div page="2" class="page right">'+htmlText+'</div>');
                BambooReader.replaceAssetURLs($('#wrapper').children().last());

                return BambooReader.loadPage(3);
            }).done(function(htmlText) {
                $('#wrapper').append('<div page="3" class="page right">'+htmlText+'</div>');
                BambooReader.replaceAssetURLs($('#wrapper').children().last());

                BambooReader.debug("after added, structure:" + $('#wrapper').html());
            });
            
            BambooReader._currentPageNumber = 1;
            BambooReader.openPage(1);
                    
            //BambooReader.debug("after added, structure:" + $('#wrapper').html());
        }, function(error) {
            console.log('could not open book.' + bookTitle);
            BambooReader._currentPageNumber = 1;
        });

        BambooReader.info("BambooReader initialized.");
    },
    animatePage: function(nextPage, direction) {
        // Modify the CSS attributes of current page and next page.
        if (direction == "right") {
            // FIXME: Those embeded css() are to make css take effect immediately
            // after the HTML insertion by JQuery. It seems Sarafi doesn't apply
            // CSS to those dynamic content generated by JQuery.
            nextPage.attr('class', "page current right" );
            //var tmp = $(nextPage).css("transform");
            nextPage.attr('class', "page current transition center");
            BambooReader._currentPage.attr('class', "page transition left" );
        } else {
            nextPage.attr('class', "page current left" );
            //var tmp = $(nextPage).css("transform");
            nextPage.attr('class', "page current transition center");
            BambooReader._currentPage.attr('class', "page transition right" );
            //tmp = $(nextPage).css("transform");
            //this.debug(tmp);
        }
        
        // Setup current page
        BambooReader._currentPage.unbind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd");
        
        BambooReader._currentPage = nextPage;
        BambooReader._currentPageNumber = parseInt($(this._currentPage).attr('page'));
        
        BambooReader._currentPage.bind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd", 
            function() { BambooReader.onPageAnimationEnd(direction); });
    },
    /*
    pageDown: function() {
            // We are at the last page of the book, no way to page down.
            if (BambooReader._currentPageNumber >= BambooReader._bookInformation.page_number) {
                return ;
            }

            BambooReader._currentPageNumber++;

            BambooReader._currentPage = $('#wrapper').find('.page.center');
            
            // Next page.
            var nextPage = BambooReader._currentPage.next().first();

            BambooReader.closePage().then(function() {
                BambooReader.animatePage(nextPage, "right");

                // Check if the we are now reach the last visible page. If so
                // Load a few pages up.
                nextPage = BambooReader._currentPage.next().first();
                if (nextPage.length == 0) {
                    // It is last visible page and we need to load 3 more new pages.
                    var nextPageNumber = BambooReader._currentPageNumber + 1;

                    BambooReader.loadPage(nextPageNumber).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                        
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());

                        return BambooReader.loadPage(nextPageNumber + 1);
                    }).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber + 1;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                        
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());

                        return BambooReader.loadPage(nextPageNumber + 2)
                    }).then(function(htmlText) {
                        var pageNumberAttr = nextPageNumber + 2;
                        var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                        BambooReader._currentPage.parent().append(nextPageHtml);
                        BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().last());
                            
                        BambooReader.debug("after added, structure:" + $('#wrapper').html());
                    }).always (function() {
                        // Discard 3 heading pages.
                        var wrapper = BambooReader._currentPage.parent();
                        // The page number of the first visible page
                        var firstPageNumber = wrapper.children().first().attr('page'); 
                        for (var i = firstPageNumber; i < BambooReader._currentPageNumber - 1; ++i) {
                            BambooReader.debug("page delete " + i);
                            wrapper.children().first().remove();
                            BambooReader.unloadPage(i);
                        }
                        
                        BambooReader.debug("after deleted, structure:" + $('#wrapper').html());
                    });
                }
            });
    },
    */
    pageUp: function() {
        // We are at the last page of the book, no way to page down.
        if (BambooReader._currentPageNumber <= 1) {
            return ;
        }

        BambooReader._currentPageNumber--;

        BambooReader._currentPage = $('#wrapper').find('.page.center');
        
        // Previous page.
        var prevPage = BambooReader._currentPage.prev().first();

        BambooReader.closePage().then(function() {
            BambooReader.animatePage(prevPage, "left");

            // Check if the we are now reach the last visible page. If so
            // Load a few pages up.
            prevPage = BambooReader._currentPage.prev().first();
            if (prevPage.length == 0) {
                // It is first visible page and we need to load 3 more new pages.
                var prevPageNumber = BambooReader._currentPageNumber - 1;

                BambooReader.loadPage(prevPageNumber).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooReader._currentPage.parent().prepend(prevPageHtml);
                    BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().first());
                    BambooReader.debug("after added, structure:" + $('#wrapper').html());

                    return BambooReader.loadPage(prevPageNumber - 1);
                }).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber - 1;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooReader._currentPage.parent().prepend(prevPageHtml);
                    BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().first());

                    BambooReader.debug("after added, structure:" + $('#wrapper').html());

                    return BambooReader.loadPage(prevPageNumber - 2)
                }).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber - 2;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooReader._currentPage.parent().prepend(prevPageHtml);
                    BambooReader.replaceAssetURLs(BambooReader._currentPage.parent().children().first());
                
                    BambooReader.debug("after added, structure:" + $('#wrapper').html());
                }).always(function() { 
                    // Discard 3 tailing pages.
                    var wrapper = BambooReader._currentPage.parent();
                    // The page number of the first visible page
                    var lastPageNumber = wrapper.children().last().attr('page'); 
                    for (var i = lastPageNumber; i > BambooReader._currentPageNumber + 1; --i) {
                        wrapper.children().last().remove();
                        BambooReader.unloadPage(i);
                    }
                    BambooReader.debug("after deleted, structure:" + $('#wrapper').html());
                });
            }
        });
    },
    
    replaceAssetURLs: function(node) {
        node.find('img').each(function() {
                              $(this).attr('src', BambooReader._bookPath + $(this).attr('src'));
                              });
        node.find('video').each(function() {
                              $(this).attr('src', BambooReader._bookPath + $(this).attr('src'));
                              });
    },

    //
    // Event callbacks
    //
    openBookSucceed: function(bookInfo) {
            },
    closeBookSucceed: function() {
        bamboo._bookInfo = null;
    },
    onPageAnimationEnd: function(direction) {
        BambooReader.openPage(BambooReader._currentPageNumber);
        
        if (BambooReader._currentPageNumber == 4) {
            photogallery.initialize();
        }
            
    },

    openBook: function(bookTitle) {
        var deferred = new $.Deferred();

        cordova.exec(function(bookInformationText) {
                        deferred.resolve(bookInformationText);
                     },
                    function() {
                        BambooReader.error("(openBook): could not open book " + bookTitle);
                    },
                    "BambooReaderPlugin", "openBook", [bookTitle]);
        
        //setTimeout(function(){
        //    deferred.resolve();
        //}, 10);
        
        return deferred.promise();
    },
    closeBook: function() {
        cordova.exec(bamboo.closeBookSucceed,
                     function() {
                       alert("Failed to close book " + bookTitle);
                     },
                     "BambooReaderPlugin", "closeBook", []);
    },
    loadPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        if (BambooReader._bookInformation.page_number == 0) {
            BambooReader.error("(loadPage): no book open yet.");
            deferred.reject();
            return deferred.promise();
        }
        if (pageNumber < 1 || pageNumber > BambooReader._bookInformation.page_number) {
            BambooReader.error("(loadPage): invalide page number (" + pageNumber + ").");
            deferred.reject();
            return deferred.promise();
        }
        
        // Load the page
        cordova.exec(function(htmlText) {
                         deferred.resolve(htmlText);
                     },
                     function() {
                         Bamboo.error("(loadPage): could not to open page " + pageNumber);
                     },
                     "BambooReaderPlugin", "loadPage", [pageNumber.toString()]);
        //setTimeout(function(){
        //    deferred.resolve('<h1 style="color: white">'+pageNumber+'</h1>');
        //}, 10);
        
        return deferred.promise();
    },
    unloadPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        cordova.exec(function() {
                         deferred.resolve();
                     },
                     function() {
                         BambooReader.error("(unloadPage): could not unload the page "+pageNumber+".");
                     },
                     "BambooReaderPlugin", "unloadPage", [pageNumber.toString()]);
        
        return deferred.promise();
    },
    openPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        cordova.exec(function() {
                         deferred.resolve();
                     },
                     function() {
                         BambooReader.error("openPage: could not open page "+pageNumber+".");
                     },
                     "BambooReaderPlugin", "openPage", [pageNumber.toString()]);
        
        return deferred.promise();
    },
    closePage: function() {
        var deferred = new $.Deferred();
        
        cordova.exec(function() {
                         deferred.resolve();
                     },
                     function() {
                         BambooReader.error("closePage: could not close the current page.");
                     },
                     "BambooReaderPlugin", "closePage", []);

        return deferred.promise();
    },
    
    //
    // Debug
    //
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
