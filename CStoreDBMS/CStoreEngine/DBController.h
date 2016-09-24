#ifndef __CSTORE_DBCONTROLLER
#define __CSTORE_DBCONTROLLER
#include "DBBase.h"

class DBController {
public:
  // �������ã� ����������������Ψһʵ��
  // �����б� N/A
  // �� �� ֵ�� �����ڴ���Ψһʵ��������
  static DBController* Invoke();
 
  // �������ã� ��ʼ����ǰ�����ݿ�����
  // �����б� 
  //       argc �����ĸ���
  //       argv ��������
  // �� �� ֵ�� N/A
  void DBInit(int argc, char* argv[]);

  // �������ã� ��ǰ�˶���ת��Ϊ��̨����
  // �����б� N/A
  // �� �� ֵ�� �����ɹ����
  bool DBStartDash();

private:
  // �������ã� ˽�еĹ�����
  // �����б� N/A
  DBController();

  // Ψһʵ��
  static DBController* Instance;

  // ��ֹ���ƺ͸�ֵ
  DISALLOW_COPY_AND_ASSIGN(DBController);
};


#endif // __CSTORE_DBCONTROLLER