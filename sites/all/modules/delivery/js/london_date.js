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
        // if now more than 15pm we will disable current day
        $('#datepicker-schedule').find("input").datepicker({
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
        var currentDay = new Date();
        var day = currentDay.getDay();
        var hours = currentDay.getHours();
        var day = currentDay.getDay();
        var value = $('#edit-delivery-delivery-date option');
       
        

        // $('#edit-delivery-delivery-date');

    });

})(jQuery, Drupal, this, this.document);

