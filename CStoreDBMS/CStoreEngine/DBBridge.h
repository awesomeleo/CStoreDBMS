#ifndef ___CSTORE_DBBRIDGE
#define ___CSTORE_DBBRIDGE
#include "DBCBase.h"
#include "LexicalAnalyzer.h"
#include "ISyntaxParser.h"
#include "CSDatabase.h"
#include "Pile.h"

CSTORE_NS_BEGIN

class DBBridge : public DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  DBBridge();

  //�������ã� ����������
  //�����б�
  //     trans ����
  //   isDebug �Ƿ�Ϊ����ģʽ
  //�� �� ֵ�� N/A
  void StartTransaction(DBTransaction&, bool = false);

private:
  //�������ã� ���ļ�����Դ����
  //�����б� 
  //      path �ļ�·��
  //�� �� ֵ�� istr ���������ļ�������ַ���
  istr readCode(istr);

  // �ʷ�������
  LexicalAnalyzer iLexiana;
  // �﷨������
  LL1SyntaxParser iParser;
  // ���������
  IPile Pile;
  // ��������
  CSDatabase iDB;
  // Դ���뵥����
  TokenStream* sourceCodeTokenStream;
  // ��ǰ����
  TokenList currentSentence;
  // ��ǰ���ӵ�����
  TokenStream currentSentenceTokenStream;
  // ��ǰ�����﷨��
  SyntaxTreeNode* currentTree;
  // ��ǰ����
  DBCProxy currentProxy;
  // Դ����
  istr sourceCode;
  // Դ����·��
  istr sourcePath;
  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBBridge);
}; /* DBBridge */

CSTORE_NS_END

#endif /* ___CSTORE_DBBRIDGE */