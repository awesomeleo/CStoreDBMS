#include "DBController.h"

CSTORE_NS_BEGIN

// ˽�еĹ�����
DBController::DBController()
  :DBObject("DBController", this) {
  this->connector = DBConnectionPool::GetInstance();
}

// ������
DBController::~DBController() {
  if (this->connector != NULL) {
    delete this->connector;
  }
  this->connector = NULL;
}

// ����������������Ψһʵ��
DBController* DBController::Invoke() {
  return DBController::Instance == NULL ?
    new DBController() : DBController::Instance;
}

// ��ǰ�˶���ת��Ϊ��̨����
void DBController::StartDash(int argc, char* argv[]) {
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
  bool rflag = true;
  bool breakFlag = false;
  FOREVER {
    // ��ȡ����̨�����ԷֺŽ���
    if (this->DebugMode) {
      PILEPRINT("SSQL# ");
    }
    else {
      PILEPRINT("SSQL> ");
    }
    char charBuilder;
    CSCommonUtil::StringBuilder queryBuilder;
    do {
      charBuilder = getchar();
      if (charBuilder == '\n') {
        charBuilder = ' ';
        if (rflag) {
          PILEPRINT("    > ");
        }
        rflag = true;
      }
      if (breakFlag) {
        breakFlag = false;
      }
      else {
        queryBuilder.Append(charBuilder);
      }
    } while (charBuilder != SENTENCETERMINATOR);
    // ��ȡ�������
    breakFlag = true;
    rflag = false;
    std::string commitQuery = queryBuilder.ToString();
    std::string _query = commitQuery;
    std::transform(_query.begin(), _query.end(), _query.begin(), tolower);
    // ����������ж�
    if (_query == "exit;") {
      TRACE("Good Bye!");
      return;
    }
    else if (this->ReservedRouter(_query) == true) {
      continue;
    }
    // �ύ��������
    this->connector->SetDebug(this->DebugMode);
    this->connector->Commit(commitQuery);
    // ���У�׼��������һ������
    PILEPRINT(NEWLINE);
  }
}

// �����ѯ�������
void DBController::Dash(const std::string& query) {
  std::string _query = query;
  std::transform(_query.begin(), _query.end(), _query.begin(), tolower);
  // ����������ж�
  if (_query == "exit;") {
    TRACE("Good Bye!");
    return;
  }
  if (this->ReservedRouter(_query) == true) {
    return;
  }
  // �ύ��������
  this->connector->SetDebug(this->DebugMode);
  this->connector->Commit(query);
}

// ������������
bool DBController::ReservedRouter(const std::string& query) {
  if (query == "debug;") {
    this->DebugMode = true;
    return true;
  }
  else if (query == "no debug;") {
    this->DebugMode = false;
    return true;
  }
  else if (query == "show transactions;") {

    return true;
  }
  else if (query == "show tables;") {
    
    return true;
  }
  return false;
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

CSTORE_NS_END
