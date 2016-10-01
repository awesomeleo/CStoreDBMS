#ifndef ___CSTORE_ISYNTAXPARSER
#define ___CSTORE_ISYNTAXPARSER
#include "DBCBase.h"

class ISyntaxParser {
public:
  //�������ã� �����﷨���������������������ʵ��
  //�����б� N/A
  //�� �� ֵ�� SyntaxTreeNode* ƥ����ϵ��﷨��
  virtual SyntaxTreeNode* Parse() = 0;

  //�������ã� ��ȡToken��ָ��
  //�����б� N/A
  //�� �� ֵ�� TokenStream* token����ָ��
  virtual TokenStream* GetTokenStream();

  //�������ã� ����Token��ָ��
  //�����б� 
  //  _myProxy Token��ָ��
  //  _mySVect ��������ָ��
  //�� �� ֵ�� N/A
  virtual void SetTokenStream(TokenStream*, StrVec*);

  //�������ã� ��ȡSentence����ָ��
  //�����б� N/A
  //�� �� ֵ�� StrVec* Sentence������ָ��
  virtual StrVec* GetSectencePtr();

private:
  //�������ã� ����Sentence����ָ��
  //�����б� 
  //  _myProxy Token��ָ��
  //�� �� ֵ�� N/A
  virtual void SetSentencePtr(StrVec*);

  //�������ã� ��λƥ����
  //�����б� N/A
  //�� �� ֵ�� N/A
  virtual void Reset() = 0;

  // ��ָ��
  TokenStream* istream;
  // ��ָ��
  StrVec* isentence;
}; /* ISyntaxParser */

class LL1SyntaxParser : public ISyntaxParser, DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  LL1SyntaxParser();

  //�������ã� ��token����sentence�����Ĺ��캯��
  //�����б�
  //  _myProxy Token��ָ��
  //  _mySVect ��������ָ��
  //�� �� ֵ�� N/A
  LL1SyntaxParser(TokenStream*, StrVec*);

  //�������ã� ��λ�﷨ƥ����
  //�����б� N/A
  //�� �� ֵ�� N/A
  virtual void Reset();

  //�������ã� �����﷨������
  //�����б� N/A
  //�� �� ֵ�� SyntaxTreeNode* ƥ����ϵ��﷨��
  virtual SyntaxTreeNode* Parse();

  //�������ã� ȡ�﷨չ��ʽ�ӽڵ�����
  //�����б�
  //     _type չ��ʽ����
  //�� �� ֵ�� SyntaxVector ����ʽ�ӽڵ���������
  SyntaxVector GetVector(CFunctionType) const;

  //�������ã� ȡƥ��ջָ��
  //�����б� N/A
  //�� �� ֵ�� SyntaxStack* ƥ��ջ��ָ��
  SyntaxStack* GetStack();

  //�������ã� ȡ��һ�ڵ��ָ��
  //�����б�
  //      _res ��ĸ�ڵ�
  //   _parser ƥ����ָ��
  //�� �� ֵ�� SyntaxTreeNode* ��һ����ȥչ���Ĳ���ʽ
  SyntaxTreeNode* NextNode(SyntaxTreeNode*, LL1SyntaxParser*);

  // ��һTokenָ��
  iPtr iPTRnextToken = 0;
  // ��һSenctence��ָ��
  iPtr iPTRnextSectence = 0;

private:
  //�������ã� ��������ָ��
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Dash();

  //�������ã� ����CSTORE���Ᵽ����
  //�����б�
  //    xtoken ��һtoken
  //   curRoot ��ǰ���ڵ�
  //�� �� ֵ�� �Ƿ���CSTORE��������
  bool CSTOREQL(Token*, SyntaxTreeNode*&);

  //�������ã� ��ʼ��Ԥ�������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void InitMap();

  //�������ã� ��ʼ����������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void InitLinkerVector();

  //�������ã� ��ʼ������������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void InitMapProperties();

  //�������ã� ��ʼ��LL1�ķ�
  //�����б� N/A
  //�� �� ֵ�� N/A
  void InitCellular();

  //�������ã� �����жϴ�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void iException();

  // ��ѡʽ���͵�����
  SyntaxDict iDict;
  // ƥ��ջ
  SyntaxStack iParseStack;
  // Ԥ�������
  LL1SyntaxParserMap* iMap = NULL;
}; /* LL1SyntaxParser */


#endif /* ___CSTORE_ISYNTAXPARSER */