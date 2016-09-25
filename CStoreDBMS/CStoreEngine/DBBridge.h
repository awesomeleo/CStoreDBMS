#ifndef ___CSTORE_DBBRIDGE
#define ___CSTORE_DBBRIDGE
#include "DBCBase.h"
#include "ILexicalAnalyzer.h"
#include "ISyntaxParser.h"
#include "IDatabase.h"
#include "IPile.h"

class DBBridge {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  DBBridge();

  //�������ã� �ŵĳ�ʼ��������ָ�����з�ʽ
  //�����б�
  //   gbState ����״̬ 
  //     query Ҫִ�еĲ�ѯ���
  //�� �� ֵ�� N/A
  void Init(RunType, const istr& = "");

  //�������ã� ����������
  //�����б�
  //   isDebug �Ƿ�Ϊ����ģʽ
  //�� �� ֵ�� N/A
  void StartDash(bool = false);

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
  IDatabase iDB;
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
  // ��������
  RunType iType;

  DISALLOW_COPY_AND_ASSIGN(DBBridge);
}; /* DBBridge */

#endif /* ___CSTORE_DBBRIDGE */