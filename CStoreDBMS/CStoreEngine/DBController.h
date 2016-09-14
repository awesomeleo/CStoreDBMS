#ifndef __CSTORE_DBCONTROLLER
#define __CSTORE_DBCONTROLLER
#include "DBBase.h"
class DBController {
public:
  // �������ã� ����������������Ψһʵ��
  // �����б� N/A
  // �� �� ֵ�� �����ڴ���Ψһʵ��������
  static DBController GetInstance();
 
  // �������ã� ��ǰ�˶���ת��Ϊ��̨����
  // �����б� 
  //     opCode ��������ö����
  // �� �� ֵ�� �����ɹ����
  bool DBExecutor(DBOperationType opCode);

private:
  // �������ã� ˽�еĹ�����
  // �����б� N/A
  DBController();

  // Ψһʵ��
  static DBController Instance;

  // ��ֹ���ƺ͸�ֵ
  DISALLOW_COPY_AND_ASSIGN(DBController);
};


#endif // __CSTORE_DBCONTROLLER