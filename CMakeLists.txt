cmake_minimum_required(VERSION 3.11.4 FATAL_ERROR)
project(irods_client_library_c LANGUAGES C CXX)

set(CMAKE_C_COMPILER /opt/irods-externals/clang6.0-0/bin/clang)
set(CMAKE_CXX_COMPILER /opt/irods-externals/clang6.0-0/bin/clang++)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#find_package(Boost 1.67 REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(IRODS_LIBRARY_NAME irods_client_c)

set(IRODS_LIBRARY_VERSION_MAJOR "0")
set(IRODS_LIBRARY_VERSION_MINOR "0")
set(IRODS_LIBRARY_VERSION_PATCH "1")
set(IRODS_LIBRARY_VERSION ${IRODS_LIBRARY_VERSION_MAJOR}.${IRODS_LIBRARY_VERSION_MINOR}.${IRODS_LIBRARY_VERSION_PATCH})

set(IRODS_API_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/api/rcAuthCheck.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcAuthPluginRequest.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcAuthRequest.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcAuthResponse.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcBulkDataObjPut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcBulkDataObjReg.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcChkNVPathPerm.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcChkObjPermAndStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcClientHints.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcCloseCollection.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcCollCreate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcCollRepl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataCopy.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataGet.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjChksum.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjClose.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjCopy.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjCreate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjCreateAndStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjGet.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjLock.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjLseek.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjOpen.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjOpenAndStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjPhymv.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjPut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjRead.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjRename.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjRepl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjRsync.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjTrim.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjTruncate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjUnlink.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataObjWrite.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcDataPut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcEndTransaction.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcExecCmd.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcExecMyRule.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcExecRuleExpression.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileChksum.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileChmod.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileClose.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileClosedir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileCreate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileGet.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileGetFsFreeSpace.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileLseek.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileMkdir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileOpen.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileOpendir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFilePut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileRead.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileReaddir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileRename.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileRmdir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileStageToCache.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileSyncToArch.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileTruncate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileUnlink.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcFileWrite.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGenQuery.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGeneralAdmin.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGeneralRowInsert.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGeneralRowPurge.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGeneralUpdate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetHierFromLeafId.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetHierarchyForResc.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetHostForGet.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetHostForPut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetLimitedPassword.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetMiscSvrInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetRemoteZoneResc.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetRescQuota.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetTempPassword.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcGetTempPasswordForOther.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcIESClientHints.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcL3FileGetSingleBuf.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcL3FilePutSingleBuf.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcModAVUMetadata.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcModAccessControl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcModColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcModDataObjMeta.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcObjStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcOpenCollection.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcOprComplete.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcPamAuthRequest.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcPhyBundleColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcPhyPathReg.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcProcStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcQuerySpecColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcReadCollection.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRegColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRegDataObj.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRegReplica.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRmColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRuleExecDel.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRuleExecMod.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcRuleExecSubmit.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcServerReport.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSimpleQuery.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSpecificQuery.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSslEnd.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSslStart.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStreamClose.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStreamRead.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStructFileBundle.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStructFileExtAndReg.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStructFileExtract.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcStructFileSync.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileClose.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileClosedir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileCreate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileGet.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileLseek.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileMkdir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileOpen.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileOpendir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFilePut.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileRead.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileReaddir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileRename.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileRmdir.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileStat.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileTruncate.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileUnlink.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSubStructFileWrite.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcSyncMountedColl.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcTicketAdmin.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcUnbunAndRegPhyBunfile.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcUnregDataObj.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcUserAdmin.cpp
    ${CMAKE_SOURCE_DIR}/src/api/rcZoneReport.cpp)

set(IRODS_CORE_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/core/apiHandler.cpp
    ${CMAKE_SOURCE_DIR}/src/core/base64.cpp
    ${CMAKE_SOURCE_DIR}/src/core/clientLogin.cpp
    ${CMAKE_SOURCE_DIR}/src/core/getRodsEnv.cpp
    ${CMAKE_SOURCE_DIR}/src/core/hashtable.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_auth_factory.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_auth_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_auth_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_buffer_encryption.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_c_api.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_children_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_client_api_table.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_client_negotiation.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_configuration_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_default_paths.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_environment_properties.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_error.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_exception.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_generic_auth_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_get_full_path_for_config_file.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_gsi_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_hierarchy_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_hostname.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_krb_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_kvp_string_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_log.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_native_auth_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_network_factory.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_network_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_network_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_osauth_auth_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_pack_table.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_pam_auth_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_parse_command_line_options.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_path_recursion.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_pluggable_auth_scheme.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_plugin_name_generator.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_random.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_serialization.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_server_properties.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_socket_information.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_ssl_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_stacktrace.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_string_tokenize.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_tcp_object.cpp
    ${CMAKE_SOURCE_DIR}/src/core/irods_virtual_path.cpp
    ${CMAKE_SOURCE_DIR}/src/core/list.cpp
    ${CMAKE_SOURCE_DIR}/src/core/miscUtil.cpp
    ${CMAKE_SOURCE_DIR}/src/core/msParam.cpp
    ${CMAKE_SOURCE_DIR}/src/core/obf.cpp
    ${CMAKE_SOURCE_DIR}/src/core/osauth.cpp
    ${CMAKE_SOURCE_DIR}/src/core/packStruct.cpp
    ${CMAKE_SOURCE_DIR}/src/core/parseCommandLine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/procApiRequest.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rcConnect.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rcGlobal.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rcMisc.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rcPortalOpr.cpp
    ${CMAKE_SOURCE_DIR}/src/core/region.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rodsLog.cpp
    ${CMAKE_SOURCE_DIR}/src/core/rodsPath.cpp
    ${CMAKE_SOURCE_DIR}/src/core/sockComm.cpp
    ${CMAKE_SOURCE_DIR}/src/core/sslSockComm.cpp
    ${CMAKE_SOURCE_DIR}/src/core/stringOpr.cpp)

set(IRODS_HASHER_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/hasher/Hasher.cpp
    ${CMAKE_SOURCE_DIR}/src/hasher/MD5Strategy.cpp
    ${CMAKE_SOURCE_DIR}/src/hasher/SHA256Strategy.cpp
    ${CMAKE_SOURCE_DIR}/src/hasher/checksum.cpp
    ${CMAKE_SOURCE_DIR}/src/hasher/irods_hasher_factory.cpp)

set(IRODS_RBUDP_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/rbudp/QUANTAnet_rbudpBase_c.cpp
    ${CMAKE_SOURCE_DIR}/src/rbudp/QUANTAnet_rbudpReceiver_c.cpp
    ${CMAKE_SOURCE_DIR}/src/rbudp/QUANTAnet_rbudpSender_c.cpp)

add_library(${IRODS_LIBRARY_NAME}
            SHARED
            ${IRODS_API_SOURCE_FILES}
            ${IRODS_CORE_SOURCE_FILES}
            ${IRODS_HASHER_SOURCE_FILES}
            ${IRODS_RBUDP_SOURCE_FILES})

set_target_properties(${IRODS_LIBRARY_NAME}
                      PROPERTIES
                      VERSION ${IRODS_LIBRARY_VERSION}
                      SOVERSION ${IRODS_LIBRARY_VERSION_MAJOR})

target_compile_definitions(${IRODS_LIBRARY_NAME}
                           PRIVATE
                           RODS_REL_VERSION="rods4.3.0"
                           RODS_API_VERSION="d"
                           IRODS_VERSION_MAJOR=4
                           IRODS_VERSION_MINOR=3
                           IRODS_VERSION_PATCHLEVEL=0)

target_compile_options(${IRODS_LIBRARY_NAME} PRIVATE -Wall -stdlib=libc++ -pthread -Wno-writable-strings)

target_include_directories(${IRODS_LIBRARY_NAME}
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/include/api
                           ${CMAKE_SOURCE_DIR}/include/core
                           ${CMAKE_SOURCE_DIR}/include/hasher
                           ${CMAKE_SOURCE_DIR}/include/rbudp
                           #${CMAKE_SOURCE_DIR}/server/core/include
                           #${CMAKE_SOURCE_DIR}/server/drivers/include
                           #${CMAKE_SOURCE_DIR}/server/icat/include
                           #${CMAKE_SOURCE_DIR}/server/re/include
                           ${OPENSSL_INCLUDE_DIR}
                           /opt/irods-externals/boost1.67.0-0/include
                           /opt/irods-externals/json3.1.2-0/include
                           /opt/irods-externals/spdlog0.17.0-0/include)

target_link_libraries(${IRODS_LIBRARY_NAME}
                      PRIVATE
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_chrono.so
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_filesystem.so
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_program_options.so
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_regex.so
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_system.so
                      /opt/irods-externals/boost1.67.0-0/lib/libboost_thread.so
                      ${OPENSSL_CRYTO_LIBRARY}
                      ${OPENSSL_SSL_LIBRARY}
                      ${CMAKE_THREAD_LIBS_INIT})

