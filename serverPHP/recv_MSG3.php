<?php
header("content-type:application/json");
header('Access-Control-Allow-Origin: https://192.168.1.202', false);
error_reporting(E_ALL);

sleep(5);

// msg1 put 1 to the file stands for current client is under attestation
// if(file_get_contents("./attestating.txt") == "1")
// {
//   exit(1);
// }
// else
// {
//   file_put_contents("./working.txt", "1");
// }
// msg3 put 0 to the file stands for current client passed the attestation

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

	// sending msg3 type to server
	$msg3_type = 2;
	socket_send($socket, pack("I",$msg3_type), 4, MSG_WAITALL);

	$client_id = $_POST["client_id"];
	socket_send($socket, pack("I",$client_id), 4, MSG_WAITALL);

	// receiving msg3
	$msg3_length = 0;
	if (false !== ($bytes = socket_recv($socket, $msg3_length, 4, MSG_WAITALL))) {
		// echo "Receiving Segment Length from socket_recv()...";
		// print_r(unpack("I",$segment_length)[1]);
		// echo "<br/>";
        // echo unpack("int",$out);

	} else {
		$err3 = "Receiving msg3 Length socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}

	$msg3;

	if (false !== ($bytes = socket_recv($socket, $msg3, unpack("I",$msg3_length)[1], MSG_WAITALL))) {
		// echo "Receiving MSG1  from socket_recv()...";
		// echo bin2hex($MSG1);
		// echo "<br/>";

	} else {
		$err4 =  "Receiving socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}

	// msg3 {mac, M}
	$msg3_mac = substr(bin2hex($msg3),0,32);
	$msg3_message = substr(bin2hex($msg3),32,2904);




	$res = array('err1' => $err1, 'err2' => $err2, 'err3' => $err3, 'err4' => $err4, 'length' => unpack("I",$msg3_length),'MSG3' => bin2hex($msg3), 'msg3_mac' => $msg3_mac, 'msg3_message' => $msg3_message);

	// update two files
	// file_put_contents("./attestating.txt", "0"); // indicating no one is attestating now
	// $left_attestation_clients = intval(file_get_contents("/isAttestationPassed.txt"));
	// if($left_attestation_clients){
	// 	$left_attestation_clients =-1 ;
	// 	file_put_contents("/isAttestationPassed.txt",strval($left_attestation_clients));
	// }

	echo json_encode($res); 

}
?>