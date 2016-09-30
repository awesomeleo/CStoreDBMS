#include "DBCCandidateFunction.h"

// CandidateFunction�Ĺ�����
CandidateFunction::CandidateFunction() : DBObject("CandidateFunction", this) {
  candidateProcesser = NULL;
}

// CandidateFunction�Ŀ���������
CandidateFunction::CandidateFunction(const CandidateFunction& _other)
  : DBObject("CandidateFunction", this) {
  this->candidateProcesser = _other.candidateProcesser;
  this->candidateType = _other.candidateType;
}

// CandidateFunction������ʽ�Ĺ�����
CandidateFunction::CandidateFunction(iHandle _proc, CFunctionType _pt)
  : DBObject("CandidateFunction", this) {
  this->SetProc(_proc, _pt);
}

// CandidateFunction���ò���ʽ
void CandidateFunction::SetProc(iHandle _proc, CFunctionType _pt) {
  this->candidateProcesser = _proc;
  this->candidateType = _pt;
}

// CandidateFunction��ò���ʽ����
iHandle CandidateFunction::GetProc() {
  return this->candidateProcesser;
}

// CandidateFunction��ò���ʽ����
CFunctionType CandidateFunction::GetType() {
  return this->candidateType;
}

// CandidateFunction���ò���ʽ
SyntaxTreeNode* CandidateFunction::Call(SyntaxTreeNode* _root, void* _parser, SyntaxType _syntax, istr _detail) {
  //return NULL;
  return this->candidateProcesser(_root, _parser, this->candidateType, _syntax, _detail);
}