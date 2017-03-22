#include "DBAllocator.h"
#include "TableManager.h"
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
  return DBAllocator::Instance == nullptr ?
    DBAllocator::Instance = new DBAllocator() : DBAllocator::Instance;
}

// �������һ��������
void* DBAllocator::Alloc(std::string _name, size_t _size) {
  // ��������
  if (this->allocPool.find(_name) != this->allocPool.end()) {
    TRACE("Expect to alloc buffer" + _name + ", but already exist.");
    return nullptr;
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
  return nullptr;
}

// �ͷ�һ��������
void DBAllocator::Free(std::string _name) {
  if (this->allocPool.find(_name) != this->allocPool.end()) {
    if (this->allocPool[_name] != nullptr) {
      std::free(this->allocPool[_name]);
      this->allocPool[_name] = nullptr;
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
    if ((*iter).second != nullptr) {
      std::free((*iter).second);
      (*iter).second = nullptr;
    }
  }
  this->allocPool.clear();
}

// �ͷ�DBMS��ģ���ڴ�׼���˳�����
void DBAllocator::Collapse() {
  TRACE("DBMS Environment Begin to Collapse");
  // �ͷ�ȫ��������
  if (DBAllocator::Instance != nullptr) {
    DBAllocator::Instance->FreeAll();
  }
  // ������
  CStore::TableManager::GetInstance()->SaveContext();
}

// Ψһʵ��
DBAllocator* DBAllocator::Instance = nullptr;

CSTORE_NS_END