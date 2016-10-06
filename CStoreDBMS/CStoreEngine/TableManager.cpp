#include "TableManager.h"

CSTORE_NS_BEGIN

// ��������
TableManager* TableManager::GetInstance() {
  return TableManager::Instance == NULL ?
    TableManager::Instance = new TableManager() : TableManager::Instance;
}

// ��ӱ�
bool TableManager::AddTable(const std::string& tabName, const std::string& loadFile) {
  // ��������
  if (this->GetTable(tabName)) {
    TRACE("Expect to create table: " + tabName + ", but table already exist.");
    return false;
  }
  // �����µı����
  DBTable* ntb = new DBTable(tabName, loadFile);
  this->tableContainer.push_back(ntb);
  // ����һ����
  DBLock* ntl = new DBLock();
  ntl->LockBinding = ntb;
  this->lockContainer.push_back(ntl);
  // ׷���±�����
  this->tableIndexDict[tabName] = this->tableContainer.size() - 1;
}

// ɾ����
bool TableManager::DropTable(const std::string& tabName) {
  // ��ȡ�����
  DBTable* tabObj = this->GetTable(tabName);
  if (tabObj == NULL) {
    return false;
  }
  // �Ƴ������󣬴˴������Ƿ��������ᴥ����ɾ��������ִ�������ж�����״̬
  for (std::vector<DBLock*>::iterator iter = this->lockContainer.begin(); iter != this->lockContainer.end(); iter++) {
    if ((*iter) != NULL && (*iter)->LockBinding->ReferenceEquals(tabObj)) {
      delete (*iter);
      this->lockContainer.erase(iter);
      return true;
    }
  }
  // �Ƴ������
  for (std::vector<DBTable*>::iterator iter = this->tableContainer.begin(); iter != this->tableContainer.end(); iter++) {
    if ((*iter) != NULL && (*iter)->TableName == tabName) {
      delete (*iter);
      this->tableContainer.erase(iter);
      return true;
    }
  }
  // ����������¼
  if (this->tableIndexDict.find(tabName) != this->tableIndexDict.end()) {
    this->tableIndexDict.erase(tabName);
  }
  this->tableIndexDict.clear();
  for (int i = 0; i < this->tableContainer.size(); i++) {
    this->tableIndexDict[this->tableContainer[i]->TableName] = i;
  }
  TRACE("Expect to delete: " + tabName + ", but table not exist.");
  return false;
}

// ��ȡ�����
DBTable* TableManager::GetTable(const std::string& tabName) {
  if (this->tableIndexDict.find(tabName) != this->tableIndexDict.end()) {
    return this->tableContainer[this->tableIndexDict[tabName]];
  }
  return NULL;
}

// �жϱ�Ĵ���
bool TableManager::ExistTable(const std::string& tabName) {
  return this->GetTable(tabName) == NULL;
}

// ��ȡ���������
DBLock* TableManager::GetTableLock(const std::string& tabName) {
  // ��ȡ�����
  DBTable* tabObj = this->GetTable(tabName);
  if (tabObj == NULL) {
    return NULL;
  }
  // ������
  for (std::vector<DBLock*>::iterator iter = this->lockContainer.begin(); iter != this->lockContainer.end(); iter++) {
    if ((*iter) != NULL && (*iter)->LockBinding->ReferenceEquals(tabObj)) {
      return (*iter);
    }
  }
  return NULL;
}

// ��ȡ���б��������Ϣ
std::string TableManager::ShowTable() {
  CSCommonUtil::StringBuilder sb;
  for (int i = 0; i < this->tableContainer.size(); i++) {
    sb.Append(this->tableContainer[i]->ToString()).Append(NEWLINE);
  }
  return sb.ToString();
}

// ͳ�����ݿ��б������
int TableManager::CountTable() {
  return this->tableContainer.size();
}

// ������ݿ����б��
void TableManager::Clear() {
  for (int i = 0; i < this->tableContainer.size(); i++) {
    if (this->tableContainer[i] != NULL) {
      delete this->tableContainer[i];
    }
  }
  this->tableIndexDict.clear();
  this->tableContainer.clear();
  this->SaveContext();
}

// ����ǰ���ݿ��״̬���浽����
void TableManager::SaveContext() {
  std::ofstream f("table.dbconf");
  for (int i = 0; i < this->tableContainer.size(); i++) {
    DBTable* t = this->tableContainer[i];
    f << t->TableName << "$";
    if (t->PiList.size() > 0) {
      f << t->PiList[0] << "@" << t->PiTypeList[0];
      for (int j = 1; j < t->PiList.size(); j++) {
        f << "," << t->PiList[j] << "@" << t->PiTypeList[j];
      }
    }
    f << NEWLINE;
  }
  f.close();
}

// ����ǰ���ݿ��״̬�Ӵ��̶���
bool TableManager::LoadContext() {
  // û�������ļ�ʱ
  std::ifstream f("table.dbconf");
  if (!f) {
    TRACE("No table config file found, DBMS will be reset.");
    f.close();
    return false;
  }
  // ��ȡ�����ļ�����ԭ�����
  char buf[256];
  while (!f.eof()) {
    f.getline(buf, 256);
    std::string line(buf);
    std::vector<std::string> lineitem = CSCommonUtil::CStrSplit(line, "$");
    if (lineitem.size() == 2) {
      if (this->AddTable(lineitem[0], lineitem[0]) == false) {
        TRACE("Cannot reload table:" + lineitem[0]);
        continue;
      }
      DBTable* tobj = this->GetTable(lineitem[0]);
      std::vector<std::string> piitem = CSCommonUtil::CStrSplit(lineitem[1], ",");
      for (int p = 0; p < piitem.size(); p++) {
        std::vector<std::string> pisingle = CSCommonUtil::CStrSplit(piitem[p], "@");
        tobj->PiList.push_back(pisingle[0]);
        tobj->PiTypeList.push_back(pisingle[1]);
        tobj->PiFileNameList[pisingle[0]] = lineitem[0] + "_" + pisingle[0] + ".db";
      }
    }
  }
  f.close();
  return true;
}

// ��������
TableManager::~TableManager() {
  this->SaveContext();
}

// ˽�еĹ��캯��
TableManager::TableManager()
  :DBObject("TableManager", this) {
  this->LoadContext();
}

// Ψһʵ��
TableManager* TableManager::Instance = NULL;

CSTORE_NS_END
