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
    // -------------------------------------------------------------- 
    // Member variables
    // -------------------------------------------------------------- 

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
    initialize: function(title) {
        this.openBook(title).then(function(informationText) {
            // Load the book basic information.
            BambooBook._information = JSON && JSON.parse(informationText[1]) || $.parseJSON(informationText[1]);
            BambooBook._fileURL = "file://" + informationText[0] + "/";
            BambooReader.info("book " + BambooBook._information.title + " has " + BambooBook._information.page_number + " pages");
                                      
            // Load the first 3 pages of the book.
            BambooBook.loadPage(1).then(function(htmlText) {
                $('#wrapper').append('<div page="1" class="page center left">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());

                $("h1").click(function(){ BambooBook.closeBook(); });
                
                return BambooBook.loadPage(2);
            }).then(function(htmlText) {
                $('#wrapper').append('<div page="2" class="page right">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());
                return BambooBook.loadPage(3);
            }).done(function(htmlText) {
                $('#wrapper').append('<div page="3" class="page right">'+htmlText+'</div>');
                BambooBook.replaceAssetURLs($('#wrapper').children().last());
                BambooReader.debug("after added, structure:" + $('#wrapper').html());
            });
            
            // Open the first page.
            BambooBook._currentPageNumber = 1;
            BambooBook.openPage(1);

            // Add the content. 
            // FIXME: put it inside wrapper rather than outside it. 
            var contentHtml = '<div class="book content"><ul>';
            if (BambooBook._information.content) {
                var content = BambooBook._information.content;
                for (var i = 0; i < content.length; ++i) {
                    if (content[i].page instanceof Array) {
                        contentHtml += '<ul>';
                        for (var j = 0; j < content[i].page.length; ++j) {
                            if (content[i].page[j].page instanceof Array) {
                                BambooReader.error('(content displaying): not able to support more than 2 levels content.');
                            } else {
                                contentHtml += '<li page="'+content[i].page[j].page+'">'+content[i].page[j].text+'</li>';
                            }
                        }
                        contentHtml += '</ul>';
                    } else {
                        contentHtml += '<li page="'+content[i].page+'">'+content[i].text+'</li>';
                    }
                }
                contentHtml += '</ul></div>';
                $('#deviceready').append(contentHtml);
                
                // Bind the content show event.
                $("#wrapper").click(function(){ 
                    $('.book.content').toggle('slide', {"direction": "left", "distance": "400px"}); 
                });

                // Bind the content navigation function.
                $('.book.content li').click(function() {
                    var page = $(this).attr('page');
                    alert(page);
                });
            }
                    
        }, function(error) {
            BambooReader.error('could not open book ' + title + '!');
            BambooBook._currentPageNumber = 1;
        });

        $$('#wrapper').on('swipeLeft', BambooBook.pageDown);
        $$('#wrapper').on('swipeRight', BambooBook.pageUp);
    },

    uninitialize: function() {
        $$('#wrapper').off('swipeLeft', BambooBook.pageDown);
        $$('#wrapper').off('swipeRight', BambooBook.pageUp);

        // TODO: Unload the pages

        // Reset member variables.
        BambooBook._information = null;
        BambooBook._currentPageNumber = 0;
        BambooBook._currentPage = null;
        BambooBook._fileURL = null;

        // Return to home page.
        BambooReader._whereabout = "home";
        $('#wrapper').empty();
        $('#deviceready').children().last().remove();
        BambooHome.initialize();
            
    },

    pageDown: function() {
        // We are at the last page of the book, no way to page down.
        if (BambooBook._currentPageNumber >= BambooBook._information.page_number) {
            return ;
        }

        BambooBook._currentPageNumber++;

        BambooBook._currentPage = $('#wrapper').find('.page.center');
        
        // Next page.
        var nextPage = BambooBook._currentPage.next().first();

        BambooBook.closePage().then(function() {
            BambooBook.animatePage(nextPage, "right");

            // Check if the we are now reach the last visible page. If so
            // Load a few pages up.
            nextPage = BambooBook._currentPage.next().first();
            if (nextPage.length == 0) {
                // It is last visible page and we need to load 3 more new pages.
                var nextPageNumber = BambooBook._currentPageNumber + 1;

                BambooBook.loadPage(nextPageNumber).then(function(htmlText) {
                    var pageNumberAttr = nextPageNumber;
                    var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().append(nextPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().last());
                    return BambooBook.loadPage(nextPageNumber + 1);
                }).then(function(htmlText) {
                    var pageNumberAttr = nextPageNumber + 1;
                    var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().append(nextPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().last());
                    return BambooBook.loadPage(nextPageNumber + 2)
                }).then(function(htmlText) {
                    var pageNumberAttr = nextPageNumber + 2;
                    var nextPageHtml = '<div page="' + pageNumberAttr + '" class="page right">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().append(nextPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().last());
                }).always (function() {
                    // Discard 3 heading pages.
                    var wrapper = BambooBook._currentPage.parent();
                    // The page number of the first visible page
                    var firstPageNumber = wrapper.children().first().attr('page'); 
                    for (var i = firstPageNumber; i < BambooBook._currentPageNumber - 1; ++i) {
                        BambooReader.debug("page delete " + i);
                        wrapper.children().first().remove();
                        BambooBook.unloadPage(i);
                    }
                    
                    BambooReader.debug("after deleted, structure:" + $('#wrapper').html());
                });
            }
        });
    },

    pageUp: function() {
        // We are at the last page of the book, no way to page down.
        if (BambooBook._currentPageNumber <= 1) {
            return ;
        }

        BambooBook._currentPageNumber--;

        BambooBook._currentPage = $('#wrapper').find('.page.center');
        
        // Previous page.
        var prevPage = BambooBook._currentPage.prev().first();

        BambooBook.closePage().then(function() {
            BambooBook.animatePage(prevPage, "left");

            // Check if the we are now reach the last visible page. If so
            // Load a few pages up.
            prevPage = BambooBook._currentPage.prev().first();
            if (prevPage.length == 0) {
                // It is first visible page and we need to load 3 more new pages.
                var prevPageNumber = BambooBook._currentPageNumber - 1;

                BambooBook.loadPage(prevPageNumber).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().prepend(prevPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().first());
                    return BambooBook.loadPage(prevPageNumber - 1);
                }).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber - 1;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().prepend(prevPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().first());
                    return BambooBook.loadPage(prevPageNumber - 2)
                }).then(function(htmlText) {
                    var pageNumberAttr = prevPageNumber - 2;
                    var prevPageHtml = '<div page="' + pageNumberAttr + '" class="page left">'+htmlText+'</div>';
                    BambooBook._currentPage.parent().prepend(prevPageHtml);
                    BambooBook.replaceAssetURLs(BambooBook._currentPage.parent().children().first());
                }).always(function() { 
                    // Discard 3 tailing pages.
                    var wrapper = BambooBook._currentPage.parent();
                    // The page number of the first visible page
                    var lastPageNumber = wrapper.children().last().attr('page'); 
                    for (var i = lastPageNumber; i > BambooBook._currentPageNumber + 1; --i) {
                        wrapper.children().last().remove();
                        BambooBook.unloadPage(i);
                    }
                    BambooReader.debug("after deleted, structure:" + $('#wrapper').html());
                });
            }
        });
    },


    gotoPage: function(pageNumber) {
    },
    
    replaceAssetURLs: function(node) {
        node.find('img').each(function() {
                              $(this).attr('src', BambooBook._fileURL+ $(this).attr('src'));
                              });
        node.find('video').each(function() {
                              $(this).attr('src', BambooBook._fileURL+ $(this).attr('src'));
                              });
    },

    // -------------------------------------------------------------- 
    // Event callback
    // -------------------------------------------------------------- 
    animatePage: function(nextPage, direction) {
        // Modify the CSS attributes of current page and next page.
        if (direction == "right") {
            // FIXME: Those embeded css() are to make css take effect immediately
            // after the HTML insertion by JQuery. It seems Sarafi doesn't apply
            // CSS to those dynamic content generated by JQuery.
            nextPage.attr('class', "page current right" );
            //var tmp = $(nextPage).css("transform");
            nextPage.attr('class', "page current transition center");
            BambooBook._currentPage.attr('class', "page transition left" );
        } else {
            nextPage.attr('class', "page current left" );
            //var tmp = $(nextPage).css("transform");
            nextPage.attr('class', "page current transition center");
            BambooBook._currentPage.attr('class', "page transition right" );
            //tmp = $(nextPage).css("transform");
            //this.debug(tmp);
        }
        
        // Setup current page
        BambooBook._currentPage.unbind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd");
        
        BambooBook._currentPage = nextPage;
        BambooBook._currentPageNumber = parseInt($(this._currentPage).attr('page'));
        
        BambooBook._currentPage.bind("transitionend webkitTransitionEnd oTransitionEnd MSTransitionEnd", 
            function() { BambooBook.onPageAnimationEnd(direction); });
    },
    
    // -------------------------------------------------------------- 
    // Call Bamboo plugin
    // -------------------------------------------------------------- 
    openBook: function(bookTitle) {
        var deferred = new $.Deferred();

        if (BambooReader._debugFrontEnd) {
            setTimeout(function(bookTitle){
                if (bookTitle == "上古青铜器") {
                    var jsonText = '{"title":"上古青铜器", \
                                 "author": "未知", \
                                 "page_number": 3, \
                                 "affliation":"未知",\
                                 "date":"2014-10-14",\
                                 "price":0,\
                                 "image":"antique.jpg",\
                                 "desc":"介绍了中国古代青铜器",\
                                 "content":[{"text":"第一页", "page":1}, {"text":"第二页", "page":2}, {"text":"第三页", "page":3}]}'; 
                    var url = "";
                    deferred.resolve([url, jsonText]);
                } else {
                    var jsonText = '{"title":"星际探索", \
                             "author": "未知", \
                             "page_number": 5, \
                             "affliation":"未知",\
                             "date":"2014-10-14",\
                             "price":0,\
                             "image":"star.jpg",\
                             "desc":"介绍了从地球到火星的探险故事",\
                             "content":[{"text":"第一章", "page":1}, {"text":"第二章", "page":[{"text":"第二节", "page":2},{"text":"第三节", "page":3}]}, {"text":"第三章", "page":4}]}'; 
                    var url = "";
                    deferred.resolve([url, jsonText]);
                }
            }, 10);
        } else {
            cordova.exec(function(bookInformationText) {
                deferred.resolve(bookInformationText);
            },
            function() {
                BambooReader.error("(openBook): could not open book " + bookTitle);
            },
            "BambooBookPlugin", "openBook", [bookTitle]);
        }

        return deferred.promise();
    },
    closeBook: function() {
        if (BambooReader._debugFrontEnd) {
            // TODO: an animation to close the book.
            BambooBook.uninitialize();
        } else {
            cordova.exec(function() {
                // TODO: an animation to close the book.
                BambooBook.uninitialize();
            },
            function() {
                BambooReader.error("(closeBook): failed to close book."); 
            },
            "BambooBookPlugin", "closeBook", []);
        }
    },
    loadPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        if (BambooBook._information.page_number == 0) {
            BambooReader.error("(loadPage): no book open yet.");
            deferred.reject();
            return deferred.promise();
        }
        if (pageNumber < 1 || pageNumber > BambooBook._information.page_number) {
            BambooReader.error("(loadPage): invalide page number (" + pageNumber + ").");
            deferred.reject();
            return deferred.promise();
        }
        
        if (BambooReader._debugFrontEnd) {
            setTimeout(function(){
                deferred.resolve('<h1 style="color: white">'+pageNumber+'</h1>');
            }, 10);
        } else {
        // Load the page
            cordova.exec(function(htmlText) {
                deferred.resolve(htmlText);
            },
            function() {
                Bamboo.error("(loadPage): could not to open page " + pageNumber);
            },
            "BambooBookPlugin", "loadPage", [pageNumber.toString()]);
        }
        
        return deferred.promise();
    },
    unloadPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        if (BambooReader._debugFrontEnd) {
            setTimeout(function(){
                deferred.resolve();
            }, 10);
        } else {
            cordova.exec(function() {
                deferred.resolve();
            },
            function() {
                BambooReader.error("(unloadPage): could not unload the page "+pageNumber+".");
            },
            "BambooBookPlugin", "unloadPage", [pageNumber.toString()]);
        }
        
        return deferred.promise();
    },
    openPage: function(pageNumber) {
        var deferred = new $.Deferred();
        
        if (BambooReader._debugFrontEnd) {
            setTimeout(function(){
                deferred.resolve();
            }, 10);
        } else {
            cordova.exec(function() {
                deferred.resolve();
            },
            function() {
                BambooReader.error("(openPage): could not open page "+pageNumber+".");
            },
            "BambooBookPlugin", "openPage", [pageNumber.toString()]);
        }
        
        return deferred.promise();
    },
    closePage: function() {
        var deferred = new $.Deferred();
        
        if (BambooReader._debugFrontEnd) {
            setTimeout(function(){
                deferred.resolve();
            }, 10);
        } else {
            cordova.exec(function() {
                deferred.resolve();
            },
            function() {
                BambooReader.error("(closePage): could not close the current page.");
            },
            "BambooBookPlugin", "closePage", []);
        }

        return deferred.promise();
    },
    
    // -------------------------------------------------------------- 
    // Event callbacks
    // -------------------------------------------------------------- 
    onPageAnimationEnd: function(direction) {
        
        BambooBook.openPage(BambooBook._currentPageNumber);
        
        // FIXME: hardcode for book "star"
        if (BambooBook._currentPageNumber == 4) {
            photogallery.initialize();
        }
    },
};

