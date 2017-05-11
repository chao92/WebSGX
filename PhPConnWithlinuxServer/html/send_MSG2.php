<?php
header("content-type:application/json");
error_reporting(E_ALL);

sleep(5);

// msg1 put 1 to the file stands for current client is under attestation
// if(file_get_contents("./attestating.txt") == "1")
// {
//   exit(1);
// }
// else
// {
//   file_put_contents("./attestating.txt", "1");
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

	// sending msg1 type to server
	$msg2_type = 1;
	socket_send($socket, pack("I",$msg2_type), 4, MSG_WAITALL);

	$client_id = $_POST["client_id"];
	socket_send($socket, pack("I",$client_id), 4, MSG_WAITALL);


	$msg2_length = $_POST['length'];
	// echo "Sending...". "<br/>";
	socket_send($socket, pack("I",$msg2_length), 4, MSG_WAITALL);

	$msg2_hexstring = $_POST['msg2'];

	$msg2_bin = hex2bin($msg2_hexstring);

	socket_send($socket, $msg2_bin, $msg2_length, MSG_WAITALL);



	// // receiving msg3

	// sleep(10);

	// $msg3_length = 0;
	// if (false !== ($bytes = socket_recv($socket, $msg3_length, 4, MSG_WAITALL))) {
	// 	// echo "Receiving Segment Length from socket_recv()...";
	// 	// print_r(unpack("I",$segment_length)[1]);
	// 	// echo "<br/>";
 //        // echo unpack("int",$out);

	// } else {
	// 	$err3 = "Receiving msg3 Length socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	// }

	// $msg3;

	// if (false !== ($bytes = socket_recv($socket, $msg3, unpack("I",$msg3_length)[1], MSG_WAITALL))) {
	// 	// echo "Receiving MSG1  from socket_recv()...";
	// 	// echo bin2hex($MSG1);
	// 	// echo "<br/>";

	// } else {
	// 	$err4 =  "Receiving socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	// }


	$res = array('err1' => $err1, 'err2' => $err2);
	echo json_encode($res); 

}
?>