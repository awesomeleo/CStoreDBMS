#ifndef ___CSTORE_DBTABLE
#define ___CSTORE_DBTABLE
#include "DBBase.h"
#include <string>
#include <vector>

class DBTable {
public:
  // ����
  std::string TableName;

  // ԭʼ�ļ���
  std::string OriginalFileName;

  // ������
  std::vector<std::string> PiList;

  // ��������Ӧ���ļ���
  std::vector<std::string> PiFileNameList;

  // �Ƿ��Ѿ�����ѹ��
  bool IsSorted;

  // ����ѹ��������������Ӧ���ļ���
  std::vector<std::string> CompressedPiFileNameList;
}; /* DBTable */

#endif /* ___CSTORE_DBTABLE */