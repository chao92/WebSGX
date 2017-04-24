//****************************Context data structure used on SP side -- Start ****************************
var g_a;
var g_b;
var vk_key;		// Shared secret key for the REPORT_DATA
var mk_key;		// Shared secret key for generating MAC's
var sk_key;		// Shared secret key for encryption
var smk_key;	// Used only for SIGMA protocol
var b;
var ps_sec_prop;
//****************************Context data structure used on SP side -- End ****************************

//****************************Padding key for derive -- Start ****************************
var SAMPLE_RA_KEY_SMK = 0;
var SAMPLE_RA_KEY_SK = 1;
var SAMPLE_RA_KEY_MK = 2;
var SAMPLE_RA_KEY_VK = 3;
//****************************Padding key for derive -- End ****************************

//**************************** Simulated keys for test -- Start ****************************
var A_private_key_object = null, A_public_key_object = null;
var B_private_key_object = null, B_public_key_object = null;
var A_derived_key = null, B_derived_key=null;
var crypto = window.crypto || window.msCrypto;
var AES_CMAC_key = null;

var raw_shared_key = null;
//**************************** Simulated keys for test -- End  ****************************

var g_is_sp_registered = false;
var g_sp_credentials = 0;
var g_autentication_token = 0;
var attestation_report = null;
var g_spid;





//**************************** hash_buffer for deriving key -- Start  ****************************

var ID_U = "SGXRAENCLAVE";
var ID_V = "SGXRASERVER";

var ID_U_Array = new Uint8Array(ID_U);
var ID_V_Array = new Uint8Array(ID_V);

var counter = new Uint8Array(4);
var shared_secret = new Uint8Array(32);
var algorithm_id = new Uint8Array(4);

//**************************** hash_buffer for deriving key -- End  ****************************


//**************************** Variables for MSG2 -- Start  ****************************
var msg2_g_b;
var msg2_spid;
var msg2_quote_type;
var msg2_kdf_id;
var msg2_sign_gb_ga;
var msg2_mac;
var msg2_sig_rl_size;
var msg2_sig_rl;

var A_public_key_x_exported; // all unit8array[32]
var A_public_key_y_exported;
var B_public_key_x_exported;
var B_public_key_y_exported;
// var A_private_key_exported;
//**************************** Variables for MSG2 -- End    ****************************


// cloning an array
Array.prototype.clone = function() {
	return this.slice(0);
};

function attestation_client(g_sp_priv_key){

	// receive msg1
	var msg1;

	//save msg1->g_a to sp context data structure with g_a
	// g_a = p_msg1->g_a;

	// generate the sp ECCDH key pair and saved to sp context data structure
	var pub_key, priv_key;
	b = priv_key;
	g_b = pub_key;

	// generate shared secret by taking the public key passed by msg1 with it's self priv_key
	dh_key = comput_shared_dhkey(priv_key, g_a);

	// derive smk_key
	smk_key = derive_key(dh_key,SAMPLE_RA_KEY_SMK); //smk is only needed for msg2 generation.
	

	// The rest of the keys are the shared secrets for future communication.
	// derive mk_key
	mk_key = derive_key(dh_key, SAMPLE_RA_KEY_MK);
	
	//derive sk_key
	sk_key = derive_key(dh_key, SAMPLE_RA_KEY_SK);

	//derive vk_key
	vk_key = derive_key(dh_key, SAMPLE_RA_KEY_VK);

	// constructing msg2
	var msg2 = g_b + spid + signed(gb_ga)

	// Generate the CMACsmk for gb||SPID||TYPE||Sigsp(gb,ga)


	// verifying msg3

}


function A_comput_shared_dhkey(A_priv_key, B_pub_key){
	var crypto = window.crypto || window.msCrypto;
	if (crypto.subtle) {
		promise_key = crypto.subtle.deriveBits(
		{
			name: "ECDH",
			namedCurve: "P-256", //can be "P-256", "P-384", or "P-521"
        	public: B_pub_key, //an ECDH public key from generateKey or importKey
        },
    	A_priv_key,//your ECDH private key from generateKey or importKey
    	256);
		promise_key.then(function(bits) {
						//returns the exported key data
						// console.log("get private key")
						console.log("A comput shared key bits and reformateed it as Uint8Array");
						// console.log(keydata);
						// var plainKey =  key;
						// $('#A_ECCDH_private_key').html(key.d);
						// A_derived_key = keydata;
						A_derived_key = bits;
						// console.log(B_derived_key);
						console.log(new Uint8Array(A_derived_key));

						$('#A_ECCDH_shared_key').html(convertArrayBufferToHexaDecimal(A_derived_key));
						// return keydata;
					});
		promise_key.catch = function(e) {
			console.log(e.message);
		}
	} else {
		alert("Cryptography API not Supported");
	}
}

function B_comput_shared_dhkey(A_priv_key, B_pub_key){
	var crypto = window.crypto || window.msCrypto;
	if (crypto.subtle) {
		promise_key = crypto.subtle.deriveBits(
		{
			name: "ECDH",
			namedCurve: "P-256", //can be "P-256", "P-384", or "P-521"
        	public: B_pub_key, //an ECDH public key from generateKey or importKey
        },
    	A_priv_key,//your ECDH private key from generateKey or importKey
    	256);
		promise_key.then(function(bits) {
						//returns the exported key data
						// console.log("get private key")
						// console.log("derived shared key type is " + typeof(bits));
						console.log("B comput shared key bits and reformateed it as Uint8Array");
						B_derived_key = bits;
						// console.log(B_derived_key);
						console.log(new Uint8Array(B_derived_key));

						$('#B_ECCDH_shared_key').html(convertArrayBufferToHexaDecimal(B_derived_key));
					});
		promise_key.catch = function(e) {
			console.log(e.message);
		}
	} else {
		alert("Cryptography API not Supported");
	}
}


function derive_smk_sk_key(dh_key, key_id){// return corresponding derived key
	console.log("start derive key");
	counter[3] = key_id;
	shared_secret = dh_key;

	console.log("ID_V_Array length is "+ID_V_Array.byteLength);
	var input = new Uint8Array(counter.byteLength + shared_secret.byteLength + algorithm_id.byteLength + ID_V_Array.byteLength + ID_U_Array.byteLength);
	input.set(new Uint8Array(counter), 0);
	input.set(shared_secret, counter.byteLength);
	input.set(algorithm_id, counter.byteLength+shared_secret.byteLength);
	input.set(ID_V_Array, counter.byteLength+shared_secret.byteLength+algorithm_id.byteLength);
	input.set(ID_U_Array, counter.byteLength+shared_secret.byteLength+algorithm_id.byteLength+ID_V_Array.byteLength);
	console.log("Length of Input buffer is", + input.byteLength);
	console.log("Input buffer is");
	console.log(input);
	var crypto = window.crypto || window.msCrypto;
	if (crypto.subtle) {
		var promise_key = crypto.subtle.digest({name: "SHA-256"}, input);
		promise_key.then(function(result) {
						//returns the exported key data
						console.log("derived smk_key and sk_key")
						console.log(result);
						console.log("to hex:");
						var temp_str = convertArrayBufferToHexaDecimal(result);
						console.log(temp_str);
						// var temp_var = new Uint8Array(result);
						// console.log("key content is :");
						// console.log(temp_var);
						smk_key = temp_str.substr(0,32);
						sk_key = temp_str.substr(32,32);
						console.log(smk_key);
						console.log(sk_key);
						$('#A_ECCDH_derived_smk_key').html(smk_key);
						$('#A_ECCDH_derived_sk_key').html(sk_key);

					});
		promise_key.catch = function(e) {
			console.log(e.message);
		}
	} else {
		alert("Cryptography API not Supported");
	}
}

function derive_mk_vk_key(dh_key, key_id){// return corresponding derived key
	console.log("start derive key");
	counter[3] = key_id;
	shared_secret = dh_key;

	console.log("ID_V_Array length is "+ID_V_Array.byteLength);
	var input = new Uint8Array(counter.byteLength + shared_secret.byteLength + algorithm_id.byteLength + ID_V_Array.byteLength + ID_U_Array.byteLength);
	input.set(new Uint8Array(counter), 0);
	input.set(shared_secret, counter.byteLength);
	input.set(algorithm_id, counter.byteLength+shared_secret.byteLength);
	input.set(ID_V_Array, counter.byteLength+shared_secret.byteLength+algorithm_id.byteLength);
	input.set(ID_U_Array, counter.byteLength+shared_secret.byteLength+algorithm_id.byteLength+ID_V_Array.byteLength);
	console.log("Length of Input buffer is", + input.byteLength);
	console.log("Input buffer is");
	console.log(input);
	var crypto = window.crypto || window.msCrypto;
	if (crypto.subtle) {
		var promise_key = crypto.subtle.digest({name: "SHA-256"}, input);
		promise_key.then(function(result) {
						//returns the exported key data
						console.log("derived mk_key and vk_key")
						console.log(result);
						console.log("to hex:");
						var temp_str = convertArrayBufferToHexaDecimal(result);
						console.log(temp_str);
						// var temp_var = new Uint8Array(result);
						// console.log("key content is :");
						// console.log(temp_var);
						mk_key = temp_str.substr(0,32);
						vk_key = temp_str.substr(32,32);
						console.log(smk_key);
						console.log(sk_key);
						$('#A_ECCDH_derived_mk_key').html(mk_key);
						$('#A_ECCDH_derived_vk_key').html(vk_key);

					});
		promise_key.catch = function(e) {
			console.log(e.message);
		}
	} else {
		alert("Cryptography API not Supported");
	}

}

function generate_MSG2(){
	var msg2_g_b = A_public_key_object;
	var msg2_spid = g_spid; // need to be conact with ISV 
	var msg2_quote_type = 1;
	var msg2_sign_gb_ga;
	var msg2_mac;
	var msg2_sig_rl_size;
	var msg2_sig_rl;

}


// ECDSA_sign(A_private_key_exported,A_public_key_x_exported,A_public_key_y_exported,B_public_key_x_exported,B_public_key_y_exported);

function ECDSA_sign(g_sp_priv_key, sp_gb, ga){
	// size of gb is unit8_t gx[32], gy[32]
	var ga_gb = new Uint8Array(128);
	ga_gb.set(sp_gb, 0);
	ga_gb.set(ga, sp_gb.byteLength);
	if (crypto.subtle) {
		var promise_key = crypto.subtle.sign(
		{
			name: "ECDSA", hash: {name: "SHA-512"},//can be "SHA-1", "SHA-256", "SHA-384", or "SHA-512"
		}, 
			g_sp_priv_key, //from generateKey or importKey above
			ga_gb //ArrayBuffer of data you want to sign
			);
		promise_key.then(function(signature) {
						//returns the exported key data
						console.log("sign gb_ga");
						console.log(new Uint8Array(signature));
					});
		promise_key.catch = function(e) {
			console.log(e.message);
		}
	} else {
		alert("Cryptography API not Supported");
	}

}


function testAesCmac() {
/**
 * <pre>
 * Subkey Generation 
 * K                2b7e1516 28aed2a6 abf71588 09cf4f3c 
 * AES-128(key,0)   7df76b0c 1ab899b3 3e42f047 b91b546f 
 * K1               fbeed618 35713366 7c85e08f 7236a8de
 * K2               f7ddac30 6ae266cc f90bc11e e46d513b
 * </pre>
 */
 var AesCmac = require("AesCmac");
 var cmac = new AesCmac();
 cmac.init("0x2b7e151628aed2a6abf7158809cf4f3c");

// Test AES-128 on zero initialization vector
var t_0 = cmac._encrypt(cmac.const_Zero);
var aes_0 = sjcl.codec.hex.toBits("0x7df76b0c1ab899b33e42f047b91b546f");
sjcl.bitArray.equal(t_0, aes_0) ? console.log("AES test passed!") : console
.error("AES test failed!");

// Test subkey equality
var subkeys = cmac.generateSubkeys();
var K1 = sjcl.codec.hex.toBits("0xfbeed618357133667c85e08f7236a8de");
sjcl.bitArray.equal(subkeys["K1"], K1) ? console.log("K1 test passed!") : console
.error("K1 test failed!");
var K2 = sjcl.codec.hex.toBits("0xf7ddac306ae266ccf90bc11ee46d513b");
sjcl.bitArray.equal(subkeys["K2"], K2) ? console.log("K2 test passed!") : console
.error("K2 test failed!");

/**
 * <pre>
 * Example 1: len = 0
 * M              &lt;empty string&gt;
 * AES-CMAC       bb1d6929 e9593728 7fa37d12 9b756746
 * </pre>
 */
 var m1 = "";
 var cmac1 = cmac.generateCmac(m1);
 var ex1 = sjcl.codec.hex.toBits("0xbb1d6929e95937287fa37d129b756746")
 sjcl.bitArray.equal(ex1, cmac1) ? console.log("cmac1 test passed!") : console
 .error("cmac1 test failed!");
 if (sjcl.codec.hex.fromBits(cmac1) !== "bb1d6929e95937287fa37d129b756746")
 	console.error(sjcl.codec.hex.fromBits(cmac1) + " !== bb1d6929e95937287fa37d129b756746");

/**
 * <pre>
 * Example 2: len = 16
 * M              6bc1bee2 2e409f96 e93d7e11 7393172a
 * AES-CMAC       070a16b4 6b4d4144 f79bdd9d d04a287c
 * </pre>
 */
 var m2 = "0x6bc1bee22e409f96e93d7e117393172a";
 var cmac2 = cmac.generateCmac(m2);
 var ex2 = sjcl.codec.hex.toBits("0x070a16b46b4d4144f79bdd9dd04a287c")
 sjcl.bitArray.equal(ex2, cmac2) ? console.log("cmac2 test passed!") : console
 .error("cmac2 test failed!");

/**
 * <pre>
 * Example 3: len = 40
 * M              6bc1bee2 2e409f96 e93d7e11 7393172a
 *                ae2d8a57 1e03ac9c 9eb76fac 45af8e51
 *                30c81c46 a35ce411
 * AES-CMAC       dfa66747 de9ae630 30ca3261 1497c827
 * </pre>
 */
 var m3 = "0x6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411";
 var cmac3 = cmac.generateCmac(m3);
 var ex3 = sjcl.codec.hex.toBits("0xdfa66747de9ae63030ca32611497c827")
 sjcl.bitArray.equal(ex3, cmac3) ? console.log("cmac3 test passed!") : console
 .error("cmac3 test failed!");

/**
 * <pre>
 * Example 4: len = 64
 * M              6bc1bee2 2e409f96 e93d7e11 7393172a
 *                ae2d8a57 1e03ac9c 9eb76fac 45af8e51
 *                30c81c46 a35ce411 e5fbc119 1a0a52ef
 *                f69f2445 df4f9b17 ad2b417b e66c3710
 * AES-CMAC       51f0bebf 7e3b9d92 fc497417 79363cfe
 * </pre>
 */
 var m4 = "0x6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
 + "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
 var cmac4 = cmac.generateCmac(m4);
 var ex4 = sjcl.codec.hex.toBits("0x51f0bebf7e3b9d92fc49741779363cfe")
 sjcl.bitArray.equal(ex4, cmac4) ? console.log("cmac4 test passed!") : console
 .error("cmac4 test failed!");
}