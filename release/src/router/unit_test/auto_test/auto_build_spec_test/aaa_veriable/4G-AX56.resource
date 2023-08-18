*** Variables ***
${BRANCH}         asuswrt-router-3004-386-rc4    # 目標分支
${DUT_NAME}       RT-AX53U    #DUT的名字
${DUT_PARA}       \    #需要開啟的其他功能
${BREANCH_PATH}    asuswrt-3004/asuswrt    #code所在的文件夾
${DUT_PATH}       release/src-ra-openwrt-4210    #make的地址
${SSH_PATH}       192.168.16.65    #code所在ssh的地址
${SSH_PORT}       22    #code所在ssh的地址的端口
${SSH_NAME}       anne    #ssh的名字
${SSH_PASSWORD}    anne@123    #ssh的密码
${GIT_NAME}       anne.zheng    #git時的名稱
${GIT_PASSWORD}    anne@123    #git時的密碼
${DOWNLOAD_PATH}    http:\/\/netwlszbuild.asuscomm.com:10010\/4G_AX56\/AUTO_TEST    #FW的下載地址
${SERVER_PATH}    \/var\/www\/html\/4G_AX56\/AUTO_TEST    #buildsever所在文件夹
${DUT_IP}         192.168.51.1    #DUT的ip地址
${DUT_USER}       admin
${DUT_PASSWORD}    admin123
${CLEAR_FLAG}     0    #是否需要清潔code，0為否，1為是
${REDMINE}        http://172.22.136.75:3000/
${URL}            http://172.22.136.75:3000/projects/asuswry/issues
${RED_USER}       anne_zheng@asus.com
${RED_PWD}        anne@123
${SPEC_PATH}      C:\\eSupport\\test\\SPEC    #spec相關文件將會保存的路徑
${SPEC_CONF}      C:\\eSupport\\test\\ROBOT_SPEC\\aaa_spec    #speccheck.conf所在的路徑
#${SPEC_READMINE} \ \ \ \
${SPEC_READMINE}    C:\\eSupport\\test\\SPEC    #從readmine上下載的csv文件的位置，若能連接readmine，則可以為空，默認放置在與spec相關文件的位置    \
@{CONFIG}         .\/.config    .\/shared\/defaults.prep    .\/bwdpi_source\/asus\/dpi_support.prep    .\/shared\/hw_auth.prep    .\/rc\/init.prep    .\/shared\/tcode.prep
${READ_END}       ${SSH_NAME}@buildserver
${DUT_END}        ${DUT_USER}@${DUT_NAME}
