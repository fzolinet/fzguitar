<?php
echo "\n\n\n";
echo "*******************************\n";
echo "* Make SmplPhotoalbum installer\n";
echo "*******************************\n";
echo "\n=> Bootloader Drupal 7\n";

$fz_dir = getcwd();
chdir("G:/wwwroot/fw/");
define('DRUPAL_ROOT', getcwd());
$_SERVER['REMOTE_ADDR']="127.0.0.1";
require_once (DRUPAL_ROOT."./includes/bootstrap.inc");
error_reporting(E_ALL ^ E_NOTICE);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
die('itt');
error_reporting(E_ALL ^ E_NOTICE);
chdir($fz_dir);
//
define("MODULE","fzguitar");
$file = MODULE.".info";
echo "=> Read ".$file."\n";
$infotxt = file_get_contents($file);

$info = system_get_info("module",MODULE);
$ver  = $info["version"];
$datestamp = $info["datestamp"];

$a = preg_split("/\./",$ver);
$newver = 1+ (int)($a[2]);

$fver = "7.x-1.".$newver;
$infotxt = preg_replace('/\"7\.x\-1\..+\"/','"7.x-1.'.$newver.'"', $infotxt);

$datestamp = mktime();
$infotxt = preg_replace('/datestamp = \".+\"/','datestamp = "'.$datestamp.'"', $infotxt);
//
echo "=> Backup the old ".MODULE.".info file to ".$file.".bak\n";
rename($file,$file.".bak");

echo "=> Save new ".MODULE.".info file\n";
file_unmanaged_save_data($infotxt,$file,FILE_EXISTS_RENAME);

define("TARGET","E:\\".MODULE);

mkdir(TARGET);

print "=> Copy files into ".TARGET."\n";
system('xcopy *.* '.TARGET.' /S /Y /EXCLUDE:m.excl');

$mdlgz  =   MODULE.'-'.$fver.'.tar.gz';
$mdlzip =  MODULE.'-'.$fver.'.zip ';

print "=> Make E:\\".MODULE."-$fver.tar.gz\n";
system('cmd /c .make\\tar\\bsdtar -cf E:\\'.$mdlgz.' -z '.TARGET);

print "=> Make E:\\".MODULE."-$fver.zip\n";
system('cmd /c .make\\zip\\zip -r -9 -q E:\\'.$mdlzip.' '.TARGET);

print "=> Delete temporary files and folders: ".TARGET."\n";
file_unmanaged_delete_recursive(TARGET);

rmdir(TARGET);
$dest = 'H:\\Backup\\_Elo_Fejlesztes\\'.MODULE.'\\'.date('Ymd')."\\";
print "=> Make Backup area: ".$dest;
mkdir($dest);

print "=> Copy E:\\".$mdlgz." => Backup area\n";
system('cmd /c copy E:\\'.$mdlgz.' '.$dest);

print "=> Copy E:\\".$mdlzip." => Backup area\n";
system('cmd /c copy E:\\'.$mdlzip.' '.$dest);

print "=> Make Zip on Backup area\n";
system('cmd /c .make\\zip\\zip -r -9 -q '.$dest.MODULE.' .');
print "=> Copy files => ftp://www.fzolee.hu/web/download/".MODULE."/ \n";

// set up basic connection
$conn_id = ftp_connect("www.fzolee.hu");

$login_result = ftp_login($conn_id, 'fzolee.hu_postmaster', 'SargaRigo99');

if($login_result)
{
    ftp_pasv($conn_id,true);

    // upload a file
    if (ftp_put($conn_id, '/web/download/'.MODULE.'/'.$mdlgz, "E:\\".$mdlgz, FTP_BINARY)) {
     echo "  => Successfully uploaded ".$mdlgz."\n";
    } else {
     echo "  => There was a problem while uploading ".$mdlgz."\n";
    }

    if (ftp_put($conn_id, '/web/download/'.MODULE.'/'.$mdlzip, "E:\\".$mdlzip, FTP_BINARY)) {
     echo "  => successfully uploaded ".$mdlzip."\n";
    } else {
     echo "  => There was a problem while uploading ".$mdlzip."\n";
    }
    // close the connection
    ftp_close($conn_id);
}
echo "=> End of compiling the module: ".MODULE."\n\n";
?>