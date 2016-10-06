#include "FileManager.h"
#include "TableManager.h"
#include <limits.h>
#include <string.h>
#include <string>
#include <stdio.h>

CSTORE_NS_BEGIN
#define COMPRESS_SIZE (64 * SIZE_PAGE)
#define EXSORT_SIZE (64 * SIZE_PAGE)
#define COLUMNBATCHSIZE (64 * SIZE_PAGE)
using namespace std;

//int externSortOrdersBufferPool[64 * SIZE_PAGE];
//int externSortCustkeyBufferPool[64 * SIZE_PAGE];
int compressedCustkeyBufferPool[64 * SIZE_PAGE];

int pagecounter = 0;
int importcountercustomer = 0;

int page_ptr = 0;
int count_spilt = 0;
int maxcount = 0;
char page_buf[SIZE_PAGE];

//�������ã� �����������ļ�
//�����б�
//     times ��ǰ����
//    piList Ҫ�������������
//   bufList ����Ļ�����������
//�� �� ֵ�� N/A
bool FileManager::LoadTableBatch(int& times, FILE* fptr, std::string tname, std::vector<int>& colindice,
  std::vector<std::string>& bufList, std::vector<std::string>& typeList, int totalCol, int offset, int& inCounter) {
  int count_reco = 0;
  int content_ptr = 0;
  int transId = CStore::TableManager::GetInstance()->GetTableLock(tname)->LockTransaction->GetId();
  std::string bufname = CSCommonUtil::StringBuilder("content_buf_" + tname + "#").Append(transId).ToString();
  char* content_buf = (char*)this->allocator->Alloc(bufname, 128 * sizeof(char));
  while (count_reco < SIZE_PAGE) {
    // page_buf������
    if (page_ptr == maxcount) {
      std::fseek(fptr, (times++ * SIZE_PAGE + offset) * sizeof(char), SEEK_SET);
      maxcount = std::fread(page_buf, sizeof(char), SIZE_PAGE, fptr);
      page_ptr = 0;
    }
    // �Ѿ�����Ϊpage_buf�����κ�����
    if (maxcount == 0) {
      break;
    }
    // �����ַ���ת����������Ӧ������
    if (page_buf[page_ptr] != '|' && page_buf[page_ptr] != '\n') {
      content_buf[content_ptr++] = page_buf[page_ptr];
    }
    else {
      count_spilt++;
      content_buf[content_ptr] = '\0';
      if (count_spilt == totalCol) {
        count_reco++;
        count_spilt = -1;
      }
      else {
        for (int ci = 0; ci < colindice.size(); ci++) {
          if (count_spilt == colindice[ci]) {
            if (typeList[ci] == "DOUBLE") {
              ((double*)this->allocator->Get(bufList[ci]))[count_reco] = std::atof(content_buf);
              break;
            }
            else {
              ((int*)this->allocator->Get(bufList[ci]))[count_reco] = std::atoi(content_buf);
              break;
            }
          }
        }
      }
      content_ptr = 0;
    }
    page_ptr++;
  }
  // �������ļ�¼��Ŀ
  inCounter = count_reco;
  this->allocator->Free(bufname);
  return maxcount == 0;
}

//�������ã� ������
//�����б�
//       tab �����
//�� �� ֵ�� �����ɹ����
bool FileManager::LoadTable(DBTable& tab) {
  // �򿪶����ļ�
  std::string filePath = tab.TableName + ".tbl";
  FILE* fin = fopen(filePath.c_str(), "rb");
  if (fin == NULL) {
    fin = fopen(("../" + filePath).c_str(), "rb");
  }
  if (fin == NULL) {
    TRACE("Exception: cannot load table from file: " + tab.TableName);
    return false;
  }
  // ����ͷ������λ��
  char Header[SIZE_PAGE];
  fgets(Header, SIZE_PAGE, fin);
  std::string strHeader(Header);
  std::vector<std::string> hItems = CSCommonUtil::CStrSplit(strHeader, "|");
  std::vector<int> colIndices;
  for (int j = 0; j < tab.PiList.size(); j++) {
    bool existFlag = false;
    for (int i = 0; i < hItems.size(); i++) {
      if (tab.PiList[j] == hItems[i]) {
        colIndices.push_back(i + 1);
        existFlag = true;
        break;
      }
    }
    if (existFlag == false) {
      TRACE("Column not exist in file: " + tab.PiList[j]);
      return false;
    }
  }
  // �����д洢�ļ�������������ļ�
  for (int i = 0; i < tab.PiList.size(); i++) {
    tab.PiFileNameList[tab.PiList[i]] = tab.TableName + "_" + tab.PiList[i] + ".db";
    FILE* fdel = std::fopen(tab.PiFileNameList[tab.PiList[i]].c_str(), "w");
    std::fclose(fdel);
  }
  // Ϊ���ǿ����ļ����
  FILE** fColPtrArr = new FILE*[tab.PiList.size()];
  for (int i = 0; i < tab.PiList.size(); i++) {
    fColPtrArr[i] = std::fopen(tab.PiFileNameList[tab.PiList[i]].c_str(), "ab");
  }
  // ���仺����
  int transId = CStore::TableManager::GetInstance()->GetTableLock(tab.TableName)->LockTransaction->GetId();
  std::vector<std::string> bufferNameVec;
  for (int i = 0; i < tab.PiList.size(); i++) {
    std::string bufName = CSCommonUtil::StringBuilder(tab.TableName + "@" + tab.PiList[i] + "#").Append(transId).ToString();
    bufferNameVec.push_back(bufName);
    if (tab.PiTypeList[i] == "DOUBLE") {
      this->allocator->Alloc(bufName, SIZE_PAGE * sizeof(double));
    }
    else {
      this->allocator->Alloc(bufName, SIZE_PAGE * sizeof(int));
    }
  }
  // ��ҳ����
  bool finflag = false;
  int pCounter = 0;
  int inCounter = 0;
  do {
    // ���ϵض����ļ���������
    finflag = this->LoadTableBatch(pCounter, fin, tab.TableName, colIndices, bufferNameVec, tab.PiTypeList, hItems.size(), strHeader.size(), inCounter);

    // ������������д���ļ�
    for (int i = 0; i < tab.PiFileNameList.size(); i++) {
      if (tab.PiTypeList[i] == "DOUBLE") {
        std::fwrite(this->allocator->Get(bufferNameVec[i]), sizeof(double), inCounter, fColPtrArr[i]);
      } else {
        std::fwrite(this->allocator->Get(bufferNameVec[i]), sizeof(int), inCounter, fColPtrArr[i]);
      }
    }
  } while (finflag != true);
  // �����ļ����ͷ���Դ
  for (int i = 0; i < tab.PiList.size(); i++) {
    std::fclose(fColPtrArr[i]);
  }
  if (fColPtrArr != NULL) {
    delete fColPtrArr;
    fColPtrArr = NULL;
  }
  std::fclose(fin);
  for (int i = 0; i < bufferNameVec.size(); i++) {
    this->allocator->Free(bufferNameVec[i]);
  }
  return true;
}

//�������ã� ͨ�������������
//�����б�
//       tab �����
//       key ������ֵ
//    record ��¼��Ӧ���ַ���
//�� �� ֵ�� �����ɹ����
bool FileManager::Retrieve(DBTable& tab, int key, std::string& record) {
  // ����������
  bool _successFlag = false;
  bool _failFlag = false;
  int transId = CStore::TableManager::GetInstance()->GetTableLock(tab.TableName)->LockTransaction->GetId();
  std::string buffername = CSCommonUtil::StringBuilder("retrieve_buffer_" + tab.TableName + "#").Append(transId).ToString();
  int* bufferPtr = (int*)this->allocator->Alloc(buffername, COLUMNBATCHSIZE * sizeof(int));
  int pcounter = 0;
  int sindex = 0;
  int _pc = 0; // ҳ��
  int _si = 0; // ҳƫ��
  int maxcount = COLUMNBATCHSIZE;
  // ��ҳ����
  do {
    this->LoadColumnBatch(tab, tab.PiList[0], tab.PiTypeList[0], pcounter++, maxcount, bufferPtr);
    if (maxcount == 0) {
      _failFlag = true;
      break;
    }
    if (bufferPtr[maxcount - 1] < key) {
      continue;
    }
    if (bufferPtr[0] > key) {
      _failFlag = true;
      break;
    }
    // ���ֲ���
    int mid = 0, low = 0, high = maxcount - 1;
    while (low <= high) {
      mid = (low + high) / 2;
      if (bufferPtr[mid] == key) {
        sindex = mid;
        _successFlag = true;
        break;
      }
      else if (bufferPtr[mid] > key) {
        high = mid - 1;
      }
      else if (bufferPtr[mid] < key) {
        low = mid + 1;
      }
    }

  } while (_successFlag != true);
  // �ͷ���Դ
  this->allocator->Free(buffername);
  // ����������
  if (_failFlag == true) {
    record = "";
    return false;
  }
  else {
    _pc = pcounter - 1;
    _si = sindex;
  }
  // �ټ���������
  CSCommonUtil::StringBuilder recordSb;
  recordSb.Append(key).Append("|");
  for (int i = 1; i < tab.PiList.size(); i++) {
    std::string outstr = "";
    this->RetrieveValueByOffset(tab, tab.PiList[i], tab.PiTypeList[i], _pc, _si, outstr);
    recordSb.Append(outstr).Append("|");
  }
  // ������
  record = recordSb.ToString();
  return true;
}

//�������ã� �����������ļ�
//�����б�
//       tab �����
//   colName Ҫ���������
//   colType ������
//     times ��ǰ����
// inCounter ��ȡ������
//�� �� ֵ�� �����ɹ����
bool FileManager::LoadColumnBatch(DBTable& tab, std::string& colName, std::string& colType, int times, int& inCounter, void* buffer) {
  FILE* fin = std::fopen((tab.PiFileNameList[colName]).c_str(), "rb");
  if (fin == NULL) {
    TRACE("Exception: cannot load column file: " + tab.PiFileNameList[colName]);
    return false;
  }
  std::fseek(fin, (long)(times * COLUMNBATCHSIZE * sizeof(int)), SEEK_SET);
  if (fin == 0) {
    return false;
  }
  if (colType == "DOUBLE") {
    inCounter = std::fread(buffer, sizeof(double), COLUMNBATCHSIZE, fin);
  }
  else {
    inCounter = std::fread(buffer, sizeof(int), COLUMNBATCHSIZE, fin);
  }
  std::fclose(fin);
  return true;
}

//�������ã� ͨ��ƫ����ȡ����Ӧλ�õ�ֵ���ַ���
//�����б�
//       tab �����
//   colName ����
//      page ҳ��
//    offset ҳƫ��
//    outStr �ȼ۵��ַ���
//�� �� ֵ�� �����ɹ����
bool FileManager::RetrieveValueByOffset(DBTable& tab, std::string colName, std::string colType, int page, int offset, std::string &outStr) {
  FILE* fin = std::fopen((tab.PiFileNameList[colName]).c_str(), "rb");
  if (fin == NULL) {
    TRACE("cannot load column file: " + tab.PiFileNameList[colName]);
    return false;
  }
  if (colType == "DOUBLE") {
    double tdouble = 0;
    std::fseek(fin, (long)((page * COLUMNBATCHSIZE + offset) * sizeof(double)), SEEK_SET);
    std::fread(&tdouble, sizeof(double), 1, fin);
    outStr = CSCommonUtil::StringBuilder().Append(tdouble).ToString();
    return true;
  }
  else {
    int tint = 0;
    std::fseek(fin, (long)((page * COLUMNBATCHSIZE + offset) * sizeof(int)), SEEK_SET);
    std::fread(&tint, sizeof(int), 1, fin);
    outStr = CSCommonUtil::StringBuilder().Append(tint).ToString();
    return true;
  }
  return false;
}

//�������ã� ������
//�����б�
//       tab �����
//       col �������
//�� �� ֵ�� N/A
void FileManager::ExternSort(DBTable& tab, std::string col, std::string cType, std::string sync, std::string sType) {
  int maxcount = 1024;
  int pc = 0;
  // ���仺����
  int tranId = CStore::TableManager::GetInstance()->GetTableLock(tab.TableName)->LockTransaction->GetId();
  std::string sortkeyBufferName = CSCommonUtil::StringBuilder(tab.TableName + "@externSort" + col + "BufferPool#").Append(tranId).ToString();
  int* externSortBufferPool = (int*)this->allocator->Alloc(sortkeyBufferName, COMPRESS_SIZE * sizeof(int));
  std::string synckeyBufferName = CSCommonUtil::StringBuilder(tab.TableName + "@externSort" + sync + "BufferPool#").Append(tranId).ToString();
  int* externSortSyncBufferPool = (int*)this->allocator->Alloc(synckeyBufferName, COMPRESS_SIZE * sizeof(int));
  // �γ�����
  while (maxcount > 0) {
    // ÿ�ζ���128ҳ
    this->LoadColumnBatch(tab, col, cType, pc, maxcount, externSortBufferPool);
    this->LoadColumnBatch(tab, sync, sType, pc, maxcount, externSortSyncBufferPool);
    if (maxcount == 0) break;
    // Ȼ�����custkey����������
    innerSort(externSortBufferPool, externSortSyncBufferPool, maxcount);
    // ��������Ϻ�д��ʱ�ļ�
    char strBuffer[10];
    std::string outFileName = "temp_exSortTempSortkey";
    sprintf(strBuffer, "%d", pagecounter);
    outFileName += strBuffer;
    FILE* fout = fopen(outFileName.c_str(), "wb");
    fwrite(externSortBufferPool, sizeof(int), maxcount, fout);
    fclose(fout);
    outFileName = "temp_exSortTempSynckey";
    outFileName += strBuffer;
    fout = fopen(outFileName.c_str(), "wb");
    fwrite(externSortSyncBufferPool, sizeof(int), maxcount, fout);
    fclose(fout);
    pagecounter++;
    pc++;
  }
  // ��·�鲢
  int min = INT_MAX - 1;
  int minindex = -1;
  int outputBuffer[SIZE_PAGE];
  int syncBuffer[SIZE_PAGE];
  int outputPointer = 0;
  int mergeBufferCapacity = (int)((126.0f * SIZE_PAGE / (double)(2.0 * pagecounter)));
  int freadflag = 0;
  string inFileName;
  int* mergePointer = new int[pagecounter];
  int* mergeTimes = new int[pagecounter];
  for (int i = 0; i < pagecounter; i++) {
    mergePointer[i] = mergeBufferCapacity;
    mergeTimes[i] = 0;
  }
  int** mergeBuffer = new int*[2 * pagecounter];
  for (int i = 0; i < 2 * pagecounter; i++) {
    mergeBuffer[i] = new int[mergeBufferCapacity];
  }
  while (true) {
    // �ȿ����Ƿ���Ҫ���뻺��������Ϊ��ͬ���ģ�����2��pagecounter��벿�ֲ���Ҫȥ������
    for (int i = 0; i < pagecounter; i++) {
      // �����������������ˣ���ô��Ҫ�������벢��λָ��
      if (mergePointer[i] == mergeBufferCapacity) {
        std::memset(mergeBuffer[i], 0, mergeBufferCapacity * sizeof(int));
        // �����ļ���
        char fstrBuffer[10];
        inFileName = "temp_exSortTempSortkey";
        std::sprintf(fstrBuffer, "%d", i);
        inFileName += fstrBuffer;
        // �������ļ�ʣ�ಿ�ַ��뻺����
        FILE* fin = std::fopen(inFileName.c_str(), "rb");
        std::fseek(fin, (long)(mergeTimes[i] * mergeBufferCapacity * sizeof(int)), SEEK_SET);
        freadflag = std::fread(mergeBuffer[i], sizeof(int), mergeBufferCapacity, fin);
        std::fclose(fin);
        // �ٴι����ļ���������Ƕ�sync���е�
        inFileName = "temp_exSortTempSynckey";
        std::sprintf(fstrBuffer, "%d", i);
        inFileName += fstrBuffer;
        // �ٴζ��ļ����Ż�����
        fin = std::fopen(inFileName.c_str(), "rb");
        std::fseek(fin, (long)(mergeTimes[i] * mergeBufferCapacity * sizeof(int)), SEEK_SET);
        freadflag = std::fread(mergeBuffer[pagecounter + i], sizeof(int), mergeBufferCapacity, fin);
        std::fclose(fin);
        // �ع�ɨ��ָ�룬����ҳ���¼ָ��
        mergePointer[i] = 0;
        mergeTimes[i]++;
      }
    }
    // ����custkey���У�����һ��pagecounter֮���ҳ���ǲ���Ҫ������
    for (int i = 0; i < pagecounter; i++) {
      // ����������ݣ���Ƚ�����
      if (mergeBuffer[i][mergePointer[i]] < min && mergeBuffer[i][mergePointer[i]] != 0) {
        // ��custkey���ԣ�����Ҫȥ�Ƚ�ÿ���α��λ�ã���ѡ��С�����뻺����
        min = mergeBuffer[i][mergePointer[i]];
        minindex = i;
      }
    }
    // ������һ������˵��ǻ��������ж��������
    if (minindex == -1) {
      if (outputPointer > 0) {
        this->WriteBufferToFile(outputBuffer, outputPointer, tab.TableName + "_" + col + "_sorted.db");
        this->WriteBufferToFile(syncBuffer, outputPointer, tab.TableName + "_" + sync + "_sorted.db");
        outputPointer = 0;
      }
      break;
    }
    // ÿһ����ɺ�������С��д�뻺��������ʱ�����pagecounter����order��ҲҪͬ��д��
    outputBuffer[outputPointer] = min;
    // ��orderkey���ԣ���ֻ��Ҫ����custkey���α���оͺ���
    syncBuffer[outputPointer++] = mergeBuffer[pagecounter + minindex][mergePointer[minindex]++];
    // ���������д���ˣ���ô������ļ��ﲢ��ջ�����
    if (outputPointer == SIZE_PAGE || (freadflag == 0 && outputPointer > 0)) {
      this->WriteBufferToFile(outputBuffer, outputPointer, tab.TableName + "_" + col + "_sorted.db");
      this->WriteBufferToFile(syncBuffer, outputPointer, tab.TableName + "_" + sync + "_sorted.db");
      outputPointer = 0;
    }
    // ����Ѿ����׶����˾ͳ���������������ʱ����
    if (freadflag == 0) {
      break;
    }
    else {
      min = INT_MAX - 1;
      minindex = -1;
    }
  }
  // �ͷ���Դ
  this->allocator->Free(sortkeyBufferName);
  this->allocator->Free(synckeyBufferName);
  // ѹ��
  this->Compress(tab, col, cType);
}

//�������ã� ѹ����
//�����б�
//       tab �����
//       col ѹ������
//     cType ������
//�� �� ֵ�� N/A
void FileManager::Compress(DBTable& tab, std::string col, std::string cType) {
  int compressBuffer[COMPRESS_SIZE];
  int outputBuffer[COMPRESS_SIZE];
  int frontPointer = 0;
  int rearPointer = 0;
  int currentValue = 0;
  int outputPointer = 0;
  int maxcount = 0;
  int ftimes = 0;
  int lastValue = 0;
  int lastLength = 0;
  bool firstRound = true;
  bool finishFlag = false;
  FILE* fin = std::fopen((tab.TableName + "_" + col + "_sorted.db").c_str(), "rb");
  while (true) {
    std::memset(compressBuffer, 0, sizeof(int)* COMPRESS_SIZE);
    if (cType == "DOUBLE") {
      maxcount = std::fread(compressBuffer, sizeof(double), COMPRESS_SIZE, fin);
    }
    else {
      maxcount = std::fread(compressBuffer, sizeof(int), COMPRESS_SIZE, fin);
    }
    firstRound = true;
    if (maxcount == 0 && outputPointer == 0) {
      break;
    }
    if (maxcount > 0) {
      currentValue = compressBuffer[0];
      // ���lastValue��currentValue�Ѿ���һһ���ˣ���ôֱ��׷��
      if (currentValue != lastValue && lastValue != 0) {
        outputBuffer[outputPointer++] = lastValue;
        outputBuffer[outputPointer++] = lastLength;
        lastLength = 0;
        lastValue = 0;
        firstRound = false;
        if (outputPointer == COMPRESS_SIZE) {
          this->WriteBufferToFile(outputBuffer, COMPRESS_SIZE, tab.TableName + "_" + col + "_compressed.db");
          outputPointer = 0;
        }
      }
    }
    while (1) {
      // ���ֵ�ı��ˣ���ô���¶�λָ��λ��
      if (currentValue != compressBuffer[rearPointer]) {
        outputBuffer[outputPointer++] = currentValue;
        // ������ǵ�һ��ɨ�������������Ҫ���ǽ���һ��ĩβ��ֵ׷�ӵ��г̳�����
        if (firstRound == true) {
          outputBuffer[outputPointer++] = rearPointer - frontPointer + lastLength;
          lastLength = 0;
          lastValue = 0;
          firstRound = false;
        }
        else
        {
          outputBuffer[outputPointer++] = rearPointer - frontPointer;
        }
        // �ƶ�������ָ��
        currentValue = compressBuffer[rearPointer];
        frontPointer = rearPointer;
        rearPointer = rearPointer;
      }
      // ���ָ�뵽����ĩβ��������ĩβ����������Ϣ��¼����Ȼ����ջ�����
      else if (rearPointer == COMPRESS_SIZE - 1) {
        lastValue = currentValue;
        lastLength = rearPointer - frontPointer + 1;
        frontPointer = 0;
        rearPointer = 0;
        break;
      }
      // ��������Ѿ�û�������˾�Ԥ������
      else if (compressBuffer[rearPointer] == 0) {
        finishFlag = true;
        break;
      }
      // �����ƶ�ָ��
      else {
        rearPointer++;
      }
      // ������������ˣ���ô�ѻ�����д���ļ�
      if (outputPointer == COMPRESS_SIZE) {
        this->WriteBufferToFile(outputBuffer, COMPRESS_SIZE, tab.TableName + "_" + col + "_compressed.db");
        outputPointer = 0;
      }
      // ����������һ�ֵ��ǻ��������������Ǿ�д��ȥ
      else if (maxcount == 0 && outputPointer > 0) {
        this->WriteBufferToFile(outputBuffer, outputPointer, tab.TableName + "_" + col + "_compressed.db");
        outputPointer = 0;
        finishFlag = true;
      }
      // ����Ѿ�û���ݾͳ���
      else if (maxcount == 0) {
        finishFlag = true;
        break;
      }
    }
    // �����ܷ�ȫ���˳�
    if (finishFlag == true) {
      if (outputPointer > 0) {
        this->WriteBufferToFile(outputBuffer, outputPointer, tab.TableName + "_" + col + "_compressed.db");
        outputPointer = 0;
      }
      break;
    }
    // �ض�λ�ļ�ָ��
    if (cType == "DOUBLE") {
      std::fseek(fin, (long)(COMPRESS_SIZE * (++ftimes) * sizeof(double)), SEEK_SET);
    }
    else {
      std::fseek(fin, (long)(COMPRESS_SIZE * (++ftimes) * sizeof(int)), SEEK_SET);
    }
  }
  // �޸ı��״̬
  tab.CompressedPiFileNameList[col] = tab.TableName + "_" + col + "_compressed.db";
  tab.IsSorted = true;
}

//�������ã� ��Ȼ���ӱ�
//�����б�
//      tab1 �����
//      tab2 �����
//�� �� ֵ�� N/A
void FileManager::Join() {
  // ������
  int joinCustkeyBuffer[32 * SIZE_PAGE];
  int joinCustomerBuffer[32 * SIZE_PAGE];
  int joinOutputOrderkeyBuffer[32 * SIZE_PAGE];
  int joinOutputCustkeyBuffer[32 * SIZE_PAGE];
  // ������ָ��
  int joinCustkeyBufferPointer = 0;
  int joinCustomerBufferPointer = 0;
  int joinOutputPointer = 0;
  // �ļ�ָ��
  FILE* joinFileOrderkey = fopen("orderkey_sorted.db", "rb");
  FILE* joinFileCustkey = fopen("custkey_compressed.db", "rb");
  FILE* joinFileCustomer = fopen("customer_custkey.db", "rb");
  // ��������
  int custkeyMaxcount = 0;
  int customerMaxcount = 0;
  int custkeyTime = 0;
  int customerTime = 0;
  long int offsetLength = 0;
  // ��ʶ����
  bool custkeyFinishFlag = false;
  bool customerFinishFlag = false;
  // �����һ������
  custkeyMaxcount = fread(joinCustkeyBuffer, sizeof(int), 32 * SIZE_PAGE, joinFileCustkey);
  customerMaxcount = fread(joinCustomerBuffer, sizeof(int), 32 * SIZE_PAGE, joinFileCustomer);
  while (true) {
    // �����ǰָ��ָ�������ǶԵȵģ���ô���������Ӧ���ݵ�orderkeyд���������
    if (joinCustkeyBuffer[joinCustkeyBufferPointer] != 0 && joinCustomerBuffer[joinCustomerBufferPointer] != 0) {
      if (joinCustkeyBuffer[joinCustkeyBufferPointer] == joinCustomerBuffer[joinCustomerBufferPointer]) {
        // ��¼�γ�
        int value = joinCustkeyBuffer[joinCustkeyBufferPointer++];
        int length = joinCustkeyBuffer[joinCustkeyBufferPointer++];
        // д�����������
        for (int _wi = 0; _wi < length; _wi++) {
          joinOutputCustkeyBuffer[joinOutputPointer] = value;
          fseek(joinFileOrderkey, (offsetLength + _wi) * sizeof(int), SEEK_SET);
          fread(joinOutputOrderkeyBuffer + joinOutputPointer, sizeof(int), 1, joinFileOrderkey);
          joinOutputPointer++;
          // ������������������ôд�ļ����建����
          if (joinOutputPointer == 32 * SIZE_PAGE) {
            this->WriteBufferToFile(joinOutputOrderkeyBuffer, joinOutputPointer, "joined_orderkey.db");
            this->WriteBufferToFile(joinOutputCustkeyBuffer, joinOutputPointer, "joined_custkey.db");
            joinOutputPointer = 0;
          }
        }
        // ���γ�
        offsetLength += length;
      }
      // �����ƶ��Ǹ�С��ָ������
      else if (joinCustkeyBuffer[joinCustkeyBufferPointer] > joinCustomerBuffer[joinCustomerBufferPointer]) {
        // ��ʱcustomer�Ƚ�С������ֻ��Ҫ�ƶ�һ��
        joinCustomerBufferPointer += 1;
      }
      else if (joinCustkeyBuffer[joinCustkeyBufferPointer] < joinCustomerBuffer[joinCustomerBufferPointer]) {
        // ���γ�
        offsetLength += joinCustkeyBuffer[joinCustkeyBufferPointer + 1];
        // ��ʱcustkey�Ƚ�С�����Ա����ƶ�������Խ���γ���
        joinCustkeyBufferPointer += 2;
      }
    }
    // ����Ѿ���ָ�뵽����0��λ�ã���ôû�м������е�������
    else {
      break;
    }
    // ������ĸ�����������߽磬��ô��Ҫ����
    if (joinOutputPointer == 32 * SIZE_PAGE) {
      this->WriteBufferToFile(joinOutputOrderkeyBuffer, joinOutputPointer, "joined_orderkey.db");
      this->WriteBufferToFile(joinOutputCustkeyBuffer, joinOutputPointer, "joined_custkey.db");
      joinOutputPointer = 0;
    }
    if (joinCustkeyBufferPointer == 32 * SIZE_PAGE) {
      memset(joinCustkeyBuffer, 0, 32 * SIZE_PAGE * sizeof(int));
      fseek(joinFileCustkey, (long)(++custkeyTime * 32 * SIZE_PAGE * sizeof(int)), SEEK_SET);
      custkeyMaxcount = fread(joinCustkeyBuffer, sizeof(int), 32 * SIZE_PAGE, joinFileCustkey);
      joinCustkeyBufferPointer = 0;
      if (custkeyMaxcount < 32 * SIZE_PAGE) {
        custkeyFinishFlag = true;
      }
    }
    if (joinCustomerBufferPointer == 32 * SIZE_PAGE) {
      memset(joinCustomerBuffer, 0, 32 * SIZE_PAGE * sizeof(int));
      fseek(joinFileCustomer, (long)(++customerTime * 32 * SIZE_PAGE * sizeof(int)), SEEK_SET);
      customerMaxcount = fread(joinCustomerBuffer, sizeof(int), 32 * SIZE_PAGE, joinFileCustomer);
      joinCustomerBufferPointer = 0;
      if (customerMaxcount < 32 * SIZE_PAGE) {
        customerFinishFlag = true;
      }
    }
    // ��������˵����������ж����Ǿ���գ������˳�
    if ((custkeyMaxcount == 0 || customerMaxcount == 0) && (joinOutputPointer > 0)) {
      this->WriteBufferToFile(joinOutputOrderkeyBuffer, joinOutputPointer, "joined_orderkey.db");
      this->WriteBufferToFile(joinOutputCustkeyBuffer, joinOutputPointer, "joined_custkey.db");
      joinOutputPointer = 0;
    }
    else if ((custkeyMaxcount == 0 || customerMaxcount == 0) && (joinOutputPointer == 0)) {
      break;
    }

  }
  // ȷ�ϰ�ȫ���˳�
  if (joinOutputPointer > 0) {
    this->WriteBufferToFile(joinOutputOrderkeyBuffer, joinOutputPointer, "joined_orderkey.db");
    this->WriteBufferToFile(joinOutputCustkeyBuffer, joinOutputPointer, "joined_custkey.db");
    joinOutputPointer = 0;
  }
  // �����ļ�
  fclose(joinFileOrderkey);
  fclose(joinFileCustkey);
  fclose(joinFileCustomer);
}

//�������ã� ��ȡ���¼����
//�����б�
//       tab �����
//      ccol �����е�����
//�� �� ֵ�� ��¼����
int FileManager::Count(DBTable& tab, std::string ccol) {
  bool compressedFlag = true;
  FILE* fin = fopen((tab.TableName + "_" + ccol + "_compressed.db").c_str(), "rb");
  if (fin == NULL) {
    fin = fopen((tab.TableName + "_" + ccol + ".db").c_str(), "rb");
    if (fin == NULL) {
      TRACE("Exception: cannot count table" + tab.TableName);
    }
    compressedFlag = false;
  }
  fseek(fin, 0L, SEEK_SET);
  int atr = 0;
  int ctr = 0;
  int buf[SIZE_PAGE];
  int it = 0;
  if (compressedFlag) {
    do {
      memset(buf, 0, sizeof(int) * SIZE_PAGE);
      ctr = fread(buf, sizeof(int), SIZE_PAGE, fin);
      for (int j = 0; j < ctr; j++) {
        if (j % 2 == 1) {
          atr += buf[j];
        }
      }
      fseek(fin, (long)(sizeof(int) * SIZE_PAGE * ++it), SEEK_SET);
    } while (ctr != 0);
  }
  else {
    do {
      memset(buf, 0, sizeof(int)* SIZE_PAGE);
      ctr = fread(buf, sizeof(int), SIZE_PAGE, fin);
      atr += ctr;
      fseek(fin, (long)(sizeof(int)* SIZE_PAGE * ++it), SEEK_SET);
    } while (ctr != 0);
  }
  return atr;
}

//�������ã� ��������д���ļ�
//�����б�
// orgBuffer ������ָ��
// incounter ����������
//     fname �ļ���
//�� �� ֵ�� N/A
void FileManager::WriteBufferToFile(int* orgBuffer, int incounter, std::string fname) {
  FILE* fout = fopen(fname.c_str(), "ab");
  if (fout == NULL) {
    TRACE("Exception: cannot write file: " + fname);
    return;
  }
  fwrite(orgBuffer, sizeof(int), incounter, fout);
  fclose(fout);
}

//�������ã�  ������
//�����б�
// _orgBuffer ԭ��
//_syncBuffer ���洮
//  _maxcount ������
//�� �� ֵ��  N/A
void FileManager::innerSort(int* _orgBuffer, int* _syncBuffer, int _maxcount) {
  quickSort(_orgBuffer, _syncBuffer, 0, _maxcount - 1);
}

//�������ã� ��������
//�����б�
//      _org ԭ��
//     _sync ���洮
//       low ��λ
//      high ��λ
//�� �� ֵ�� N/A
void FileManager::quickSort(int _org[], int _sync[], int low, int high) {
  if (low < high) {
    int _key = partition(_org, _sync, low, high);
    quickSort(_org, _sync, low, _key - 1);
    quickSort(_org, _sync, _key + 1, high);
  }
}

//�������ã� ����ȡ�зָ����
//�����б�
//      _org ԭ��
//     _sync ���洮
//      _low ��λ
//     _high ��λ
//�� �� ֵ�� ��Ԫ�±�
int FileManager::partition(int* _org, int *_sync, int _low, int _high) {
  // ������ʹ�� data[_mid] < data[_low] < data[_high]
  int _mid = (_low + _high) / 2;
  if (_org[_low] < _org[_mid]) {
    swap(_org[_low], _org[_mid]);
    swap(_sync[_low], _sync[_mid]);
  }
  if (_org[_high] < _org[_mid]) {
    swap(_org[_high], _org[_mid]);
    swap(_sync[_high], _sync[_mid]);
  }
  if (_org[_high] < _org[_low]) {
    swap(_org[_high], _org[_low]);
    swap(_sync[_high], _sync[_low]);
  }
  // ��data[_low]�������ֵ��Ϊ��Ԫ
  int key = _org[_low];
  int i = _low;

  for (int j = _low + 1; j <= _high; j++) {
    if (_org[j] <= key) {
      i++;
      if (i != j) {
        swap(_org[i], _org[j]);
        swap(_sync[i], _sync[j]);
      }
    }
  }
  swap(_org[i], _org[_low]);
  swap(_sync[i], _sync[_low]);
  return i;
}

//�������ã� ��������������
//�����б�
//         a ��������
//         b ��������
//�� �� ֵ�� N/A
void FileManager::swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}

//�������ã� ����������������Ψһʵ��
//�����б� N/A
//�� �� ֵ�� �ļ���������Ψһʵ��
FileManager* FileManager::GetInstance() {
  return FileManager::Instance == NULL ?
    FileManager::Instance = new FileManager() : FileManager::Instance;
}

//�������ã� ˽�еĹ�����
//�����б� N/A
//�� �� ֵ�� N/A
FileManager::FileManager() {
  this->allocator = DBAllocator::GetInstance();
}

// Ψһʵ��
FileManager* FileManager::Instance = NULL;

CSTORE_NS_END