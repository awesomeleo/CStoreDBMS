#include "DBConnectionPool.h"
#include "DBBridge.h"

CSTORE_NS_BEGIN

// ����������������Ψһʵ��
DBConnectionPool* DBConnectionPool::GetInstance() {
  return DBConnectionPool::instance == NULL ? 
    DBConnectionPool::instance = new DBConnectionPool() : DBConnectionPool::instance;
}

// ˽�еĹ�����
DBConnectionPool::DBConnectionPool()
  :DBObject("DBConnectionPool", this) {
  this->ProcCounter = 0;
  this->SetThreadNum(CONNECTORLIMIT);
  this->quitFlag = false;
  this->isDebug = false;
}

// ������
DBConnectionPool::~DBConnectionPool() {
  TRACE("DBConnectionPool is going to collapse, all transactions will be TERMINATED!");
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
  TRACE("DBConnectionPool is already collapsed");
}

// �������ύ�����ݿ�����
void DBConnectionPool::Commit(const std::string query) {
  queueMutex.lock();
  DBTransaction* trans = new DBTransaction(query);
  this->transactionQueue.push(trans);
  queueMutex.unlock();
}

// ǿ���ж�����������ն���
void DBConnectionPool::KillAll() {
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
    this->threadPool.push_back(std::thread(DBConnectionPool::TransactionHandler));
  }
}

// ����������
void DBConnectionPool::ClearQueue() {
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
int DBConnectionPool::CountQueue() {
  int retNum = 0;
  queueMutex.lock();
  retNum = this->transactionQueue.size();
  queueMutex.unlock();
  return retNum;
}

// ���ڽ��е���������
int DBConnectionPool::CountProcessing() {
  int retNum = 0;
  queueMutex.lock();
  retNum = this->ProcCounter;
  queueMutex.unlock();
  return retNum;
}

// ��ȡ����ɵ�����˵��
std::string DBConnectionPool::ShowFinishedTransaction() {
  std::string sb = "";
  for (int i = 0; i < this->finishedTransactionVector.size(); i++) {
    if (this->finishedTransactionVector[i] != NULL) {
      sb += this->finishedTransactionVector[i]->ToString() + NEWLINE;
    }
  }
  return sb;
}

// ��������
void DBConnectionPool::TransactionHandler() {
  DBConnectionPool* core = DBConnectionPool::GetInstance();
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
    else {
      core->queueMutex.unlock();
      continue;
    }
    core->processingTransactionVector.push_back(proTrans);
    core->ProcCounter++;
    core->queueMutex.unlock();
    // �����������
    if (proTrans != NULL) {
      DBBridge* IBridge = new DBBridge();
      IBridge->StartTransaction(proTrans->GetCode(), core->isDebug);
      proTrans->Finish();
      core->queueMutex.lock();
      for (std::vector<DBTransaction*>::iterator iter = core->processingTransactionVector.begin();
        iter != core->processingTransactionVector.end(); iter++) {
        if ((*iter)->ReferenceEquals(proTrans)) {
          core->processingTransactionVector.erase(iter);
          break;
        }
      }
      core->finishedTransactionVector.push_back(proTrans);
      core->ProcCounter--;
      core->queueMutex.unlock();
      PILEPRINTLN("      Query OK, cost: " + proTrans->GetDuration() + " sec(s)");
    }
  }
}

// �����߳���
void DBConnectionPool::SetThreadNum(int tnum) {
  // ���д����е�����ʱ��ֹ�ı�
  queueMutex.lock();
  if (this->transactionQueue.size() != 0 ||
    this->processingTransactionVector.size() != 0) {
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
  if (this->threadPool.size() < tnum) {
    for (int i = 0; i < tnum - threadPool.size(); i++) {
      this->threadPool.push_back(std::thread(DBConnectionPool::TransactionHandler));
    }
  }
  // ɾ�������
  else {
    this->threadPool.erase(this->threadPool.begin() + tnum, this->threadPool.end());
  }
  queueMutex.unlock();
}

// ����DEBUG״̬
void DBConnectionPool::SetDebug(bool state) {
  this->isDebug = state;
}

// Ψһʵ��
DBConnectionPool* DBConnectionPool::instance = NULL;

CSTORE_NS_END