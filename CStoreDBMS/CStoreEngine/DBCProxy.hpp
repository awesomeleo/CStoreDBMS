#ifndef ___CSTORE_DBCPROXY
#define ___CSTORE_DBCPROXY
#include "DBCBase.h"

class DBCProxy : public DBObject {
public:
  CONSTRUCTOR_SIMPLE_DBOBJECT(DBCProxy, "DBCProxy");
  // ���Ӻ�
  int id;
  // ��ͨ���
  bool isStar;
  // �������������ʴ���
  int encounter;
  // ����λ
  bool errorBit;
  // ������
  DashType opCode;
  // ������
  istr opTable;
  // �����Ӿ��ָ��
  SyntaxTreeNode* condPtr;
  // ����������
  StrVec Pi;
  // �и�ֵ����
  IntVec ProxyPi;
  // ��ʼֵ����
  TablePileDictionary DefaPi;
  // ����������
  StrVec CondPi;
  // ����������
  StrVec PrimaryPi;
}; /* DBCProxy */

#endif /* ___CSTORE_DBCPROXY */