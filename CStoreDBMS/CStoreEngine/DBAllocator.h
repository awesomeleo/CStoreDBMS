#ifndef ___CSTORE_DBALLOCATOR
#define ___CSTORE_DBALLOCATOR
#include "DBBase.h"
CSTORE_NS_BEGIN

class DBAllocator : public DBObject {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� �ڴ��������Ψһʵ��
  static DBAllocator* GetInstance();

  //�������ã� ��������
  //�����б� N/A
  //�� �� ֵ�� N/A
  ~DBAllocator();

  //�������ã� �������һ��������
  //�����б�
  //     _name ����������
  //     _size �������ߴ�
  //�� �� ֵ�� ������ָ��
  void* Alloc(std::string, size_t);

  //�������ã� ����һ��������
  //�����б�
  //     _name ����������
  //�� �� ֵ�� ������ָ��
  void* Get(std::string);

  //�������ã� �ͷ�һ��������
  //�����б�
  //     _name ����������
  //�� �� ֵ�� N/A
  void Free(std::string);

  //�������ã� �ͷ����з���Ļ�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void FreeAll();

  //�������ã� �ͷ�DBMS��ģ���ڴ�׼���˳�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  static void Collapse();

private:
  //�������ã� ˽�еĹ��캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  DBAllocator();

  // �ѷ������ֵ�
  std::map<std::string, void*> allocPool;
  // Ψһʵ��
  static DBAllocator* Instance;
  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBAllocator);
}; /* DBAllocator */

CSTORE_NS_END
#endif /* ___CSTORE_DBALLOCATOR */