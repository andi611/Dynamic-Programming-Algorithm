# Algorithm_dynamic-programming_C_plusplus
## A program that generates the Bézier curve for a given set of points based on dynamic programming, Implementation in C++

## Description
- 使用之程式語言：**< C++ >**
- 使用之編譯器：**< g++ (gcc version 5.4.0) >**
- 各檔案說明：
	- main.cpp : C++ source code 主程式
	- pa1  : main.cpp compile 後的 Unix executable binary code執行檔
	- Readme.md : 本說明文字檔

- 編譯方式說明：        	
   	* 主程式：main.cpp
		- 請在主程式的目錄下，鍵入以下指令，即可完成編譯：
		- g++ -O3 main.cpp -std=c++11
		- 在主程式的目錄下會產生一個名為 a.out 的執行檔
		- 如果要命名編譯後的執行檔，請先鍵入以下指令：
		- g++ -O3 main.cpp -std=c++11 -o [my_name]
		- 如：g++ -O3 main.cpp -std=c++11 -o pa1

- 執行、使用方式說明：
   	* 主程式：
    	- 編譯完成後，在檔案目錄下會產生一個 a.out 的執行檔
   	* 執行檔的命令格式為：
	   	- ./a.out <input file name> <output file name>
	   	- 例如：./a.out test*.in test*.out
	   	- 若是命名過後的執行檔(如pa1)，則執行檔的命令格式為：
		- ./my_name <input file name> <output file name>
		- 例如：./pa1 test*.in test*.out
     
- 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   	* 主程式：
		- 主程式執行時會在 console 輸出讀到的檔案資訊，以及目前執行的步驟。
		- 例如：執行 ./pa1 test_7x7.in test_7x7.out，console會輸出
		
		- Number of control points: 49
		- Number of sampled points: 1000000
		- Number of rows in input table: 49
		- Computing bezier curve...
		- Result successfully saved to test_7x7.out

       

