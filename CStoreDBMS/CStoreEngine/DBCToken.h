#ifndef ___CSTORE_DBCTOKEN
#define ___CSTORE_DBCTOKEN
#include "DBCDeclaration.h"
class Token {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  Token();

  //�������ã� �������캯��
  //�����б�
  //    _other ����Դ
  //�� �� ֵ�� N/A
  explicit Token(const Token&);

  // ������
  TokenType aType;
  // ������
  int aLine;
  // ������
  int aColumn;
  // ����ֵ
  int aTag;
  // λ�ô�
  int indexOfCode;
  // ��Գ�
  int length;
  // ����λ
  int errorCode;
  // ������
  istr errorInfo;
  // ԭ��Ϣ
  istr detail;
}; /* Token */

#endif /* ___CSTORE_DBCTOKEN */