先選擇要執行`easy input`還是`hard input`，`hard input`要輸入`entries`

# class `Predictor` 的主要功能如下：

首先是`public`裡的內容

1. `void execute(string ans)`
    這個是執行`easy input`的程式。
	先呼叫`init(0)`初始化，接著根據`branch`的數目呼叫`next_state`，最後再將結果印出。
   
2. `void execute(vector<string> V, int entry)`
    這個是執行`hard input`的程式。
	先呼叫`init(entry)`初始化，接著將`instruction`拆解並儲存，分別存入`PC`、`Label`及`program`
	之後呼叫`solve()`處理，最後再將結果印出。
	
以下是`private`的內容

1. `void init(int a)` 將所有資料初始化
	
2. `string binary(int his)` 將`his`轉為2進位

3. `string status(char A, int e)` 用來判斷下一個狀態

4. `void next_state(char A, int e)` 新增一筆`Data`存入`tmp2`(之後存入`ret`)，預測`branch`或`not branch`

5. `char compiler()` 執行`instruction`並將`program counter`移到下一個位置