*** Variables ***
${IP}       192.168.50.1
${NAME}     admin
${PWD}      admin123
#${DUT_NAME}     GT-AXE16000
#${DUT_NAME}      RT-AC86U
#${DUT_NAME}                 ZenWiFi_Pro_ET12
${DUT_NAME}     ZenWiFi_XT9
#${DUT_NAME}     ZenWiFi_XT8
${RE_IP}        192.168.51.200
${RE_NAME}      RT-AC86U
#${RE_NAME}      ET12
#${RE_NAME}      GT-AXE16000
#${RE_NAME}                 ZenWiFi_XT9
${DUT_END}      ${NAME}@${DUT_NAME}
${LOG_PATH}      C:\\Users\\Anne_Zheng\\Documents\\CAP\\FILE\\
#${LOCAL_TO_DUT_PATH}     C:\\Users\\Anne_Zheng\\Documents\\CAP\\FILE\\CAP_XX\\GT-AXE16000(Quband)\\
${LOCAL_TO_DUT_PATH}      C:\\Users\\Anne_Zheng\\Documents\\CAP\\FILE\\TEST\\
${CAP_PATH}     ${LOCAL_TO_DUT_PATH}CAP
${RE_PATH}      ${LOCAL_TO_DUT_PATH}RE
${CAP_PATTERN}      '.*:.*:.*\\|.*json'
${CAP_PATTERN_PY}   .*:.*:.*|.*json
${RE_PATTERN}      '.*json\\|cfg_.*\\|amas'
${RE_PATTERN_PY}   .*json|cfg_.*|amas
${HANDLE_TIME}      50
${SMART_CONNECT_OPTION}     //tr[@id="smart_connect_field"]//td//select//option
${SMART_CONNECT_INPUT}      //tr[@id="smart_connect_band_field"]//td//input
${BANDINDEX_DOWN_PATH}      http://netwlszbuild.asuscomm.com:10010/4G-AX56/amas_bandindex_test
${CHUNK_SIZE}        600       # 每个片段的大小
@{CAP_RE_OTHER_KEY}     wpa_psk   crypto      wep_x   key   key1   key2   key3    key4    macmode   maclist_x     closed     radius_ipaddr    radius_key     radius_port       ap_isolate
@{CAP_RE_OTHER_VALUE}    admin123    aes         0     1      \     \     \    \     disabled      \     0      \     \    1812    0
&{CAP_RE_VALUE}     2G=admin         5G=admin_5G     5GL=admin_5G-1     5GH=admin_5G-2      6G=admin_6G     6GL=admin_6G-1    6GH=admin_6G-2
${BAND_NAME}    TRIBAND6G_v1