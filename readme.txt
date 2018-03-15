
 C++ 序列化探索
===================================================

一、定义
--------

    在编写应用程序的时候往往需要将程序的某些数据存储在内存中，然后将其写入某个文件或是将它传输到网络中的另一台计算机上以实现通讯。这个将 
程序数据转化成能被存储并传输的格式的过程被称为“序列化”（Serialization），而它的逆过程则可被称为“反序列化” （Deserialization）。简单来
说，序列化就是将对象实例的状态转换为可保持或传输的格式的过程。与序列化相对的是反序列化，它根据流重构对象。这两个过程结合起来，可以轻 松地
存储和传输数据。例如，可以序列化一个对象，然后使用 HTTP 通过 Internet 在客户端和服务器之间传输该对象。
    对象序列化通常用于两个目的：  
     （1） 将对象存储于硬盘上，便于以后反序列化使用
     （2）在网络上传送对象的字节序列
     
二、当前已有的序列化方式
--------

 （1）JSON文本序列化方式
  例如使用jsoncpp或cJSON等库，将内存中的数据结构序列化为JSON格式的文本串。
 （2）Google Protocol Buffers（protobuf）
   Google Protocol Buffers (GPB)是Google内部使用的数据编码方式，旨在用来代替XML进行数据交换。可用于数据序列化与反序列化。
   github地址：https://github.com/google/protobuf
   使用示例：http://blog.csdn.net/k346k346/article/details/51754431
 （3）Boost的序列化库Serialization
   Boost.Serialization可以创建或重建程序中的等效结构，并保存为二进制数据、文本数据、XML或者有用户自定义的其他文件。
 （4）MFC的序列化
  Windows平台下可使用MFC中的序列化方法。MFC 对 CObject 类中的序列化提供内置支持。因此，所有从 CObject 派生的类都可利用 CObject 的序列化协议。
  (5)msgpack的序列化
   MessagePack（简称msgpack)是一个高效的二进制序列化格式。它让你像JSON一样可以在各种语言之间交换数据。但是它比JSON更快、更小。小的整数会被编码
成一个字节，短的字符串仅仅只需要比它的长度多一字节的大小。
   官方网站： http://msgpack.org/ 。
 说明：为了考虑平台的移植性、适用性和高效性，推荐大家使用Google的protobuf和Boost的序列化方案。
 
三、C++序列化的探索
--------

  本部分代码所实现的C++序列化，支持如下特性：
 （1） 对基本数据类型的序列化；
 （2）支持序列化为socket流；
 （3）支持对std::vector数组的序列化;

四、参考文献
--------

 参考：http://blog.csdn.net/Kiritow/article/details/53129096
