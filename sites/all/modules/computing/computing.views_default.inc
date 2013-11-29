<?php

/**
 * Implements hook_views_default_views().
 */
function computing_views_default_views() {
  $view = _computing_default_view();
  $views = array (
    $view->name => $view,
  );
  return $views;
}


/**
 * @return view exported from Views UI.
 */
function _computing_default_view() {
  $view = new view;
  $view->name = 'computing_record';
  $view->description = 'A view to show all computing records';
  $view->tag = 'computing';
  $view->base_table = 'computing_record';
  $view->human_name = 'Computing record report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Computing record report';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer site configuration';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Hybrid Computing: Uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'computing_record';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Relationship: Hybrid Computing: Nid */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'computing_record';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 0;
  /* Field: Hybrid Computing: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'computing_record';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['id']['hide_alter_empty'] = 1;
  /* Field: Hybrid Computing: Application */
  $handler->display->display_options['fields']['app']['id'] = 'app';
  $handler->display->display_options['fields']['app']['table'] = 'computing_record';
  $handler->display->display_options['fields']['app']['field'] = 'app';
  $handler->display->display_options['fields']['app']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['app']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['app']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['app']['alter']['external'] = 0;
  $handler->display->display_options['fields']['app']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['app']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['app']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['app']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['app']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['app']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['app']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['app']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['app']['alter']['html'] = 0;
  $handler->display->display_options['fields']['app']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['app']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['app']['hide_empty'] = 0;
  $handler->display->display_options['fields']['app']['empty_zero'] = 0;
  $handler->display->display_options['fields']['app']['hide_alter_empty'] = 1;
  /* Field: Hybrid Computing: Command */
  $handler->display->display_options['fields']['command']['id'] = 'command';
  $handler->display->display_options['fields']['command']['table'] = 'computing_record';
  $handler->display->display_options['fields']['command']['field'] = 'command';
  $handler->display->display_options['fields']['command']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['command']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['command']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['command']['alter']['external'] = 0;
  $handler->display->display_options['fields']['command']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['command']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['command']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['command']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['command']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['command']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['command']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['command']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['command']['alter']['html'] = 0;
  $handler->display->display_options['fields']['command']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['command']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['command']['hide_empty'] = 0;
  $handler->display->display_options['fields']['command']['empty_zero'] = 0;
  $handler->display->display_options['fields']['command']['hide_alter_empty'] = 1;
  /* Field: Hybrid Computing: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'computing_record';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['description']['alter']['external'] = 0;
  $handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['description']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['description']['alter']['html'] = 0;
  $handler->display->display_options['fields']['description']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['description']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['description']['hide_empty'] = 0;
  $handler->display->display_options['fields']['description']['empty_zero'] = 0;
  $handler->display->display_options['fields']['description']['hide_alter_empty'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = 'Node Title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Hybrid Computing: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'computing_record';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Hybrid Computing: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'computing_record';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  /* Field: Hybrid Computing: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'computing_record';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  $handler->display->display_options['fields']['weight']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['external'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['weight']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['weight']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['weight']['alter']['html'] = 0;
  $handler->display->display_options['fields']['weight']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['weight']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['weight']['hide_empty'] = 0;
  $handler->display->display_options['fields']['weight']['empty_zero'] = 0;
  $handler->display->display_options['fields']['weight']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['weight']['format_plural'] = 0;
  /* Field: Hybrid Computing: Start timestamp */
  $handler->display->display_options['fields']['start']['id'] = 'start';
  $handler->display->display_options['fields']['start']['table'] = 'computing_record';
  $handler->display->display_options['fields']['start']['field'] = 'start';
  $handler->display->display_options['fields']['start']['label'] = 'Started';
  $handler->display->display_options['fields']['start']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['start']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['start']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['start']['alter']['external'] = 0;
  $handler->display->display_options['fields']['start']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['start']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['start']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['start']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['start']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['start']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['start']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['start']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['start']['alter']['html'] = 0;
  $handler->display->display_options['fields']['start']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['start']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['start']['hide_empty'] = 0;
  $handler->display->display_options['fields']['start']['empty_zero'] = 0;
  $handler->display->display_options['fields']['start']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['start']['date_format'] = 'short';
  /* Field: Hybrid Computing: End timestamp */
  $handler->display->display_options['fields']['end']['id'] = 'end';
  $handler->display->display_options['fields']['end']['table'] = 'computing_record';
  $handler->display->display_options['fields']['end']['field'] = 'end';
  $handler->display->display_options['fields']['end']['label'] = 'Ended';
  $handler->display->display_options['fields']['end']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['end']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['end']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['end']['alter']['external'] = 0;
  $handler->display->display_options['fields']['end']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['end']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['end']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['end']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['end']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['end']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['end']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['end']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['end']['alter']['html'] = 0;
  $handler->display->display_options['fields']['end']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['end']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['end']['hide_empty'] = 0;
  $handler->display->display_options['fields']['end']['empty_zero'] = 0;
  $handler->display->display_options['fields']['end']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['end']['date_format'] = 'short';
  /* Field: Hybrid Computing: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'computing_record';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['message']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['message']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['message']['alter']['external'] = 0;
  $handler->display->display_options['fields']['message']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['message']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['message']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['message']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['message']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['message']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['message']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['message']['alter']['html'] = 0;
  $handler->display->display_options['fields']['message']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['message']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['message']['hide_empty'] = 0;
  $handler->display->display_options['fields']['message']['empty_zero'] = 0;
  $handler->display->display_options['fields']['message']['hide_alter_empty'] = 1;
  /* Field: Hybrid Computing: Updated timestamp */
  $handler->display->display_options['fields']['updated']['id'] = 'updated';
  $handler->display->display_options['fields']['updated']['table'] = 'computing_record';
  $handler->display->display_options['fields']['updated']['field'] = 'updated';
  $handler->display->display_options['fields']['updated']['label'] = 'Updated';
  $handler->display->display_options['fields']['updated']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['external'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['updated']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['updated']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['updated']['alter']['html'] = 0;
  $handler->display->display_options['fields']['updated']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['updated']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['updated']['hide_empty'] = 0;
  $handler->display->display_options['fields']['updated']['empty_zero'] = 0;
  $handler->display->display_options['fields']['updated']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['updated']['date_format'] = 'short';
  /* Sort criterion: Hybrid Computing: ID */
  $handler->display->display_options['sorts']['id']['id'] = 'id';
  $handler->display->display_options['sorts']['id']['table'] = 'computing_record';
  $handler->display->display_options['sorts']['id']['field'] = 'id';
  $handler->display->display_options['sorts']['id']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/reports/computing';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Computing record report';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  return $view;
}