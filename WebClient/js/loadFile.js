// start of loadfile
var fileContent = ""; 
var fileLength = 0;
var samfileContent = "";
var samfileLength = 0;
var reffileContent = "", reffileLength = 0, refindexfileContent = "", refindexfileLength = 0;


// loading file from local disk
$('#input_file').change(function(ev) {
    console.log("selecting file");
    var file = document.getElementById("input_file").files[0];
    if (file) {
        var reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function(evt) {
            console.log("type is");
            var plainContent = evt.target.result;
            console.log(typeof(evt.target.result));
            console.log(plainContent);
            console.log("content length is");
            console.log(plainContent.length);
            fileLength = plainContent.length;
            console.log("conver string to ArrayBuffer");
            fileContent = convertStringToArrayBufferView(plainContent);
            console.log(fileContent);
                        // document.getElementById("fileContents").innerHTML = evt.target.result;
                    }
                    reader.onerror = function(evt) {
                        console.log("error reading file");
                        // document.getElementById("fileContents").innerHTML = "error reading file";
                    }
                }
            });

$('#input_file1').change(function(ev) {
    console.log("selecting sam file");
    var file = document.getElementById("input_file1").files[0];
    if (file) {
        var reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function(evt) {
            console.log("type is");
            var plainContent = evt.target.result;
            console.log(typeof(evt.target.result));
            console.log(plainContent);
            console.log("sam file content length is");
            console.log(plainContent.length);
            samfileLength = plainContent.length;
            console.log("conver string to ArrayBuffer");
            samfileContent = convertStringToArrayBufferView(plainContent);
            console.log(samfileContent);
                        // document.getElementById("fileContents").innerHTML = evt.target.result;
                    }
                    reader.onerror = function(evt) {
                        console.log("error reading sam file");
                        // document.getElementById("fileContents").innerHTML = "error reading file";
                    }
                }
            });

$('#input_file2').change(function(ev) {
    console.log("selecting sam file");
    var file = document.getElementById("input_file2").files[0];
    if (file) {
        var reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function(evt) {
            console.log("type is");
            var plainContent = evt.target.result;
            console.log(typeof(evt.target.result));
            console.log(plainContent);
            console.log("ref file content length is");
            console.log(plainContent.length);
            reffileLength = plainContent.length;
            console.log("conver string to ArrayBuffer");
            reffileContent = convertStringToArrayBufferView(plainContent);
            console.log(reffileContent);
                        // document.getElementById("fileContents").innerHTML = evt.target.result;
                    }
                    reader.onerror = function(evt) {
                        console.log("error reading ref file");
                        // document.getElementById("fileContents").innerHTML = "error reading file";
                    }
                }
            });

$('#input_file3').change(function(ev) {
    console.log("selecting sam file");
    var file = document.getElementById("input_file3").files[0];
    if (file) {
        var reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function(evt) {
            console.log("type is");
            var plainContent = evt.target.result;
            console.log(typeof(evt.target.result));
            console.log(plainContent);
            console.log("content length is");
            console.log(plainContent.length);
            refindexfileLength = plainContent.length;
            console.log("conver string to ArrayBuffer");
            refindexfileContent = convertStringToArrayBufferView(plainContent);
            console.log(refindexfileContent);
                        // document.getElementById("fileContents").innerHTML = evt.target.result;
                    }
                    reader.onerror = function(evt) {
                        console.log("error reading ref index file");
                        // document.getElementById("fileContents").innerHTML = "error reading file";
                    }
                }
            });
// end of loadfile