cmake_minimum_required(VERSION 3.12)

set(SIGN_COMMAND "AzureSignTool sign \
-kvu \"$ENV{ AZURE_KEY_VAULT_URI }\" \
-kvi \"$ENV{ AZURE_CLIENT_ID }\" \
-kvt \"$ENV{ AZURE_TENANT_ID }\" \
-kvs \"$ENV{ AZURE_CLIENT_SECRET }\" \
-kvc $ENV{ AZURE_CERT_NAME } \
-tr http://timestamp.digicert.com -v \
")
message(STATUS "Sign command: ${SIGN_COMMAND}")

file(GLOB DLL_FILES "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/bin/*.dll")

foreach(DLL_FILE ${DLL_FILES})
    message(STATUS "Signing ${DLL_FILE}")
    execute_process(
        COMMAND ${SIGN_COMMAND} ${DLL_FILE}
        RESULT_VARIABLE SIGN_RESULT
        OUTPUT_VARIABLE SIGN_OUTPUT
    )
    if(SIGN_RESULT EQUAL 0)
        message(STATUS "Command executed successfully on ${DLL_FILE}")
    else()
        message(STATUS "Command failed on ${DLL_FILE}")
        message(STATUS "Output: ${SIGN_OUTPUT}")
    endif()
endforeach()