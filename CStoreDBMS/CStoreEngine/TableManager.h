#ifndef ___CSTORE_TABLEMANAGER
#define ___CSTORE_TABLEMANAGER
#include "DBBase.h"
#include "DBTable.hpp"
#include <string>
#include <vector>

class TableManager {
public:
  //�������ã� ��ӱ�
  //�����б�
  //   tabName ����
  //  loadFile Ҫ����ı��ļ�
  //�� �� ֵ�� �����ɹ����
  bool AddTable(std::string, std::string);

  //�������ã� ɾ����
  //�����б�
  //   tabName ����
  //�� �� ֵ�� �����ɹ����
  bool DropTable(std::string);

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
  //�� �� ֵ�� �����ɹ����
  bool SaveContext();

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
  std::vector<DBTable> tableContainer;

  // ����������
  std::vector<TableLockType> lockContainer;

  // Ψһʵ��
  static TableManager* instance;
}; /* TableManager */

#endif /* ___CSTORE_TABLEMANAGER */