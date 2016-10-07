#ifndef ___CSTORE_DBLOCK
#define ___CSTORE_DBLOCK
#include "DBBase.h"
#include "DBTransaction.hpp"

CSTORE_NS_BEGIN

class DBLock : public DBObject {
public:
  // ������
  CONSTRUCTOR_SIMPLE_DBOBJECT(DBLock, "DBLock");

  //�������ã� ��д�ַ���������
  //�����б� N/A
  //�� �� ֵ�� ����˵��
  virtual std::string ToString() {
    CSCommonUtil::StringBuilder sb(this->GetTypename() + " [Type:");
    switch (this->Type)
    {
    case TableLockType::tblock_share:
      sb.Append("Share");
      break;
    case TableLockType::tblock_mutex:
      sb.Append("Mutex");
      break;
    case TableLockType::tblock_none:
      sb.Append("None");
      break;
    }
    sb.Append(", Binding:" + (this->LockBinding == NULL ? "NULL" : this->LockBinding->ToString()) + "]");
    return sb.ToString();
  }

  //�������ã� ��������
  //�����б�
  //    locker ������������
  //�� �� ֵ�� N/A
  inline void LockWrite(DBTransaction* locker) {
    this->synLockObjWrite.lock();
    this->synLockObjRead.lock();
    this->LockTransaction = locker;
    this->Type = TableLockType::tblock_mutex;
  }

  //�������ã� ��������
  //�����б�
  //    locker ������������
  //�� �� ֵ�� N/A
  inline void LockRead(DBTransaction* locker) {
    this->lockMutex.lock();
    if (this->Type == TableLockType::tblock_mutex) {
      this->synLockObjRead.lock();
    }
    else {
      this->synLockObjRead.try_lock();
    }
    this->readHandleNum++;
    this->LockTransaction = locker;
    this->Type = TableLockType::tblock_share;
    this->lockMutex.unlock();
  }

  //�������ã� �������
  //�����б� N/A
  //�� �� ֵ�� N/A
  inline void UnlockWrite() {
    this->synLockObjWrite.unlock();
    this->synLockObjRead.unlock();
    this->Type = TableLockType::tblock_none;
  }

  //�������ã� �������
  //�����б� N/A
  //�� �� ֵ�� N/A
  inline void UnlockRead() {
    this->lockMutex.lock();
    this->readHandleNum--;
    if (readHandleNum == 0) {
      this->synLockObjRead.unlock();
      this->Type = TableLockType::tblock_none;
    }
    this->lockMutex.unlock();
  }

  //�������ã� ��ȡ����״̬
  //�����б� N/A
  //�� �� ֵ�� ��״̬ö��
  inline TableLockType GetState() {
    return this->Type;
  }

  //�������ã� ��λ��
  //�����б� N/A
  //�� �� ֵ�� N/A
  inline void Reset() {
    this->LockBinding = NULL;
    this->LockTransaction = NULL;
  }

  // ��ʩ�Ӷ����ָ��
  DBObject* LockBinding = NULL;
  // �������������
  DBTransaction* LockTransaction = NULL;

private:
  // ����״̬
  TableLockType Type = TableLockType::tblock_none;
  // д��
  std::mutex synLockObjWrite;
  // ����
  std::mutex synLockObjRead;
  // ����
  std::mutex lockMutex;
  // ���������
  int readHandleNum = 0;

  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBLock);
}; /* DBLock */

CSTORE_NS_END

#endif /* ___CSTORE_DBLOCK */