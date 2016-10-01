#ifndef ___CSTORE_ILEXICALANALYZER
#define ___CSTORE_ILEXICALANALYZER
#include "DBCBase.h"

CSTORE_NS_BEGIN

class LexicalAnalyzer : public DBObject {
public:
  //�������ã� ���캯��
  //�����б� N/A 
  //�� �� ֵ�� N/A
  LexicalAnalyzer();

  //�������ã� ��Դ��Ĺ��캯��
  //�����б� 
  //    _src   ������Դ����
  //�� �� ֵ�� N/A
  explicit LexicalAnalyzer(const istr&);

  //�������ã� ����SSQL�ʷ�����DFA�Զ���
  //�����б� N/A
  //�� �� ֵ�� TokenStream* ��Դ���������ϵ�token��
  TokenStream* Analyze();

  //�������ã� ���ô�����Դ����
  //�����б�
  //    _src   Դ�����ַ���
  //�� �� ֵ�� N/A
  void SetSourceCode(const istr&);

  //�������ã� ��ȡԴ����
  //�����б� N/A
  //�� �� ֵ�� std::string Դ������ַ���
  istr GetSourceCode();

  //�������ã� ��λ�ʷ�������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Reset();

  //�������ã� ȡ�þ�������
  //�����б� N/A
  //�� �� ֵ�� StrVec* ����������Դ����ľ��ӵ�����ָ��
  StrVec* GetStrVec();

private:
  //�������ã� ����ָ����Ծ����������
  //�����б�
  //      _go  ɨ���ľ���
  //�� �� ֵ�� N/A
  void Jump(int);

  //�������ã� DFA
  //�����б� N/A
  //�� �� ֵ�� Token* ��һ��Token��ָ��
  Token* NextToken();

  //�������ã� ��ȡ�����õ��հ׶�Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetSpace(Token*);

  //�������ã� ��ȡ�����õ�δ֪�ַ���Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetUnknown(Token*);

  //�������ã� ��ȡ�����õ���ʶ����Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetIdentifier(Token*);

  //�������ã� ��ȡ�����õ�������Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetConstantNumber(Token*);

  //�������ã� ��ȡ�����õ����ַ����Ŷ�Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetOneCharaCalculator(Token*);

  //�������ã� ��ȡ�����õ�����Ϊ˫�ַ���Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetTwoCharaCalculator(Token*);

  //�������ã� ��ȡ�����õ������ֶ�Ӧ��Token
  //�����б�
  //   *result ���Token�������
  //�� �� ֵ�� Boolean �Ƿ�ɹ��õ���Token
  bool GetReservedCalculator(Token*);

  //�������ã� �ж�һ���ַ�������������
  //�����б�
  //        c  ���ж��ַ�
  //�� �� ֵ�� CharaType �ַ�c������
  CharaType GetCharType(const char&);

  // Դ�����ִ�
  istr sourceCode = "";
  // Token��
  TokenStream* currentList;
  // ���α�
  int currentLine;
  // ���α�
  int currentColumn;
  // ���ַ�ָ��
  iPtr PTRnextLetter;
  // ��ʱ����
  istr TempStr = "";
  // ��������
  StrVec sentenceContainer;
}; /* LexicalAnalyzer */

CSTORE_NS_END

#endif /* ___CSTORE_ILEXICALANALYZER */