function main(){
	// loading configure files
	// 1. local upload priv_key
	var g_sp_priv_key;
	// constructure client
	client(g_sp_priv_key);
}

function client(g_sp_priv_key){

	// First attestation with username and password

	// Second attestation with g_sp_priv_key
	attestation_client(g_sp_priv_key)
}