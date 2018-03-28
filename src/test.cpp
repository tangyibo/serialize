#include "serialize.h"
#include "testlib/lut.h"
#include <string.h>
#include <iostream>

class MyTest : public Serializable
{
public:
    std::string m_name;
    int m_age;
    float m_salary;

public:

    MyTest( )
    {
        m_name.clear( );
        m_age=0;
        m_salary=0.;
    }

    MyTest( const char *name, const int age, const float salary )
    {
        m_name.assign( name );
        m_age=age;
        m_salary=salary;
    }

    MyTest( const MyTest& other )
    {
        m_name=other.m_name;
        m_age=other.m_age;
        m_salary=other.m_salary;
    }

    ~MyTest( )
    {
        m_name.clear( );
    }

    MyTest& operator=( const MyTest& other )
    {
        if ( this != &other )
        {
            m_name=other.m_name;
            m_age=other.m_age;
            m_salary=other.m_salary;
        }

        return *this;
    }

    virtual std::string serialize( )
    {
        OutStream os;
        os << m_name << m_age << m_salary;
        return os.str( );
    }

    virtual int deserialize( std::string &str )
    {
        InStream is( str );
        is >> m_name >> m_age>>m_salary;
        return is.size( );
    }

    void display( )
    {
        std::cout << m_name << "," << m_age << "," << m_salary << std::endl;
    }
} ;

bool operator ==( const MyTest &lhs, const MyTest &rhs )
{
    return lhs.m_name.compare(rhs.m_name) &&
            lhs.m_age == rhs.m_age &&
            lhs.m_salary == rhs.m_salary;
}

////////////////////////////////////////////////////////////////////

TEST( Serialize, BasicType )
{
    short a=1;
    int b=2;
    long c=3;
    float d=4;
    long long e=5;

    OutStream os;
    os << a << b << c << d << e ;

    std::string serializestr=os.str( );

    short a1;
    int b1;
    long c1;
    float d1;
    long long e1;

    InStream is( serializestr );
    is >> a1 >> b1 >> c1 >> d1 >> e1;

    ASSERT_EQ( a, a1 );
    ASSERT_EQ( b, b1 );
    ASSERT_EQ( c, c1 );
    ASSERT_EQ( d, d1 );
    ASSERT_EQ( e, e1 );
}

TEST( Serialize, String )
{
    std::string f="hello";
    
     OutStream os;
    os << f ;

    std::string serializestr=os.str( );
    
    std::string f1;

    InStream is( serializestr );
    is >>f1;

    std::ostringstream oss;
    oss << "hello";
    std::string f2=oss.str( );

    ASSERT_EQ( f, f1 );
    ASSERT_TRUE( f != f1 );
    ASSERT_TRUE( f == f2 );
}

TEST( Serialize, ClassObject )
{
    MyTest t( "zhang", 23, 3200.2 );

    OutStream os;
    os << t ;

    std::string serializestr=os.str( );

    MyTest t1;

    InStream is( serializestr );
    is >> t1 ;

    //t.display( );
    //t1.display( );
    ASSERT_TRUE( t == t1 );
}


TEST( Serialize, ContainerVector )
{
    std::vector<MyTest> n;
    n.push_back( MyTest( "aaa", 111, 222 ) );
    n.push_back( MyTest( "bbb", 222, 333 ) );
    n.push_back( MyTest( "ccc", 333, 444 ) );

    OutStream os;
    os << n;
    std::string serializestr=os.str( );

    std::vector<MyTest> n1;
    
    InStream is( serializestr );
    is >> n1;
    
    ASSERT_EQ(n.size(),n1.size());
    
     for ( size_t i=0;i<n.size();++i)
     {
         ASSERT_TRUE(n[i]==n1[i]);
     }
}

TEST( Serialize, ContainerList )
{
    std::list<std::string> strarr;
    strarr.push_back( "hello" );
    strarr.push_back( "world" );
    strarr.push_back( "hello" );

    OutStream os;
    os << strarr;

    std::string codestr=os.str( );

    std::list<std::string> newstrarr;
    InStream is( codestr );
    is>>newstrarr;
    
    ASSERT_EQ(strarr.size(),newstrarr.size());
     std::list<std::string>::iterator it,itnew;
     for ( it=strarr.begin( ),itnew=newstrarr.begin(); it != strarr.end( ) &&  itnew != newstrarr.end( ); ++it,++itnew )
    {
        ASSERT_EQ((*it),(*itnew));
    }
}

TEST( Serialize, ContainerSet )
{
    std::set<std::string> strarr;
    strarr.insert( "hello" );
    strarr.insert( "world" );
    strarr.insert( "hello" );

    OutStream os;
    os << strarr;

    std::string codestr=os.str( );

    std::set<std::string> newstrarr;
    InStream is( codestr );
    is>>newstrarr;
    
    ASSERT_EQ(strarr.size(),newstrarr.size());
     std::set<std::string>::iterator it,itnew;
     for ( it=strarr.begin( ),itnew=newstrarr.begin(); it != strarr.end( ) &&  itnew != newstrarr.end( ); ++it,++itnew )
    {
        ASSERT_EQ((*it),(*itnew));
    }
}

int main( int argc, char *argv[] )
{
    return ::lut::RunAllTests();  
}
