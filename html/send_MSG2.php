<?php
header("content-type:application/json");
error_reporting(E_ALL);

sleep(5);

if ($_POST) {
	$service_port = 7891;
	$address = "127.0.0.1";

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Unable to create socket\n");

    // socket_set_nonblock($socket) or die("Unable to set nonblock on socket\n");
	if ($socket === false){
		$err1 = "Failed: ". socket_strerror(socket_last_error($socket))."\n";
	}

	// echo "Attempting to connect to '$address' on port '$service_port'...'"."<br />";
	$result = socket_connect($socket, $address, $service_port);

	if ($result === false){
		$err2 = "Failed: ". socket_strerror(socket_last_error($socket))."\n";
	}

	$res = array('err1' => $err1, 'err2' => $err2);

	// $socket = $GLOBALS['socket'];

	$msg2_length = $_POST['length'];
	// echo "Sending...". "<br/>";
	socket_send($socket, pack("I",$msg2_length), 32, MSG_WAITALL);

	$msg2_hexstring = $_POST['msg2'];

	$msg2_bin = hex2bin($msg2_hexstring);

	socket_send($socket, $msg2_bin, $msg2_length, MSG_WAITALL);

	echo json_encode($res); 

}
?>