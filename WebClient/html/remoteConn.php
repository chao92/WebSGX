<?php
// if(!defined('MSG_DONTWAIT')) define('MSG_DONTWAIT', 0x40);
if (isset($_GET['submit'])){
    $SSLSetting = $_GET['askSSL'];
    echo "receiving from client that SSL Setting is ".$SSLSetting."<br />";
    error_reporting(E_ALL);
    $service_port = 7891;
    $address = "127.0.0.1";

    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Unable to create socket\n");

    // socket_set_nonblock($socket) or die("Unable to set nonblock on socket\n");
    if ($socket === false){
        echo "Failed: ". socket_strerror(socket_last_error($socket))."\n";
    }

    echo "Attempting to connect to '$address' on port '$service_port'...'"."<br />";
    $result = socket_connect($socket, $address, $service_port);

    if ($result === false){
        echo "Failed: ". socket_strerror(socket_last_error($socket))."\n";
    }

    $out = '';

    echo "Sending...". "<br/>";  
    socket_send($socket, $SSLSetting, strlen($SSLSetting), MSG_WAITALL);

    echo "Reading response:"."<br/>";
    // time.sleep(2);
    if (false !== ($bytes = socket_recv($socket, $out, 5, MSG_WAITALL))) {
        echo "Read $bytes bytes from socket_recv()...".$out."<br/>";

    } else {
        echo "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }

    if (strcmp($out,"auth")) {
        echo "send out username and password"."<br/>";
        $username="user1";
        $password="13313";
        $size_d = 2 + strlen($username) + strlen($password);
        // echo "size is".$size_d;
        // echo PHP_INT_SIZE;
        socket_send($socket, pack("I",$size_d), 32, MSG_WAITALL);
        // echo "username length is ".strlen($username);
        socket_send($socket, $username."\0", strlen($username)+1, MSG_WAITALL);
        socket_send($socket, $password."\0", strlen($password)+1, MSG_WAITALL);
    }

    echo "CLIENT: Authentication Pass!"."<br/>";
    $segment_length = 0;
    if (false !== ($bytes = socket_recv($socket, $segment_length, 4, MSG_WAITALL))) {
        echo "Receiving Segment Length from socket_recv()...";
        print_r(unpack("I",$segment_length)[1]);
        echo "<br/>";
        // echo unpack("int",$out);

    } else {
        echo "Receiving Segment Length socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }
    $compress_flag = 0;
    if (false !== ($bytes = socket_recv($socket, $compress_flag, 4, MSG_WAITALL))) {
        echo "Receiving Compression Flag  from socket_recv()...";
        print_r(unpack("I",$compress_flag)[1]);
        echo "<br/>";

    } else {
        echo "Receiving Compression Flag socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }
    $summary_report = 0;
    if (false !== ($bytes = socket_recv($socket, $summary_report, 4, MSG_WAITALL))) {
        echo "Receiving Report Summary from socket_recv()...";
        print_r(unpack("I",$summary_report)[1]);
        echo "<br/>";

    } else {
        echo "Receiving Report Summary socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }
    // socket_close($socket);
    echo "CLIENT: Start Attestation"."<br/>";
    $msg1_length = 0;
    if (false !== ($bytes = socket_recv($socket, $msg1_length, 4, MSG_WAITALL))) {
        echo "Receiving MSG1 length from socket_recv()...";
        print_r(unpack("I",$msg1_length)[1]);
        echo "<br/>";

    } else {
        echo "Receiving MSG1 socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }
    $MSG1;
    echo "MSG1 body is"."<br/>";
    if (false !== ($bytes = socket_recv($socket, $MSG1, unpack("I",$msg1_length)[1], MSG_WAITALL))) {
        echo bin2hex($MSG1);
        echo "Receiving MSG1  from socket_recv()... with Binary with length is"."<br/>";
        echo $MSG1;

        echo "<br/>";
        echo "using unpack"."<br/>";
        print_r(unpack("H",$MSG1));

    } else {
        echo "Receiving socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
    }

    // construct MSG2

    // 1. extract ga_x, ga_y from MSG1
    $ga_x = substr(bin2hex($MSG1),0,64);
    // echo "ga _x based 64"."<br/>";
    // echo base64_encode($ga_x);
    $ga_y = substr(bin2hex($MSG1),64,64);
    // echo "ga _y based 64"."<br/>";
    // echo base64_encode($ga_y);
    echo "ga_x: is ".$ga_x."<br/>"."ga_y is ".$ga_y."<br/>";

    // echo "close connection";
}
?>  

<html>
<head>
</head>
<body>
    <form action="remoteConn.php" method="get">
        Ask SSL?: <input type="text" name="askSSL" placeholder="SSL! or nSSL">
        <button type="submit" name="submit">Submit!</button>
        
    </form>
    <button id="A_btn_gen_ECCDH_keypair" >Generate Keypair</button>
    <p>Public Key: <span id="A_ECCDH_public_key" class="ciphertext">-</span></p>
    <p>Private Key: <span id="A_ECCDH_private_key" class="ciphertext">-</span></p>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <script src="js/bootstrap.min.js"></script>

    <!-- test web crypto  -->
    <script type="text/javascript" src="js/webcrypto.js"></script>
    <script type="text/javascript" src="js/client_main.js"></script>
    <script type="text/javascript" src="js/attestation_client.js"></script>
    
    <!-- end of web crypto -->
    <script type="text/javascript">
        $('#A_btn_gen_ECCDH_keypair').click(function(event) {
            var crypto = window.crypto || window.msCrypto;
            if (crypto.subtle) {
                promise_key = crypto.subtle.generateKey({name: "ECDH", namedCurve: "P-256",}, true, ["deriveKey", "deriveBits"]);
                promise_key.then(function(key) {
                    A_private_key_object = key.privateKey;
                    A_public_key_object = key.publicKey;

                    exported_public_key = crypto.subtle.exportKey("jwk", A_public_key_object);//base64url encoding
                    exported_public_key.then(function(key) {
                                //returns the exported key data
                                
                                var plainKey = "<br> x: " + Base64URL2HEX(key.x) +"<br>"+ "y: " + Base64URL2HEX(key.y);
                                A_public_key_x_exported = hexStringToByte(Base64URL2HEX(key.x));
                                A_public_key_y_exported = hexStringToByte(Base64URL2HEX(key.y));

                                $('#A_ECCDH_public_key').html(plainKey);

                                
                            });
                    exported_public_key.catch = function(e) {
                        console.log(e.message);
                    }

                    exported_priv_key = crypto.subtle.exportKey("jwk", A_private_key_object);//base64url encoding
                    exported_priv_key.then(function(key) {
                                //returns the exported key data
                                console.log("private key");

                                // A_private_key_exported = hexStringToByte(Base64URL2HEX(key.d));

                                $('#A_ECCDH_private_key').html(Base64URL2HEX(key.d));
                                
                            });
                    exported_priv_key.catch = function(e) {
                        console.log(e.message);
                    }
                });
                promise_key.catch = function(e) {
                    console.log(e.message);
                }
            } else {
                alert("Cryptography API not Supported");
            }

        });
    </script>
</body>
</html>