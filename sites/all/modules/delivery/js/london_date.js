/**
 * @file
 * scripts for show/hide selectbox
 * @param {type} $
 * @param {type} Drupal
 * @param {type} window
 * @param {type} document
 * @param {type} undefined
 * @returns {undefined}
 */
(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        var currentDay = new Date();
        var day = currentDay.getDay();
        var hours = currentDay.getHours();
        var day = currentDay.getDay();
        if (hours < 13) {
            $('#edit-delivery').find("input").datepicker({
                minDate: 0,
                dateFormat: 'dd M y',
                changeMonth: false,
                changeYear: false,
                beforeShowDay: function(date) {
                    var currentDay = new Date();
                    var day = currentDay.getDay();
                    var hours = currentDay.getHours();
                    var day = date.getDay();
                    return [day !== 0 && day !== 6, ""];
                }
            });
        }
        if (hours >= 13) {
            $('#edit-delivery').find("input").datepicker({
                minDate: '+1D',
                dateFormat: 'dd M y',
                changeMonth: false,
                changeYear: false,
                beforeShowDay: function(date) {
                    var currentDay = new Date();
                    var day = currentDay.getDay();
                    var hours = currentDay.getHours();
                    var day = date.getDay();
                    return [day !== 0 && day !== 6, ""];
                }
            });
        }

        var value = $('#edit-delivery-delivery-date option');

        $('select#edit-delivery-delivery-date').removeAttr('disabled');

    });

})(jQuery, Drupal, this, this.document);

