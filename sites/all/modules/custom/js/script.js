/**
 * Created by neo on 18.11.13.
 */
(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        var $basket = $('#edit-cart-contents');
        var $address = $('#edit-customer-profile-billing');
        var $delivery = $('#edit-delivery');

        $basket.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/bas.png" />' +
            '<p class="title">Shopping Basket</p></div>');
        $address.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/mail.png" />' +
            '<p class="title">Address Details</p></div>');
        $delivery.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/clock.png" />' +
            '<p class="title">Delivery Date</p></div>');

        $('#edit-customer-profile-billing').append($('#customer-profile-shipping-ajax-wrapper'));
        $('#edit-customer-profile-billing').append($('#edit-buttons'));

       // $('#edit-continue').attr('value', 'CONTINUE');

        //$('#edit-back').attr('value', 'Cancel');
    });
})(jQuery, Drupal, this, this.document);