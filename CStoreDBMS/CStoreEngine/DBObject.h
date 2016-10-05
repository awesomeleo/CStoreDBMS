#ifndef ___CSTORE_DBOBJECT
#define ___CSTORE_DBOBJECT
#include "DBMarcos.h"
#include <string>
#include <typeinfo>

CSTORE_NS_BEGIN
class DBObject {
public:
  //�������ã� ��ʵ��ת��Ϊ���ַ�����ʾ
  //�����б� N/A
  //�� �� ֵ�� �����ඨ���ʵ���ַ�����﷽ʽ
  virtual std::string ToString();

  //�������ã� �Ա����������Ƿ���ȣ�������಻��д����������൱��ReferenceEquals
  //�����б�
  //    RefObj Ҫ�ȽϵĶ�������
  //�� �� ֵ�� �Ƿ����
  virtual bool Equals(DBObject&);

  //�������ã� �Ա����������Ƿ�ָ��ͬһ�ڴ��ַ
  //�����б�
  //    RefObj Ҫ�ȽϵĶ�������
  //�� �� ֵ�� �Ƿ�Ϊͬһ����
  bool ReferenceEquals(const DBObject&);

  //�������ã� �Ա����������Ƿ�ָ��ͬһ�ڴ��ַ
  //�����б�
  //    RefObj Ҫ�ȽϵĶ���ָ��
  //�� �� ֵ�� �Ƿ�Ϊͬһ����
  bool ReferenceEquals(const DBObject*);

  //�������ã� �Ա����������Ƿ���ͬ������
  //�����б�
  //    RefObj Ҫ�ȽϵĶ�������
  //�� �� ֵ�� �Ƿ�ͬ������
  bool EqualsType(const DBObject&);

  //�������ã� ��������������������
  //�����б� N/A
  //�� �� ֵ�� �������ַ���
  std::string GetTypename();

  //�������ã� �����������
  //�����б� N/A
  //�� �� ֵ�� �����ָ��
  void* GetObject();

  //�������ã� ������
  //�����б�
  //    TyName ���͵�����
  //    ObjPtr �����ָ��
  //�� �� ֵ�� N/A
  DBObject(std::string, void*);

  //�������ã� ������
  //�����б� N/A
  //�� �� ֵ�� N/A
  ~DBObject();

protected:
  //�������ã� Ϊ��ǰʵ��ע��һ��Object��Ϣ
  //�����б�
  //    TyName ���͵�����
  //    ObjPtr �����ָ��
  //�� �� ֵ�� N/A
  void ObjectRegister(std::string, void*);

private:
  // ������������
  std::string typeName;

  // �������ڴ��ָ��
  void* allocPointer;
}; /* DBObject */
CSTORE_NS_END

#endif /* ___CSTORE_DBOBJECT */