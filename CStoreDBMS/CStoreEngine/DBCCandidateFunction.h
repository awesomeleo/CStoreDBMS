#ifndef ___CSTORE_DBCCANDIDATEFUNCTION
#define ___CSTORE_DBCCANDIDATEFUNCTION
#include "DBCDeclaration.h"

class CandidateFunction {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  CandidateFunction();

  //�������ã� �������캯��
  //�����б�
  //    _other ����Դ
  //�� �� ֵ�� N/A
  CandidateFunction(const CandidateFunction&);

  //�������ã� ������ʽ�Ĺ��캯��
  //�����б�
  //     _proc ���⴦��ĺ���ָ��
  //       _pt ����ʽ����
  //�� �� ֵ�� N/A
  CandidateFunction(iHandle, CFunctionType);

  //�������ã� �󶨲���ʽ����������
  //�����б�
  //     _proc ���⴦��ĺ���ָ��
  //       _pt ����ʽ����
  //�� �� ֵ�� N/A
  void SetProc(iHandle, CFunctionType);

  //�������ã� ��ò���ʽ����
  //�����б� N/A
  //�� �� ֵ�� iHandle ����ʽ�Ĵ�����
  iHandle GetProc();

  //�������ã� ��ò���ʽ����
  //�����б� N/A
  //�� �� ֵ�� CFunctionType ����ʽ����
  CFunctionType GetType();

  //�������ã� ���ò���ʽ������
  //�����б�
  //     _root ƥ�������ڵ�
  //   _parser ƥ����ָ��
  //   _syntax �﷨����
  //   _detail �ڵ���Ϣ
  //�� �� ֵ�� iHandle ����ʽ�Ĵ�����
  SyntaxTreeNode* Call(SyntaxTreeNode*, void*, SyntaxType, istr);

private:
  // ����ʽ���⴦��ָ��
  iHandle candidateProcesser = NULL;
  // ����ʽ����
  CFunctionType candidateType;
};

#endif /* ___CSTORE_DBCCANDIDATEFUNCTION */