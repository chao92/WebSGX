<?php

header("content-type:application/json");
error_reporting(E_ALL);

if ($_POST) {
	$quote = $_POST["quote"];
	// $quote="AgABAG4NAAAEAAQAAAAAACpXw+ZXPX6y1W3iHPtsATIAAAAAAAAAAAAAAAAAAAAABAQC//8BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABwAAAAAAAAAHAAAAAAAAAL1voRS5YFOAWdFIncdvAccxo6wo6UMiFLJEEapu49c6AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACD1xnnferKFHD2uvYqTXdDA8iZ22kCD5xw7h38CMfOngAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACjNuL/aehiKvT9mHP2ZH+nExDKLMWUm5uL9Qp4uRf47wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAqAIAAJp8A76k14xtFfImN34UwSTY7wkp60YEMfbQQnrETWhSH4EdvAklnkWRCoa3l2kSfXfIemLDBNeHHNFu78Y6Mf0Lx7GGJ9/VVIiOnwAvRE12148xG4+LdDeZgA8F0UMGw8OkFoM99uTpHBePXXCkVklUvQ6KtJ6v9cVtitOSHM8tpnIUKMkTDJqBkukYc6h3QIV/Vh3l5eP9FOyYHMPXpBAXP2Fle9yfg7QkTZMR7V3aY99HC6+m/kzqvBBLwpiVziTaVtSrvd61F4DG0Z0K6BEWxzR8QpsX5zYut1y9lM1RdJ3KmRATEkFSlxzIxcxuhSbhmKtXG+zEyV+/byyhTM9q8Kido4uldEBBO8Af/87DzgXgFDdPIwOwBna644rtl5Do34xMag95M3n5NWgBAAB0bR4MF7B/wGpLIwPeof2g/+n/pEewscf/vruo4lTDic7fZp1Ou34BeoTApLr7R5PE8JuFtW7vrB2b4tdGuFNdBtAPeuVTamJeZLyQvY8aewKGPSTMkMyiUVm8uscdUxbgC/3A/ODVzjruAT7wqqjY+kb4JhGWklz3dpVfi465CXww/6avlY2KrTs0dRVe/pXJbRcZkG1BrlPXgDhQTzwWGxwTuJPDArRS831YP9gUQY+BVrMLx3LhiZpuvYQLTJwYaedmcDfMOielpSatghOTw/40C/uL4SN0OV/JbPYadGjbuPGzjK4JUP9+LSoF/D5F+rIIuiBY1d+aC8IC3Bj32ts9uohB6iqTxAi3tyNKX9d1tvOJo8yjCB1Vnf2QNj7VIfpOUPANI9GItgacrSL/BEuIVhypLQSXIoQOdG605G+lA+2+XH5FvW0lAgwoDCFBKvLHb/7vH1NA+iKFLJ4/6rbxtTmAEICQ1nFwGaqegtUcuKilfFJi";
	$data = array('isvEnclaveQuote' => $quote);
	$url = "https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/report";
	$handle = curl_init($url);
	curl_setopt($handle, CURLOPT_RETURNTRANSFER, TRUE);
	curl_setopt($handle, CURLOPT_SSLCERT, getcwd()."/IAS-cURL/bob_pfx.pem");
	curl_setopt($handle, CURLOPT_POST, true);
	curl_setopt($handle, CURLOPT_POSTFIELDS, $data);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array(
		'Content-Type: application/json'
		));
	// echo getcwd()."/IAS-cURL/bob_pfx.pem"."<br>";
	curl_setopt($handle, CURLOPT_VERBOSE, 1);
	$response = curl_exec($handle);
	// print curl_error($handle);
	/* Check for 404 (file not found). */
	$httpcode = curl_getinfo($handle, CURLINFO_HTTP_CODE);

	$curlinfo = curl_getinfo($handle);
	// print_r($curlinfo);
	// print_r($response);
	// echo "curl content-length is <br>";

	// print_r($curlinfo['download_content_length']);

	if ($httpcode == 201) {
		$result = "0";
		
	}else{
		$result = "1";
	}

	curl_close($handle);
}
$res = array('result' => $result, 'quote' => $quote, 'attestation_report' => $response);
echo json_encode($res);
?>


