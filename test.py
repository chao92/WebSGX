import os
from selenium import webdriver

chromedriver = "/Users/chaojiang/Downloads/chromedriver"
os.environ["webdriver.chrome.driver"] = chromedriver

webpage = r"https://192.168.1.202/" # edit me
# notes for test
# ga_x: bceb54eb1696d6783e77633e7d332fa0644363f33a1389f479438f81017f08f8
# ga_y: 653b91f70a9d69eaa9bec43fa6f58be37c101c2451df6e55a93a414b05642bf4
# gid:  00000d6e
# end of notes
user1 = {}
user1['userName'] = "user1"
user1['userPassword'] = "13313"
user1['privateKey'] = "018c03d1533457adeaaeb6653b6a861fec879c4311de663bcea1522dbb6ce790"
user1['publicKeyX'] = "72128a7a17526ebf85d03a623730aead3e3daaee9c60731db05be8621c4beb38"
user1['publicKeyY'] = "d48140d950e2577b26eeb741e7c614e224b7bdc903f29a28a83cc81011145e06"

user2 = {}
user2['userName'] = "user2"
user2['userPassword'] = "f322"
user2['privateKey'] = "7d765c220fcc805e57a185d3f5c4038f0635da25a2bec756155e986dd932b4ac"
user2['publicKeyX'] = "d11e9594eca01daa8a7939e946b033c2f323c527348e40b5cd23a5ccea161aa0"
user2['publicKeyY'] = "44b34aa1847b81825016e217d3cd2177dd4105ad9f32ec491f292afaf8a76cdf"

user3 = {}
user3['userName'] = "user3"
user3['userPassword'] = "ehh45h"
user3['privateKey'] = "fa84312601208234bd67980425d5f96cd3165262ddf130ee0701afed9c5c00ba"
user3['publicKeyX'] = "9d5dcf574d946d902152afb5285e8effa8e2373d392e5bc01a8d16e0ac89a494"
user3['publicKeyY'] = "775bff94c5c38653c36c2efb3913b3cd82617d3b16c47e26d7434411a8c5ee21"

user4 = {}
user4['userName'] = "user4"
user4['userPassword'] = "133134"
user4['privateKey'] = "aacc14a8a7fd228908428e8365818d357c12c76d401e579ce8e02a966ac51c51"
user4['publicKeyX'] = "6345441c724bdc82b3bbf23452883cbf9909df47356b88de74ff123910daf553"
user4['publicKeyY'] = "5d2e6faea1737575af5c346d5c5c6a4294969fe43018122d369d13b63ebe7dcc"

user5 = {}
user5['userName'] = "user5"
user5['userPassword'] = "f3225"
user5['privateKey'] = "0cb3fa879c10fc8e1dd420d804f58614559fd91056f3ff2c75581aa204e779cb"
user5['publicKeyX'] = "f97be111a1f9d6157fc094629f13a993908af886a4cbe8561ebd2d704be83271"
user5['publicKeyY'] = "a6162fea2faf8e237741ff0ecb2cce72904355ffc2f05d091f981afb5b3205ec"

user6 = {}
user6['userName'] = "user6"
user6['userPassword'] = "ehh456h"
user6['privateKey'] = "16fc18e2980a2a089fe0c7539901477c2027e68b2df251c3c9791d7d8d492866"
user6['publicKeyX'] = "ebf0065a9a9658736641c93b822d4f2265e4954aefcf8eac765bb85217a6cb29"
user6['publicKeyY'] = "5e52dc4be51556ece81374d74b178aed87fa9282f535cc5dfcfdd50e316971e0"

user7 = {}
user7['userName'] = "user7"
user7['userPassword'] = "133137"
user7['privateKey'] = "0d859633218991f28b33d55de8f4cefdf4e1deff9be2703c4defae12a661bc38"
user7['publicKeyX'] = "04dae004eac2287eaa0bdd7516794839f2d654eb675f175959a3d9e207ad9c10"
user7['publicKeyY'] = "bcfbe5a024bc2ad4b16acbf768c27863c39cdd7acf33618a69ccc8b1f73e09d2"

user8 = {}
user8['userName'] = "user8"
user8['userPassword'] = "f3228"
user8['privateKey'] = "6b746bb9ed662173b6161027027c4930792f8a0bd8fc6057236b17c56b748e1f"
user8['publicKeyX'] = "691e08d0aa421973698a77bc0a7f66e9b18f6b7d208d396fee815c05e719983e"
user8['publicKeyY'] = "d16539f4d8ba917d4193d4861696f3d1365b223e8f98771c887811d7c8b0d25a"

user9 = {}
user9['userName'] = "user9"
user9['userPassword'] = "ehh459h"
user9['privateKey'] = "679af993c549ae7b290e9d3d3eddbda4fd176d4dc7f891721a92eb4be0f2542a"
user9['publicKeyX'] = "96f7c94ee44db1fb72bd680015774d8dd6143151208cceac776d04ea71152ce0"
user9['publicKeyY'] = "cb944a2d9928c9a5b7538212cf1b3fd0f9b77880e4ca586ee0a0a1cdc5b05fe0"

user10 = {}
user10['userName'] = "user10"
user10['userPassword'] = "133130"
user10['privateKey'] = "c76b586ce5496ad5d7580c53add68e460d9304074ac68125e5d7792cc6f8ca02"
user10['publicKeyX'] = "719bbc8e48056dfff2d55872067f8f145bedc5cde4dae96f4b23123f66ae487b"
user10['publicKeyY'] = "3e56ae4ea1967fe7955e40ffdcc55176bd787f45be6be2b62d2b5ca645e23d2b"

user11 = {}
user11['userName'] = "user11"
user11['userPassword'] = "f32211"
user11['privateKey'] = "b7173c49d45c5ad87351ed1bb5e23b8c9509bd14e899ea4e9eca6da89f54048b"
user11['publicKeyX'] = "d7797f7169103e1196bbc389d864d7a9de3bb568ab78144253bc895e34308f2b"
user11['publicKeyY'] = "7913c151db041991cf40ea29223694a8b0d0db96677d97b5879e1386834320b5"

user12 = {}
user12['userName'] = "user12"
user12['userPassword'] = "ehh45h2"
user12['privateKey'] = "f7e717078f79287a7b0565bc5ac80870cbae5405e6a9ff9f2b43b1b54f262923"
user12['publicKeyX'] = "78ccbb1657f228cc94727551c2156e10135b2b7673118d8022bc43a65fe495d5"
user12['publicKeyY'] = "dff17067d6e819641b80825d3e9795b326818d0be1154772112de7dc2953b412"

driver = webdriver.Chrome(chromedriver)
driver.get(webpage)

userNameInput = driver.find_element_by_id('usr');
userNameInput.send_keys("user1")

userPasswordInput = driver.find_element_by_id('pwd');
userPasswordInput.send_keys(user1['userPassword']);

userPrivateKeyInput = driver.find_element_by_id('priKey');
userPrivateKeyInput.send_keys(user1['privateKey']);

userPublicKeyXInput = driver.find_element_by_id('pubKey_x');
userPublicKeyXInput.send_keys(user1['publicKeyX']);

userPublicKeyYInput = driver.find_element_by_id('pubKey_y');
userPublicKeyYInput.send_keys(user1['publicKeyY']);


startAttestation = driver.find_element_by_id("A_btn_start_Attestation")
startAttestation.click()

driver.close();



driver2 = webdriver.Chrome(chromedriver)
driver2.get(webpage)

userNameInput = driver2.find_element_by_id('usr');
userNameInput.send_keys("user2")

userPasswordInput = driver2.find_element_by_id('pwd');
userPasswordInput.send_keys(user2['userPassword']);

userPrivateKeyInput = driver2.find_element_by_id('priKey');
userPrivateKeyInput.send_keys(user2['privateKey']);

userPublicKeyXInput = driver2.find_element_by_id('pubKey_x');
userPublicKeyXInput.send_keys(user2['publicKeyX']);

userPublicKeyYInput = driver2.find_element_by_id('pubKey_y');
userPublicKeyYInput.send_keys(user2['publicKeyY']);

driver2.close();


startAttestation = driver2.find_element_by_id("A_btn_start_Attestation")
startAttestation.click()
