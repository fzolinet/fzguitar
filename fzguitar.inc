<?php
/**
 *
 * @author Administrator
 *
 */
class FZGuitar{
  public $img;
  private $p;
  private $dx;  //width of image
  private $dy;  //height of image
//Todo:  Modify----Tuning and instrument----
  private $notes = array('e','E','a','A','d','D','g','G','h','H','b','B','e1','E1');
//Todo:  Modify----Tuning and instrument----
  private $s     = array();
  
  function __construct(){
     $path = drupal_get_path('module', 'fzguitar');
//Todo:  Modify----Tuning and instrument----
     $p = $path ."/images/guitarneck.png";

     $size = getimagesize($p);
     $this->dx = $size[0];
     $this->dy = $size[1];
     $this->type = $size['mime'];
  }
    
  function Shows($m,$dx = 0, $dy= 0 ){
     $text="";
     $url =  drupal_get_path('module', 'fzguitar'). "/view.php?";
     $p = explode('|',$m);
     $par= "";
     foreach($p As $i => $e){
        if(strstr($e,"text")){
          $text = str_replace(array('text','='),'',$e);
        }else{
          
        }
     }
//Todo:  Modify----Tuning and instrument----
     $m = str_replace("#","!-!",$m);
     $url.= "p=".$m;
     $out ='<img src="'.$url.'" height="'.$this->dy.'px" width="'.$this->dx.'px" style="fzguitar"  alt="'.$text.'"/>';
     
     return $out;
  }
}

/**
 * Class of showing neck andsigns of guitar
 */
class FZG {
  private $dx0 = 33;
  private $dx1 = 50;
  private $dx  = 65;
  private $dy  = 139;
  private $fsize = 14;
  private $font = 'arial.ttf';
  private $fontbuilt = 4;
  private $r1 = 0;        //rgb default => blue
  private $g1 = 0;
  private $b1 = 128;
  
  private $r2 = 128;      //rgb second => red
  private $g2 = 0;
  private $b2 = 0;
  
  private $s     = Array();    // frets on the strings
  private $ys    = Array(140, 117, 95, 71, 48, 25);      //position of strings
  private $def   = Array(161 ,139, 117, 95, 72, 49, 27);
  private $pitch = Array('E','A','D','G','H','E1');
  private $notes    = Array();
  private $noteaut  = False;      //calculate automatic the pitches on strings
  private $defnotes = array('c','c#','d','d#','e','f','f#','g','g#','a','b','h');
  private $fdx   = 8;     // width of characters
  private $chord_write_out=false;
  private $chord ="";
    
  /**
   * initialize the class
   * @param string $get - parameters of showing neck of guitar and signs on it
   */
  function __construct($get)
  {
    $get=str_replace("!-!","#",$get);
    if (strstr("%",$get)){
      $get = html_entity_decode($get);
    }

    $str = 'E';
    // setting the colors
    $this->set_rgb();
    $rgb2 = $this->match("rgb2",$get);;
    if($rgb2){
       $this->RGB($rgb2,2);
    }
    // setting the colors
    $rgb = $this->match("rgb",$get);;
    if($rgb){
        $this->RGB($rgb);
    }
  
    // Pitch - with name
    $pitch = $this->match("pitch", $get);
    if($pitch){
      $this->Pitch($pitch);
    }
    // Instrument - with name, tunes
    $instr = $this->match("instr", $get);
    if($instr){
      $this->Instr($instr);
    }

    $this->noteaut = ($this->match("noteaut", $get) || variable_get('fzguitar_noteaut', false) );
    if($this->noteaut ){
      $this->Note();
    }

    //Chords
    $chord = $this->match("chord", $get);
    if($chord){
      $this->chord = $chord;
      $get .= $this->Chord($chord,$instr);
      $this->chord_write_out = variable_get('fzguitar_chord',true);
    }
        
    $this->match("fzguitar", $get);
    // From here the concrete sounds
    $get = str_replace("||","|",$get);
    $p = explode("|",$get);
        
  
    foreach($p AS $i => $e){
      if(!empty($e)){
        if(strstr($e,'=')){                // Is there string sign?
          $v = explode("=",$e);
          $str = strtoupper($v[0]);       // Witch string
          $loc = (int) $v[1];
          if(in_array($str, $this->pitch )){
            if($str =='B') $str = 'H';
          }
        }else{                                // There is not string sign then the use last string sign
          $loc = (int) $e;
        }
        $db = count($this->s[$str]);
        $this->s[$str][$db]['loc'] = $loc;    // here must to write out the string
        preg_match('/\D.*/i',$e,$m);
        $txt = $m[0];
        if($txt=='x') $txt = "X";
        $this->s[$str][$db]['txt'] = $txt;    // This sign and text is written out to the neck
      }
    }
  }
    
  /**
  *  Shows the neck of guitar and the signs
  */
  function Shows()
  {
    $uj = version_compare(PHP_VERSION,"5.4.0",">=");
    $db = count($this->pitch);
    $this->font = dirname(realpath(__FILE__))."/resource/arial.ttf";
    $path  = drupal_get_path('module', 'fzguitar'). "/images/guitarneck".$db.".png";
    $img   = imagecreatefrompng($path);
    $rgb1  = imagecolorallocate($img, $this->r1, $this->g1, $this->b1);
    $rgb2  = imagecolorallocate($img, $this->r2, $this->g2, $this->b2);
    $black = imagecolorallocate($img, 0,0,0);

    //Write out the name of string to the end of neck
    foreach($this->pitch AS $i =>$e){
      $y = $this->ys[$i];
      if($uj){
        imagettftext($img, $this->fsize , 0, 2, $y, 1 , $this->font, $e );
      } else{
        imagestring($img, 5 , 2, $y-15, $e, $rgb1 );
      }
    }
    //
    $j = 0;
    
    unset($this->s['TEXT']);

    foreach( $this->s AS $str => $signs){ // 
      $y = $this->ys[$j];
      foreach($signs AS $i => $sign){
        $txt = $sign['txt'];
        $loc = $sign['loc'];
        if($txt == 'X' || $loc == 0){ //If the plucking string is not the 0. bar
          $x = $this->dx0;
        }else{
          $x = $this->dx1 + (int) ($loc-1) * $this->dx;
        }        
        //Automatic write out the name of notes to the neck
        if(  ( $this->noteaut ) && ($txt != 'X')){
          if( $loc > 0 ){
            $txt = $this->notes[$j][$loc];
          }else{
            
          }
        }
        //Set the color
        if( stripos($txt,"[rgb2]") === false ){
          $rgb = $rgb1;
        }else{
          $txt = str_ireplace("[rgb2]","",$txt);
          $rgb = $rgb2;
        }
        //
        $ps = strpos($txt,"=");
        if($ps !== false && $ps >0 ){
          $txt = substr($txt,$ps+2);
        }
        //Do not write out the number of finger
        if(!variable_get("fzguitar_finger",false)){
          $txt = preg_replace("/\(\d\)/","",$txt);          
        }
        //Do not write out the direction of picking
        if(!variable_get("fzguitar_picking",false)){
          $txt = str_replace('↓',"",$txt);
          $txt = str_replace('↑',"",$txt);
        }
        // image manipulation
        if($uj ){
          if( $txt !="X" ){     
            $l = strlen($txt)*2;
            imagefilledellipse($img , $x +($this->fsize / 2), $y-($this->fsize / 2)-1, $this->fsize, $this->fsize, $rgb);
            imagettftext($img, $this->fsize , 0, $x +20, $y, $rgb, $this->font, $txt );
          }else{
            imagettftext($img, $this->fsize , 0, $x - 10, $y, $rgb, $this->font, $txt );
          }
        }else{
          if( $txt !="X" ){
            $fntdx = imagefontwidth($this->fontbuilt);
            $fntdy = imagefontheight($this->fontbuilt)-4;
            $gdx   = -1;
            $d     = strpos($txt,'↓');
            if( $d !== false ){
              $txt = str_replace('↓'," ",$txt);
              $gdx = $d * $fntdx;
            }

            $gux   = -1;
            $u     = strpos($txt,'↑');
            if( $u !== false ){
              $txt = str_replace('↑'," ",$txt);
              $gux = $u * $fntdx;
            }

            imagefilledellipse($img , $x + ($fntdx/2)-1, $y-($fntdy/2)-1, $fntdx, $fntdy, $rgb);
            imagestring($img, $this->fontbuilt , $x +  $fntdx, $y-15, $txt, $rgb );
            if( $d !== false || $u !== false){
              $this->Arrow($img, $x , $y, $gdx, $gux, $rgb);
            }
          }else{
            imagestring($img, 5 , $x-10  , $y-15, $txt, $rgb );
          }   //'X'
                    
        } //PHP version if
      }//foreach
      $j++;
    }//foreach

    //Chord write out on the fret
    if($this->chord_write_out){
      $cx = $this->dx1 +10*$this->dx;
      $cy = ( reset($this->ys) + end($this->ys) ) / 2;
      if($uj){
        imagettftext($img, $this->fsize+6 , 0, $cx , $cy, $rgb, $this->font, "Chord: ".$this->chord );
      }else{
        imagestring($img, 5,  $cx , $cy-15,  "Chord: ".$this->chord, $rgb);
      }
    }    
    return $img;
  }

    /**
     * Draw an Arrow
     * @param resource $img        Image
     * @param int $x          x coordinate
     * @param int $y          y coordinate
     * @param int $gdx         down arrow
     * @param int $gux         up arrow
     * @param RGB $textcolor
     */
    function Arrow(&$img, $x, $y, $gdx = 0, $gux = 0, $textcolor){
        if ( $gdx > -1 ){
            $dx = $x + 10+ $gdx;
            imagefilledrectangle($img,    $dx-1 , $y-15, $dx  , $y-3 , $textcolor);
            imagefilledpolygon($img,array($dx-3 , $y-3,  $dx+2, $y-3 , $dx, $y + 0), 3, $textcolor );
        }
        if ( $gux > -1 ){
            $dx = $x +10+$gux;
            imagefilledrectangle($img,    $dx-1 , $y-12, $dx  , $y   , $textcolor);
            imagefilledpolygon($img,array($dx-3 , $y-12, $dx+2, $y-12, $dx, $y-15), 3, $textcolor );
        }
    }
    // It searches the parameters
    function match($p,&$txt){
        $s = "/".$p."\=([^|]*)(\|)*/i";
        $db = preg_match($s,$txt,$match);
        if($db>0){
            $m = $match[0];
            $m = str_replace("|","",$m);
            $v=explode("=",$m);
            $out = $v[1];
            $txt= preg_replace($s,"",$txt);
            return $out;
        }
        return false;
    }
        
    /**
     * Prepare to show an accord from the database
     * @param string $chord - name of chord in database, let's see the readme.txt
     * @param string $instr - name of instrument in database, let's see the readme.txt
     */
    function Chord($chord, $instr){
        $chord = strtoupper($chord);

        //query the tones of chord
        if(!$instr) $instr ="GUITAR";
        $q = db_select("fzguitar","g")
              ->fields('g',array("tones"))
              ->condition("pitchID",$instr,"=")
              ->condition("name",$chord,"=");

        $res = $q->execute()->fetchAssoc();
        
        $res['tones'] = str_replace(",","|",$res['tones']);
        $tones = explode('|',$res['tones']);

        // query the pitch of instrument
        $q1 = db_select('fzguitar_pitch','t')
                ->fields("t",array("pitch"))
                -> condition("ID",$instr,"=");
        $res1 = $q1->execute()->fetchAssoc();

        //old version default separatecharacter
        $res1['pitch'] = str_replace(",","|",$res1['pitch']);
        
        $pitch = explode('|',$res1['pitch']);
        $get = "";
        foreach($pitch AS $i => $s){
            $get .= "|".$s."=".$tones[$i];
        }
        return $get;
    }
   

    /**
     * Settings the colors
     * @param string $col - "r,g,b" or #rrggbb
     */
    function RGB($col, $c=''){
        if(strpos($col,"#") == 1){
            if( strlen($col) >3 )  $rgb[0] = hexdec(substr($col,1,2));
            if( strlen($col) >5 )  $rgb[1] = hexdec(substr($col,3,2));
            if( strlen($col) >7 )  $rgb[2] = hexdec(substr($col,5,2));
        }else{
            $rgb = explode(",",$col);
        }
       if(isset($rgb[0]) && is_integer($rgb[0]) ) {
           if($c==2) {
               $this->r2 = $rgb[0];
           }else{
               $this->r1 = $rgb[0];
           }
       }
       if(isset($rgb[1]) && is_integer($rgb[1]) ){
           if($c==2) {
               $this->g2 = $rgb[1];
           }else{
               $this->g1 = $rgb[1];
           }
       }
       if(isset($rgb[2]) && is_integer($rgb[2]) ){
           if($c==2) {
               $this->b2 = $rgb[2];
           }else{
               $this->b1 = $rgb[2];
           }
       }
    }
    
    function set_rgb(){
        $rgb1 = variable_get("fzguitar_rgb1","#000080");
        $this->r1 = hexdec(substr($rgb1,1,2));
        $this->g1 = hexdec(substr($rgb1,3,2));
        $this->b1 = hexdec(substr($rgb1,5,2));
        $rgb2 = variable_get("fzguitar_rgb2","#800000");
        $this->r2 = hexdec(substr($rgb2,1,2));
        $this->g2 = hexdec(substr($rgb2,3,2));
        $this->b2 = hexdec(substr($rgb2,5,2));
    }

    /**
     * simple tunes of a guitar
     * @param string $tunes
     */
    function Pitch($pitch){
        $pitch = strtoupper( $pitch );
        $str   = explode(",",$pitch);
        $db          = count($this->def);
        $this->ys    = Array();
        $this->pitch = Array();
        $i0 = $db - count($str);
        foreach($str AS $i =>$e){
           if($e == 'B') $e = 'H';
           $this->ys[$i] = $this->def[$i0 + $i ];       // y coordinates
           $this->pitch[$i]= $e;
        }
    }

    /*
    * Call the tunes from database
    * @param string $instr - name of instrument in the database, let' see the readme.txt
    */
    function Instr($instr){
        $q = db_select("fzguitar_pitch","t")
            ->fields('t',array('pitch'))
            ->condition('ID',$instr,'=');
       
        $result = $q->execute()->fetchAssoc();
        $pitch = $result["pitch"];
        
        $this->Pitch($pitch);
   }
   
   /**
    * calculate the pitches on the strings
    */
   function Note(){
       $db  = count($this->pitch);
       $dbn = count($this->defnotes);
       for($i=0; $i<$db; $i++ ){
           $st = strtolower(str_replace(array('0','1','2','3','4','5','6','7','8','9'),'', $this->pitch[$i]));
           $this->notes[$i][0] = $st;
           $bkp =$this->notes[$i][0];
           for($j = 1; $j<13; $j++){
               $k = ( ( array_search($bkp,$this->defnotes) + 1 ) % $dbn);
               $this->notes[$i][$j] = $this->defnotes[$k];
               $bkp = $this->notes[$i][$j];
           }
       }
   }
}