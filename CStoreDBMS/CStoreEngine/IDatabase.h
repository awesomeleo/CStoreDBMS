#ifndef ___CSTORE_IDATABASE
#define ___CSTORE_IDATABASE
#include "DBCBase.h"

class IDatabase {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  IDatabase();

  //�������ã� �г����еı�
  //�����б� N/A
  //�� �� ֵ�� N/A
  void ShowTable();

  //�������ã� ��λ���ݿ�
  //�����б� N/A
  //�� �� ֵ�� N/A
  void Reset();

  //�������ã� ���ر������
  //�����б� N/A
  //�� �� ֵ�� int ���ݿ��б������
  int Size();

  //�������ã� ����ִ��һ��SSQL���
  //�����б�
  //     proxy ����ִ�е�����
  //�� �� ֵ�� bool �Ƿ�ɹ�����
  bool Interpreter(DBCProxy&);

  //�������ã� ��ѯһ��ִ���е��ֵ�ֵ
  //�����б�
  //  paraname �е�����
  //�� �� ֵ�� int �����������е�λ��
  int Reference(istr);

  //�������ã� ��������﷨������ʽֵ
  //�����б�
  //    mynode �ݹ�ڵ�
  //    myexec ���ݿ�ָ��
  //   myproxy ������ָ��
  //�� �� ֵ�� bool ���ʽ��ֵ
  bool static AST(SyntaxTreeNode*, IDatabase*, DBCProxy*);

private:
  //�������ã� �쳣������
  //�����б�
  //     _info �쳣��Ϣ
  //    _index �쳣���ӱ��
  //�� �� ֵ�� bool ��Ϊfalse����Ϊִ��ʧ��
  bool iException(istr, int = -1);

  //�������ã� ��ѯһ�����Ƿ����
  //�����б�
  //      name �������
  //�� �� ֵ�� bool ��Ĵ�����
  bool Exist(istr);

  //�������ã� ����һ���±�
  //�����б�
  //      name ����
  //        pi ������
  //       ppi ��������
  //       def ��ʼֵ����
  //    dpflag �ظ������������
  //  errorbit �������λ
  //�� �� ֵ�� bool �����ɹ����
  bool Create(istr, StrVec&, StrVec&, TablePileDictionary&, int&, bool&);

  //�������ã� ɾ��һ�����е�ָ����
  //�����б�
  //      name ����
  //      cond �����Ӿ�����ָ��
  //    iproxy ����ָ��
  //�� �� ֵ�� bool �����ɹ����
  bool Delete(istr, StrVec&, SyntaxTreeNode* = NULL, DBCProxy* = NULL);

  //�������ã� ��һ��������ѯ
  //�����б�
  //      name ����
  //    pilist ��ֵ������
  //   pivalue ��ֵֵ����
  //  errorbit �������λ
  //�� �� ֵ�� bool �����ɹ����
  bool Insert(istr, StrVec&, IntVec&, bool&);

  //�������ã� ��һ��������ѯ
  //�����б�
  //      name ����
  //        pi ��ѯ������
  //      star �Ƿ�ͨ��������
  //      cond �����Ӿ�����ָ��
  //    iproxy ����ָ��
  //�� �� ֵ�� bool �����ɹ����
  bool Select(istr, StrVec&, bool, StrVec&, SyntaxTreeNode* = NULL, DBCProxy* = NULL);

  //�������ã� ��ѯ������
  //�����б�
  //      name �������
  //�� �� ֵ�� int �������ݿ�������е����
  int TableNo(istr);

  // �����Ӿ��ֵ�
  TablePileDictionary _param;
}; /* IDatabase */

#endif /* ___CSTORE_IDATABASE */