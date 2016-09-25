#ifndef __CSTORE_DBCONTROLLER
#define __CSTORE_DBCONTROLLER
#include "DBBase.h"
#include "DBBridge.h"
#include <string>

class DBController {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� �����ڴ���Ψһʵ��������
  static DBController* Invoke();
 
  //�������ã� ��ʼ����ǰ�����ݿ�����
  //�����б� 
  //      argc �����ĸ���
  //      argv ��������
  //�� �� ֵ�� N/A
  void DBInit(int argc, char* argv[]);

  //�������ã� ��ǰ�˶���ת��Ϊ��̨����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void DBStartDash();

  //�������ã� ��ȡ��ǰ���ݿ�״̬
  //�����б� N/A
  //�� �� ֵ�� ��ǰ���ݿ�״̬��ö��
  RunType GetRunType();

  //�������ã� ��ȡ���ݿ������ϵͳ������·��
  //�����б� N/A
  //�� �� ֵ�� ����·���ַ���
  std::string GetRunPath();

  //�������ã� ������
  //�����б� N/A
  ~DBController();

private:
  //�������ã� �������̨�ն�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Terminal();

  //�������ã� �����ѯ�������
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Dash(const std::string& query);

  //�������ã� ˽�еĹ�����
  //�����б� N/A
  DBController();

  // Ψһʵ��
  static DBController* Instance;

  // ��ǰ״̬
  RunType runType = RunType::RUN_CONSOLE;

  // ���е�·��
  std::string runPath = "";

  // ��������ʱ��ѯ���
  std::string tempQuery = "";

  // ������
  DBBridge* IBridge = NULL;

  // ��ֹ���ƺ͸�ֵ
  DISALLOW_COPY_AND_ASSIGN(DBController);
};


#endif // __CSTORE_DBCONTROLLER