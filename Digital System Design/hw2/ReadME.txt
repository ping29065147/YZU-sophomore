
 ( 在k_map.cpp中的readfile中可修改讀入的檔案名稱。 )

直接執行程式即可產生結果。

k_map::readfile
	讀入檔案至inFile中，將Case的數量存入num，如果檔案不存在，則輸出File doesn't exist.並結束程式。

k_map::getnum
	回傳num

k_map::readdata
	將-1存入square[0]，將input的資料從square[1]開始存入，共存入(variable*variable)(4*4)個

k_map::dealwithkmap
	先呼叫k_map::printkmap印出kmap，將square中的資料按照kmap的編排排序
	m0  m1  m3  m2
	m4  m5  m7  m6
	m12 m13 m15 m16
	m8  m9  m11 m10

k_map::find_ones_dontcare
	找出square中的1's compliment 與 don't care compliment，將雙方轉成二進位後存入m，將1's compliment存入ones
	呼叫k_map::print_ones_dontcare，印出1's compliment、don't care compliment 與轉為二進位後的值

k_map::compare
	從m中進行兩兩比對，將差異為1的數據存入save，並將不同的值改為2
	例如: 1000 與 1001 存成 1002

k_map::printcompare
	將比對後的數據印出

k_map::searchminterm
	利用dfs找出所有可覆蓋kmap的組合，並存入final_result

k_map::printresults
	找出可覆蓋kmap的最小組合，並將他們全部印出

k_map::clear
	清除資料，繼續下一個case