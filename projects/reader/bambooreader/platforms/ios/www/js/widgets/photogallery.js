var photogallery = {
    current    : 0,
    nmb_thumbs : 0,

    //cache some elements
    $loader        : null,
    $btn_next      : null,
    $btn_prev      : null,	
    $thumbScroller : null,
				

    initialize: function() {
        $('#bwpg_fp_gallery').ready(function() {
            sliderLeft=$('#bwpg_thumbScroller .bwpg_container').position().left;
            padding=$('#bwpg_outer_container').css('paddingRight').replace("px", "");
            sliderWidth=$(window).width()-padding;
            $('#bwpg_thumbScroller').css('width',sliderWidth);
            var totalContent=0;
            $('#bwpg_thumbScroller .bwpg_content').each(function () {
                totalContent+=$(this).width();
                $('#bwpg_thumbScroller .bwpg_container').css('width',totalContent);
            });
            $('#bwpg_thumbScroller').mousemove(function(e){
                if($('#bwpg_thumbScroller  .bwpg_container').width()>sliderWidth){
                    var mouseCoords=(e.pageX - this.offsetLeft);
                    var mousePercentX=mouseCoords/sliderWidth;
                    var destX=-(((totalContent-(sliderWidth))-sliderWidth)*(mousePercentX));
                    var thePosA=mouseCoords-destX;
                    var thePosB=destX-mouseCoords;
                    var animSpeed=600; //ease amount
                    var easeType='easeOutCirc';
                    if(mouseCoords==destX){
                        $('#bwpg_thumbScroller .bwpg_container').stop();
                    }
                    else if(mouseCoords>destX){
                        //$('#thumbScroller .container').css('left',-thePosA); //without easing
                        $('#bwpg_thumbScroller .bwpg_container').stop().animate({left: -thePosA}, animSpeed,easeType); //with easing
                    }
                    else if(mouseCoords<destX){
                        //$('#thumbScroller .container').css('left',thePosB); //without easing
                        $('#bwpg_thumbScroller .bwpg_container').stop().animate({left: thePosB}, animSpeed,easeType); //with easing
                    }
                }
            });
            $('#bwpg_thumbScroller  .bwpg_thumb').each(function () {
                $(this).fadeTo(fadeSpeed, 0.3);
            });
            var fadeSpeed=200;
            $('#bwpg_thumbScroller .bwpg_thumb').hover(
                    function(){ //mouse over
                        $(this).fadeTo(fadeSpeed, 1);
                    },
                    function(){ //mouse out
                        $(this).fadeTo(fadeSpeed, 0.3);
                    }
            );

            //current thumb's index being viewed
            photogallery.current	    = 1;
            //cache some elements
            photogallery.$loader		= $('#bwpg_fp_loading');
            photogallery.$btn_next		= $('#bwpg_fp_next');
            photogallery.$btn_prev		= $('#bwpg_fp_prev');
            photogallery.$thumbScroller	= $('#bwpg_thumbScroller');
				
            photogallery.$btn_next.bind('click', photogallery.showNext);
	    photogallery.$btn_prev.bind('click', photogallery.showPrev);
				
            //total number of thumbs
            photogallery.nmb_thumbs		= photogallery.$thumbScroller.find('.bwpg_content').length;

            //preload thumbs
            for(var i=0;i<photogallery.nmb_thumbs;++i) {
                var $thumb = photogallery.$thumbScroller.find('.bwpg_content:nth-child('+parseInt(i+1)+')');
                $('<img/>').load(function(){ }).attr('src',$thumb.find('img').attr('src'));
            }
                
            //show the navigation arrows
            photogallery.showNav();
				
            photogallery.$thumbScroller.find('.bwpg_content').bind('click',function(e){
					var $content= $(this);
					var $elem 	= $content.find('img');
					//keep track of the current clicked thumb
					//it will be used for the navigation arrows
					photogallery.current 	= $content.index()+1;
					//get the positions of the clicked thumb
					var pos_left 	= $elem.offset().left;
					var pos_top 	= $elem.offset().top;
					//clone the thumb and place
					//the clone on the top of it
					var $clone 	= $elem.clone()
					.addClass('bwpg_clone')
					.css({
						'position':'fixed',
						'left': pos_left + 'px',
						'top': pos_top + 'px'
					}).insertAfter($('BODY'));
					
					var windowW = $(window).width();
					var windowH = $(window).height();
					
					//animate the clone to the center of the page
					$clone.stop()
					.animate({
						'left': windowW/2 + 'px',
						'top': windowH/2 + 'px',
						'margin-left' :-$clone.width()/2 -5 + 'px',
						'margin-top': -$clone.height()/2 -5 + 'px'
					},500,
					function(){
						var $theClone 	= $(this);
						var ratio		= $clone.width()/120;
						var final_w		= 400*ratio;
						
						photogallery.$loader.show();
						
						//expand the clone when large image is loaded
						$('<img class="bwpg_fp_preview"/>').load(function(){
							var $newimg 		= $(this);
							var $currImage 	= $('#bwpg_fp_gallery').children('img:first');
							$newimg.insertBefore($currImage);
							photogallery.$loader.hide();
							//expand clone
							$theClone.animate({
								'opacity'		: 0,
								'top'			: windowH/2 + 'px',
								'left'			: windowW/2 + 'px',
								'margin-top'	: '-200px',
								'margin-left'	: -final_w/2 + 'px',
								'width'			: final_w + 'px',
								'height'		: '400px'
							},1000,function(){$(this).remove();});
							//now we have two large images on the page
							//fadeOut the old one so that the new one gets shown
							$currImage.fadeOut(2000,function(){
								$(this).remove();
							});
							//show the navigation arrows
							photogallery.showNav();
						}).attr('src',$elem.attr('alt'));
					});
					//hide the thumbs container
					e.preventDefault();
				});
				
        });
    },

    hideNav : function() {
        photogallery.$btn_next.stop().animate({'right':'-50px'},500);
        photogallery.$btn_prev.stop().animate({'left':'-50px'},500);
    },

    showNav : function() {
        photogallery.$btn_next.stop().animate({'right':'0px'},500);
        photogallery.$btn_prev.stop().animate({'left':'0px'},500);
    },
				
    showNext :function() {
        ++photogallery.current;
        var $e_next	= photogallery.$thumbScroller.find('.bwpg_content:nth-child('+photogallery.current+')');
        if($e_next.length == 0) {
            photogallery.current = 1;
            $e_next	= photogallery.$thumbScroller.find('.bwpg_content:nth-child('+photogallery.current+')');
        }
        photogallery.$loader.show();
        $('<img class="bwpg_fp_preview"/>').load(function(){
            var $newimg 		= $(this);
            var $currImage 		= $('#bwpg_fp_gallery').children('img:first');
            $newimg.insertBefore($currImage);
            photogallery.$loader.hide();
            $currImage.fadeOut(2000,function(){$(this).remove();});
        }).attr('src',$e_next.find('img').attr('alt'));
    },
				
    showPrev : function() {
        --photogallery.current;
        var $e_next	= photogallery.$thumbScroller.find('.bwpg_content:nth-child('+photogallery.current+')');
        if($e_next.length == 0){
            photogallery.current = photogallery.nmb_thumbs;
            $e_next	= photogallery.$thumbScroller.find('.bwpg_content:nth-child('+photogallery.current+')');
        }
        photogallery.$loader.show();
        $('<img class="bwpg_fp_preview"/>').load(function(){
            var $newimg 		= $(this);
            var $currImage 		= $('#bwpg_fp_gallery').children('img:first');
            $newimg.insertBefore($currImage);
            photogallery.$loader.hide();
            $currImage.fadeOut(2000,function(){$(this).remove();});
        }).attr('src',$e_next.find('img').attr('alt'));
    }
};
    //$(window).resize(function() {
    //    //$('#thumbScroller .container').css('left',sliderLeft); //without easing
    //    $('#bwpg_thumbScroller .bwpg_container').stop().animate({left: sliderLeft}, 400,'easeOutCirc'); //with easing
    //    $('#bwpg_thumbScroller').css('width',$(window).width()-padding);
    //    sliderWidth=$(window).width()-padding;
    //});
