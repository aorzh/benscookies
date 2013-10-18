//edit-giftcard-giftcard-cards //form-item-giftcard-giftcard-print
(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        $('#edit-giftcard-giftcard-cards').find('label').each(function() {
            $(this).append('<div class="div-label ' + $(this).attr("for") + '">' + $(this).text() + '</div>');
        });
        $('#edit-giftcard').find('.grippie').removeClass('grippie');
        $('.form-item-giftcard-giftcard-pane-field-display').append(
                '<div class="cart-sample"><p>Standart Card Sample</p><div class="cart-sample-img"></div></div>'
                );
        $('.page-checkout-giftcard').find('#page-title').prepend('<div class="box"></div>');

    });
})(jQuery, Drupal, this, this.document);


