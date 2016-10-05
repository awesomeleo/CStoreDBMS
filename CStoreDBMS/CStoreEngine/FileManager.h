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
  // inCounter ��ȡ������
  //�� �� ֵ�� �����ɹ����
  bool LoadTableBatch(int& times, FILE* fptr, std::string tname, std::vector<int>& colindice, std::vector<std::string>& bufList, std::vector<std::string>& typeList, int totalCol, int& inCounter);

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






  /* OLD FUNCTIONS */
  //bool getMiniOrders(int &times, int* custkeyBuffer, int* orderkeyBuffer, double* totalpriceBuffer, int* shippriorityBuffer);
  //bool getMiniCustomers(int &times, int* custkeyBuffer);
  //int* getOrdersBuffer(int _times, int &_maxcount);
  //void importOrders();
  //void importCustomers();
  void getEXOrdersBuffer(int _times, int &_maxcount);
  void getEXCustkeyBuffer(int _times, int &_maxcount);
  void externSort();
  void innerSort(int* _orgBuffer, int* _syncBuffer, int _maxcount);
  void writeCustkeyOutputBufferToFile(int* _orgBuffer, int _incounter);
  void writeOrdersOutputBufferToFile(int* _orgBuffer, int _incounter);
  void compressCustkey();
  void join();
  void writeJoinedItemToFile(int* _orderBuffer, int* _custBuffer, int _incounter);
  void writeCompressedCustkeyToFile(int* _orgBuffer, int _incounter);
  void quickSort(int _org[], int _sync[], int low, int high);
  void swap(int& a, int& b);
  int partition(int* _org, int *_sync, int _low, int _high);
  int count();
  
  bool insertIntoOrders(int orderkeyBuffer, int custkeyBuffer, double totalpriceBuffer, int shippriorityBuffer);
  bool select_orders_orderkey(int _key);
  int getCustKeyRunLength(int _key);
  int* getCompressedCustkeyBuffer(int _times, int &_maxcount);
  int* getOrderkeyBuffer(int _times, int &_maxcount);
  double* getTotalpriceBuffer(int _times, int &_maxcount);
  int* getShippriorityBuffer(int _times, int &_maxcount);
  void writeTotalpriceOutputBufferToFile(double* _orgBuffer, int _incounter);
  void writeShippriorityOutputBufferToFile(int* _orgBuffer, int _incounter);

private:
  long int pos;
  long int tpos;
  int page[1024];
  
  //int Buffer_custkey[1024];
  //int Buffer_orderkey[1024];
  //double Buffer_totalprice[1024];
  //int Buffer_shippriority[1024];

  DBAllocator* allocator;

  static FileManager* __instance;
  FileManager();
};

CSTORE_NS_END
#endif /* ___CSTORE_FILEMANAGER */