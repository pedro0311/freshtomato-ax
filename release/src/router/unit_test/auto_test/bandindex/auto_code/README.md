#使用方式：
1.目前已編譯sdk為5.02hnd和5.04axhnd.675x,其他需要重新編譯，編譯方式詳見code文件夾中的README。
2.DUT的對應機型目前支持DUALBAND，QUADBAND，TRIBAND，TRIBAND6G的測試，請根據DUT選擇相應的文件夾進行測試。
3.請將amas_bandindex_test程序與2步驟中選擇的文件夾內的CAP與RE文件夾放入DUT的/jffs下。
  註意：預留的amas_bandindex_test需要將括號及括號內去掉。
4.3步驟中的移動方式可使用u盤或使用testcase搬運（速度較慢），但是amas_bandindex_test程序不能使用testcase移至DUT內，請使用其他方式移動。
5.DUT在設定時請將DUT的無線網絡設置為：2G：admin，5G-1：admin_5G-1,5G-2:admin_5G-2,5G:admin_5G,6G-1:admin_6G-1,6G-2:admin_6G-2,6G:admin_6G,其密碼均為admin123
6.將文件全部移動後，即可直接使用auto test中的CAP_ALL_TEST與RE_ALL_TEST進行全部測試。

#其他：
1.若DUT為其他BAND，則需要連接實體機器，先進行抓相應的配置，再進行測試，可使用auto test中的Get_All_File進行自動獲取（可能部分需要手動輔助）。

