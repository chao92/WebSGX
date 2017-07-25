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

```
index.html -> receiveMSG1andProcess() -> deriveKeys() -> retreiveRL() -> generatAndSendMSG2() -> receiveMSG3andProcess() -> sendMSG4withFile() -> sendSamFile() -> sendRefFile() -> sendIndexFile()
```

### Web Client Library (Node.js)


### IAS Communication

#### IAS certificate
We submitted the pfx to apply for a SPID with Intel SGX IAS, and installed the keys for communicat with IAS during attestation. the password for import the key is within the file named 'password'.