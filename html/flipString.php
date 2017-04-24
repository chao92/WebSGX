<?php
function flipString($input){
	$arr_string = str_split($input,2);
	$arr_fliped = array();
	
	foreach ($arr_string as $num) {
		array_push($arr_fliped,$num[1]);
		array_push($arr_fliped,$num[0]);
	}
	// print_r($arr_fliped);
	return implode('', $arr_fliped);
}

function changeString($input){
	$arr_string = str_split($input,2);
	$arr_fliped = array();
	for ($i= count($arr_string)-1;$i>=0 ; $i--){ 
      // echo $arr_string[$i];
      array_push($arr_fliped,$arr_string[$i]);
    } 
	
	// print_r($arr_fliped);
	return implode('', $arr_fliped);
}
$input = "72a828d7fbce6bad2fa98f3d26d1a3cd253500e12fd02382673cb899cde5b88d";
echo "input"."<br/>";
echo $input;
echo "<br/>";
echo "flipped"."<br/>";
echo flipString($input);
echo "<br/>";
echo "reversed"."<br/>";
echo strrev($input);
echo "<br/>";
echo "changed"."<br/>";
echo changeString($input);
?>

