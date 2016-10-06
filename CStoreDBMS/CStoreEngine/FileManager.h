#ifndef ___CSTORE_FILEMANAGER
#define ___CSTORE_FILEMANAGER
#include "DBBase.h"
#include "DBTable.hpp"
#include "DBAllocator.h"
CSTORE_NS_BEGIN

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#define SIZE_LINE 512
#define SIZE_PAGE 1024

class FileManager {
public:
  //�������ã� ����������������Ψһʵ��
  //�����б� N/A
  //�� �� ֵ�� �ļ���������Ψһʵ��
  static FileManager* GetInstance();

  //�������ã� �����������ļ�
  //�����б�
  //     times ��ǰ����
  //      fptr ��Դ�ļ�ָ��
  // colindice ���±�
  //   bufList ����Ļ�����������
  //  typeList ��������
  //  totalCol ������
  //    offset ��ͷƫ����
  // inCounter ��ȡ������
  //�� �� ֵ�� �����ɹ����
  bool LoadTableBatch(int& times, FILE* fptr, std::string tname, std::vector<int>& colindice, std::vector<std::string>& bufList, std::vector<std::string>& typeList, int totalCol, int offset, int& inCounter);

  //�������ã� ������
  //�����б�
  //       tab �����
  //�� �� ֵ�� �����ɹ����
  bool LoadTable(DBTable& tab);
  
  //�������ã� �����������ļ�
  //�����б�
  //       tab �����
  //   colName Ҫ���������
  //   colType ������
  //     times ��ǰ����
  // inCounter ��ȡ������
  //�� �� ֵ�� �����ɹ����
  bool LoadColumnBatch(DBTable& tab, std::string& colName, std::string& colType, int times, int& inCounter, void* buffer);

  //�������ã� ͨ�������������
  //�����б�
  //       tab �����
  //       key ������ֵ
  //    record ��¼��Ӧ���ַ���
  //�� �� ֵ�� �����ɹ����
  bool Retrieve(DBTable& tab, int key, std::string& record);

  //�������ã� ͨ��ƫ����ȡ����Ӧλ�õ�ֵ���ַ���
  //�����б�
  //       tab �����
  //   colName ����
  //   colType ������
  //      page ҳ��
  //    offset ҳƫ��
  //    outStr �ȼ۵��ַ���
  //�� �� ֵ�� �����ɹ����
  bool RetrieveValueByOffset(DBTable& tab, std::string colName, std::string colType, int page, int offset, std::string &outStr);

  //�������ã� ��ȡ���¼����
  //�����б�
  //       tab �����
  //      ccol �����е�����
  //�� �� ֵ�� ��¼����
  int Count(DBTable&, std::string ccol);

  //�������ã� ��Ȼ���ӱ�
  //�����б�
  //      tab1 �����
  //      tab2 �����
  //�� �� ֵ�� N/A
  void Join();

  //�������ã� ������
  //�����б�
  //       tab �����
  //       col �������
  //      sync ͬ���仯��
  //�� �� ֵ�� N/A
  void ExternSort(DBTable& tab, std::string col, std::string cType, std::string sync, std::string sType);

  //�������ã� ��������д���ļ�
  //�����б�
  // orgBuffer ������ָ��
  // incounter ����������
  //     fname �ļ���
  //�� �� ֵ�� N/A
  void WriteBufferToFile(int* _orgBuffer, int _incounter, std::string fname);

  //�������ã� ѹ����
  //�����б�
  //       tab �����
  //       col ѹ������
  //     cType ������
  //�� �� ֵ�� N/A
  void Compress(DBTable& tab, std::string col, std::string cType);


private:
  //�������ã� ˽�еĹ�����
  //�����б� N/A
  //�� �� ֵ�� N/A
  FileManager();

  //�������ã� ��������
  //�����б�
  //      _org ԭ��
  //     _sync ���洮
  //       low ��λ
  //      high ��λ
  //�� �� ֵ�� N/A
  void quickSort(int _org[], int _sync[], int low, int high);

  //�������ã� ��������������
  //�����б�
  //         a ��������
  //         b ��������
  //�� �� ֵ�� N/A
  void swap(int& a, int& b);

  //�������ã� ����ȡ�зָ����
  //�����б�
  //      _org ԭ��
  //     _sync ���洮
  //      _low ��λ
  //     _high ��λ
  //�� �� ֵ�� ��Ԫ�±�
  int partition(int* _org, int *_sync, int _low, int _high);

  //�������ã�  ������
  //�����б�
  // _orgBuffer ԭ��
  //_syncBuffer ���洮
  //  _maxcount ������
  //�� �� ֵ��  N/A
  void innerSort(int* _orgBuffer, int* _syncBuffer, int _maxcount);

  // �ڴ������
  DBAllocator* allocator;
  // Ψһʵ��
  static FileManager* Instance;

};

CSTORE_NS_END
#endif /* ___CSTORE_FILEMANAGER */