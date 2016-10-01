#ifndef ___CSTORE_DBLOCK
#define ___CSTORE_DBLOCK
#include "DBBase.h"

CSTORE_NS_BEGIN

class DBLock : public DBObject {
public:
  // ������
  CONSTRUCTOR_SIMPLE_DBOBJECT(DBLock, "DBLock");

  // �����ַ���������
  virtual std::string ToString() {
    std::string sb = this->GetTypename() + " [Type:";
    switch (this->Type)
    {
    case TableLockType::tblock_share:
      sb += "Share";
      break;
    case TableLockType::tblock_mutex:
      sb += "Mutex";
      break;
    case TableLockType::tblock_none:
      sb += "None";
      break;
    }
    sb += ", Binding:" + (this->LockBinding == NULL ? "NULL" : this->LockBinding->ToString());
    return sb;
  }

  // ����״̬
  TableLockType Type;

  // ��ʩ�Ӷ����ָ��
  DBObject* LockBinding = NULL;
}; /* DBLock */

CSTORE_NS_END

#endif /* ___CSTORE_DBLOCK */