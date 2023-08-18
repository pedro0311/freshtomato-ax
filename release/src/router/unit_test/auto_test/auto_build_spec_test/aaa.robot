*** Settings ***
Library           SSHLibrary
Library           String
Library           Collections
Library           Telnet
Library           aaa_function/aaa_function.py
Library           aaa_function/speccheck.py
Library           aaa_function/specparse.py
Resource          TEST_DUT.txt
Resource          aaa_veriable/${VAR_PATH}.resource

*** Test Cases ***
check_build_FW
    check_build_FW

*** Keywords ***
check_build_FW
    SSHLibrary.open connection    ${SSH_PATH}    prompt_is_regexp=yes    timeout=2000    port=${SSH_PORT}
    SSHLibrary.login    ${SSH_NAME}    ${SSH_PASSWORD}
    ${tmp}    Write_Read    SSH    0    cd ${BREANCH_PATH}    NULL
    ${branch_new}    Write_Read    SSH    0    git branch |grep \'*\'    ${READ_END}
    ${flag}    String_contain    ${branch_new}    ${BRANCH}    " "
    #Determine if it is currently on the correct branch.
    IF    ${flag}==0
    #If not, replace with the correct branch.
        ${output}    Write_Read    SSH    0    git checkout ${BRANCH}    ${READ_END}
        ${branch_new}    Write_Read    SSH    0    git branch |grep \'*\'    ${READ_END}
        ${branch_flag}    String_contain    ${branch_new}    ${BRANCH}    ${READ_END}
        IF    ${branch_flag}==0
            FAIL
        END
    END
    #Update Code.
    ${tmp}    Write_Read    SSH    0    git pull origin ${BRANCH}    Username for
    ${tmp}    Write_Read    SSH    0    ${GIT_NAME}    Password for
    ${tmp}    Write_Read    SSH    0    ${GIT_PASSWORD}    ${READ_END}
    Write_Read    SSH    2    cd ${DUT_PATH}    ${READ_END}
    #build FW
    #Write_Read    SSH    100    make -s ${DUT_NAME} ${DUT_PARA} > make.txt    ${READ_END}
    #Determine if FW exists.
    ${fw_flag}    Write_Read    SSH    0    find .\/image -name \"${DUT_NAME}_*.trx\" |tail -1    ${READ_END}
    ${fw_len}    Get length    ${fw_flag}
    ${READ_END_LEN}    Get length    \r\n${READ_END}
    IF    ${fw_len}>${READ_END_LEN}
        ${fw_path}    String_sqlit_end    ${fw_flag}    ${READ_END}
        @{docement_list}=    String.Split String    ${fw_path}    \/
        ${fw_name}    Get From List    ${docement_list}    -1
    #Create model files.
        ${spec_path_true}    Get_Spec_Folder    ${SPEC_PATH}    ${DUT_NAME}
        ${fw_size}    Write_Read    SSH    0    du -b ${fw_path}    ${READ_END}
        ${models}    Write_Read    SSH    0    cat ..\/models    ${READ_END}
        Get_Other_File    ${spec_path_true}    ${models}    ${branch_new}    ${fw_size}    ${DUT_NAME}    ${READ_END}
        Write_Read    SSH    0    cd ..\/src\/router    ${READ_END}
        Get_Config_File    ${spec_path_true}
    #end
        speccheck    ${SPEC_CONF}    ${spec_path_true}
        ${config_flag}    specparse    ${REDMINE}    ${URL}    ${spec_path_true}    ${SPEC_READMINE}    ${spec_path_true}    ${RED_USER}    ${RED_PWD}
        if    ${config_flag}==0
            FAIL
        END
        Write_Read    SSH    0    cp -rf ${fw_path} ${SERVER_PATH}    ${READ_END}
    ELSE
        FAIL
    END
    #Clean up newly generated files.
    IF    ${CLEAR_FLAG}==1
        Write_Read    SSH    100    cd ../../ && rm -rf release    ${READ_END}
        Write_Read    SSH    100    git checkout release    ${READ_END}
    END
    SSHLibrary.Close connection
    #Connect to the DUT's telnet.
    Telnet.open connection    ${DUT_IP}    prompt_is_regexp=yes    prompt=\#    timeout=2000
    Telnet.login    ${DUT_USER}    ${DUT_PASSWORD}
    #Determine whether the current FW is consistent with the newly generated FW.
    ${fw_name_new}    Write_Read    Telnet    0    nvram get extendno    ${DUT_END}
    ${fw_flag}    String_contain    ${fw_name}    ${fw_name_new}    ${DUT_END}
    IF    ${fw_flag}==0
    #If inconsistent, update FW.
        Write_Read    Telnet    2    cd /tmp    ${DUT_END}
        Write_Read    Telnet    2    wget ${DOWNLOAD_PATH}\/${fw_name}    ${DUT_END}
        Write_Read    Telnet    3    mv ${fw_name} linux.trx    ${DUT_END}
        Write_Read    Telnet    3    rc rc_service restart_upgrade    NULL
        Telnet.Close connection
        sleep    200
        Telnet.open connection    ${DUT_IP}    prompt_is_regexp=yes    prompt=\#    timeout=100
        Telnet.login    ${DUT_USER}    ${DUT_PASSWORD}
        ${fw_name_new}    Write_Read    Telnet    0    nvram get extendno    ${DUT_END}
        ${fw_flag}    String_contain    ${fw_name}    ${fw_name_new}    ${DUT_END}
    #Determine if the update was successful.
        IF    ${fw_flag}==0
            LOG    Update failed.
            FAIL
        END
    ELSE
            LOG    FW is already the latest.
    END
    Telnet.Close connection

Write_Read
    [Arguments]    ${type}    ${time}    ${option}    ${end}
    IF    '${type}'=="SSH"
        SSHLibrary.write    ${option}
        sleep    ${time}
        IF    '${end}'=="NULL"
            ${requset}    SSHLibrary.Read
        ELSE
            ${requset}    SSHLibrary.Read until    ${end}
        END
    ELSE IF    '${type}'=='Telnet'
        Telnet.Write    ${option}
        sleep    ${time}
        IF    '${end}'=="NULL"
            ${requset}    Telnet.Read
        ELSE
            ${requset}    Telnet.Read until    ${end}
        END
    END
    [Return]    ${requset}

Get_Config_File
    [Arguments]    ${path}
    FOR    ${config_file}    IN    @{CONFIG}
        LOG    ${config_file}
        ${config_request}    Write_Read    SSH    0    cat ${config_file}    ${READ_END}
        Config_write_File    ${config_request}    ${config_file}    ${path}    ${READ_END}    ${DUT_NAME}
    END
