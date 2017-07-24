## WebSGX Attestation Javascript Library

![Overview](/Overview.png "Overview of WebSGX Attestation")

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

### WebClient

### IAS Communication