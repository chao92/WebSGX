(function(window) {
 // You can enable the strict mode commenting the following line  
 // 'use strict';

 // This function will contain all our code
 function webSGXAttestation() {

 	
 	var _webSGX = {};




  // This variable will be inaccessible to the user, only can be visible in the scope of your library.
  var settings = {
  	client_ID,
  	ga,
  	gb,
  	ga_little,
  	gb_little,
  	ga_x,
  	ga_y,
  	GID,

  	smk_key,
  	mk_key,
  	sk_key,
  	vk_key
  };




  // We will add functions to our library here !

  _webSGX.processMSG1 = function(ga_x, ga_y, GID) {

  	// set ga and ga_little
  	settings.ga = new Uint8Array(64);
  	settings.ga_little = new Uint8Array(64);

  	var ga_x = hexStringToByte(ga_x);
  	var ga_y = hexStringToByte(ga_y);
  	
  	settings.ga.set(ga_x, 0);
  	settings.ga.set(ga_y, ga_x.byteLength);
  	
  	settings.ga_little.set(hexStringToByte(changeEndian(ga_x)), 0);
  	settings.ga_little.set(hexStringToByte(changeEndian(ga_y)), hexStringToByte(ga_x).byteLength);

  	console.log("ga big-endiness is");
  	console.log(settings.ga);
  	console.log("ga little-endienss is");
  	console.log(settings.ga_little);

  	// now ready for generating MSG2 and keys
  	// for IE11
  	window.crypto = window.crypto || window.msCrypto; 
  	// for Safari
  	if (window.crypto.webkitSubtle) {
  		window.crypto.subtle = window.crypto.webkitSubtle; 
  	}

  };

  _webSGX.sigRL = function() {

  };

  _webSGX.iasReport = function() {

  };

  return _webSGX;
}

 // We need that our library is globally accesible, then we save in the window
 if (typeof(window.webSGXLib) === 'undefined') {
 	window.webSGXLib = webSGXAttestation();
 }
})(window); // We send the window variable withing our function


// functions
function hexStringToByte(str) {
	if (!str) {
		return new Uint8Array();
	}

	var a = [];
	for (var i = 0, len = str.length; i < len; i += 2) {
		a.push(parseInt(str.substr(i, 2), 16));
	}

	return new Uint8Array(a);
}

function changeEndian(str){
	var arr = str.match(/.{1,2}/g);
	return arr.reverse().join("").toString();
}

// Then we can call it using
console.log(webSGXLib);