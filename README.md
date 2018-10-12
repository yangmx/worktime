# worktime

## 项目介绍
* 工作时长统计工具。

## 编译命令
* 命令行：`gcc -O3 -o out\wt.exe src\*.c `

## 操作方法
* 增加任务：`t <p[number]> [title]`或`task <p[number]> [title]`
* 完成任务：`c [number]`或`complete [number]`
    * `number`可带或不带`#`
* 删除任务：`d [number]`或`delete [number]`
    * `number`可带或不带`#`
* 增加工时：`h [number] [hour]<h> <title>`或`hour [number] [hour]<h> <title>`
    * `number`可带或不带`#`
    * `hour`可为整数，或带`.5`小数。小数若不为`5`则不使用小数。
* 显示任务：`l <options>`或`list <options>`
    * 以下参数可拼接使用，不限顺序，如：`ahtd10`
    * 显示所有任务：`a`
    * 隐藏工时信息：`h`
    * 显示任务日期信息：`t`
    * 指定日期：`d<days>`，如查询10号的，`d10`
* 恢复历史：`r [number]`或`restore [number]`
    * `number`可带或不带`#`，会将`.wt_histories`目录中序号最大的文件还原。
* 手动备份：`ac [suffix]`或`restore [suffix]`
    * `suffix`一般是日期，比如`1810`代表2018年10月,会在当前目录下`.wt_backups`中创建`worktime.wt.1810`的备份文件
* 查看备份：`ck [suffix] <options>`或`check [suffix]`
    * `suffix`一般是日期，比如`1810`代表2018年10月
