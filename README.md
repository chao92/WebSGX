## WebSGX Attestation Javascript Library

![Overview](/Overview.png "Overview of WebSGX Attestation")

Please check our google doc for detial information: [Link to Development Document](https://docs.google.com/document/d/1ODTzUhnvWBszTK9oGUVX6fY3PCQHI7qbXqOfrc2Q5ek/edit?usp=sharing)

### Server

#### Environment
Nginx + PHP + Intel SGX SDK 1.7 running on Ubuntu 14.04.5 LTS

#### Linux Server
Through PHP we communicate with Linux Server by socket. Our Client call this PHP via AJAX, and get json return. Due to the discontinue of the whole attestation process. whenever each client send a message to server. it should send its identification ID and the message type. So server can process it. 

#### Server PHP
It used for exchange information between server and web client.
#### Running command:

```
./sample -c ../../server_config.txt
```

### Web Client
Call path: start from index.html, the function are called sequently as below, and the functions are defined at remoteAttestation.js.
```
index.html -> receiveMSG1andProcess() -> deriveKeys() -> retreiveRL() -> generatAndSendMSG2() -> receiveMSG3andProcess() -> sendMSG4withFile() -> sendSamFile() -> sendRefFile() -> sendIndexFile()
```

### Web Client Library (Node.js)


### IAS Communication

#### IAS certificate

bob_pfx.pfx is the file we submitted to Intel SGX IAS for get the SPID.
keys.txt file contains all the requried input for the Intel WebSGX attestation.
password file contains the passwrod for improting the certification to the computer when we want to communicate with IAS.

The left three files (RK_PUB.PEM, cert.pem, key.pem) are the different format key of the certification


