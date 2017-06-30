<?php
// $contents = file_get_contents("https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/sigrl/00000BCD");
// print_r($contents);
// $base = "https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/sigrl/";
// $gid = "00000BCD";
// $url = $base.$gid;
// echo $url."<br>";
header("content-type:application/json");
error_reporting(E_ALL);

if ($_POST) {
	$gid = $_POST["GID"];
	// $gid = "00000BCD";
	$base = "https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/sigrl/";
	$url = $base.$gid;
	// echo $url;
	$handle = curl_init($url);
	curl_setopt($handle, CURLOPT_RETURNTRANSFER, TRUE);
	curl_setopt($handle, CURLOPT_SSLCERT, getcwd()."/IAS-cURL/bob_pfx.pem");
	// echo getcwd()."/IAS-cURL/bob_pfx.pem"."<br>";
	curl_setopt($handle, CURLOPT_VERBOSE, 1);
	$response = curl_exec($handle);
	// print curl_error($handle);
	/* Check for 404 (file not found). */
	$httpcode = curl_getinfo($handle, CURLINFO_HTTP_CODE);
	// echo "http status code is ".$httpcode."<br>";

	$curlinfo = curl_getinfo($handle);
	// echo "curl content-length is <br>";

	// print_r($curlinfo['download_content_length']);

	if ($httpcode == 200) {
		$contentLength = $curlinfo['download_content_length'];
		$result = "0";
		$sig_rl_size = 0;
		if ($contentLength !=0) {
			$sig_rl = $response;
			$sig_rl_size = $contentLength;
		}
	}else{
		$result = "1";
	}

	curl_close($handle);
}
$res = array('result' => $result, 'sig_rl_size' => $sig_rl_size, 'sig_rl' => $sig_rl);
echo json_encode($res);
?>


