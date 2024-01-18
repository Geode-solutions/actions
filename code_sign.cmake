cmake_minimum_required(VERSION 3.18)

set(SIGN_COMMAND "AzureSignTool sign \
--azure-key-vault-url \"$ENV{AZURE_KEY_VAULT_URI}\" \
--azure-key-vault-client-id \"$ENV{AZURE_CLIENT_ID}\" \
--azure-key-vault-tenant-id \"$ENV{AZURE_TENANT_ID}\" \
--azure-key-vault-client-secret \"$ENV{AZURE_CLIENT_SECRET}\" \
--azure-key-vault-certificate $ENV{AZURE_CERT_NAME} \
--timestamp-rfc3161 http://timestamp.digicert.com -v \
")
message(STATUS "Sign command: ${SIGN_COMMAND}")
message(STATUS "Temporary install directory: ${CPACK_TEMPORARY_INSTALL_DIRECTORY}")

find_program(AZURE_SIGN_TOOL AzureSignTool REQUIRED)

message(STATUS "AzureSignTool: ${AZURE_SIGN_TOOL}")

execute_process(
    COMMAND "AzureSignTool --help"
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
)

file(GLOB_RECURSE DLL_FILES "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/*.dll")
message(STATUS "DLL files: ${DLL_FILES}")


foreach(DLL_FILE ${DLL_FILES})
    message(STATUS "Signing ${DLL_FILE}")
    execute_process(
        COMMAND ${SIGN_COMMAND} ${DLL_FILE}
        RESULT_VARIABLE SIGN_RESULT
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
    )
    if(SIGN_RESULT EQUAL 0)
        message(STATUS "Command executed successfully on ${DLL_FILE}")
    else()
        message(STATUS "Command failed on ${DLL_FILE}")
    endif()
endforeach()