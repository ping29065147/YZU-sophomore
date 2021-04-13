一開始選擇直接使用鍵盤輸入或是讀檔
鍵盤輸入按 1
讀檔按 2

區別在於鍵盤輸入為一次輸出一行，讀檔會全部讀完後一次輸出

class risc-v主要有5個function

1. load(string input, int line)：先呼叫reset()清除，將讀入的整行字串分割並存入vector<string> V，如果有label則將label與它所在的位置存入map<string, string> label

2. set(vector<string> v)：將空格移除後存入vector<string> V

3. convert()：先將指令轉為大寫，根據每一個指令的type，將對應的machine code存入result

4. print()：將結果印出

5. print_detail()：將結果的詳細資料印出

還有一些其他無法直接從外部呼叫function

1. constructor：將各個指令必要的machine code存入各自的map，例如func3、opcode等

2.reset()：清除vector<string> V及result

3. binary(string rs, int len)：讀入一個數字字串與長度，將此數字轉為二進制並補足0至長度，回傳此二進制字串