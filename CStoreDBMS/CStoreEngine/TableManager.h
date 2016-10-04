#ifndef ___CSTORE_TABLEMANAGER
#define ___CSTORE_TABLEMANAGER
#include "DBBase.h"
#include "DBLock.hpp"
#include "DBTable.hpp"

CSTORE_NS_BEGIN

class TableManager : public DBObject {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� ���������Ψһʵ��
  static TableManager* GetInstance();

  //�������ã� ��ӱ�
  //�����б�
  //   tabName ����
  //  loadFile Ҫ����ı��ļ�
  //�� �� ֵ�� �����ɹ����
  bool AddTable(const std::string&, const std::string&);

  //�������ã� ɾ����
  //�����б�
  //   tabName ����
  //�� �� ֵ�� �����ɹ����
  bool DropTable(const std::string&);

  //�������ã� �жϱ�Ĵ���
  //�����б�
  //   tabName ����
  //�� �� ֵ�� ���Ƿ����
  bool ExistTable(const std::string&);

  //�������ã� ��ȡ�����
  //�����б�
  //   tabName ����
  //�� �� ֵ�� ������ָ��
  DBTable* GetTable(const std::string&);

  //�������ã� ��ȡ���������
  //�����б�
  //   tabName ����
  //�� �� ֵ�� ������ָ��
  DBLock* GetTableLock(const std::string&);

  //�������ã� ��ȡ���б��������Ϣ
  //�����б� N/A
  //�� �� ֵ�� ��������ַ���
  std::string ShowTable();

  //�������ã� ͳ�����ݿ��б������
  //�����б� N/A
  //�� �� ֵ�� �������
  int CountTable();

  //�������ã� ������ݿ����б��
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Clear();

  //�������ã� ����ǰ���ݿ��״̬���浽����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void SaveContext();

  //�������ã� ����ǰ���ݿ��״̬�Ӵ��̶���
  //�����б� N/A
  //�� �� ֵ�� �����ɹ����
  bool LoadContext();

private:
  //�������ã� ˽�еĹ��캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  TableManager();

  // ���������
  std::vector<DBTable*> tableContainer;

  // ����������
  std::vector<DBLock*> lockContainer;

  // ���������е��±�����
  std::map<std::string, int> tableIndexDict;

  // Ψһʵ��
  static TableManager* Instance;
}; /* TableManager */

CSTORE_NS_END

#endif /* ___CSTORE_TABLEMANAGER */