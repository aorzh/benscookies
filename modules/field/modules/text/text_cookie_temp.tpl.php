<style>
    .cookie-selector-info {
        padding: 1em;
    }
    .cookie-wrapper {
        
    }
    .cookie-item {
        float:left;
        font-size: 2em;
        text-align: center;
    }
    .small-title-cookie {
        font-size:12px;
    }
    
</style>
<pre>
<?php
//foreach(get_defined_vars() as $k=>$v) echo $k.'<br>';
//echo $field_cookie_capacity;
  ?>
</pre>
<br />
<div id="cookie-selector">
    <input type="radio" name="box-type" id="radio-box-type-pres" value="1"> Buy a preselected package <br />
    <input type="radio" name="box-type" id="radio-box-type-sel" value="2"> Select the cookies you want <br />
    
    <div id="cookie-selector-block" style="display: none;">
        <div class="cookie-selector-info">
            You have <span id="cookie-selector-num-left">0</span> cookies left.
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
            
            foreach ($items as $c) { ?>
            <div class="cookie-item">
                
                <img src="<?php echo file_create_url ($c->field_image_main['und'][0]['uri']); ?>" style="width:50px;height:50px;">
                <br />
                <a href="#" class="cookie-remove-btn" num="<?php echo $c->vid; ?>">-</a>
                <span class="small-title-cookie">&nbsp; <?php echo $c->title ?> &nbsp;</span>
                <a href="#" class="cookie-add-btn" num="<?php echo $c->vid; ?>">+</a>
                <br />
                <p class="cookie-counter" num="<?php echo $c->vid; ?>">0</p>
            </div>
            <?php } ?> 
            
            <div class="clearfix"></div>
        </div>
    </div>
    <input type="hidden" id="cookie-input" name="line_item_fields[field_cookie_selector_1][und][0][value]" />
    
</div>


<script type="text/javascript">
    jQuery(document).ready(function ($) {
    
    var cookieMax = 8; 
    
    
    function GetAllCookies()
    {
        var sum = 0;
        $('.cookie-counter').each(function() {
            sum += parseInt($(this).text());
        });
        return sum;
    }
    
    function UpdateNumLeft()
    { 
        $('#cookie-selector-num-left').html((cookieMax-GetAllCookies()));
    }
    
    UpdateNumLeft();
    
    function UpdateInput()
    {
        var obj = [];
        
        $(".cookie-counter").each(function() {
            obj.push( { vid: $(this).attr('num'), count: parseInt( $(this).text()) } );
        });
        
        $('#cookie-input').val( JSON.stringify(obj) );
    }
     
    
    //adding a cookie
    $(".cookie-add-btn").click(function(event) {
        event.preventDefault();
        if (GetAllCookies()<cookieMax)
        {
            var num = $(this).attr("num");
            var count = parseInt($('.cookie-counter[num="'+num+'"]').text());
            $('.cookie-counter[num="'+num+'"]').text(count+1);
            UpdateNumLeft();
            UpdateInput();
        }
    });
    
    //removing a cookie
    $(".cookie-remove-btn").click(function() {
        event.preventDefault();
        var num = $(this).attr("num");
        var count = parseInt($('.cookie-counter[num="'+num+'"]').text());
        if (count>0)
        {
            $('.cookie-counter[num="'+num+'"]').text(count-1); 
            UpdateNumLeft();
            UpdateInput();
        }
    });
     
    $("#radio-box-type-sel").click(function() {  
        $("#cookie-selector-block").show();
    });
    $("#radio-box-type-pres").click(function() { 
        $("#cookie-selector-block").hide();
    });
    
    });
    
</script>