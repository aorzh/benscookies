<div class="pick" style="display: none;">Pick your oun Cookies</div>
<div class="switch" style="display: none;">Switch to pre-selected assortment</div>
<div id="cookie-selector">
    <div class="title-cookie">How would you like them?</div>
    <div class="title-wrap">
        <label for="1"><input type="radio" name="box-type" id="radio-box-type-pres" value="1"><img src="/sites/default/files/val1.jpg"></label>
        <label for="2"><input type="radio" name="box-type" id="radio-box-type-sel" value="2"><img src="/sites/default/files/val2.jpg"></label>
    </div>

    <div id="cookie-selector-block" style="display: none;">
        <div class="cookie-selector-info">
            You have <span id="cookie-selector-num-left">0</span> cookies left to pick
        </div>
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
                $('#cookie-selector-block').hide();
                $('input#edit-submit').removeClass('none');
                $('input#edit-submit').removeAttr('style');
                $('.form-item-quantity').removeClass('to-top');
                $('input#edit-submit').removeClass('to-top');

            }
            if ($val == 2) {
                $('.form-item-quantity').addClass('to-top');
                $('input#edit-submit').addClass('to-top');
                $('.disabled-bnt').attr('style', 'background-color: darkgray; cursor: default; pointer-events: none;');
                $('.commerce-product-field-commerce-price').hide();
                $('.field-name-body').hide();
                $('.title-wrap').hide();
                $('.title-cookie').hide();
                $('.pick').show();
                $('.switch').show();
                $('#block-views-other-products-block').hide();
                $('article > .container-24:last-child').append($('#cookie-selector-block'));
                $('#cookie-selector-block').show();
                $('input#edit-submit').removeClass('none');
            }
        });
        $('.switch').click(function(){
            $('.commerce-product-field-commerce-price').show();
            $('.field-name-body').show();
            $('.title-wrap').show();
            $('.title-cookie').show();
            $('.pick').hide();
            $('.switch').hide();
            $('#cookie-selector-block').hide();
            $('#block-views-other-products-block').show();
            $('input#edit-submit').addClass('none');
            $('.form-item-quantity').hide();
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