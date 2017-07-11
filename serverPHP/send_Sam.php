<?php
header("content-type:application/json");
header('Access-Control-Allow-Origin: https://192.168.1.202', false);
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

	// sending msg4 type to server
	$sam_type = 4;
	socket_send($socket, pack("I",$sam_type), 4, MSG_WAITALL);

	$client_id = $_POST["client_id"];
	socket_send($socket, pack("I",$client_id), 4, MSG_WAITALL);

	

	// $socket = $GLOBALS['socket'];
	$out='';
	// send request msg4 to client
	if (false !== ($bytes = socket_recv($socket, $out, 5, MSG_WAITALL))) {
		// echo "Read $bytes bytes from socket_recv()...".$out."<br/>";

	} else {
		$err3 = "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}

	if (strcmp($out,"data")) {
		$out = 'samfile';
		$sam_length=$_POST['length'];
		// $highMap = 0xffffffff00000000; 
		// $lowMap = 0x00000000ffffffff; 
		// $higher = ($msg4_length & $highMap) >>32; 
		// $lower = $msg4_length & $lowMap; 
		// $packed = pack('NN', $higher, $lower); 
		$packed_length = pack("I",$sam_length);

		socket_send($socket, $packed_length, 4, MSG_WAITALL);

		$sam_hexstring = $_POST['sam'];

		$sam_bin = hex2bin($sam_hexstring);

		socket_send($socket, $sam_bin, $sam_length, MSG_WAITALL);
	}


	$res = array('err1' => $err1, 'err2' => $err2, 'err3' => $err3, 'signal'=>$out);
	echo json_encode($res); 

}
?>