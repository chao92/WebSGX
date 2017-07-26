require('crypto-js');
var fs = require("fs");
var aesContent = fs.readFileSync("./cryptojs-extension-master/lib/cryptojs-aes.min.js", "utf8");
var cmacContent = fs.readFileSync("./cryptojs-extension-master/build/cmac.min.js", "utf8");
// require('./cryptojs-extension-master/build/all.min.js');
// require('./cryptojs-extension-master/lib/cryptojs-aes.min.js');
// require('./cryptojs-extension-master/build/cmac.min.js');




(function() {
    // You can enable the strict mode commenting the following line  
    // 'use strict';

    // This function will contain all our code
    function webSGXAttestation() {


        var _webSGX = {};

        // require('bootstrap');



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
            vk_key,

            // generated key pair
            g_private_key_object,
            g_public_key_object
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

            if (window.crypto.subtle) { // support webcrypto
                window.crypto.subtle.generateKey({
                        name: "ECDH",
                        namedCurve: "P-256",
                    }, true, ["deriveKey", "deriveBits"])
                    .then(function(key) {
                        settings.g_private_key_object = key.privateKey;
                        settings.g_public_key_object = key.publicKey;
                        //base64url encoding
                        window.crypto.subtle.exportKey("jwk", settings.g_public_key_object)
                            .then(function(key) {
                                //returns the exported key data

                                var plainKey = "<br> x: " + Base64URL2HEX(key.x) + "<br>" + "y: " + Base64URL2HEX(key.y);
                                settings.gb = new Uint8Array(64);
                                var tmp_public_key_x_exported = hexStringToByte(Base64URL2HEX(key.x));
                                var tmp_public_key_y_exported = hexStringToByte(Base64URL2HEX(key.y));

                                settings.gb.set(tmp_public_key_x_exported, 0);
                                settings.gb.set(tmp_public_key_y_exported, tmp_public_key_x_exported.byteLength);

                                console.log("gb big-endiness is");
                                console.log(settings.gb);

                                // gb_little
                                settings.gb_little = new Uint8Array(64);
                                var gb_little_x_exported = hexStringToByte(changeEndian(Base64URL2HEX(key.x)));
                                var gb_little_y_exported = hexStringToByte(changeEndian(Base64URL2HEX(key.y)));
                                settings.gb_little.set(gb_little_x_exported, 0);
                                settings.gb_little.set(gb_little_y_exported, gb_little_x_exported.byteLength);

                                console.log("gb little-endienss is");
                                console.log(gb_little);

                                console.log(plainKey);

                            }).catch = function(e) {
                                console.log("export public key error");
                                console.log(e.message);
                            }
                        //base64url encoding
                        window.crypto.subtle.exportKey("jwk", settings.g_private_key_object)
                            .then(function(key) {

                              console.log("generated private key is");
                              console.log(Base64URL2HEX(key.d));
                              console.log(key);

                            }).catch = function(e) {
                                console.log("export private key error");
                                console.log(e.message);
                            }
                    }).catch = function(e) {
                        console.log("generate key error");
                        console.log(e.message);
                    }
            }
        };

        _webSGX.sigRL = function() {

        };

        _webSGX.iasReport = function() {

        };

        return _webSGX;
    }

    // We need that our library is globally accesible, then we save in the window
    // if (typeof(window.webSGXLib) === 'undefined') {
    //  window.webSGXLib = webSGXAttestation();
    // }
})(); // We send the window variable withing our function


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

function changeEndian(str) {
    var arr = str.match(/.{1,2}/g);
    return arr.reverse().join("").toString();
}

// // Then we can call it using
console.log(_webSGX.processMSG1);