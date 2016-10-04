#ifndef ___CSTORE_DBCPROXY
#define ___CSTORE_DBCPROXY
#include "DBCBase.h"

CSTORE_NS_BEGIN

class DBCProxy : public DBObject {
public:
  // ������
  CONSTRUCTOR_SIMPLE_DBOBJECT(DBCProxy, "DBCProxy");
  // ���Ӻ�
  int id;
  // ��ͨ���
  bool isStar;
  // �������������ʴ���
  int encounter;
  // ����λ
  bool errorBit;
  // ����ֵ
  double aTag;
  // ������
  DashType opCode;
  // ������
  istr opTable;
  // �������
  istr opTablePaired;
  // Ҫ������ļ���
  istr loadFile;
  // �����Ӿ��ָ��
  SyntaxTreeNode* condPtr;
  // ����������
  StrVec Pi;
  // ������
  StrVec PiType;
  // �и�ֵ����
  IntVec ProxyPi;
  // ��ʼֵ����
  TablePileDictionary DefaPi;
  // ����������
  StrVec CondPi;
  // ����������
  StrVec PrimaryPi;
}; /* DBCProxy */

CSTORE_NS_END

#endif /* ___CSTORE_DBCPROXY */