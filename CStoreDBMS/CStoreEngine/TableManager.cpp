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
}

// ����ǰ���ݿ��״̬���浽����
void TableManager::SaveContext() {

}

// ����ǰ���ݿ��״̬�Ӵ��̶���
bool TableManager::LoadContext() {

  return true;
}

// ˽�еĹ��캯��
TableManager::TableManager()
  :DBObject("TableManager", this) {

}

// Ψһʵ��
TableManager* TableManager::Instance = NULL;

CSTORE_NS_END
