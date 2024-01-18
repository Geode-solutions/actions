cmake_minimum_required(VERSION 3.18)

find_program(AZURE_SIGN_TOOL AzureSignTool REQUIRED)
file(GLOB_RECURSE DLL_FILES "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/*.dll")
foreach(DLL_FILE ${DLL_FILES})
    message(STATUS "Signing ${DLL_FILE}")
    execute_process(
        COMMAND "${AZURE_SIGN_TOOL}" sign --azure-key-vault-url "$ENV{AZURE_KEY_VAULT_URI}" --azure-key-vault-client-id "$ENV{AZURE_CLIENT_ID}" --azure-key-vault-tenant-id "$ENV{AZURE_TENANT_ID}" --azure-key-vault-client-secret "$ENV{AZURE_CLIENT_SECRET}" --azure-key-vault-certificate $ENV{AZURE_CERT_NAME} --timestamp-rfc3161 http://timestamp.digicert.com -v ${DLL_FILE}
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
    )
endforeach()