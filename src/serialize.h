#ifndef _SERIALIZE_HEADER_H_
#define _SERIALIZE_HEADER_H_
#include <sstream>   //std::stringstream
#include <vector>    //std::vector
#include <stdint.h>  //uint8_t uint16_t uint32_t uint64_t
#include <string.h>  //memcpy

////////////////////////////////////////////////
//First,Tools function for bytes order in socket
////////////////////////////////////////////////
uint16_t swap_2(uint16_t v)
{
	uint8_t arr[2];
	memcpy(arr,&v,2);
	uint8_t byte;

	byte = arr[0];
	arr[0] = arr[1];
	arr[1] = byte;
	return *(uint16_t*)arr;	
}

uint32_t swap_4(uint32_t v)
{
	uint8_t arr[4];
	memcpy(arr,&v,4);
	uint8_t byte;

	byte = arr[0];
	arr[0] = arr[3];
	arr[3] = byte;

	byte = arr[1];
	arr[1] = arr[2];
	arr[2] = byte;

	return *(uint32_t*)arr;
}

uint64_t swap_8(uint64_t v)
{
	uint32_t low = (uint32_t) (v & 0x00000000FFFFFFFFLL);
	uint32_t high = (uint32_t) ((v & 0xFFFFFFFF00000000LL) >> 32);

	low = swap_4(low);
	high = swap_4(high);

	return  (uint64_t) high + (((uint64_t) low) << 32);
}

bool is_little_endian()
{
	int temp=1;
	uint8_t byte_order=((uint8_t *)&temp)[0];
	return (byte_order==1);
}

////////////////////////////////////////////////////
// Second,define normal template function
////////////////////////////////////////////////////

template<typename SerializableType>
std::string serialize(SerializableType& a)
{
	return a.serialize();
}

template<typename SerializableType>
int deserialize(std::string &str,SerializableType& a)
{
	return a.deserialize(str);
}

/////////////////////////////////////////////////
//Third,define special template function
//Serialize for C/C++ basic type
//examples: short,int,float,long long,double
/////////////////////////////////////////////////
#define DEF_BASIC_TYPE_SERIALIZE(Type) \
 template<> \
std::string serialize(Type& b) \
{ \
	std::string ret; \
	Type a=b; \
	if(is_little_endian()) \
	{ \
		size_t size=sizeof(Type);\
		switch(size) \
		{ \
			case 2:a=swap_2(a);break; \
			case 4:a=swap_4(a);break; \
			case 8:a=swap_8(a);break; \
			default:break; \
		}; \
	}\
	ret.append((const char*)&a,sizeof(Type)); \
	return ret; \
}

#define DEF_BASIC_TYPE_DESERIALIZE(Type)  \
 template<> \
int deserialize(std::string& str,Type& b)\
{ \
	Type a; \
	memcpy(&a,str.data(),sizeof(Type)); \
        if(is_little_endian()) \
        { \
                size_t size=sizeof(Type);\
                switch(size) \
                { \
                        case 2:a=swap_2(a);break; \
                        case 4:a=swap_4(a);break; \
                        case 8:a=swap_8(a);break; \
                        default:break; \
                }; \
        } \
	b=a; \
	return sizeof(Type); \
}

#define DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(Type) \
	DEF_BASIC_TYPE_SERIALIZE(Type) \
	DEF_BASIC_TYPE_DESERIALIZE(Type)

DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(char)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned char)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(short int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned short int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(float)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(long long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned long long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(double)

//////////////////////////////////////
//Fourth,Serialize for type string
/////////////////////////////////////

// for c++ type std::string
template<>
std::string serialize(std::string& s)
{
	int len=s.size()+1;
	std::string ret;
	ret.append(::serialize(len));
	ret.append(s.data(),len);
	return ret;
}

template<>
int deserialize(std::string &str,std::string& s)
{
	int len;
	::deserialize(str,len);
	s=str.substr(sizeof(len),len);
	return sizeof(int)+len;
}

////////////////////////////////////////////
//Fifth, define input and output stream
//for serialize data struct
////////////////////////////////////////////

class OutStream
{
public:
	OutStream():os(std::ios::binary){}

	template<typename SerializableType>
	OutStream& operator << (SerializableType& a)
	{
		std::string x=::serialize(a);
		os.write(x.data(),x.size());
		return *this;
	}

	template<typename BasicType>
	OutStream& operator << (std::vector<BasicType>& a)
	{
		int len=a.size();
		std::string x=::serialize(len);
		os.write(x.data(),x.size());

		for(int i=0;i<len;++i)
		{
			std::string item=::serialize(a[i]);
			os.write(item.data(),item.size());
		}
		
		return *this;
	}

	std::string str()
	{
		return os.str();
	}

public:
	std::ostringstream os;
};

class InStream
{
public:
	InStream(std::string &s):str(s),total(s.size()) {}
	
	template<typename SerializableType>
	InStream& operator >> (SerializableType& a)
	{
		int ret=::deserialize(str,a);
		str=str.substr(ret);
		return *this;
	}

	template<typename BasicType>
	InStream& operator >> (std::vector<BasicType>& a)
	{
		int len=0;
		int ret=::deserialize(str,len);
		str=str.substr(ret);
	
		for(int i=0;i<len;++i)
		{
			BasicType item;
			int size=::deserialize(str,item);
			str=str.substr(size);
			a.push_back(item);
		}

		return *this;
	}

	int size()
	{
		return total-str.size();	
	}
	
protected:
	std::string str;
	int total;
};

////////////////////////////////////////////
//Sixth,Serialize for custom class object
//If your class object want to be serialized,
//Plese derive for this base class
///////////////////////////////////////////
class Serializable
{
public:
    virtual std::string serialize()=0;
    virtual int deserialize(std::string&)=0;
};

///////////////////////////////////////////
//!!!!!!!!!!!!!
//!!   NOTE  !!
//!!!!!!!!!!!!!
//Now,we can't serialize pointer type,likes
//char* var,char var[],int *var,int var[]
//etc,if you need to serialize array data,
//we suggest you use std::vector instead.
///////////////////////////////////////////

#endif
