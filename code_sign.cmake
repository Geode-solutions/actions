cmake_minimum_required(VERSION 3.18)

# CPack pre-build script: signs the binaries of the package with Azure Artifact Signing.
# The environment comes from the install/azure-sign action, which only sets it on master and next.
if(NOT DEFINED ENV{ARTIFACT_SIGNING_TOKEN})
    message(STATUS "Code signing skipped: no Artifact Signing token")
    return()
endif()

find_program(JAVA java REQUIRED)
file(GLOB_RECURSE SIGNED_FILES
    "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/*.dll"
    "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/*.exe"
    "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/*.pyd"
)
if(NOT SIGNED_FILES)
    return()
endif()
list(LENGTH SIGNED_FILES NB_FILES)
message(STATUS "Signing ${NB_FILES} files")
string(REPLACE ";" "\n" FILE_LIST "${SIGNED_FILES}")
file(WRITE "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/../code_sign_files.txt" "${FILE_LIST}\n")
execute_process(
    COMMAND "${JAVA}" -jar "$ENV{JSIGN_JAR}"
        --storetype TRUSTEDSIGNING
        --keystore "$ENV{ARTIFACT_SIGNING_ENDPOINT}"
        --storepass env:ARTIFACT_SIGNING_TOKEN
        --alias "$ENV{ARTIFACT_SIGNING_PROFILE}"
        --tsaurl http://timestamp.acs.microsoft.com
        --tsmode RFC3161
        --tsretries 3
        --tsretrywait 10
        "@${CPACK_TEMPORARY_INSTALL_DIRECTORY}/../code_sign_files.txt"
    RESULT_VARIABLE SIGN_RESULT
)
if(NOT SIGN_RESULT EQUAL 0)
    message(FATAL_ERROR "Code signing failed (${SIGN_RESULT})")
endif()
