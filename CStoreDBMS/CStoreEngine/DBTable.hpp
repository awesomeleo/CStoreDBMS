#ifndef ___CSTORE_DBTABLE
#define ___CSTORE_DBTABLE
#include "DBBase.h"

CSTORE_NS_BEGIN

class DBTable : public DBObject {
public:
  //�������ã� ������
  //�����б�
  //   tabName ����
  //  fileName ��Դ�ļ�
  //�� �� ֵ�� N/A
  DBTable(const std::string tabName, const std::string fileName)
    :DBObject("DBTable", this) {
    this->IsSorted = false;
    this->TableName = tabName;
    this->OriginalFileName = fileName;
  }

  //�������ã� ��д�ַ���������
  //�����б� N/A
  //�� �� ֵ�� ���˵��
  virtual std::string ToString() {
    CSCommonUtil::StringBuilder sb(this->GetTypename() + " [Name:" + this->TableName + ", Cols:{");
    if (this->PiList.size() > 0) {
      sb.Append(this->PiList[0] + " " + this->PiTypeList[0]);
      for (int it = 1; it < this->PiList.size(); it++) {
        sb.Append("|" + this->PiList[it] + " " + this->PiTypeList[it]);
      }
    }
    sb.Append("}, Compressed:").Append(this->IsSorted ? "Y" : "N").Append("]");
    return sb.ToString();
  };

  // ����
  std::string TableName;

  // ԭʼ�ļ���
  std::string OriginalFileName;

  // ������
  std::vector<std::string> PiList;

  // ����������
  std::vector<std::string> PiTypeList;

  // ��������Ӧ���ļ���
  std::map<std::string, std::string> PiFileNameList;

  // �Ƿ��Ѿ�����ѹ��
  bool IsSorted;

  // ����ѹ��������������Ӧ���ļ���
  std::vector<std::string> CompressedPiFileNameList;

private:
  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBTable);
}; /* DBTable */

CSTORE_NS_END

#endif /* ___CSTORE_DBTABLE */