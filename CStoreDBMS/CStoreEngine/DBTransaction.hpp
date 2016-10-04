#ifndef ___CSTORE_DBTRANSACTION
#define ___CSTORE_DBTRANSACTION
#include "DBBase.h"

CSTORE_NS_BEGIN

class DBTransaction : public DBObject {
public:
  //�������ã� ������
  //�����б�
  // queryCode ��ѯ����
  //�� �� ֵ�� N/A
  DBTransaction(std::string);

  //�������ã� ��ȡ����ı��
  //�����б� N/A
  //�� �� ֵ�� ���������ŵ�����
  inline int GetId() { return this->id; }

  //�������ã� ��ȡ����Ĳ�ѯ����
  //�����б� N/A
  //�� �� ֵ�� ��ѯ����ַ���
  inline std::string GetCode() { return this->code; }

  //�������ã� ��ȡ�����������
  //�����б� N/A
  //�� �� ֵ�� �Ƿ��Ѿ����
  inline bool GetState() { return this->successFlag; }

  //�������ã� ��ȡ��������ʱ��
  //�����б� N/A
  //�� �� ֵ�� ���ʱ���ַ���
  inline std::string GetFinishTimestamp() { return this->finishstamp; }

  //�������ã� ��ȡ����Ŀ�ʼʱ��
  //�����б� N/A
  //�� �� ֵ�� ���ʱ���ַ���
  inline std::string GetBeginTimestamp() { return this->timestamp; }

  //�������ã� �������ɹ�
  //�����б� N/A
  //�� �� ֵ�� N/A
  inline void Finish() { this->successFlag = true; }

  //�������ã� ��ȡ�������ʱ��
  //�����б� N/A
  //�� �� ֵ�� ����ʱ���ַ���������Ϊ��λ
  std::string GetDuration();

  //�������ã� ��д�ַ���������
  //�����б� N/A
  //�� �� ֵ�� �����˵��
  virtual std::string ToString();

private:
  // ����id
  int id;

  // �����Ƿ��ѳɹ����
  bool successFlag;

  // ����Ĳ�ѯ����
  std::string code;

  // ������ʱ���
  std::string timestamp;

  // ��ɵ�ʱ���
  std::string finishstamp;

  // ����ʱ�������
  std::clock_t beginStampObj;

  // ���������
  static int encounter;

  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBTransaction);
}; /* DBTransaction */

CSTORE_NS_END

#endif /* ___CSTORE_DBTRANSACTION */