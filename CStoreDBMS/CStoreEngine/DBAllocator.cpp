#include "DBAllocator.h"
CSTORE_NS_BEGIN

// ˽�еĹ��캯��
DBAllocator::DBAllocator()
  :DBObject("DBAllocator", this) { }

// ��������
DBAllocator::~DBAllocator() {
  this->FreeAll();
}

// ����������������Ψһʵ��
DBAllocator* DBAllocator::GetInstance() {
  return DBAllocator::Instance == NULL ?
    DBAllocator::Instance = new DBAllocator() : DBAllocator::Instance;
}

// �������һ��������
void* DBAllocator::Alloc(std::string _name, size_t _size) {
  // ��������
  if (this->allocPool.find(_name) != this->allocPool.end()) {
    TRACE("Expect to alloc buffer" + _name + ", but already exist.");
    return NULL;
  }
  void* memblock = std::malloc(_size);
  std::memset(memblock, 0, sizeof(memblock));
  this->allocPool.insert(std::pair<std::string, void*>(_name, memblock));
  return memblock;
}

// ����һ��������
void* DBAllocator::Get(std::string _name) {
  if (this->allocPool.find(_name) != this->allocPool.end()) {
    return this->allocPool[_name];
  }
  TRACE("Expect to get buffer" + _name + ", but not exist.");
  return NULL;
}

// �ͷ�һ��������
void DBAllocator::Free(std::string _name) {
  if (this->allocPool.find(_name) != this->allocPool.end()) {
    if (this->allocPool[_name] != NULL) {
      std::free(this->allocPool[_name]);
      this->allocPool[_name] = NULL;
    }
    this->allocPool.erase(_name);
  } else {
    TRACE("Expect to release buffer" + _name + ", but not exist.");
  }
}

// �ͷ����з���Ļ�����
void DBAllocator::FreeAll() {
  for (std::map<std::string, void*>::iterator iter = this->allocPool.begin();
    iter != this->allocPool.end(); iter++) {
    if ((*iter).second != NULL) {
      std::free((*iter).second);
      (*iter).second = NULL;
    }
  }
  this->allocPool.clear();
}

// �ͷ�DBMS��ģ���ڴ�׼���˳�����
void DBAllocator::Collapse() {
  this->FreeAll();
}

// Ψһʵ��
DBAllocator* DBAllocator::Instance = NULL;

CSTORE_NS_END