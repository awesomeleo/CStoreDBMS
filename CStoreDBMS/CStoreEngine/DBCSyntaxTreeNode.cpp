#include "DBCSyntaxTreeNode.h"

CSTORE_NS_BEGIN

// SyntaxTreeNode������
SyntaxTreeNode::SyntaxTreeNode()
  : DBObject("SyntaxTreeNode", this) {
  children.clear();
  parent = nullptr;
  candidateFunction = nullptr;
}

// SyntaxTreeNode����������
SyntaxTreeNode::SyntaxTreeNode(const SyntaxTreeNode& _other)
  : DBObject("SyntaxTreeNode", this) {
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
void SyntaxTreeNode::GetTree(istr &builder, SyntaxTreeNode* myNode, int &identation) const {
  // ����վ�û��Ҫ������
  if (myNode == nullptr) {
    return;
  }
  // ����
  builder += DrawTree(myNode) + myNode->nodeName;
  if (myNode->nodeSyntaxType >= SyntaxType::SyntaxUnknown
    && myNode->nodeSyntaxType != SyntaxType::epsilonLeave
    && myNode->nodeSyntaxType != SyntaxType::tail_startEndLeave) {
    builder += " (" + myNode->nodeValue + ")";
  }
  builder += NEWLINE;
  // ��������ӡ���
  identation++;
  for (int i = 0; i < static_cast<int>(myNode->children.size()); i++) {
    GetTree(builder, myNode->children[i], identation);
  }
  // �ع�����
  identation--;
}

// SyntaxTreeNode��ȡ����
istr SyntaxTreeNode::DrawTree(SyntaxTreeNode* myNode) const {
  // ���վͲ���Ҫ������
  if (myNode == nullptr) {
    return "";
  }
  // ȡ��ĸ�ڵ㣬���վͲ���Ҫ������
  SyntaxTreeNode* parent = myNode->parent;
  if (parent == nullptr) {
    return "";
  }
  // �����ѯ�游ĸ�ڵ�������ĸ�ڵ����λ
  std::vector<bool> lstline;
  while (parent != nullptr)
  {
    SyntaxTreeNode* pp = parent->parent;
    int indexOfParent = 0;
    if (pp != nullptr) {
      for (; indexOfParent < static_cast<int>(pp->children.size()); indexOfParent++) {
        if (parent == pp->children[indexOfParent]) {
          break;
        }
      }
      lstline.push_back(indexOfParent < static_cast<int>(pp->children.size()) - 1);
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
  for (; indexOfParent < static_cast<int>(parent->children.size()); indexOfParent++) {
    if (myNode == parent->children[indexOfParent]) {
      break;
    }
  }
  // ��������һ���Ͳ�Ҫ��ͷ��
  if (indexOfParent < static_cast<int>(parent->children.size()) - 1) {
    builder += "����";
  }
  else {
    builder += "����";
  }
  return builder;
}

CSTORE_NS_END
