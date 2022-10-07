# Paging-management
實作Memory中的分頁法，包括FIFO(先進先出), LRU(最近罕用),  LFU + LRU，(被參考或修改最少次數的 page 優先)，MFU + FIFO ，(參考或修改最多次數的 page 優先) ， MFU + LRU (被參考或修改最多次數的 page 優先)。
# 開發環境 : 
macOS Monterey
# 使用開發環境 : 
Xcode Version 13.2.1 (13C100) 
# 使用的程式語言 : 
C++
# 實作方法與流程

# 流程:
1. 根據使用者的檔名先去判斷檔案是否存在，若不存在則再輸入一次， 反之則進行下一步驟。
2. 成功開啟檔案後，讀取檔案第一行的數字，分別紀錄為全域變數的處 理方法及 page_frame。
3. 再依序讀入所需資料且存入 vector<int> page_Reference;
4. 根據讀入的方法，做相對應的處理。
5. 輸出檔案
  <img width="517" alt="截圖 2022-10-07 下午6 26 42" src="https://user-images.githubusercontent.com/95215851/194532963-7df2dcc8-1b43-4d83-b08c-0fef9cf909b4.png">

### 實作方法:
1. 讀入使用者輸入的檔名，打開檔案，先讀入方法、pageframe的個數後，再讀入 Page Reference 的次序。
2. 再依照讀入的方法，判斷要做何種處理原則。
3. 每次取出PageReference第0筆來做處理，加入pageframe中，判斷是否需要置換、是否有 Page Faults 的情況發生。

用 struct 來儲存被參考到的 page 次數及 page reference。

* 方法一: FIFO，採取先到先置換的方式，
  當有需要置換時，優先選擇最先進入 queue 中的 page，且不需再改變時間標記，意 即每次選擇 queue 中的第 0 筆置換。
* 方法二: LRU，採取將過去最久沒用到的page優先置換，
  當有需要置換時，優先選擇最久沒使用到的 page，並且當每次 page 被參考時，需要再改變時間標記，意即每次選擇 queue 中的第 0 筆置換。
* 方法三: LFU + LRU，採取被參考或修改最少次數的 page 優先置換。 
  
  每個頁框均有一個對應的計數器，起始值為 0，當被參考或修改則加一，若是有相同計數器值時，採取將過去最久沒用到的 page 優先置換。
* 方法四: MFU + FIFO ，採取被參考或修改最多次數的 page 優先置換。
  
  每個頁框均有一個對應的計數器，起始值為 0，當被參考或修改則加一，若是有相同計數器值時，採取採取先到 page 先置換的方式。
* 方法五: MFU + LRU ，採取被參考或修改最多次數的 page 優先置換。
  
  每個頁框均有一個對應的計數器，起始值為 0，當被參考或修改則加一，若是有相同計數器值時，採取將過去最久沒用到的 page 優先置換。
* 方法六: 為執行1-6方法後的結果。

# 不同方法的比較 :
  <img width="521" alt="截圖 2022-10-07 下午6 27 53" src="https://user-images.githubusercontent.com/95215851/194533181-7bb42ca1-4f44-440f-9667-96caafd45be0.png">
<img width="556" alt="截圖 2022-10-07 下午6 27 59" src="https://user-images.githubusercontent.com/95215851/194533195-07bd78f1-57a9-4e09-bbf5-7c414e211a9a.png">

  
