# Project 3: Tomasulo

## input data
```cpp
ADDI F1, F2, 1
SUB F1, F3, F4
DIV F1, F2, F3
MUL F2, F3, F4
ADD F2, F4, F2
ADDI F4, F1, 2
MUL F5, F5, F5
ADD F1, F4, F4
```

## 程式說明

建構子先將一些必需的資料初始化
1. `RF table`設為 2 的倍數
2. `RAT table`設為空
3. `RS table`設為空
4. `BUFFER`設為空
5. 設定一些基本的`register`順序
6. 設定不同`instruction`的`cycle time`

將`input`資料讀入`vector<string> code`

呼叫`print_input()`印出`input data`
並呼叫`execute()`開始處理`input`

---

主程式`void execute()`，主要由各種判斷組成，判斷是否要進行`issue`、`dispatch`、`write back`等動作
跑一個無窮迴圈，從`cycle 1`開始執行程式，迴圈結束條件為所有`instruction`都跑完

首先判斷是否還有剩餘的`instruction`，若還有剩餘，則拆解後嘗試`issue`
若當前的`RS table`，則呼叫`void issue(vector<string> V, int i)`將`instruction`的資料存入`RS table`與`RAT table`

第二步先將在`BUFFER`裡的程式倒數`1`
若倒數結束(為0)，將計算完的結果`write back`，呼叫`void write_back(int n)`將在`RS table`裡的`register`填入對應值，儲存結果
並清除那個`register`在`RS table`與`BUFFER`的資料

第三步判斷在`RS table`裡的資料是否已經`ready`
若已經`ready`且當前`BUFFER`的資料`dispatch`，呼叫`void dispatch(int i, int n)`將`RS table`的資料寫入當前`BUFFER`

最後，如果有改變的話，將這個`cycle`的結果印出來
並判斷是否做完了所有的`instruction`

---

跳出迴圈後，將先前儲存的結果再寫回`RF table`

