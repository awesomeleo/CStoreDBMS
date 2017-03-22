#ifndef __CSTORE_DBCONTROLLER
#define __CSTORE_DBCONTROLLER
#include "DBBase.h"
#include "DBConnectionPool.h"

CSTORE_NS_BEGIN

class DBController : public DBObject {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� �����ڴ���Ψһʵ��������
  static DBController* Invoke();

  //�������ã� ��ʼ����ǰ�����ݿ����沢��ʼ����
  //�����б�
  //      argc �����ĸ���
  //      argv ��������
  //�� �� ֵ�� N/A
  void StartDash(int, char**);

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
  void Dash(const std::string&);

  //�������ã� ������������
  //�����б�
  //     query ��ѯ���
  //�� �� ֵ�� N/A
  bool ReservedRouter(const std::string&);

  //�������ã� ˽�еĹ�����
  //�����б� N/A
  DBController();

  // ��������ʱ��ѯ���
  std::string tempQuery = "";
  // �Ƿ���Debugģʽ
  bool DebugMode = false;
  // ��ǰ״̬
  RunType runType = RunType::RUN_CONSOLE;
  // ���е�·��
  std::string runPath = "";
  // ������
  DBConnectionPool* connector = nullptr;
  // Ψһʵ��
  static DBController* Instance;
  // ��ֹ��������
  DISALLOW_COPY_AND_ASSIGN(DBController);
};

CSTORE_NS_END

#endif // __CSTORE_DBCONTROLLER