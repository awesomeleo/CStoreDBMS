#include "DBObject.h"

// ��ʵ��ת��Ϊ���ַ�����ʾ
std::string DBObject::ToString() {
  return this->typeName;
}

// �Ա����������Ƿ���ȣ�������಻��������������൱��ReferenceEquals
bool DBObject::Equals(const DBObject& RefObj) {
  return this->ReferenceEquals(RefObj);
}

// �Ա����������Ƿ�ָ��ͬһ�ڴ��ַ
bool DBObject::ReferenceEquals(const DBObject& RefObj) {
  return this->ReferenceEquals(&RefObj);
}

// �Ա����������Ƿ�ָ��ͬһ�ڴ��ַ
bool DBObject::ReferenceEquals(const DBObject* RefObj) {
  return this->allocPointer == RefObj->allocPointer;
}

// �Ա����������Ƿ���ͬ������
bool DBObject::EqualsType(const DBObject& RefObj) {
  return typeid(this) == typeid(RefObj);
}

// �����������������
std::string DBObject::GetTypename() {
  return this->typeName;
}

// �����������
void* DBObject::GetObject() {
  return this->allocPointer;
}

// ������
DBObject::DBObject(std::string TyName, void* ObjPtr)
  :typeName(TyName), allocPointer(ObjPtr) { };

// ������
DBObject::~DBObject() {
  this->allocPointer = NULL;
}