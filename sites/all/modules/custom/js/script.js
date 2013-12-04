/**
 * Created by neo on 18.11.13.
 */
(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        var $basket = $('#edit-cart-contents');
        var $address = $('#edit-account');
        var $delivery = $('#edit-delivery');

        $basket.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/bas.png" />' +
            '<p class="title">Shopping Basket</p></div>');
        $address.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/mail.png" />' +
            '<p class="title">Address Details</p></div>');
        $delivery.prepend('<div class="checkout-logo"><img id="basket-img" src="/sites/all/themes/bens_cookies/images/clock.png" />' +
            '<p class="title">Delivery Date</p></div>');


        
        
        $('#edit-account').append($('#edit-customer-profile-shipping'));
        $('#edit-customer-profile-shipping').append($('#customer-profile-billing-ajax-wrapper'));
        $('#edit-customer-profile-shipping').append($('#edit-buttons'));
        
        $('#edit-commerce-payment').append($('#edit-buttons'));
        
        $('#edit-delivery').append($('#edit-commerce-shipping'));
        $('#edit-commerce-shipping').append($('#edit-buttons'));
        
        
        
        $( "legend:contains('Account information')" ).css( "display", "none" )
        $( "legend:contains('Account information')" ).css( "display", "none" )
        


       // $('#edit-continue').attr('value', 'CONTINUE');

        //$('#edit-back').attr('value', 'Cancel');
    });
})(jQuery, Drupal, this, this.document);