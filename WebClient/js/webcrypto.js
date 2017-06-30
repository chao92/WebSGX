// function base64toHEX(base64) {

//   var raw = atob(base64);

//   var HEX = '';

//   for ( i = 0; i < raw.length; i++ ) {

//     var _hex = raw.charCodeAt(i).toString(16)

//     HEX += (_hex.length==2?_hex:'0'+_hex);

// }
// return HEX.toUpperCase();

// }

// function Base64DecodeUrl(str){
//     str = (str + '=').slice(0, str.length + (str.length % 4));
//     return str.replace(/-/g, '+').replace(/_/g, '/');
// }

function hexToBase64(hexstring) {
    return btoa(hexstring.match(/\w{2}/g).map(function(a) {
        return String.fromCharCode(parseInt(a, 16));
    }).join(""));
}

function HEX2Base64URL(str){
    var hexArray = str.replace(/\r|\n/g, "")
    .replace(/([\da-fA-F]{2}) ?/g, "0x$1 ")
    .replace(/ +$/, "")
    .split(" ");
    var byteString = String.fromCharCode.apply(null, hexArray);
    var base64string = window.btoa(byteString);
    var base64stringURL = base64string.replace(/\+/g, '-').replace(/\//g, '_').replace(/\=+$/, '');
    return base64stringURL; 
}

function Base64URL2HEX(str){
    str = (str + '=').slice(0, str.length + (str.length % 4));
    var base64 = str.replace(/-/g, '+').replace(/_/g, '/');
    var raw = atob(base64);

    var HEX = '';

    for ( i = 0; i < raw.length; i++ ) {

        var _hex = raw.charCodeAt(i).toString(16)

        HEX += (_hex.length==2?_hex:'0'+_hex);
    }
    return HEX.toUpperCase();
}

// function parseHexString(str) { 
//     var result = [];
//     while (str.length >= 2) { 
//         result.push(parseInt(str.substring(0, 2), 16));

//         str = str.substring(2, str.length);
//     }

//     return result;
// }

function convertStringToArrayBufferView(str)
{
    var bytes = new Uint8Array(str.length);
    for (var iii = 0; iii < str.length; iii++) 
    {
        bytes[iii] = str.charCodeAt(iii);
    }

    return bytes;
}

function convertArrayBufferToHexaDecimal(buffer) 
{
    var data_view = new DataView(buffer)
    var iii, len, hex = '', c;

    for(iii = 0, len = data_view.byteLength; iii < len; iii += 1) 
    {
        c = data_view.getUint8(iii).toString(16);
        if(c.length < 2) 
        {
            c = '0' + c;
        }

        hex += c;
    }

    return hex;
}

function byteToHexString(uint8arr) {
  if (!uint8arr) {
    return '';
}

var hexStr = '';
for (var i = 0; i < uint8arr.length; i++) {
    var hex = (uint8arr[i] & 0xff).toString(16);
    hex = (hex.length === 1) ? '0' + hex : hex;
    hexStr += hex;
}

return hexStr.toUpperCase();
}

function hexStringToByte(str) {
  if (!str) {
    return new Uint8Array();
    }

var a = [];
for (var i = 0, len = str.length; i < len; i+=2) {
    a.push(parseInt(str.substr(i,2),16));
}

return new Uint8Array(a);
}

function unitArrayChangeEndian(uint8arr){
    var tmpHex = byteToHexString(uint8arr);
    var changedStr = changeEndian(tmpHex);
    return hexStringToByte(changedStr);
}

function changeEndian(str){
    var arr = str.match(/.{1,2}/g);
    return arr.reverse().join("").toString();
}

function flippedString(str){
    var str = "018c03d1533457adeaaeb6653b6a861fec879c4311de663bcea1522dbb6ce790";
    var arr = str.match(/.{1,2}/g);
    var flipped_arr=[];
    for(var i=0; i < arr.length; i++){
        var tmp = arr[i].split("").reverse().join("");
        flipped_arr.push(tmp);
    }
    return flipped_arr.join("").toString();
}

function derive_smk_key(shared_key){

    var key = CryptoJS.enc.Hex.parse("00000000000000000000000000000000");
    var message = CryptoJS.enc.Hex.parse(shared_key);
    var key_derive_key = CryptoJS.CMAC(key, message);

    // console.log("first key");
    // console.log(key_derive_key.toString());

    var derived_buffer = new Uint8Array(7);
    derived_buffer.set([1],0);
    var lable = convertStringToArrayBufferView("SMK");
    // console.log(lable);
    derived_buffer.set(lable,1);

    var key_length = new Uint16Array(1);
    key_length.set([128],0);
    var key_length_Uint8 = new Uint8Array(key_length.buffer);
    // console.log(key_length_Uint8);

    derived_buffer.set(key_length_Uint8,5);
    // console.log("derived buffer");
    // console.log(byteToHexString(derived_buffer));

    var second_key = CryptoJS.enc.Hex.parse(key_derive_key.toString());
    var second_msg = CryptoJS.enc.Hex.parse(byteToHexString(derived_buffer));

    var smk = CryptoJS.CMAC(second_key,second_msg);
    return smk.toString();
}

function derive_mk_key(shared_key){

    var key = CryptoJS.enc.Hex.parse("00000000000000000000000000000000");
    var message = CryptoJS.enc.Hex.parse(shared_key);
    var key_derive_key = CryptoJS.CMAC(key, message);

    // console.log("first key");
    // console.log(key_derive_key.toString());

    var derived_buffer = new Uint8Array(6);
    derived_buffer.set([1],0);
    var lable = convertStringToArrayBufferView("MK");
    // console.log(lable);
    derived_buffer.set(lable,1);

    var key_length = new Uint16Array(1);
    key_length.set([128],0);
    var key_length_Uint8 = new Uint8Array(key_length.buffer);
    // console.log(key_length_Uint8);

    derived_buffer.set(key_length_Uint8,4);
    // console.log("derived buffer");
    // console.log(byteToHexString(derived_buffer));

    var second_key = CryptoJS.enc.Hex.parse(key_derive_key.toString());
    var second_msg = CryptoJS.enc.Hex.parse(byteToHexString(derived_buffer));

    var mk = CryptoJS.CMAC(second_key,second_msg);
    return mk.toString();
}

function derive_sk_key(shared_key){

    var key = CryptoJS.enc.Hex.parse("00000000000000000000000000000000");
    var message = CryptoJS.enc.Hex.parse(shared_key);
    var key_derive_key = CryptoJS.CMAC(key, message);

    // console.log("first key");
    // console.log(key_derive_key.toString());

    var derived_buffer = new Uint8Array(6);
    derived_buffer.set([1],0);
    var lable = convertStringToArrayBufferView("SK");
    // console.log(lable);
    derived_buffer.set(lable,1);

    var key_length = new Uint16Array(1);
    key_length.set([128],0);
    var key_length_Uint8 = new Uint8Array(key_length.buffer);
    // console.log(key_length_Uint8);

    derived_buffer.set(key_length_Uint8,4);
    // console.log("derived buffer");
    // console.log(byteToHexString(derived_buffer));

    var second_key = CryptoJS.enc.Hex.parse(key_derive_key.toString());
    var second_msg = CryptoJS.enc.Hex.parse(byteToHexString(derived_buffer));

    var sk = CryptoJS.CMAC(second_key,second_msg);
    return sk.toString();
}

function derive_vk_key(shared_key){

    var key = CryptoJS.enc.Hex.parse("00000000000000000000000000000000");
    var message = CryptoJS.enc.Hex.parse(shared_key);
    var key_derive_key = CryptoJS.CMAC(key, message);

    // console.log("first key");
    // console.log(key_derive_key.toString());

    var derived_buffer = new Uint8Array(6);
    derived_buffer.set([1],0);
    var lable = convertStringToArrayBufferView("VK");
    // console.log(lable);
    derived_buffer.set(lable,1);

    var key_length = new Uint16Array(1);
    key_length.set([128],0);
    var key_length_Uint8 = new Uint8Array(key_length.buffer);
    // console.log(key_length_Uint8);

    derived_buffer.set(key_length_Uint8,4);
    // console.log("derived buffer");
    // console.log(byteToHexString(derived_buffer));

    var second_key = CryptoJS.enc.Hex.parse(key_derive_key.toString());
    var second_msg = CryptoJS.enc.Hex.parse(byteToHexString(derived_buffer));

    var vk = CryptoJS.CMAC(second_key,second_msg);
    return vk.toString();
}
// function deriveKey(dh_shared_key, keyType){

//     switch(keyType){
//         case 0:
//         case 1:
//         case 2:

//     }
// }

// var data = "QNimate";

// var crypto = window.crypto || window.msCrypto;

// if(crypto.subtle)
// {
//     alert("Cryptography API Supported");

//     var promise = crypto.subtle.digest({name: "SHA-256"}, convertStringToArrayBufferView(data));   

//     promise.then(function(result){
//         var hash_value = convertArrayBufferToHexaDecimal(result);
//     });
// }
// else
// {
//     alert("Cryptography API not Supported");
// }