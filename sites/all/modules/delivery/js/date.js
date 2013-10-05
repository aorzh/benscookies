/**
 * This number relates to which day of the week you want to disable.
    Sunday = 0
    Monday = 1
    Tuesday = 2
    Wednesday = 3
    Thursday = 4
    Friday = 5
    Saturday = 6
 * @param {type} $
 * @param {type} Drupal
 * @param {type} window
 * @param {type} document
 * @param {type} undefined
 * @returns {undefined}
 */
(function($, Drupal, window, document, undefined) {
    $(document).ready(function() {
        $("input").datepicker({
            beforeShowDay: function(date) {
                var day = date.getDay();
                return [day !== 1 && day !==0 && day !==6, ""];
            }
        });

    });

})(jQuery, Drupal, this, this.document);
