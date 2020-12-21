
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