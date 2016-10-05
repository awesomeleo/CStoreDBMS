#ifndef ___CSTORE_CSDATABASE
#define ___CSTORE_CSDATABASE
#include "DBCBase.h"
#include "FileManager.h"
#include "TableManager.h"

CSTORE_NS_BEGIN

class CSDatabase {
public:
  //�������ã� ���캯��
  //�����б� N/A
  //�� �� ֵ�� N/A
  CSDatabase();

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

  //�������ã� �����﷨������ʽֵ
  //�����б�
  //    mynode �ݹ�ڵ�
  //    myexec ���ݿ�ָ��
  //   myproxy ������ָ��
  //�� �� ֵ�� bool ���ʽ��ֵ
  bool static AST(SyntaxTreeNode*, CSDatabase*, DBCProxy*);

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
  //    pitype ����������
  //�� �� ֵ�� �����ɹ����
  bool Create(istr, StrVec&, StrVec&);

  //�������ã� ɾ��һ�����е�ָ����
  //�����б�
  //      name ����
  //   condVec ������
  //      cond �����Ӿ�����ָ��
  //    iproxy ����ָ��
  //�� �� ֵ�� �����ɹ����
  bool Delete(istr, StrVec&, SyntaxTreeNode* = NULL, DBCProxy* = NULL);

  //�������ã� ��һ��������ѯ
  //�����б�
  //      name ����
  //    pilist ��ֵ������
  //   pivalue ��ֵֵ����
  //  errorbit �������λ
  //�� �� ֵ�� �����ɹ����
  bool Insert(istr, StrVec&, IntVec&, bool&);

  //�������ã� ��һ��������ѯ
  //�����б�
  //      name ����
  //        pi ��ѯ������
  //      star �Ƿ�ͨ��������
  //      cond �����Ӿ�����ָ��
  //    iproxy ����ָ��
  //�� �� ֵ�� �����ɹ����
  bool Select(istr, StrVec&, bool, StrVec&, SyntaxTreeNode* = NULL, DBCProxy* = NULL);

  //�������ã� �������������
  //�����б�
  //     tname ����
  //�� �� ֵ�� �����ɹ����
  bool Load(istr);

  //�������ã� ͨ��������ȡ��¼
  //�����б�
  //     tname ����
  //      tkey ������ֵ
  //�� �� ֵ�� �����ɹ����
  bool Retrieve(istr, int);

  //�������ã� ѹ����
  //�����б�
  //     tname ����
  //        pi Ҫѹ��������
  //�� �� ֵ�� �����ɹ����
  bool Compress(istr, istr);

  //�������ã� ��Ȼ���ӱ�
  //�����б�
  //    t1name ����
  //    t2name ����
  //�� �� ֵ�� �����ɹ����
  bool Join(istr, istr);

  //�������ã� �����¼��Ŀ
  //�����б�
  //     tname ����
  //�� �� ֵ�� �����ɹ����
  bool Count(istr);

  // ������
  std::mutex dbMutex;
  // �ļ�������
  FileManager* fileMana;
  // �������
  TableManager* tableMana;
  // �����Ӿ��ֵ�
  TablePileDictionary _param;
}; /* CSDatabase */

CSTORE_NS_END

#endif /* ___CSTORE_CSDATABASE */