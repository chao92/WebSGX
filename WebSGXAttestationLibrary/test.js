var websgx = require('./src/index.js');



var ga_x = 'bceb54eb1696d6783e77633e7d332fa0644363f33a1389f479438f81017f08f8';
var ga_y = '653b91f70a9d69eaa9bec43fa6f58be37c101c2451df6e55a93a414b05642bf4';
var gid = '00000d6e';


var msg1 = websgx.receiveMSG1(ga_x, ga_y, gid);
var msg2 = websgx.generateMSG2();
// console.log(msg2);
