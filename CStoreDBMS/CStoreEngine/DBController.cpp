#include "DBController.h"
#include "DBCBase.h"

// ˽�еĹ�����
DBController::DBController()
  :DBObject("DBController", this) {
  this->IBridge = new DBBridge();
}

// ������
DBController::~DBController() {
  if (this->IBridge != NULL) {
    delete this->IBridge;
  }
  this->IBridge = NULL;
}

// ����������������Ψһʵ��
DBController* DBController::Invoke() {
  return DBController::Instance == NULL ?
    new DBController() : DBController::Instance;
}

// ��ʼ����ǰ�����ݿ�����
void DBController::DBInit(int argc, char* argv[]) {
  // ��������·��
  this->runPath = argv[0];
  // ������ֱ��ִ��ʱ
  if (argc == 1) {
    this->runType = RunType::RUN_CONSOLE;
  }
  // ����״̬�����ѯ���
  else {
    this->runType = RunType::RUN_COMMAND;
    std::string queryBuilder = "";
    for (int qb = 1; qb < argc; qb++) {
      queryBuilder += " " + std::string(argv[qb]);
    }
    queryBuilder += SENTENCETERMINATOR;
    this->tempQuery = queryBuilder;
    PILEPRINTLN(">> Build Query From Terminal Input: " + queryBuilder);
  }
}

// ��ǰ�˶���ת��Ϊ��̨����
void DBController::DBStartDash() {
  switch (this->runType) {
  case RunType::RUN_CONSOLE:
    this->Terminal();
    break;
  case RunType::RUN_COMMAND:
    this->Dash(this->tempQuery);
    break;
  default:
    PILEPRINTLN(">>>>>>>> EXCEPTION: DBMS Runstate Not Clear");
    break;
  }
}

// �������̨�ն�����
void DBController::Terminal() {
  PILEPRINTLN(">> Welcome to Simplifie-CStore DBMS <<");
  this->IBridge->Init(this->runType);
  this->IBridge->StartDash();
}

// �����ѯ�������
void DBController::Dash(const std::string& query) {
  this->IBridge->Init(this->runType, this->tempQuery);
  this->IBridge->StartDash();
}

// ��ȡ��ǰ���ݿ�״̬
RunType DBController::GetRunType() {
  return this->runType;
}

// ��ȡ���ݿ������ϵͳ������·��
std::string DBController::GetRunPath() {
  return this->runPath;
}

// Ψһʵ��
DBController* DBController::Instance = NULL;