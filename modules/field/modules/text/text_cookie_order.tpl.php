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
    
</style>

<br />
<div> 
    <div id="cookie-selector-block" >
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
                <p class="cookie-counter" num="<?php echo $c->vid; ?>">0</p>
            </div>
            <?php } ?> 
            
            <div class="clearfix"></div>
        </div>
    </div> 
    
</div>


<script type="text/javascript">
    jQuery(document).ready(function ($) {
    
    });
    
</script>