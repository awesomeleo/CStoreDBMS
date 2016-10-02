#include "DBConnector.h"
#include "DBBridge.h"
#include <thread>
#include <future>

CSTORE_NS_BEGIN

// ����������������Ψһʵ��
DBConnector* DBConnector::GetInstance() {
  return DBConnector::instance == NULL ? 
    DBConnector::instance = new DBConnector() : DBConnector::instance;
}

// ˽�еĹ�����
DBConnector::DBConnector()
  :DBObject("DBConnector", this) {
  this->ProcCounter = 0;
  this->HandleNum = CONNECTORLIMIT;
  this->quitFlag = false;
}

// ������
DBConnector::~DBConnector() {
  TRACE("DBConnector is going to collapse, all transactions will be TERMINATED!");
  // �������з�ֹ���������
  this->queueMutex.lock();
  // ��������ȫ������
  this->quitFlag = true;
  this->ProcCounter = 0;
  // �ͷ���Դ
  this->threadPool.clear();
  this->ClearQueue();
  for (int i = 0; i < this->finishedTransactionVector.size(); i++) {
    if (this->finishedTransactionVector[i] != NULL) {
      delete this->finishedTransactionVector[i];
    }
  }
  this->finishedTransactionVector.clear();
  this->queueMutex.unlock();
  TRACE("DBConnector is already collapsed");
}

// �������ύ�����ݿ�����
void DBConnector::Commit(const std::string query) {
  queueMutex.lock();
  DBTransaction* trans = new DBTransaction(query);
  this->transactionQueue.push(trans);
  queueMutex.unlock();
}

// ǿ���ж�����������ն���
void DBConnector::KillAll() {
  TRACE("Killing all processing transaction");
  // �������з�ֹ���������
  this->queueMutex.lock();
  // ��������ȫ������
  this->quitFlag = true;
  this->ProcCounter = 0;
  // �ͷ���Դ
  this->threadPool.clear();
  this->ClearQueue();
  this->queueMutex.unlock();
  TRACE("Killed all processing transaction");
  // ���¿�������
  this->quitFlag = false;
  for (int i = 0; i < this->HandleNum; i++) {
    this->threadPool.push_back(std::thread(DBConnector::TransactionHandler));
  }
}

// ����������
void DBConnector::ClearQueue() {
  queueMutex.lock();
  int dropTransaction = 0;
  while (!this->transactionQueue.empty()) {
    DBTransaction* t = this->transactionQueue.front();
    if (t != NULL) {
      delete t;
    }
    this->transactionQueue.pop();
    dropTransaction++;
  }
  char buf[16];
  _itoa(dropTransaction, buf, 10);
  TRACE("Droped " + std::string(buf) + " unsolve transaction");
  queueMutex.unlock();
}

// ����������Ŷӵ�����
int DBConnector::CountQueue() {
  int retNum = 0;
  queueMutex.lock();
  retNum = this->transactionQueue.size();
  queueMutex.unlock();
  return retNum;
}

// ���ڽ��е���������
int DBConnector::CountProcessing() {
  int retNum = 0;
  queueMutex.lock();
  retNum = this->ProcCounter;
  queueMutex.unlock();
  return retNum;
}

// ��ȡ����ɵ�����˵��
std::string DBConnector::ShowFinishedTransaction() {
  std::string sb = "";
  for (int i = 0; i < this->finishedTransactionVector.size(); i++) {

  }


  return "";
}

// ��������
void DBConnector::TransactionHandler() {
  DBConnector* core = DBConnector::GetInstance();
  while (true) {
    // ������߳�Ҫ���˳�
    if (core->quitFlag) {
      TRACE("Thread forced to exit");
      return;
    }
    // �Ӷ�����ȡδ���������
    DBTransaction* proTrans = NULL;
    core->queueMutex.lock();
    if (core->transactionQueue.size() > 0) {
      proTrans = core->transactionQueue.front();
      core->transactionQueue.pop();
    }
    core->ProcCounter++;
    core->queueMutex.unlock();
    // �����������
    if (proTrans != NULL) {
      DBBridge* IBridge = new DBBridge();
      IBridge->Init(RunType::RUN_COMMAND, proTrans->GetCode());
      IBridge->StartDash();
      proTrans->Finish();
      core->queueMutex.lock();
      core->finishedTransactionVector.push_back(proTrans);
      core->ProcCounter--;
      core->queueMutex.unlock();
    }
  }
}


// �����߳���
void DBConnector::SetThreadNum(int tnum) {
  // ���д����е�����ʱ��ֹ�ı�
  queueMutex.lock();
  if (this->transactionQueue.size() != 0) {
    encounterMutex.lock();
    if (this->ProcCounter != 0) {
      TRACE("Transaction is processing or transaction queue not empty, cannot resize thread number");
      return;
    }
    encounterMutex.unlock();
  }
  // �쳣ֵ�Ͳ���
  if (this->threadPool.size() == tnum || tnum < 1) {
    TRACE("thread num not change");
    return;
  }
  // �޸ļ�¼
  this->HandleNum = tnum;
  // ��ӵ����
  if (this->threadPool.size() > tnum) {
    for (int i = 0; i < tnum - threadPool.size(); i++) {
      this->threadPool.push_back(std::thread(DBConnector::TransactionHandler));
    }
  }
  // ɾ�������
  else {
    this->threadPool.erase(this->threadPool.begin() + tnum, this->threadPool.end());
  }
  queueMutex.unlock();
}

// Ψһʵ��
DBConnector* DBConnector::instance = NULL;

CSTORE_NS_END