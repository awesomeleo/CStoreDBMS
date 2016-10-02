#ifndef ___CSTORE_DBCONNECTOR
#define ___CSTORE_DBCONNECTOR
#include "DBBase.h"
#include "DBTransaction.hpp"
#include <thread>
#include <mutex>

CSTORE_NS_BEGIN

class DBConnector : public DBObject {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� ��������Ψһʵ��
  static DBConnector* GetInstance();

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

  //�������ã� ������
  //�����б� N/A
  //�� �� ֵ�� N/A
  ~DBConnector();

private:
  //�������ã� ˽�еĹ�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  DBConnector();

  //�������ã� ��������
  //�����б� N/A
  //�� �� ֵ�� N/A
  static void TransactionHandler();

  // ������־
  bool quitFlag;

  // �������
  std::queue<DBTransaction*> transactionQueue;

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

  // Ψһʵ��
  static DBConnector* instance;

  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBConnector);
}; /* DBConnector */

CSTORE_NS_END
#endif /* ___CSTORE_DBCONNECTOR */