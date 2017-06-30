# from bs4 import BeautifulSoup 
# import requests 
# resp = requests.get('https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/sigrl/00000BCD', verify='./cert.pem');




import requests
cert_file_path = "./cert.pem"
key_file_path = "./key.pem"
requests.packages.urllib3.disable_warnings()
url = "https://test-as.sgx.trustedservices.intel.com/attestation/sgx/v1/sigrl/00000d6e"
# params = {"param_1": "value_1", "param_2": "value_2"}
cert = (cert_file_path, key_file_path)
r = requests.get(url, cert=cert, verify=False)
print(r)