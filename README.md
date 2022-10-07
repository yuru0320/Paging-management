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
  * 方法一: FIFO，採取先到先置換的方式
  當有需要置換時，優先選擇最先進入 queue 中的 page，且不需再改變時間標記，意即每次選擇 queue 中的第 0 筆置換。
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

  ### 依照上圖及各置換方法所採取的特性，可以得知以下:
1. FIFO: 為先到先置換的處理方法，理由是:最早調入主記憶體的 頁面不再被使用的可能性最大。可以透過增加頁框數來減少 Page Fault、頁置換的狀況發生，但有少數特例為畢雷笛反例，增加頁 框數反而增加了 Page Fault、Page Replaces。此種演算法只在按線 性順序訪問地址空間時才是理想的，否則效率不高。因為常被訪 問的 page，可能會因先到達的順序不得不被置換出去，ex: input2。其中，上圖的 input1 就是畢雷笛反例，所以若增加頁框 數反而會增加 Page Fault、Page Replaces。
  
2. LRU : 當需要置換一個頁面時，選擇最久不用的頁面，理由是: 過去一段時間裡不曾被訪問過的頁面，在最近的將來可能也不會 再被訪問，此演算法是經常採用的頁面置換演算法，並被認為是 相當好的。在 LRU 演算法中，隨著頁的增加，Page Fault、Page Replaces 會減少。
  
3. LFU + LRU: 當需要置換頁面時，選擇最少使用的 page，若相同 則選擇最久不用的頁面，理由是:過去一段時間裡較少被訪問的 頁面，在最近的將來可能也不會再被訪問。在 input2 可以看出此 方法的 Page Fault、Page Replaces 都為最少的，在 input1 也沒有 與其他方法相差許多。可以看出 page 越多 Page Fault、Page Replaces 與其他相比就會減少。
  
4. MFU + FIFO : 當需要置換頁面時，選擇最多使用的 page，若相 同則選擇最久的頁面，理由是:過去一段時間裡較多被訪問的頁 面，可能已經處理完畢，所以在最近的將來可能也不會再被訪 問。在 input2 可以看出此方法由於搭配 FIFO 而導致效率並不 高，Page Fault、Page Replaces 次數較多。
  
5. MFU + LRU:與上述 MFU 方法同理，不過相同則選擇最久不被使 用的頁面，使得在 input2 可以看出此方法搭配 LRU 為更好的方 法，Page Fault、Page Replaces 次數大幅下降。
  
* 畢雷笛反例(Belady’s Anomaly):為少數特例，在 FIFO 的置換方法下，增加頁 框數，反而造成更多的 Page Fault、Page Replaces。將記憶體容量增加以增加分 頁可用欄位的時候，應該會減少分頁錯誤的次數;可是有時因為可用欄位的增 加而降低了 CPU 的使用率，OS 因而加入了更多分頁，導致可用欄位相對不 足，分頁錯誤便沒有減少。這種違反一般趨勢的現象，即畢雷笛反例。這次作 業的 input1 就是畢雷笛反例，當我增加 Page Frames=4 時，Page Fault = 10 相 比於 Page Frames=3 時增加了，可以看出增加頁框數並不一定能夠減少 Page Fault。
