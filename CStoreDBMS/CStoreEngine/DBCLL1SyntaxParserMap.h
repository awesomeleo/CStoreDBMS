#ifndef ___CSTORE_DBCLL1SYNTAXPARSERMAP
#define ___CSTORE_DBCLL1SYNTAXPARSERMAP
#include "DBCDeclaration.h"
#include "DBCCandidateFunction.h"

CSTORE_NS_BEGIN

class LL1SyntaxParserMap : public DBObject {
public:
  //�������ã� ���ߴ�Ĺ�����
  //�����б�
  //       row �г���
  //       col �г���
  //�� �� ֵ�� N/A
  LL1SyntaxParserMap(int, int);

  //�������ã� ����������
  //�����б�
  //       row �г���
  //      left ���ս������
  //�� �� ֵ�� N/A
  void SetRow(int, SyntaxType);

  //�������ã� ����������
  //�����б�
  //       col �г���
  //     leave Token����
  //�� �� ֵ�� N/A
  void SetCol(int, TokenType);

  //�������ã� ���ýڵ��������
  //�����б�
  //       row ��ָ��
  //       col ��ָ��
  //      proc ��ѡʽ����
  //�� �� ֵ�� N/A
  void SetCellular(int, int, CandidateFunction*);

  //�������ã� ���ýڵ��������
  //�����б�
  //      left ���ս������
  //     leave Token����
  //      proc ��ѡʽ����
  //�� �� ֵ�� N/A
  void SetCellular(SyntaxType, TokenType, CandidateFunction*);

  //�������ã� ȡ�ýڵ�Ĵ�����
  //�����б�
  //       row ��ָ��
  //       col ��ָ��
  //�� �� ֵ�� CandidateFunction* ��ѡʽ����ָ��
  CandidateFunction* GetCFunction(int, int);

  //�������ã� ȡ�ýڵ�Ĵ�����
  //�����б�
  //      left ���ս������
  //     leave Token����
  // nilserver epsilon������
  //�� �� ֵ�� CandidateFunction* ��ѡʽ����ָ��
  CandidateFunction* GetCFunction(SyntaxType, TokenType, iHandle);

private:
  // �и���
  int iRowCount;
  // �и���
  int iColCount;
  // ����ʽ���ֵ�
  SyntaxDictionary iLeftNodes;
  // ����ʽ���ֵ�
  TokenDictionary iNextLeaves;
  // LL1Ԥ���
  ParserMap iParserMap;
};

CSTORE_NS_END

#endif /* ___CSTORE_DBCLL1SYNTAXPARSERMAP */