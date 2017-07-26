var webSGXAttestation = function() {
    "use strict";
    let crypto;
    var
        client_ID,
        ga,
        gb,
        ga_little,
        gb_little,
        ga_x,
        ga_y,
        GID,

        smk_key,
        mk_key,
        sk_key,
        vk_key,

        g_private_key_object,
        g_public_key_object,
        quote_verify_request_data;

    // ----------------------utilities functions----------------------
    var _hexStringToByte = function(str) {
        if (!str) {
            return new Uint8Array();
        }
        var a = [];
        for (var i = 0, len = str.length; i < len; i += 2) {
            a.push(parseInt(String(str).substr(i, 2), 16));
        }
        return new Uint8Array(a);
    }

    var _changeEndian = function(str) {
        var arr = String(str).match(/.{1,2}/g);
        return arr.reverse().join("").toString();
    }

    var _byteToHexString = function(uint8arr) {
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
    var _convertStringToArrayBufferView = function(str) {
        var bytes = new Uint8Array(str.length);
        for (var iii = 0; iii < str.length; iii++) {
            bytes[iii] = str.charCodeAt(iii);
        }

        return bytes;
    }

    var _unitArrayChangeEndian = function(uint8arr) {
        var tmpHex = _byteToHexString(uint8arr);
        var changedStr = _changeEndian(tmpHex);
        return _hexStringToByte(changedStr);
    }

    var _derive_smk_key = function(shared_key) {

        var aesCmac = require('node-aes-cmac').aesCmac;
        var key = "00000000000000000000000000000000";
        var key_derive_key = aesCmac(key, shared_key);

        var derived_buffer = new Uint8Array(7);
        derived_buffer.set([1], 0);
        var lable = _convertStringToArrayBufferView("SMK");
        // console.log(lable);
        derived_buffer.set(lable, 1);

        var key_length = new Uint16Array(1);
        key_length.set([128], 0);
        var key_length_Uint8 = new Uint8Array(key_length.buffer);
        // console.log(key_length_Uint8);

        derived_buffer.set(key_length_Uint8, 5);

        var second_key = key_derive_key.toString();
        var second_msg = _byteToHexString(derived_buffer);

        var smk = aesCmac(second_key, second_msg);
        smk_key = smk.toString();

        return smk_key;
    }

    var _derive_mk_key = function(shared_key) {

        var aesCmac = require('node-aes-cmac').aesCmac;
        var key = "00000000000000000000000000000000";
        var key_derive_key = aesCmac(key, shared_key);

        var derived_buffer = new Uint8Array(6);
        derived_buffer.set([1], 0);
        var lable = _convertStringToArrayBufferView("MK");
        // console.log(lable);
        derived_buffer.set(lable, 1);

        var key_length = new Uint16Array(1);
        key_length.set([128], 0);
        var key_length_Uint8 = new Uint8Array(key_length.buffer);
        // console.log(key_length_Uint8);

        derived_buffer.set(key_length_Uint8, 4);

        var second_key = key_derive_key.toString();
        var second_msg = _byteToHexString(derived_buffer);

        var mk = aesCmac(second_key, second_msg);
        mk_key = mk.toString();
        return mk_key;
    }

    var _derive_sk_key = function(shared_key) {

        var aesCmac = require('node-aes-cmac').aesCmac;
        var key = "00000000000000000000000000000000";
        var key_derive_key = aesCmac(key, shared_key);

        var derived_buffer = new Uint8Array(6);
        derived_buffer.set([1], 0);
        var lable = _convertStringToArrayBufferView("SK");
        // console.log(lable);
        derived_buffer.set(lable, 1);

        var key_length = new Uint16Array(1);
        key_length.set([128], 0);
        var key_length_Uint8 = new Uint8Array(key_length.buffer);
        // console.log(key_length_Uint8);

        derived_buffer.set(key_length_Uint8, 4);

        var second_key = key_derive_key.toString();
        var second_msg = _byteToHexString(derived_buffer);

        var sk = aesCmac(second_key, second_msg);
        sk_key = sk.toString();

        return sk_key;
    }

    var _derive_vk_key = function(shared_key) {

        var aesCmac = require('node-aes-cmac').aesCmac;
        var key = "00000000000000000000000000000000";
        var key_derive_key = aesCmac(key, shared_key);

        var derived_buffer = new Uint8Array(6);
        derived_buffer.set([1], 0);
        var lable = _convertStringToArrayBufferView("VK");
        // console.log(lable);
        derived_buffer.set(lable, 1);

        var key_length = new Uint16Array(1);
        key_length.set([128], 0);
        var key_length_Uint8 = new Uint8Array(key_length.buffer);
        // console.log(key_length_Uint8);

        derived_buffer.set(key_length_Uint8, 4);

        var second_key = key_derive_key.toString();
        var second_msg = _byteToHexString(derived_buffer);

        var vk = aesCmac(second_key, second_msg);
        vk_key = vk.toString();
        return vk_key;
    }
    // ----------------------utilities functions----------------------

    var _requestMSG1 = function(client_userName, client_password) {

        var msg1_request_data = {
            'username': client_userName,
            'password': client_password,
            'SSL': "nSSL",
        };
        var request = require('request');
        request.post({
                url: 'https://vpn.centromereinc.com/recv_MSG1.php',
                rejectUnhauthorized : false,
                form: msg1_request_data
            },
            function(err, httpResponse, body) {
                console.log('body is'+body);
                console.log(err);
                // C_RECEIVED_ga_x = text['ga_x'];
                // C_RECEIVED_ga_y = text['ga_y'];
                // C_RECEIVED_GID = text['GID'];
            });
    }


    var receiveMSG1 = function(r_gax, r_gay, r_GID) {
        // set ga and ga_little
        ga = new Uint8Array(64);
        ga_little = new Uint8Array(64);

        var ga_x = _hexStringToByte(r_gax);
        var ga_y = _hexStringToByte(r_gay);

        ga.set(ga_x, 0);
        ga.set(ga_y, ga_x.byteLength);

        // console.log(_hexStringToByte(ga_x).byteLength);
        ga_little.set(_hexStringToByte(_changeEndian(r_gax)), 0);
        // console.log(_hexStringToByte(ga_x).byteLength);
        ga_little.set(_hexStringToByte(_changeEndian(r_gay)), ga_x.byteLength);

        GID = r_GID;

        return [r_gax, r_gay, r_GID];

    }

    var sigRLRequestData = function() {
        // first get sig_rl from IAS based on GID;
        var sig_rl_size = 0;
        var sig_rl = "";
        var sig_rl_request_data = {
            'GID': GID
        };
        return sig_rl_request_data;
    }

    var generateMSG2 = function() {
        try {
            // check node.js whether support crypto js
            const crypto = require('crypto');
            // const curves = crypto.getCurves();

            // dispaly curves that supported by current system
            // console.log(curves); 

            const alice = crypto.createECDH('prime256v1');
            const aliceKey = alice.generateKeys();

            gb = new Uint8Array(64);
            gb_little = new Uint8Array(64);
            var gb_string = alice.getPublicKey('hex');
            var gb_x = _hexStringToByte(gb_string.substring(2, 66));
            var gb_y = _hexStringToByte(gb_string.substring(66, 130));
            gb.set(gb_x, 0);
            gb.set(gb_y, gb_x.byteLength);

            gb_little.set(_hexStringToByte(_changeEndian(gb_string.substring(2, 66))), 0);
            gb_little.set(_hexStringToByte(_changeEndian(gb_string.substring(66, 130))), gb_x.byteLength);

            var imported_public_key = '04' + _byteToHexString(ga);

            const dh_shared_key = alice.computeSecret(imported_public_key, 'hex', 'hex');

            try {
                // derived key based on shared_key for future use
                smk_key = _derive_smk_key(dh_shared_key);
                mk_key = _derive_mk_key(dh_shared_key);
                sk_key = _derive_sk_key(dh_shared_key);
                vk_key = _derive_vk_key(dh_shared_key);
                // return [smk_key, mk_key, sk_key, vk_key];
            } catch (err) {
                console.log('derive shared key fail')
            }

        } catch (err) {
            console.log('cryptojs support is disabled!');
        }

        // sign gb_ga with the client private key
        // 1. create gb_ga it's big-endiness format
        var gb_ga = new Uint8Array(128);

        gb_ga.set(gb, 0);
        gb_ga.set(ga, gb.byteLength);

        var gb_ga_little_endian = new Uint8Array(128);
        gb_ga_little_endian.set(gb_little, 0);
        gb_ga_little_endian.set(ga_little, gb_little.byteLength);

        var EC = require('elliptic').ec;
        var ec = new EC('p256'); // the defualt set is hash.sha256
        var prv = ('018c03d1533457adeaaeb6653b6a861fec879c4311de663bcea1522dbb6ce790');

        var pub = {
            x: _changeEndian("72128a7a17526ebf85d03a623730aead3e3daaee9c60731db05be8621c4beb38"),
            y: _changeEndian("d48140d950e2577b26eeb741e7c614e224b7bdc903f29a28a83cc81011145e06")
        };
        var key = ec.keyFromPublic(pub, 'hex');

        var prv_key = ec.keyFromPrivate(prv, 'hex');
        var signature = prv_key.sign(gb_ga_little_endian);

        var sigObj = JSON.parse(JSON.stringify(signature));
        // console.log('Stringify Signature: ' + JSON.stringify(signature));

        var msg2_sign_gb_ga = new Uint8Array(64);
        msg2_sign_gb_ga.set(_hexStringToByte(sigObj.r), 0);
        msg2_sign_gb_ga.set(_hexStringToByte(sigObj.s), 32);

        // console.log('signature: '+msg2_sign_gb_ga);
        var msg2_sign_gb_ga_x = new Uint8Array(32);
        msg2_sign_gb_ga_x.set(msg2_sign_gb_ga.subarray(0, 32), 0);
        var msg2_sign_gb_ga_y = new Uint8Array(32);
        msg2_sign_gb_ga_y.set(msg2_sign_gb_ga.subarray(32, 64), 0);

        var msg2_sign_gb_ga_little_endian = new Uint8Array(64);
        msg2_sign_gb_ga_little_endian.set(_unitArrayChangeEndian(msg2_sign_gb_ga_x), 0);
        msg2_sign_gb_ga_little_endian.set(_unitArrayChangeEndian(msg2_sign_gb_ga_y), msg2_sign_gb_ga_x.byteLength);

        // console.log(key.verify(gb_ga_little_endian, signature));

        // generate msg2_header: lengthis 8 bytes {type, status, size, align, body[]}
        var msg2_head = new Uint8Array(8);

        var msg2_head_type = new Uint8Array(1);
        msg2_head_type.set([1], 0);
        // add type Uint8Array(1) to msg2_head 
        msg2_head.set(msg2_head_type, 0)

        var msg2_head_status = new Uint8Array(2);
        msg2_head_status.set([0, 0], 0);
        // add status Uint8Array(2) to msg2_head 
        msg2_head.set(msg2_head_status, msg2_head_type.byteLength);

        var msg2_head_size = new Uint32Array(1);
        msg2_head_size.set([168], 0);
        var msg2_head_size_Uint8 = new Uint8Array(msg2_head_size.buffer);

        msg2_head.set(msg2_head_size_Uint8, msg2_head_type.byteLength + msg2_head_status.byteLength);

        var msg2_head_align = new Uint8Array(1);
        msg2_head.set(msg2_head_align, msg2_head_type.byteLength + msg2_head_status.byteLength + msg2_head_size_Uint8.byteLength);

        // generate msg2: length is 168 bytes {gb, spid, quote_type, kdf_id, sign_gb_ga, mac, sig_rl_size}
        var msg2 = new Uint8Array(168);
        // add gb Uint8Array(64) to msg2 gb is now little-endiness
        msg2.set(gb_little, 0)
        var msg2_spid = new Uint8Array(16);

        // msg2_spid.set(convertStringToArrayBufferView("Service X"), 0);
        msg2_spid.set(_hexStringToByte("2A57C3E6573D7EB2D56DE21CFB6C0132"), 0);

        // add spid Uint8Array(16) to msg2
        msg2.set(msg2_spid, gb.byteLength);
        // 1: linkable, 0 unlinkable
        var msg2_quote_type = new Uint16Array(1)
        msg2_quote_type.set([1], 0);
        var msg2_quote_type_Uint8 = new Uint8Array(msg2_quote_type.buffer);

        // add quote_type Uint8Array(2) to msg2
        msg2.set(msg2_quote_type_Uint8, gb.byteLength + msg2_spid.byteLength);

        var msg2_kdf_id = new Uint16Array(1);
        msg2_kdf_id.set([1], 0);
        var msg2_kdf_id_Uint8 = new Uint8Array(msg2_kdf_id.buffer);
        // add kdf_id Uint8Array(2) to msg2 
        msg2.set(msg2_kdf_id_Uint8, gb.byteLength + msg2_spid.byteLength + msg2_quote_type_Uint8.byteLength);

        // add msg2_sign_gb_ga or msg2_sign_gb_ga_little_endian

        msg2.set(msg2_sign_gb_ga_little_endian, gb.byteLength + msg2_spid.byteLength + msg2_quote_type_Uint8.byteLength + msg2_kdf_id_Uint8.byteLength);

        // Generate the CMACsmk for gb||SPID||TYPE||kdf_id||Sigsp(gb,ga)

        var msg2_to_be_maced = new Uint8Array(148);
        msg2_to_be_maced.set(msg2.subarray(0, 148), 0);


        var message = _byteToHexString(msg2_to_be_maced);
        var aesCmac = require('node-aes-cmac').aesCmac;
        var mac = _hexStringToByte(aesCmac(smk_key, message));

        msg2.set(mac, gb.byteLength + msg2_spid.byteLength + msg2_quote_type_Uint8.byteLength + msg2_kdf_id_Uint8.byteLength + msg2_sign_gb_ga.byteLength);

        var msg2_sig_rl_size = new Uint32Array(1);
        var msg2_sig_rl_size_Uint8 = new Uint8Array(msg2_sig_rl_size.buffer);

        msg2.set(msg2_sig_rl_size_Uint8, gb.byteLength + msg2_spid.byteLength + msg2_quote_type_Uint8.byteLength + msg2_kdf_id_Uint8.byteLength + msg2_sign_gb_ga.byteLength + mac.byteLength);

        // concate msg2_head and msg2
        // 8+168
        var msg2_full = new Uint8Array(176)
        msg2_full.set(msg2_head, 0);
        msg2_full.set(msg2, msg2_head.byteLength);

        var hexstring_msg2 = _byteToHexString(msg2_full);
        console.log(hexstring_msg2);

        var msg2_request_data = {
            'client_id': client_ID,
            'length': 176,
            'msg2': hexstring_msg2,
        };
        return msg2_request_data;

    }


    var receiveMSG3 = function(response) {
        // msg3 {mac(16), msg2 message}

        var msg3_length = response['length'];
        var msg3 = response['MSG3'];
        var msg3_mac = response['msg3_mac'];
        var msg3_message = response['msg3_message'];
        var msg3_ga = msg3_message.substring(0, 128);

        if (msg3_ga.toUpperCase().localeCompare(byteToHexString(ga_little)) != 0) {
            console.log("error ga is not match");
        }

        var verify_message = msg3_message;
        var aesCmac = require('node-aes-cmac').aesCmac;
        var tmp_mac = aesCmac(smk_key, verify_message);

        // compare mac in message 3 with generated mac using SMK key

        if (msg3_mac.localeCompare(tmp_mac) != 0) {
            console.log("error mac is not match");
        }

        var ps_sec_prop = msg3_message.substring(128, 640);

        // extract quote from msg3 to local
        var p_quote = msg3_message.substring(640, 2872);
        var p_quote_base64 = hexToBase64(p_quote);

        // need to verify the quote with IAS
        quote_verify_request_data = {
            'quote': p_quote_base64
        };

        var report_body = p_quote.substring(96, 864);
        // verify the report_data in the Quote, the first 32 bytes of report_data are sha256 hash of (ga|gb|vk)
        // the second 32 bytes of report_data are set to zero
        // 64+64+16
        var ga_gb_vk = new Uint8Array(144);
        ga_gb_vk.set(ga_little, 0);
        ga_gb_vk.set(gb_little, ga_little.byteLength);
        ga_gb_vk.set(hexStringToByte(C_vk_key), ga_little.byteLength + gb_little.byteLength);

        var report_data = report_body.substring(640, 768);
        const sha256hash = require('crypto').createHmac('sha256', ga_gb_vk).digest('hex');
        var tmp_first_32 = report_data.substring(0, 64);
        if (tmp_first_32.toUpperCase().localeCompare(sha256hash.toUpperCase()) != 0) {
            console.log("report_data the first 32 bytes dose not match");
        } else {
            // TODO: verify enclave policy, verify quote with IAS
            // print attestation report:

            // TODO: verify the enclave policy report

            // simulate function ias_verify_attestation_evidence to generate attestation report {input p_quote, output, p_attestation_report}

            // 1.Decrypt the Quote signature and verify
            var p_attestation_report_id = new Uint32Array(); // 0x12345678
            // p_attestation_report_id.of(0x12345678);
            p_attestation_report_id_Uint8 = new Uint8Array(p_attestation_report_id.buffer);

            var p_attestation_report_status = new Uint32Array(1);
            p_attestation_report_status.set([0], 0);
            var p_attestation_report_status_Uint8 = new Uint8Array(p_attestation_report_status.buffer);

            var p_attestation_report_revocation_reason = new Uint32Array(1);
            p_attestation_report_revocation_reason.set([0], 0);
            var p_attestation_report_revocation_reason_Uint8 = new Uint8Array(p_attestation_report_revocation_reason.buffer);

            var p_attestation_report_info_blob_epid_group_status = new Uint32Array(1);
            p_attestation_report_info_blob_epid_group_status.set([0], 0);
            p_attestation_report_info_blob_epid_group_status_Uint8 = new Uint8Array(p_attestation_report_info_blob_epid_group_status.buffer);

            var p_attestation_report_info_blob_tcb_evaluation_status = new Uint32Array(1);
            p_attestation_report_info_blob_tcb_evaluation_status.set([0], 0);
            p_attestation_report_info_blob_tcb_evaluation_status_Uint8 = new Uint8Array(p_attestation_report_info_blob_tcb_evaluation_status.buffer);

            var p_attestation_report_info_blob_pse_evaluation_status = new Uint32Array(1);
            p_attestation_report_info_blob_pse_evaluation_status.set([0], 0);
            p_attestation_report_info_blob_pse_evaluation_status_Uint8 = new Uint8Array(p_attestation_report_info_blob_pse_evaluation_status.buffer);

            var p_attestation_report_info_blob_equivalent_tcb_psvn = new Uint8Array(18);
            p_attestation_report_info_blob_equivalent_tcb_psvn.set([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], 0);

            var p_attestation_report_info_blob_pse_isvsvn = new Uint8Array(2);
            p_attestation_report_info_blob_pse_isvsvn.set([0, 0], 0);

            var p_attestation_report_info_blob_psda_svn = new Uint8Array(4);
            p_attestation_report_info_blob_psda_svn.set([0, 0, 0, 0], 0);

            var p_attestation_report_info_blob_rekey_gid = new Uint8Array(4);
            p_attestation_report_info_blob_rekey_gid.set([0, 0, 0, 0], 0);

            // signing the attestation_report using private key

            var p_attestation_report_pse_status = new Uint32Array(1);
            p_attestation_report_pse_status.set([0], 0);

            var p_attestation_report_policy_report_size = new Uint32Array(1);
            p_attestation_report_policy_report_size.set([0], 0);

        }

    }

    var iasQuoteRequestData = function() {
        return quote_verify_request_data;
    }

    var generateMSG4 = function() {
        // assemble msg4 
        // generate msg4_header: lengthis 8 bytes {type, status, size, align, body[]}
        var msg4_head = new Uint8Array(8);

        var msg4_head_type = new Uint8Array(1);
        msg4_head_type.set([3], 0);
        msg4_head.set(msg4_head_type, 0) // add type Uint8Array(1) to msg2_head 

        var msg4_head_status = new Uint8Array(2);
        msg4_head_status.set([0, 0], 0);
        msg4_head.set(msg4_head_status, msg4_head_type.byteLength); // add status Uint8Array(2) to msg2_head 

        var msg4_head_size = new Uint32Array(1); // 145head + g_secret 8
        msg4_head_size.set([1001], 0);
        var msg4_head_size_Uint8 = new Uint8Array(msg4_head_size.buffer);

        msg4_head.set(msg4_head_size_Uint8, msg4_head_type.byteLength + msg4_head_status.byteLength);

        var msg4_head_align = new Uint8Array(1);
        msg4_head.set(msg4_head_align, msg4_head_type.byteLength + msg4_head_status.byteLength + msg4_head_size_Uint8.byteLength);


        // generate msg4_body: length is 145 + 8(g_secret:8) bytes {platform_info_blob(97), mac(16), secret(32 including a mac_tag:16) + 8}

        var msg4_body_platform_info = new Uint8Array(97);

        // generat mac based on the mk key
        var msg4_body_mac = new Uint8Array(16);

        var key = CryptoJS.enc.Hex.parse(C_mk_key);
        var message = CryptoJS.enc.Hex.parse(byteToHexString(msg4_body_platform_info));

        var mac = hexStringToByte(CryptoJS.CMAC(key, message).toString());

        msg4_body_mac.set(mac, 0);

        var msg4_body_secret = new Uint8Array(32);

        var msg4_body_secret_payload_size = new Uint32Array(1);
        msg4_body_secret_payload_size.set([856], 0);
        var msg4_body_secret_payload_size_Uint8 = new Uint8Array(msg4_body_secret_payload_size.buffer);
        console.log("size of secret is 856");
        console.log(msg4_body_secret_payload_size_Uint8)

        msg4_body_secret.set(msg4_body_secret_payload_size_Uint8, 0);

        console.log("seccret part is");
        console.log(msg4_body_secret);

        var msg4_body_sent_out_secrets = new Uint8Array(856);
        // msg4_body_g_secrets.set([0,1,2,3,4,5,6,7],0);

        var secret_plain_tex = new Uint8Array(8);
        secret_plain_tex.set([0, 1, 2, 3, 4, 5, 6, 7], 0);

    }

    return {
        _requestMSG1: _requestMSG1,
        receiveMSG1: receiveMSG1,
        sigRLRequestData: sigRLRequestData,
        generateMSG2: generateMSG2,
        receiveMSG3: receiveMSG3,
        iasQuoteRequestData: iasQuoteRequestData,
        generateMSG4: generateMSG4
    }


}();

module.exports = webSGXAttestation;