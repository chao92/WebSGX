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