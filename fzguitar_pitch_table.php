<?php
function fzguitar_pitch_table(){
return
array(
  array('ID' => 'GUITAR' ,'descr' => 'guitar with 6 strings and normal tunes','pitch'  => 'E|A|D|G|H|E1'),
  array('ID' => 'GUITARD','descr' => 'guitar with 6 strings and with D tunes ','pitch' => 'D0|A|D|G|H|D1'),
  array('ID' => 'BASS'   ,'descr' => 'bassguitar with 4 strings','pitch'               => 'E|A|D|G'),
  array('ID' => 'BASS5'  ,'descr' => 'bassguitar with 5 strings','pitch'               => 'H|E|A|D|G'),
  array('ID' => 'BASS6'  ,'descr' => 'bassguitar with 6 strings','pitch'               => 'H|E|A|G|C')
);
}