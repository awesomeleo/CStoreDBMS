#ifndef ___CSTORE_DBCTOKENSTREAM
#define ___CSTORE_DBCTOKENSTREAM
#include "DBCDeclaration.h"
#include "DBCToken.h"

CSTORE_NS_BEGIN

class TokenStream : public DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  TokenStream();

  //�������ã� �������캯��
  //�����б�
  //    _other ����Դ
  //�� �� ֵ�� N/A
  explicit TokenStream(const TokenStream&);

  //�������ã� ��λ��
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Reset();

  //�������ã� ��ӵ���
  //�����б�
  //      _org ����ӵ�tokenʵ��
  //�� �� ֵ�� N/A
  void Add(Token*);

  //�������ã� �õ����ĳ���
  //�����б� N/A
  //�� �� ֵ�� int �������ĳ���
  int Length() const;

  //�������ã� �������׼�����
  //�����б�
  //        os ��׼�����
  //        ts �����������
  //�� �� ֵ�� ios& ��׼�����
  friend ios& operator<<(ios&, const TokenStream&);

  //�������ã� ȡ����һ��Token��ָ��
  //�����б� N/A
  //�� �� ֵ�� Token* ��һTokenָ��
  Token* Next();

  //�������ã� ȡ����һ�����ӵĵ�������
  //�����б� N/A
  //�� �� ֵ�� TokenList ��һ���ӵ�������
  TokenList NextSentence();

  //�������ã� ���������ı�������
  //�����б� N/A
  //�� �� ֵ�� istr ���������ַ�����ʾ
  istr ToString();

  // ������
  TokenList _tokenContainer;

private:
  // ��ָ��
  iPtr _iPointer;
  // ��ָ��
  iPtr _sPointer;
}; /* TokenStream */

CSTORE_NS_END

#endif /* ___CSTORE_DBCTOKENSTREAM */