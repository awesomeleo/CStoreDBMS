#ifndef ___CSTORE_IBRIDGE
#define ___CSTORE_IBRIDGE
#include "DBCBase.h"
#include "ILexicalAnalyzer.h"
#include "ISyntaxParser.h"
#include "IDatabase.h"
#include "IPile.h"

class IBridge {
public:
  //�������ã� ��������
  //�����б� N/A
  //�� �� ֵ�� IBridge* �ŵ�Ψһʵ��
  static IBridge* Invoke();

  //�������ã� �ŵĳ�ʼ��������ָ�����з�ʽ
  //�����б�
  //   _myArgc ��������б���
  //   _myArgv ��������б�
  //�� �� ֵ�� N/A
  void Init(int, char**);

  //�������ã� ����������
  //�����б�
  //   isDebug �Ƿ�Ϊ����ģʽ
  //�� �� ֵ�� N/A
  void StartDash(bool = false);

private:
  //�������ã� ����ģʽ˽�й��캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  IBridge();

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
  // Ψһ����
  static IBridge* iInstance;
}; /* IBridge */

#endif /* ___CSTORE_IBRIDGE */