#include "serialize.h"
#include <stdio.h>
#include "convert.h"

class MuPosition :public Serializable
{
private:
	short m_poiid;         //2 bytes
	short m_mapid;         //2 bytes
	int m_poitime;         //4 bytes
	unsigned char m_mac[6];//6 bytes
	std::string m_school;  //4+m_school.size() bytes

public:
	MuPosition()
	{
		m_poiid=0;
		m_mapid=0;
		m_poitime=0;
		memset(m_mac,0,sizeof(m_mac));
		m_school.clear();	
	}

	MuPosition(const int &poiid,const int &mapid,const int &poitime,const unsigned char *mac,const std::string &school)
	{
		m_poiid=poiid;
		m_mapid=mapid;
		m_poitime=poitime;
		memcpy(m_mac,mac,6);
		m_school.assign(school);
	}

	MuPosition(const MuPosition& other)
	{
                m_poiid=other.m_poiid;
                m_mapid=other.m_mapid;
                m_poitime=other.m_poitime;
                memcpy(m_mac,other.m_mac,6);
                m_school.assign(other.m_school);
	}

	~MuPosition()
	{
		m_school.clear();
	}

	MuPosition& operator=(const MuPosition& other)
	{
		if(this!=&other)
		{
			m_poiid=other.m_poiid;
			m_mapid=other.m_mapid;
			m_poitime=other.m_poitime;
			memcpy(m_mac,other.m_mac,6);
			m_school.assign(other.m_school);
		}

		return *this;
	}

	virtual std::string serialize()
	{
		OutStream os;

		os<<m_poiid<<m_mapid<<m_poitime;
		for(int i=0;i<6;++i)
			os<<m_mac[i];
		os<<m_school;

		return os.str();
	}

	virtual int deserialize(std::string &str)
	{
		InStream is(str);

		is>>m_poiid>>m_mapid>>m_poitime;
		for(int i=0;i<6;++i)
			is>>m_mac[i];
		is>>m_school;

		return is.size();
	}

	std::string dumpstr()
	{
		char buffer[1024];
		snprintf(buffer,1024,"POIID=%d,MAPID=%d,POITIME=%d,MAC=%02X:%02X:%02X:%02X:%02X:%02X,SCHOOL=%s",
			m_poiid,m_mapid,m_poitime,m_mac[0],m_mac[1],m_mac[2],m_mac[3],m_mac[4],m_mac[5],
			m_school.c_str());
		return std::string(buffer);
	}
}; 

int main(int argc,char *argv[])
{
	std::vector<MuPosition> org_list;

	char macstr[18]="2C:2C:00:00:C0:EA";
	unsigned char machex[6]={0};
	Convert::mac_str2bin(macstr,machex);
	
	std::string school="qtech";

	for(int i=0;i<10;++i)
	{
		org_list.push_back(MuPosition(1+i,2+i,3+i,machex,school));
	}

	OutStream os;
	os<<org_list;
	std::string serializestr=os.str();
	printf("serialize bytes is %d\n",(int)serializestr.size());

	std::vector<MuPosition> new_list;
	InStream is(serializestr);
	is>>new_list;
	for(std::vector<MuPosition>::iterator it=new_list.begin();it!=new_list.end();++it)
		printf("%s\n",(*it).dumpstr().c_str());
	
	return 0;
}
