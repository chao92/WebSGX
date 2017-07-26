var is_attestating = 0;
var is_attestation_passed = false;

function isAttestating()
{
   var rawFile = new XMLHttpRequest();
    rawFile.open("GET", ".././attestating.txt", false); // zero stands for no client running, one stands for client is running.
    rawFile.send();

    var allText = rawFile.responseText;

    if(allText == '0')
    {
        if(is_attestating == 0)
        {
          alert("successfully start attestating.");
          is_attestating = 1;
          return false;

        }
        else
        {
          alert("someone is attestating. Please wait...");
          return true;
        }
    }
    else
    {
      alert("someone is attestating. Please wait...");
      is_attestating = 0;
      return true;
    }
}

function isAttestationPassed()
{
   var rawFile = new XMLHttpRequest();
    rawFile.open("GET", ".././isAttestationPassed.txt", false);
    rawFile.send();

    var allText = rawFile.responseText;

    if(allText == '0')
    {
        if(!is_attestation_passed)
        {
          alert("all clients attestation passed.");
          is_attestation_passed = true;
          return true;

        }
        else
        {
          console.log("someone is still attestating. Please wait...");
          return false;
        }
    }
    else
    {
      console.log("someone is still attestating. Please wait...");
      is_attestation_passed = false;
      return false;
    }
}