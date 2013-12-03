<div id="cookie-selector">
    <div class="title-cookie">How would you like them?</div>
    <div class="title-wrap">
        <label for="1"><input type="radio" name="box-type" id="radio-box-type-pres" value="1"><img src="/sites/default/files/val1.jpg"></label>
        <label for="2"><input type="radio" name="box-type" id="radio-box-type-sel" value="2"><img src="/sites/default/files/val2.jpg"></label>
    </div>


    


    <input type="hidden" id="cookie-input" name="line_item_fields[field_cookie_selector_1][und][0][value]"/>

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