#include "DBController.h"
#include "DBCBase.h"
#include <iostream>

// ˽�еĹ�����
DBController::DBController() { }

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
    PILEPRINTLN(">> Welcome to Simplified CStore DBMS <<");
    break;
  case RunType::RUN_COMMAND:
    
    break;
  default:
    PILEPRINTLN(">>> ERROR: DBMS Runstate Not Clear");
    break;
  }
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