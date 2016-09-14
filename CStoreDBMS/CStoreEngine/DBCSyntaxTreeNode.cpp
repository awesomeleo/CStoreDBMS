#include "DBCSyntaxTreeNode.h"

// SyntaxTreeNode������
SyntaxTreeNode::SyntaxTreeNode() {
  children.clear();
  parent = NULL;
  candidateFunction = NULL;
}

// SyntaxTreeNode����������
SyntaxTreeNode::SyntaxTreeNode(const SyntaxTreeNode& _other) {
  this->candidateFunction = _other.candidateFunction;
  this->parent = _other.parent;
  this->errorCode = _other.errorCode;
  this->errorInfo = _other.errorInfo;
  this->mappedBegin = _other.mappedBegin;
  this->mappedLength = _other.mappedLength;
  this->mappedList = _other.mappedList;
  this->children = _other.children;
  this->nodeValue = _other.nodeValue;
  this->nodeName = _other.nodeName;
  this->nodeType = _other.nodeType;
  this->aTag = _other.aTag;
}

// SyntaxTreeNode�������
istr SyntaxTreeNode::ToString() {
  istr builder = "";
  int identation = 0;
  GetTree(builder, this, identation);
  return builder;
}

// SyntaxTreeNode�ݹ������ʾ
void SyntaxTreeNode::GetTree(istr &builder, SyntaxTreeNode* myNode, int &identation)
{
  // ����վ�û��Ҫ������
  if (myNode == NULL) {
    return;
  }
  // ����
  builder += DrawTree(myNode) + myNode->nodeName;
  if (myNode->nodeSyntaxType >= SyntaxType::Unknown
    && myNode->nodeSyntaxType != SyntaxType::epsilonLeave
    && myNode->nodeSyntaxType != SyntaxType::tail_startEndLeave) {
    builder += " (" + myNode->nodeValue + ")";
  }
  builder += NEWLINE;
  // ��������ӡ���
  identation++;
  for (int i = 0; i < (int)myNode->children.size(); i++) {
    GetTree(builder, myNode->children[i], identation);
  }
  // �ع�����
  identation--;
}

// SyntaxTreeNode��ȡ����
istr SyntaxTreeNode::DrawTree(SyntaxTreeNode* myNode) {
  // ���վͲ���Ҫ������
  if (myNode == NULL) {
    return "";
  }
  // ȡ��ĸ�ڵ㣬���վͲ���Ҫ������
  SyntaxTreeNode* parent = myNode->parent;
  if (parent == NULL) {
    return "";
  }
  // �����ѯ�游ĸ�ڵ�������ĸ�ڵ����λ
  std::vector<bool> lstline;
  while (parent != NULL)
  {
    SyntaxTreeNode* pp = parent->parent;
    int indexOfParent = 0;
    if (pp != NULL) {
      for (; indexOfParent < (int)pp->children.size(); indexOfParent++) {
        if (parent == pp->children[indexOfParent]) {
          break;
        }
      }
      lstline.push_back(indexOfParent < (int)pp->children.size() - 1);
    }
    parent = pp;
  }
  // ��������
  istr builder = "";
  for (int i = lstline.size() - 1; i >= 0; i--) {
    builder += lstline[i] ? "��  " : "    ";
  }
  // ����Լ����ֵܽ����е�����
  parent = myNode->parent;
  int indexOfParent = 0;
  for (; indexOfParent < (int)parent->children.size(); indexOfParent++) {
    if (myNode == parent->children[indexOfParent]) {
      break;
    }
  }
  // ��������һ���Ͳ�Ҫ��ͷ��
  if (indexOfParent < (int)parent->children.size() - 1) {
    builder += "����";
  }
  else {
    builder += "����";
  }
  return builder;
}