#ifndef ___CSTORE_DBCSYNTAXTREENODE
#define ___CSTORE_DBCSYNTAXTREENODE
#include "DBCBase.h"

class SyntaxTreeNode : public DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  SyntaxTreeNode();

  //�������ã� �������캯��
  //�����б�
  //    _other ����Դ
  //�� �� ֵ�� N/A
  explicit SyntaxTreeNode(const SyntaxTreeNode&);

  //�������ã� �����ı�������
  //�����б� N/A
  //�� �� ֵ�� istr �����ַ�����ʾ
  virtual std::string ToString();

  // �󶨴�����
  CandidateFunction* candidateFunction;
  // ��������
  SyntaxTreeList children;
  // ��ָ��
  SyntaxTreeNode* parent;
  // �����﷨�ṹ����
  SyntaxType nodeSyntaxType;
  // ����token����ֵ
  istr nodeValue;
  // ���в���ʽ����
  CFunctionType nodeType;
  // �ڵ�����
  istr nodeName;
  // ����ֵ
  double aTag;
  // ����λ
  int errorCode;
  // ������
  istr errorInfo;
  // ����Token������
  TokenStream* mappedList;
  // ����Tokenλ�ô�
  int mappedBegin;
  // ����Token����
  int mappedLength;

private:
  //�������ã� ���ĵݹ�����ı���
  //�����б� N/A
  //�� �� ֵ�� istr �����ַ�����ʾ
  void GetTree(istr&, SyntaxTreeNode*, int&);

  //�������ã� ���ı������̵���������
  //�����б� N/A
  //�� �� ֵ�� istr �����ַ�����ʾ
  istr DrawTree(SyntaxTreeNode*);
}; /* SyntaxTreeNode */

#endif /* ___CSTORE_DBCSYNTAXTREENODE */