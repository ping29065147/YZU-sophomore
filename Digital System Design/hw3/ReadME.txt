(28 - 37行)
第一步先將 .kiss 檔讀入
將 .i .o .p .s .r 分別以 in out term state reset 儲存

(38 - 46行)
以 map< string, vector<tp> > data 儲存每個 current state的 next state, input, output
並將所有 state 存入 vector<string> V
將所有 output 的數量存入 map<string, int> same;

(50 - 80行)
接著開始化簡
若符合以下兩個情況其中一個，則可化簡
1. 兩個 current state 的 next state, input, output 皆相同
2. 兩個 current state 的 input 沒有衝突

(83 - 88行)
特殊情況化簡
當所有 output 皆相同時，可化簡至1個 state

(91 - 98行)
印出 data 的內容

(102 - 113行)
印出結果(.kiss檔格式)