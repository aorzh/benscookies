<div id="product-block">
<article<?php print $attributes; ?>>
<?php print $user_picture; ?>
<?php print render($title_prefix); ?>
<?php if (!$page && $title): ?>
    <header>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    </header>
<?php endif; ?>
<?php print render($title_suffix); ?>
<?php if ($display_submitted): ?>
    <footer class="submitted">
        <?php print $submitted; ?>
    </footer>
<?php endif; ?>

<div<?php print $content_attributes; ?>>
    <?php
    // We hide the comments and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    ?>
<div id="product_display_left" style="width: 56%; float:left"><?php print render($content['product:field_images']);?></div>  
<div id="product_display_right" style="width: 43%; float:right">
    <?php if ($title): ?>
          <h1<?php print $title_attributes; ?>>
            <a href="<?php print $node_url; ?>" rel="bookmark">
              <?php print $title; ?>
            </a>
          </h1>
        <?php endif; ?>
    <?php print render($content['product:commerce_price']);?>
    <?php print render($content['body']); ?>
    <?php print render($content['field_product']); ?>
  
    



    <input type="hidden" id="cookie-input" name="line_item_fields[field_cookie_selector_1][und][0][value]"/>

</div>
    
    
</div>





</div>


<div class="clearfix">
    <?php if (!empty($content['links'])): ?>
        <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
</div>
   </article>
</div>
<div id="cookie_product_bar">

    <div id-"cookie_product_bar_inner" >
        <div class="cookie-selector-info" style="display: none">
            <div id="cookies-info-bar" style="display:none">
            You have <span id="cookie-selector-num-left">0</span> cookies left to pick
            </div>            
            <div id="cookies-info-bar-1" style="display:none">
            You have selected Ben's Assortment.
            </div>
    </div>
            
                <div id="cookie-selector-block" style="display: none;">

        <div class="cookie-wrapper">
            <?php

            $query = new EntityFieldQuery();

            $query->entityCondition('entity_type', 'node');
            $query->entityCondition('bundle', 'cookie_type');
            $result = $query->execute();

            if (isset($result['node'])) {
                $items_nids = array_keys($result['node']);
                $items = entity_load('node', $items_nids);
            }

            foreach ($items as $c) {
                ?>
                <div class="cookie-item">
                    <div class ="image-cook">
                        <img src="<?php echo file_create_url($c->field_image_main['und'][0]['uri']); ?>"
                            style="width:50px;height:50px;">
                    </div>
                    <div class="small-title-cookie">&nbsp; <?php echo $c->title ?> &nbsp;</div>
                    <div class="selectors">
                        <div class="cookie-remove-btn" num="<?php echo $c->vid; ?>">-</div>
                        <div class="cookie-counter" num="<?php echo $c->vid; ?>">0</div>
                        <div class="cookie-add-btn" num="<?php echo $c->vid; ?>">+</div>
                    </div>

                </div>
            <?php } ?>

            <div class="clearfix"></div>
        </div>
        <div class="add-to"></div>
    </div>
            
            
        </div>    

</div>

















<?php
$nid = arg(1);
$node = node_load($nid);
$node_wrapper = entity_metadata_wrapper('node', $node);
$id = $node_wrapper->field_product->value();
$product = commerce_product_load($id[0]->product_id);
$product_wrapper = entity_metadata_wrapper('commerce_product', $product);
$display = $product_wrapper->field_number_of_cookies->value();
?>
<script type="text/javascript">
    jQuery(document).ready(function ($) {

        $('.form-item-quantity').hide();
        $('input#edit-submit').addClass('none');
        $(".title-wrap img").click(function () {
            $('.form-item-quantity').show();
            $('#edit-submit').show();
            $(this).prev().attr('checked', true);
            var $val = $(this).prev().val();
            if ($val == 1) {
                $('#cookie-selector-block').slideUp();
                $('input#edit-submit').removeClass('none');
                $('input#edit-submit').removeAttr('style');
                $('.form-item-quantity').removeClass('to-top');
                $('input#edit-submit').removeClass('to-top');
                $('.cookie-selector-info').slideDown();
                $('#cookies-info-bar').slideUp();
                $('#cookies-info-bar-1').slideDown();
                $('#cookies-info-bar-1').append($('#edit-submit'));
                $('#cookies-info-bar-1').append($('.form-item-quantity'));

            }
            if ($val == 2) {
                $('.form-item-quantity').addClass('to-top');
                $('input#edit-submit').addClass('to-top');
                $('.disabled-bnt').attr('style', 'background-color: darkgray; cursor: default; pointer-events: none;');

                $('.pick').show();
                $('.switch').show();
                $('#block-views-other-products-block').hide();
                $('.cookie-selector-info').slideDown();
                $('#cookies-info-bar').slideDown();
                $('#cookie-selector-block').slideDown();
                $('input#edit-submit').removeClass('none');
                $('html,body').animate({scrollTop: $(window).scrollTop() + 200});
                $('#cookies-info-bar').append($('#edit-submit'));
                $('#cookies-info-bar').append($('.form-item-quantity'));
                $('#cookies-info-bar-1').slideUp();
            }
        });
        $('.switch').click(function(){
            $('.commerce-product-field-commerce-price').show();
            $('.field-name-body').show();
            $('.title-wrap').show();
            $('.title-cookie').show();
            $('.pick').hide();
            $('.switch').hide();
            $('#cookie-selector-block').slideUp();
            $('#block-views-other-products-block').show();
            $('input#edit-submit').addClass('none');
            $('.form-item-quantity').hide();
            $('#cookie-selector-info').slideUp();
            $('#cookies-info-bar').slideUp();
            
        });




        $('.commerce-add-to-cart').find('#edit-submit').addClass('disabled-bnt');
        $('.disabled-bnt').attr('style', 'background-color: darkgray; cursor: default; pointer-events: none;');

        var cookieMax = <?php echo $display; ?>;

        function GetAllCookies() {
            var sum = 0;
            $('.cookie-counter').each(function () {
                sum += parseInt($(this).text());
            });
            return sum;
        }

        function UpdateNumLeft() {
            $('#cookie-selector-num-left').html((cookieMax - GetAllCookies()));
        }

        UpdateNumLeft();

        function UpdateInput() {
            var obj = [];

            $(".cookie-counter").each(function () {
                obj.push({ vid: $(this).attr('num'), count: parseInt($(this).text()) });
            });

            $('#cookie-input').val(JSON.stringify(obj));
        }


        //adding a cookie
        $(".cookie-add-btn").click(function (event) {
            event.preventDefault();
            if (GetAllCookies() < cookieMax) {
                var num = $(this).attr("num");
                var count = parseInt($('.cookie-counter[num="' + num + '"]').text());
                $('.cookie-counter[num="' + num + '"]').text(count + 1);
                UpdateNumLeft();
                UpdateInput();
            }
            if ((GetAllCookies() - cookieMax) == 0) {
                $('.commerce-add-to-cart').find('#edit-submit').removeClass('disabled-bnt');
                $('.commerce-add-to-cart').find('#edit-submit').removeAttr('style');
                $('html,body').animate({scrollTop: $(window).scrollTop() - 420});
            }
        });

        //removing a cookie
        $(".cookie-remove-btn").click(function (event) {
            event.preventDefault();
            var num = $(this).attr("num");
            var count = parseInt($('.cookie-counter[num="' + num + '"]').text());
            if (count > 0) {
                $('.cookie-counter[num="' + num + '"]').text(count - 1);
                UpdateNumLeft();
                UpdateInput();
            }
            if ((GetAllCookies() - cookieMax) != 0) {
                $('.commerce-add-to-cart').find('#edit-submit').addClass('disabled-bnt');
                $('.commerce-add-to-cart').find('#edit-submit').attr('style', 'background-color: darkgray; cursor: default; pointer-events: none;');
            }
        });

        $("#radio-box-type-sel").click(function () {
            $("#cookie-selector-block").show();
        });
        $("#radio-box-type-pres").click(function () {
            $("#cookie-selector-block").hide();
        });


    });

</script>

