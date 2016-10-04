#ifndef ___CSTORE_DBMARCOS
#define ___CSTORE_DBMARCOS
#include <stdio.h>
#include <ctime>
#include <mutex>

// ��ֹ���������쿽�����캯���͸�ֵ����
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;                \
  TypeName& operator=(const TypeName&) = delete

#define CONSTRUCTOR_SIMPLE_DBOBJECT(Type, TypeName) \
  Type() : DBObject(TypeName, this) { }

// Ϊ����ṩͳһ�ӿ�
#define PILEPRINT(strpar) { \
    __CSTORE_COMMON_MUTEX__.lock(); \
    std::cout << strpar; \
    __CSTORE_COMMON_MUTEX__.unlock(); \
  }

// Ϊ��������ṩͳһ�ӿ�
#define PILEPRINTLN(strpar) { \
    __CSTORE_COMMON_MUTEX__.lock(); \
    std::cout << strpar << std::endl; \
    __CSTORE_COMMON_MUTEX__.unlock(); \
  }

// Ϊ��ϸ������Ϣ�ṩͳһ�ӿ�
#define TRACE(strpar) { \
    __CSTORE_COMMON_MUTEX__.lock(); \
    std::string __CSPUREFILE__(__FILE__); \
    int __CSPUREFILEPOS__ = __CSPUREFILE__.rfind('\\'); \
    time_t __CSTRACETIMESTAMP__ = time(NULL); \
    tm* __CSTRACETIMESTAMPPTR__ = localtime(&__CSTRACETIMESTAMP__); \
    printf("[%02d:%02d:%02d %s:%d %s] Trace Info", \
    __CSTRACETIMESTAMPPTR__->tm_hour, __CSTRACETIMESTAMPPTR__->tm_min, __CSTRACETIMESTAMPPTR__->tm_sec, \
    __CSPUREFILE__.substr(__CSPUREFILEPOS__ + 1, __CSPUREFILE__.length() - __CSPUREFILEPOS__ - 1).c_str(), \
    __LINE__, __FUNCTION__); \
    std::cout << std::endl << strpar << std::endl; \
    __CSTORE_COMMON_MUTEX__.unlock(); \
  }

// CStore�����ռ俪ʼ
#define CSTORE_NS_BEGIN namespace CStore {

// CStore�����ռ����
#define CSTORE_NS_END }

// ����ѭ���ӿ�
#define FOREVER while (true)

// ����������ֵ
#define FZERO 1e-15

// Ϊ�����ṩͳһ����
#define NEWLINE "\n"

// Ϊ��ѯ����ṩͳһ�Ľ�������
#define SENTENCETERMINATOR ';'

// ������Parser���ķ�����ʼ�к�
#define COLSTARTNUM 1

// ������Parser���ķ�����ʼ�к�
#define ROWSTARTNUM 1

// ������Parser���ķ���������
#define LL1PARSERMAPROW 69

// ������Parser���ķ���������
#define LL1PARSERMAPCOL 35

// �����߳�����
#define CONNECTORLIMIT 4

// ����ǰ׺
#define PREFIXCOLFILE "cstore_"

// Ϊ������ʾ�ӿ��ṩ������
static std::mutex __CSTORE_COMMON_MUTEX__;

#endif /* ___CSTORE_DBMARCOS */