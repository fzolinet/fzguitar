<?php
while (!@stat('includes/bootstrap.inc')) {
    chdir('..');
}
define('DRUPAL_ROOT',getcwd());
require './includes/bootstrap.inc';
require './includes/file.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL); // See http://drupal.org/node/211378#comment-924059
$GLOBALS['devel_shutdown'] = FALSE;

//require "fzguitar.inc";
$p = isset($_GET["p" ]) ? $_GET["p"] : "x";

$g = new FZG($p);
$img = $g->Shows();
//imagepng($img, "E:/test.png");
header("Content-type: image/png");
imagepng($img);
imagedestroy($img);
die();