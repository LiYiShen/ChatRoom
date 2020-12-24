#define _GNU_SOURCE  /* 在包含头文件之前,需要定义_GNU_SOURCE,以便各个头文件启用这些功能. */
#include <string.h>  
/* basename() GNU版本从不修改它的参数,当路径的结尾是一个斜杠时返回空字符串.
 * 没有dirname()的GNU版本.当包含<libgen.h>时,将获得basename()的POSIX版本,否则将获得GNU版本. 
 */

#include <libgen.h>
char *dirname(char *path);
char *basename(char *path);
/* basename()有两个不同的版本————此描述是posix版本
 * dirname()和basename(): 将以空结尾的路径名字符串分解为目录和文件名组件.
 * 即: dirname()返回的字符串 + “/” + basename()返回的字符串 ==> 生成完整的路径名
 */

零拷贝: 避免CPU将数据从一块存储拷贝到另外一块存储,减少不必要的拷贝.

Linux的I/O操作默认是缓冲I/O.

sendfile()只适用于将数据从文件拷贝到套接字上,限定了它的使用范围.Linux在2.6.17版本引入splice()系统调用,用于在两个文件描述符中移动数据.同sendfile()函数一样,也是零拷贝(zero-copy). splice调用利用了Linux提出的管道缓冲区机制, 所以fdin和fdout必须至少有一个是管道文件描述符.

函数原型：
#include <fcntl.h> 
ssize_t splice(int fdin, loff_t *offin, int fdout, loff_t *offout, size_t len, unsigned int flags);

参数意义：
fdin参数：待读取数据的文件描述符. 
offin参数：指示从输入数据的何处开始读取,为NULL表示从当前位置.如果fdin是一个管道描述符,则offin必须为NULL. 
fdout参数：待写入数据的文件描述符. 
offout参数：同offin,不过用于输出数据. 
len参数：指定移动数据的长度. 

flags参数：表示控制数据如何移动,可以为以下值的按位或：
SPLICE_F_MOVE：按整页内存移动数据,存在bug,自内核2.6.21后,实际上没有效果.
SPLICE_F_NONBLOCK：非阻塞splice操作,实际会受文件描述符本身阻塞状态影响.
SPLICE_F_MORE：提示内核：后续splice将调用更多数据.
SPLICE_F_GIFT：对splice没有效果.

返回值：
返回值>0：表示移动的字节数. 
返回0：表示没有数据可以移动,如果从管道中读,表示管道中没有被写入数据. 
返回-1；表示失败,并设置errno.

errno值如下：
EBADF：描述符有错.
EINVAL：目标文件不支持splice,或者目标文件以追加方式打开,或者两个文件描述符都不是管道描述符.
ENOMEM：内存不够.
ESPIPE：某个参数是管道描述符,但其偏移不是NULL.

tee(): 在两个管道文件描述符之间复制数据,同是零拷贝.但它不消耗数据,数据被操作之后,仍然可以用于后续操作. 

函数原型：
#include <fcntl.h> 
ssize_t tee(int fdin, int fdout, size_t len, unsigned int flags);

参数意义：
fdin参数：待读取数据的文件描述符. 
fdout参数：待写入数据的文件描述符. 
len参数：表示复制的数据的长度. 
flags参数：同splice( )函数.

fdin和fdout必须都是管道文件描述符. 

返回值：
返回值>0：表示复制的字节数. 
返回0：表示没有复制任何数据. 
返回-1：表示失败,并设置errno.


Ubuntu：

sudo apt-get update
sudo apt-get -f install
sudo apt-get upgrade
sudo apt-get autoclean  清理旧版本的软件缓存
sudo apt-get clean  清理所有软件缓存
sudo apt-get autoremove  删除系统不再使用的孤立软件
whatis -w "loca*": 简要说明命令的作用

info command: 详细的说明文档

man 3 printf / man -k keyword:

man页面所属的分类标识:
(1) 用户可以操作的命令或者是可执行文件
(2) 系统核心可调用的函数与工具等
(3) 一些常用的函数与数据库
(4) 设备文件的说明
(5) 设置文件或者某些文件的格式
(6) 游戏
(7) 惯例与协议等。例如Linux标准文件系统、网络协议、ASCⅡ，码等说明内容
(8) 系统管理员可用的管理条令
(9) 与内核有关的文件

which command: 程序binary文件所在路径

whereis command: 程序搜索路径

创建：mkdir
删除：rm
删除非空目录：rm -rf file目录
删除日志 rm *log (等价: $find ./ -name “*log” -exec rm {} ;)
移动：mv
复制：cp (复制目录：cp -r )

find ./ | wc -l: 当前目录下文件个数

cp -r source_dir  dest_dir: 复制目录

目录切换:
找到文件/目录位置：cd
切换到上一个工作目录： cd -
切换到home目录： cd or cd ~
显示当前路径: pwd
更改当前工作路径为path: $cd path

列出目录项:
显示当前目录下的文件 ls (-a: 隐藏文件)
按时间排序，以列表的方式显示目录项 ls -lrt
ls | cat -n: 给每项文件前面增加一个id编号

查找目录及文件 find/locate
搜寻文件或目录: find ./ -name "core*" | xargs file
查找目标文件夹中是否有obj文件: find ./ -name '*.o'
递归当前目录及子目录删除所有.o文件: find ./ -name "*.o" -exec rm {} \;
find是实时查找，如果需要更快的查询，可试试locate；locate会为文件系统建立索引数据库，如果有文件更新，需要定期执行更新命令来更新索引库。
寻找包含有string的路径: locate string
与find不同，locate并不是实时查找。需要更新数据库，以获得最新的文件索引信息： updatedb

查看文件：cat vi head tail more
显示时同时显示行号: cat -n
按页显示列表内容: ls -al | more
只看前10行: head -10 filename
显示文件倒数第五行: tail -5 filename
查看两个文件间的差别: diff file1 file2
动态显示文本最新信息: tail -f crawler.log

创建符号链接/硬链接:
ln cc ccAgain : 硬连接；删除一个，将仍能找到；
ln -s cc ccTo : 符号链接(软链接)；删除源，另一个无法使用；（后面一个ccTo 为新建的文件）

Ctl-U ： 删除光标到行首的所有字符,在某些设置下,删除全行
Ctl-W ： 删除当前光标到前边的最近一个空格之间的字符
Ctl-H ： backspace,删除光标前边的字符

uname -a  查看系统位数

dpkg -i <package.deb>  安装一个 Debian 软件包
dpkg -c <package.deb>  列出 <package.deb> 的内容
dpkg -I <package.deb>  从 <package.deb> 中提取包裹信息
dpkg -r <package>  移除一个已安装的包裹，只删掉数据和可执行文件
dpkg -P <package>  完全清除一个已安装的包裹，purge删除所有的配制文件

源码安装：
解压缩源代码包
./configure 检测文件是否缺失，创建Makefile，检查编译环境
make 编译源码
sudo make install 把库和可执行文件安装到系统路径下
sudo make distclean 删除和卸载软件

C程序编译步骤：
预处理：宏定义展开、头文件展开、条件编译等，删除注释，不会检查语法；
编译：检查语法，生成汇编文件；
汇编：生成目标文件（二进制文件）
链接：将各种依赖库链接到最终可执行程序中。

gcc编译过程： 一步编译  gcc -o hello.exe hello.c (-I 包含头文件路径  -L 包含库文件路径)
预处理：gcc -E hello.c -o hello.i
编译：  gcc -S hello.i -o hello.s
汇编：  gcc -c hello.s -o hello.o
链接：  gcc hello.o -o hello_elf

打开终端：ctrl+alt+t
清屏：ctrl+l  clear

history 查看历史命令
根目录 /
/bin  可执行文件
/media  挂载设备媒体,u盘,光驱等
/mnt  让用户挂载别的文件系统
/usr  unix system resources 大多应用安装目录
/usr/local  用户自己安装软件的目录
/sbin  超级管理员root的可执行文件目录
/proc  系统内存的映射，会保留进程运行的一些信息
/etc  系统软件的启动和配置目录
/dev  设备文件

用户名 机器名 ~家目录 $普通用户 #管理员

计算机中一切皆文件。
Linux中文件无扩展名，可通过颜色或者命令来区分。区分大小写。
文件分为：普通文件(-f)、目录文件(d)、设备文件(字符设备c、块设备b)、管道文件(s)、链接文件(l)

文件权限：r-read  w-write  x-execute

d rwx rwx rwx 三组： 目录 所属用户 所属组 其他用户

帮助：命令 --help

man功能键：
空格键  显示手册页下一屏
Enter键  一次滚动手册页的一行
b  回滚一屏
f  前滚一屏
q  退出man命令
h  列出所有功能键
/word  搜索word字符串

常用命令：
ls  -a  -l  -lh  -lrt(按照时间排序)
ll -->  ls -all

pwd  显示当前目录
cd ~ 回到主目录
cat 文件名  查看文件内容  cat a.txt b.txt > c.txt
head -n 文件名  默认查看前十行
tail -n(-f  一直跟踪文件末尾) 文件名  默认查看后十行

mkdir 目录名
mkdir one/two -p  创建目录及子目录 递归创建
rmdir  删除一个空目录
rm -r  强制删除目录下的文件 递归删除
tree 目录  显示目录结构

创建文件：touch 文件名

通配符：
*  匹配任意多个字符(0-256)
?  匹配任意一个字符
[a-z]  区间法  匹配a到z中任意一个字符
[abc]  穷举法  匹配abc中的任意一个字符 
\*  转义字符
^a  匹配以a起始的字符串
a$  匹配以a结尾的字符串
.  匹配任意一个非换行字符

>  输出重定向  将命令执行结果重定向到一个文件  会覆盖原来的内容
>> 附加重定向  在文件内容后面追加

管道： |  一个命令的输出可以通过管道作为另一个命令的输入。
more： 分屏显示文件内容   ll | more

建立链接文件：ln
ln  源文件  链接文件    （硬链接--复制，但是始终保持文件内容相同）
ln -s  源文件  链接文件 （软链接）
删除链接文件：unlink

文本搜索：grep
grep -v 'content' 文件名  显示除了content的所有行,可以结合通配符
-n 显示匹配行及行号
-i 忽略大小写
-vn 结合 -v -n

计算文件行数或字数：wc 文件名  -l (只返回行数)
查看目录占用空间：du -h
显示磁盘空间信息：df -h

ps -l: 查看自己的进程
ps aux: 查看系统所有进程

top: 实时显示进程信息
top -d 2: 两秒钟刷新一次

netstat: 查看占用端口的进程
netstat -anp | grep port: 查看特定端口的进程

查找文件：find ./ -name "*.txt"  -size 大小  -type 类型

拷贝文件：cp
拷贝目录：cp -a

移动文件：mv

获取文件类型：file 文件名
输出变量或字符串：echo

归档：tar -cvf 归档文件名.tar 文件1 文件2 
解归档：tar -xvf 归档文件名.tar 路径
压缩：gzip 归档文件名.tar
解压缩：gzip -d 归档文件名.tar.gz

gz:
一步归档压缩：tar -zcvf 文件名.tar.gz 文件1 文件2
一步解归档压缩：tar -zxvf 文件名.tar.gz -C 路径

bz2:
tar -jcvf test.tar.bz2 文件1 文件2
tar -jxvf test.tar.bz2 -C 路径

zip:
zip myzip *.*
unzip -d 路径 myzip.zip

压缩：rar a -r 压缩包名 文件1 文件2
解压：rar x 压缩包名

查看命令位置：which
which ls

配置命令别名：alias

whoami  查看用户
su root  切换用户  
su - root  切换用户并切换工作目录空间
exit  退出用户

查看登录用户：who  -m

添加、删除组账号(root权限)：
groupadd 组名
groupdel 组名

修改用户所在组：
usermod -g 组名 用户名

添加新用户：
useradd -d 目录 用户名 -g 组名 -m 

为普通用户设置或修改口令：passwd 用户名

删除用户：userdel 用户名
删除用户，同时删除用户主目录：userdel -r 用户名

查询用户登录情况：last

修改文件权限：chmod
chmod u/g/o/a +/-/= rwx 文件名    u-文件所有者 g-用户组 o-其他用户 a-三者
chmod 777 文件名   r-4 w-2 x-1 '-'-0
chmod 777 test/ -R  递归test目录下所有文件加777权限

修改文件所有者：sudo chown 用户名 文件名

修改文件所属组：sudo chgrp 组名 文件或目录名

查看当前日历：cal -y 显示整年

显示或设置时间：date

查看运行进程信息：ps aux  (ps ajx 追溯进程血缘关系)
动态显示进程：top
给进程发送信号：kill -signal pid  信号值从0到15，其中9为绝对终止
kill -l 查看信号值列表

后台程序：& jobs fg
将一个前台执行的程序调入后台执行：命令 &
如果程序已经在执行，ctrl+z 可以将程序调入后台
jobs 查看后台运行程序
fg 编号：将后台运行程序调出到前台

关机：
reboot  重启操作系统
shutdown -r now  立刻重启操作系统
shutdown -h now  立刻关机
shutdown -h 20:00  到点关机
shutdown -h +10  延迟关机
init 0 关机   
init 6 重启

init 3  切换到字符界面
init 5  切换到图形界面

Makefile(makefile): 向下检索，构建关系依赖树；向上执行命令，生成最终目标文件
目标：依赖
    命令

模式规则：%.o:%.c
自动变量：只能在规则的命令中使用
$<：规则中第一个依赖
$@：规则中的目标
$^：规则中的所有依赖

vi文本编辑器：
vim 
vi 文件名

ZZ 保存退出  :wq 保存退出  :w 文件名 保存到指定文件
:q 退出  :q! 退出，不保存

esc：进入命令模式（默认模式）
命令模式变为编辑模式：
i 在光标前插入  a 在光标后插入  I 在行首插入  A 在行尾插入  o/O 在下/上一行插入  s 删除当前字母  S 删除当前行 u 撤销
: 进入末行模式
:x 保存退出
:set nu 设置行号
:s/src/des 替换当前行第一个匹配的src为des
:%s/src/des/g 替换文件内容所有的src为des
:r 文件名  在光标当前位置载入另一个文件内容

gg  定位第一行行首  G或:$  定位最后一行行首  mgg或mG  定位到第m行行首

删除内容：x 删除光标所在字母  X 删除光标前字母  dw 删除单词(从光标开始)  d0 删除光标到行首  d$(D) 删除光标到行尾  dd 删除光标所在行  ndd 删除光标所在开始n行

复制粘贴：yy 复制一行  dd 剪切一行  nyy 复制n行  p/P 粘贴  r 单字符替换
可视模式：v 进入可视模式，移动光标选中内容

/内容  查找字符串(n寻找下一个，N寻找上一个)
光标指向查找字符串，按 # 也可以进行查找

gg=G 格式调整  >> (<<) 当前行右移(左移)一个tab  n>> (n<<)

gdb调试：b main 断点  run 运行  p 查看信息

vim教程：vimtutor

Linux系统远程连接到Linux服务器：sudo ssh -l 用户名 ip地址
上传下载：
lcd  切换本地路径
cd  切换远程服务器路径
上传  put 文件1 文件2 目录
下载  get 文件1 文件2 目录

创建网页：
确定http目录 -- vi hello.c 编辑代码(C语言嵌套HTML) -- 启动程序 ./myhttp start -- ip访问

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	
	system("pause");
	return EXIT_SUCCESS;
}

百度文库下载：wenku.baiduvvv.com （替换）

jupyter notebook:
jt -t solarizedl -T -N    (monokai、gruvboxl、grade3)
jt -r 默认

markdown:
password: 123456

copy/b 1.txt+1.mp4 new.txt

切换到blog目录
hexo init
hexo new ""
hexo clean 
hexo g 
hexo s
hexo d

git config --system --unset credential.helper

import os
os.environ['path']
os.getenv('path')

import sys
current_path=os.getcwd()
sys.path.append(current_path)

git config --system --unset credential.helper

git init
git remote add origin <git path>
git status
git add
git commit -m
git push
git pull origin master:master
git push -u origin master

在linux下，初始使用Vi的时候有两个典型的问题：
1、在编辑模式下使用方向键的时候，并不会使光标移动，而是在命令行中出现A、B、C、D四个字母；
2、当编辑出现错误，想要删除时，发现Backspace键不起作用，只能用Delete键来删除。
方法:
sudo gedit /etc/vim/vimrc.tiny 
修改倒数第二行set compatible，将其改为set nocompatible。
在set nocompatible下面添加一行：set backspace=2。

sudo vim /etc/vim/vimrc 进入vim编辑器添加以下命令
set tabstop=4（tab键）
set shiftwidth=4(系统自动缩进值)

man:

代码  代表内容
1    普通的命令
2    内核调用的函数与工具
3    常见的函数与函数库
4    设备文件的说明
5    配置文件
6    游戏
7    惯例与协议
8    管理员可用的命令
9    内核相关的文件

结构名称     代表意义
NAME        命令的名称
SYNOPSYS    参数的大致使用方法
DESCRIPTION 介绍说明
EXAMPLES    演示（附带简单说明）
OVERVIEW    概述
DEFAULTS    默认的功能
OPTIONS     具体的可用选项（带介绍）
ENVIRONMENT 环境变量
FILES       用到的文件
SEE ALSO    相关的资料
HISTORY     维护历史与联系方式

按键          用处
空格键       向下翻一页。
[Page Down] 向下翻一页。
[Page Up]   向上翻一页。
[HOME]      直接前往首页。
[END]       直接前往尾页。
/关键词      从上至下搜索某个关键词
?关键词      从下至上搜索某个关键词
n           定位到下一个搜索到的关键词。
N           定位到上一个搜索到的关键词。
q           退出帮助文档。
!commond

切换执行等级run level：init
run level 0：关机　　//关机可以执行 init 0
run level 3：纯命令行模式
run level 5：含有图形界面模式
run level 6：重启

忘记root密码：以单用户维护模式登录进行root账号密码更改
具体流程（以grub引导装载程序为例）：重启 -> 读秒时按下任意键 -> 按下e（进入grub编辑模式）-> 将光标移到kernel那行，再次按下e对该行编辑 -> 在最后输入single -> 按下[Enter]确认后，按b进入单用户维护模式（此模式在天tty1终端中直接以root身份登录，无需密码）-> 输入passwd命令修改root密码 

nc 127.0.0.1 8888 建立临时连接

netstat -apn | grep 8888  查看端口占用

三次握手、四次挥手  允许通信双方中的一方单向关闭连接 半关闭状态
2MSL:
MTU：最大传输单元
MSS：最大报文段长度
滑动窗口：win 流量控制

ulimit -a

AF = Address Family
PF = Protocol Family
AF_INET = PF_INET
                    
为了让鼠标可以在几个屏幕间自由切换：
按："Esc"键 + “：”，输入：set mouse=a                
                    
[d 查看宏定义

nslookup
server
默认DNS服务器地址                
                    
!Qnr0rTJRs#0                    
                    
UPDATE mysql.user SET authentication_string=password("root") WHERE user='root' AND Host='localhost';                          
                    
update mysql.user set authentication_string=PASSWORD('root') where User='root';               
                    
sudo vim /etc/mysql/debian.cnf            
sudo netstat -tap | grep mysql

删除MySQL:
dpkg --list|grep mysql

sudo apt-get autoremove --purge mysql-server-5.7
sudo apt-get remove mysql-server
sudo apt-get autoremove mysql-server
sudo apt-get remove mysql-common
dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P                   

{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "g++ - 生成和调试活动文件",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "C/C++: g++ build active file",
            "miDebuggerPath": "/usr/bin/gdb"
        }
    ]
}
     
               
{
	"version": "2.0.0",    
	"tasks": [
		{
			"label": "build",                
			"type": "shell",                
			"command": "g++",
			"args":["-g","$[file]"],
		}
	]
}

#ifndef:宏定义判断，防止多重定义。

函数原型：extern char *strpbrk(char *str1, char *str2);
参数说明：str1待比较的字符串，str2为指定被搜索的字符串。    
所在库名：#include <string.h>
函数功能：比较字符串str1和str2中是否有相同的字符，如果有，则返回该字符在str1中的位置的指针。
返回说明：返回指针，搜索到的字符在str1中的索引位置的指针。

函数原型：size_t strspn(const char *str, const char * accept);
函数功能：计算字符串str中连续有几个字符都属于字符串accept
返回说明：返回字符串str开头连续包含在字符串accept内的字符数目。如果str所包含的字符都属于accept，那么返回 str的长度；如果str的第一个字符不属于accept，那么返回0。

stack栈：存放局部非静态变量，用来保存现场(堆栈作用)，从高地址向低地址生长。
heap堆：存放malloc和new开的内存
bss(Block Started by Symbol)：存放全局未初始化变量，静态变量。bss不占实际的磁盘空间，运行时分配内存。编译的可执行文件中不包含这部分。
data(data segment)：存放已初始化的全局变量，已初始化的静态变量
text：code segment/text segmen，存放系统代码，只读区域

SIGCHLD: 当一个子进程改变了它的状态时（停止运行，继续运行或者退出），有两件事会发生在父进程中：
得到 SIGCHLD 信号；waitpid() 或者 wait() 调用会返回。
其中子进程发送的 SIGCHLD 信号包含了子进程的信息，比如进程 ID、进程状态、进程使用 CPU 的时间等。在子进程退出时，它的进程描述符不会立即释放，这是为了让父进程得到子进程信息，父进程通过 wait() 和 waitpid() 来获得一个已经退出的子进程的信息。

僵尸进程: 一个子进程的进程描述符在子进程退出时不会释放，只有当父进程通过 wait() 或 waitpid() 获取了子进程信息后才会释放。如果子进程退出，而父进程并没有调用 wait() 或 waitpid()，那么子进程的进程描述符仍然保存在系统中，这种进程称之为僵尸进程。僵尸进程通过 ps 命令显示出来的状态为 Z（zombie）。

系统所能使用的进程号是有限的，如果产生大量僵尸进程，将因为没有可用的进程号而导致系统不能产生新的进程。要消灭系统中大量的僵尸进程，只需要将其父进程杀死，此时僵尸进程就会变成孤儿进程，从而被 init 进程所收养，这样 init 进程就会释放所有的僵尸进程所占有的资源，从而结束僵尸进程。
