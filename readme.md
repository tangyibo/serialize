
# C++ 序列化探索

## 一、定义
> 在编写应用程序的时候往往需要将程序的某些数据存储在内存中，然后将其写入某个文件或是将它传输到网络中的另一台计算机上以实现通讯。这个将 
程序数据转化成能被存储并传输的格式的过程被称为“序列化”（Serialization），而它的逆过程则可被称为“反序列化” （Deserialization）。简单来
说，序列化就是将对象实例的状态转换为可保持或传输的格式的过程。与序列化相对的是反序列化，它根据流重构对象。这两个过程结合起来，可以轻 松地
存储和传输数据。例如，可以序列化一个对象，然后使用 HTTP 通过 Internet 在客户端和服务器之间传输该对象。
    对象序列化通常用于两个目的：  
- （1）将对象存储于硬盘上，便于以后反序列化使用
- （2）在网络上传送对象的字节序列
     
## 二、当前已有的序列化方式
- （1）JSON文本序列化方式
> 例如使用jsoncpp或cJSON等库，将内存中的数据结构序列化为JSON格式的文本串。
  
- （2）Google Protocol Buffers（protobuf）
> Google Protocol Buffers (GPB)是Google内部使用的数据编码方式，旨在用来代替XML进行数据交换。可用于数据序列化与反序列化。
> github地址：https://github.com/google/protobuf
> 使用示例：http://blog.csdn.net/k346k346/article/details/51754431
   
- （3）Boost的序列化库Serialization
> Boost.Serialization可以创建或重建程序中的等效结构，并保存为二进制数据、文本数据、XML或者有用户自定义的其他文件。
   
- （4）MFC的序列化
> Windows平台下可使用MFC中的序列化方法。MFC 对 CObject 类中的序列化提供内置支持。因此，所有从 CObject 派生的类都可利用 CObject 的序列化协议。
  
- (5)msgpack的序列化
> MessagePack（简称msgpack)是一个高效的二进制序列化格式。它让你像JSON一样可以在各种语言之间交换数据。但是它比JSON更快、更小。小的整数会被编码
成一个字节，短的字符串仅仅只需要比它的长度多一字节的大小。
> 官方网站： http://msgpack.org/ 。
   
>说明：为了考虑平台的移植性、适用性和高效性，推荐大家使用Google的protobuf和Boost的序列化方案。
 
## 三、C++序列化的探索

> 本部分代码所实现的C++序列化，支持如下特性：
- （1）对基本数据类型char,short,int,long,string的序列化；
- （2）支持序列化为socket流；
- （3）支持对std::vector、std::list、std::set、std::map的序列化;

## 四、使用示例

- 1、对于基本数据类型的序列化

```
    char x = 'a';
    short a = 1;
    int b = 2;
    long c = 3;
    float d = 4;
    long long e = 5;

    OutStream os;
    os << x << a << b << c << d << e;

    std::string serializestr = os.str();

    char x1;
    short a1;
    int b1;
    long c1;
    float d1;
    long long e1;

    InStream is(serializestr);
    is >> x1 >> a1 >> b1 >> c1 >> d1 >> e1;
```

- 2、对于自定义类的序列化

步骤一：编写自定义类：自定义的类需要继承自Serializable,并实现serialize()和deserialize()方法：

```
class MyTest : public Serializable
{
public:
    std::string m_name;
    int m_age;
    float m_salary;

public:

    MyTest()
    {
        m_name.clear();
        m_age = 0;
        m_salary = 0.;
    }

    MyTest(const char *name, const int age, const float salary)
    {
        m_name.assign(name);
        m_age = age;
        m_salary = salary;
    }

    MyTest(const MyTest& other)
    {
        m_name = other.m_name;
        m_age = other.m_age;
        m_salary = other.m_salary;
    }

    ~MyTest()
    {
        m_name.clear();
    }

    MyTest& operator=(const MyTest& other)
    {
        if (this != &other)
        {
            m_name = other.m_name;
            m_age = other.m_age;
            m_salary = other.m_salary;
        }

        return *this;
    }

    virtual std::string serialize()
    {
        OutStream os;
        os << m_name << m_age << m_salary;
        return os.str();
    }

    virtual int deserialize(std::string &str)
    {
        InStream is(str);
        is >> m_name >> m_age>>m_salary;
        return is.size();
    }

    void display()
    {
        std::cout << m_name << "," << m_age << "," << m_salary << std::endl;
    }
};

bool operator==(const MyTest &lhs, const MyTest &rhs)
{
    return lhs.m_name == rhs.m_name &&
            lhs.m_age == rhs.m_age &&
            lhs.m_salary == rhs.m_salary;
}
```

步骤2：对自定义类的对象进行序列化：

```
    std::vector<MyTest> n;
    n.push_back(MyTest("aaa", 111, 222));
    n.push_back(MyTest("bbb", 222, 333));
    n.push_back(MyTest("ccc", 333, 444));

    OutStream os;
    os << n;
    std::string serializestr = os.str();

    std::vector<MyTest> n1;

    InStream is(serializestr);
    is >> n1;
```


- 3、对于std::list的序列化:

```
    std::list<std::string> strarr;
    strarr.push_back("hello");
    strarr.push_back("world");
    strarr.push_back("hello");

    OutStream os;
    os << strarr;

    std::string codestr = os.str();

    std::list<std::string> newstrarr;
    InStream is(codestr);
    is>>newstrarr;
```

- 4、对于std::set的序列化:

```
    std::set<std::string> strarr;
    strarr.insert("hello");
    strarr.insert("world");
    strarr.insert("hello");

    OutStream os;
    os << strarr;

    std::string codestr = os.str();

    std::set<std::string> newstrarr;
    InStream is(codestr);
    is>>newstrarr;
```

- 5、对于std::map的序列化:

```
    std::map<std::string, int> themap;
    themap["first"] = 1;
    themap["second"] = 2;
    themap["third"] = 3;
    themap["fourth"] = 4;

    OutStream os;
    os << themap;

    std::string codestr = os.str();

    std::map<std::string, int> newmap;
    InStream is(codestr);
    is>>newmap;
```

## 五、FAQ

