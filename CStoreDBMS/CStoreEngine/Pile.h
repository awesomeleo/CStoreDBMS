#ifndef ___CSTORE_PILE
#define ___CSTORE_PILE
#include "DBCBase.h"
#include "IDatabase.h"

class IPile : public DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  IPile();

  //�������ã� ��λ����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Reset();

  //�������ã� �����﷨������
  //�����б�
  //   _myNode ƥ�������ڵ�
  //�� �� ֵ�� N/A
  void SetParseTree(SyntaxTreeNode*);

  //�������ã� �������������
  //�����б� N/A
  //�� �� ֵ�� IBridgeProxy �����﷨���͵Ĵ�������
  DBCProxy Semanticer();

private:
  //�������ã� �﷨������
  //�����б� 
  //   myproxy �﷨���͵Ĵ�������
  //    mynode �ݹ�ڵ�
  //      flag ��ʶ�����������ͣ�0-��1-�У�2-���У�3-�еĳ�ֵ
  //�� �� ֵ�� N/A
  void Mise(DBCProxy&, SyntaxTreeNode*, int);

  // �﷨��
  SyntaxTreeNode* parseTree;
  // ��ǰ���
  int proxyID;
  // ��������
  istr lastPi;
}; /* IPile */

#endif /* ___CSTORE_PILE */