# worktime

## 项目介绍
* 工作时长统计工具。

## 操作方法
* 增加任务：`worktime at #1`
* 显示未完成任务+所有工时信息：`worktime ls`
* 显示所有任务：`worktime ls -a`
* 隐藏工时信息：`worktime ls -h`
* 增加工时：`worktime ad #2 3.5`
* 完成任务：`worktime c #1`
* 删除任务：`worktime d #1`

## 编译
* gcc -o out\wt.exe -O3 src\*.c 
