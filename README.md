## WebSGX Attestation Javascript Library

![Overview](/Overview.png "Overview of WebSGX Attestation")

### Server

#### Environment
Nginx + PHP + Intel SGX SDK 1.7 running on Ubuntu 14.04.5 LTS

#### Linux Server
Through PHP we communicate with Linux Server by socket. Our Client call this PHP via AJAX, and get json return. Due to the discontinue of the whole attestation process. whenever each client send a message to server. it should send its identification ID and the message type. So server can process it. 

### Web Client

### IAS Communication