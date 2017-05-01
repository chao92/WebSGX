<?php
error_reporting(E_ALL);
// phpinfo();// 5.5.9
$length = 5;
// $packed_length = pack("n", $length);
// print_r($length);
// echo gettype($packed_length);
// echo strlen($packed_length);
// var_dump($packed_length);
// echo PHP_INT_MAX;
// $value = PHP_INT_MAX;
// $highMap = 0xffffffff00000000; 
// $lowMap = 0x00000000ffffffff; 
// $higher = ($value & $highMap) >>32; 
// $lower = $value & $lowMap; 
// $packed = pack('NN', $higher, $lower); 
// var_dump($packed);
$binarydata = pack("nvc*", 0x1234, 0x5678, 65, 66);
echo $binarydata;
?>