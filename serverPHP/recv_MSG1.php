<?php
header("content-type:application/json");
header('Access-Control-Allow-Origin: https://192.168.1.202', false);
error_reporting(E_ALL);

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
	$username = $_POST['username'];
	$password = $_POST['password'];
	// $private_key = $_POST['private_key'];
	$SSLSetting = $_POST['SSL'];
	// $res = array('username' => $username, 'password' => $password);
	// echo json_encode($res);
	$service_port = 7891;
	$address = "127.0.0.1";

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Unable to create socket\n");

    // socket_set_nonblock($socket) or die("Unable to set nonblock on socket\n");
	if ($socket === false){
		echo "Failed: ". socket_strerror(socket_last_error($socket))."\n";
	}

	// echo "Attempting to connect to '$address' on port '$service_port'...'"."<br />";
	$result = socket_connect($socket, $address, $service_port);

	if ($result === false){
		echo "Failed: ". socket_strerror(socket_last_error($socket))."\n";
	}

	// $GLOBALS['socket'] = $socket;


	// sending msg1 type to server
	$msg1_type = 0;
	socket_send($socket, pack("I",$msg1_type), 4, MSG_WAITALL);

	$out = '';

	// echo "Sending...". "<br/>";  
	socket_send($socket, $SSLSetting, strlen($SSLSetting), MSG_WAITALL);

	// echo "Reading response:"."<br/>";
    // time.sleep(2);
	if (false !== ($bytes = socket_recv($socket, $out, 5, MSG_WAITALL))) {
		// echo "Read $bytes bytes from socket_recv()...".$out."<br/>";

	} else {
		echo "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}

	if (strcmp($out,"auth")) {
		// echo "send out username and password"."<br/>";
		$size_d = 2 + strlen($username) + strlen($password);
        // echo "size is".$size_d;
        // echo PHP_INT_SIZE;
		socket_send($socket, pack("I",$size_d), 4, MSG_WAITALL);//32
        // echo "username length is ".strlen($username);
		socket_send($socket, $username."\0", strlen($username)+1, MSG_WAITALL);
		socket_send($socket, $password."\0", strlen($password)+1, MSG_WAITALL);
	}

	// echo "CLIENT: Authentication Pass!"."<br/>";
	// $segment_length = 0;
	// if (false !== ($bytes = socket_recv($socket, $segment_length, 4, MSG_WAITALL))) {
	// 	// echo "Receiving Segment Length from socket_recv()...";
	// 	// print_r(unpack("I",$segment_length)[1]);
	// 	// echo "<br/>";
 //        // echo unpack("int",$out);

	// } else {
	// 	echo "Receiving Segment Length socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	// }
	// $compress_flag = 0;
	// if (false !== ($bytes = socket_recv($socket, $compress_flag, 4, MSG_WAITALL))) {
	// 	// echo "Receiving Compression Flag  from socket_recv()...";
	// 	// print_r(unpack("I",$compress_flag)[1]);
	// 	// echo "<br/>";

	// } else {
	// 	echo "Receiving Compression Flag socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	// }
	// $summary_report = 0;
	// if (false !== ($bytes = socket_recv($socket, $summary_report, 4, MSG_WAITALL))) {
	// 	// echo "Receiving Report Summary from socket_recv()...";
	// 	// print_r(unpack("I",$summary_report)[1]);
	// 	// echo "<br/>";

	// } else {
	// 	echo "Receiving Report Summary socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	// }
    // socket_close($socket);
	// echo "CLIENT: Start Attestation"."<br/>";
	$msg1_length = 0;
	if (false !== ($bytes = socket_recv($socket, $msg1_length, 4, MSG_WAITALL))) {
		// echo "Receiving MSG1 length from socket_recv()...";
		// print_r(unpack("I",$msg1_length)[1]);
		// echo "<br/>";

	} else {
		echo "Receiving MSG1 socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}
	$MSG1;
	// echo "MSG1 body is"."<br/>";
	if (false !== ($bytes = socket_recv($socket, $MSG1, unpack("I",$msg1_length)[1], MSG_WAITALL))) {
		// echo "Receiving MSG1  from socket_recv()...";
		// echo bin2hex($MSG1);
		// echo "<br/>";

	} else {
		echo "Receiving socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
	}

    // construct MSG2

    // 1. extract ga_x, ga_y from MSG1 in the format of hex
	// $ga_x = strrev(substr(bin2hex($MSG1),0,64));
	// $ga_y = strrev(substr(bin2hex($MSG1),64,64));
	// $GID = strrev(substr(bin2hex($MSG1),128,8));

	// $ga_x = substr(bin2hex($MSG1),0,64);
	// $ga_y = substr(bin2hex($MSG1),64,64);
	// $GID = substr(bin2hex($MSG1),128,8);

	$ga_x = changeString(substr(bin2hex($MSG1),0,64));
	$ga_y = changeString(substr(bin2hex($MSG1),64,64));
	$GID = changeString(substr(bin2hex($MSG1),128,8));

	// $ga_x = flipString(substr(bin2hex($MSG1),0,64));
	// $ga_y = flipString(substr(bin2hex($MSG1),64,64));
	// $GID = flipString(substr(bin2hex($MSG1),128,8));
	
	$res = array('ga_x' => $ga_x, 'ga_y' => $ga_y, 'GID'=> $GID);

	socket_shutdown($socket, 2);
    socket_close($socket);
    unset($socket);
	echo json_encode($res); 
	// echo "ga_x: is ".$ga_x."<br/>"."ga_y is ".$ga_y."<br/>";

    // echo "close connection";
}
?>