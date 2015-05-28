# c-emojicheck

在移动应用开发的过程中发现 消息中有大量的emoji 编码字符
emoji编码的字符 现在的使用C语言 进行 识别 和 查找 会存在问题
jni 对 emoji 的是识别 也会有问题， 会出现 crash 
为了解决这个问题，专门开发了这个项目，
其中emoji列表 可能存在遗漏，请大家指正


使用说明：
  int check_emoji( const char * src, int slen, int * pos );
  
  src 字符串， slen 字符串真的总字节数
  pos 要检查的 字节起始位置， pos 值会被修改成 下一个字(word)的起始位置
  
  返回 0/-1
  
  具体使用例子 请参考 demo.c
