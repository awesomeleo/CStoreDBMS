#ifndef ___CSTORE_DBTABLE
#define ___CSTORE_DBTABLE
#include "DBBase.h"
#include <string>
#include <vector>

class DBTable : public DBObject {
public:
  // ������
  CONSTRUCTOR_SIMPLE_DBOBJECT(DBTable, "DBTable");

  // �����ַ���������
  virtual std::string ToString() {
    std::string sb = this->GetTypename() + " [Name:" + this->TableName + ", Cols:{";
    if (this->PiList.size() > 0) {
      sb += PiList[0];
      for (int it = 1; it < this->PiList.size(); it++) {
        sb += "|" + PiList[it];
      }
    }
    sb += "}, Compressed:" + this->IsSorted ? "Y" : "N";
    sb += "]";
    return sb;
  };

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