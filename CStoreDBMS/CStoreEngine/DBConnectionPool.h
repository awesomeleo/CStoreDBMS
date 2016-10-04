#ifndef ___CSTORE_DBCONNECTIONPOOL
#define ___CSTORE_DBCONNECTIONPOOL
#include "DBBase.h"
#include "DBTransaction.hpp"

CSTORE_NS_BEGIN

class DBConnectionPool : public DBObject {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� ��������Ψһʵ��
  static DBConnectionPool* GetInstance();

  //�������ã� �������ύ�����ݿ�����
  //�����б�
  //     query Ҫ�ύ�������ѯ��伯��
  //�� �� ֵ�� ��������Ψһʵ��
  void Commit(const std::string);

  //�������ã� ǿ���ж�����������ն���
  //�����б� N/A
  //�� �� ֵ�� N/A
  void KillAll();

  //�������ã� ����������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void ClearQueue();

  //�������ã� ����������Ŷӵ�����
  //�����б� N/A
  //�� �� ֵ�� �Ŷ�������
  int CountQueue();

  //�������ã� ���ڽ��е���������
  //�����б� N/A
  //�� �� ֵ�� ������������
  int CountProcessing();

  //�������ã� ��ȡ����ɵ�����˵��
  //�����б� N/A
  //�� �� ֵ�� ���������������ϸ˵�����ַ���
  std::string ShowFinishedTransaction();

  //�������ã� �����߳���
  //�����б�
  //      tnum �߳���
  //�� �� ֵ�� N/A
  void SetThreadNum(int);

  //�������ã� ����DEBUG���״̬
  //�����б�
  //     state �Ƿ�debug
  //�� �� ֵ�� N/A
  void SetDebug(bool);

  //�������ã� ������
  //�����б� N/A
  //�� �� ֵ�� N/A
  ~DBConnectionPool();

private:
  //�������ã� ˽�еĹ�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  DBConnectionPool();

  //�������ã� ��������
  //�����б� N/A
  //�� �� ֵ�� N/A
  static void TransactionHandler();

  // ������־
  bool quitFlag;
  // �������
  std::queue<DBTransaction*> transactionQueue;
  // ��������������
  std::vector<DBTransaction*> processingTransactionVector;
  // �������������
  std::vector<DBTransaction*> finishedTransactionVector;
  // �̳߳�
  std::vector<std::thread> threadPool;
  // ��������
  std::mutex encounterMutex;
  // ������
  std::mutex queueMutex;
  // �߳���
  int HandleNum;
  // �������̼߳�����
  int ProcCounter;
  // �Ƿ����DEBUG��Ϣ
  bool isDebug;
  // Ψһʵ��
  static DBConnectionPool* instance;
  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBConnectionPool);
}; /* DBConnectionPool */

CSTORE_NS_END
#endif /* ___CSTORE_DBCONNECTIONPOOL */