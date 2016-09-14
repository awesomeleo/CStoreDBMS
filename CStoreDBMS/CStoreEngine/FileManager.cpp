#include "FileManager.h"
#include <limits.h>
#include <string.h>
#include <string>

#define COMPRESS_SIZE 65536

using namespace std;

int externSortOrdersBufferPool[64 * SIZE_PAGE];
int externSortCustkeyBufferPool[64 * SIZE_PAGE];
int pagecounter = 0;
int importcounter = 0;
int importcountercustomer = 0;

char page_buf[SIZE_PAGE];
int page_ptr = 0;
int count_spilt = 0;
int maxcount = 0;

bool FileManager::getMiniOrders(
  int &times,
  int* orderkeyBuffer,
  int* custkeyBuffer,
  double* totalpriceBuffer,
  int* shippriorityBuffer) {
  memset(custkeyBuffer, 0, sizeof(custkeyBuffer));
  memset(orderkeyBuffer, 0, sizeof(orderkeyBuffer));
  memset(totalpriceBuffer, 0, sizeof(totalpriceBuffer));
  memset(shippriorityBuffer, 0, sizeof(shippriorityBuffer));

  FILE* fin = fopen("../orders.tbl", "rb");
  if (fin == NULL) {
    fin = fopen("orders.tbl", "rb");
  }
  if (fin == NULL) {
    std::cout << "FILE CANNOT READ: orders.tbl" << std::endl;
  }
  int count_reco = 0;
  int content_ptr = 0;
  char content_buf[128];

  while (count_reco < SIZE_PAGE) {
    // page_buf������
    if (page_ptr == maxcount) {
      fseek(fin, times++ * SIZE_PAGE * sizeof(char), SEEK_SET);
      maxcount = fread(page_buf, sizeof(char), SIZE_PAGE, fin);
      page_ptr = 0;
    }
    // �Ѿ�����Ϊpage_buf�����κ�����
    if (maxcount == 0)
      break;
    // �����ַ���ת����������Ӧ������
    if (page_buf[page_ptr] != '|' && page_buf[page_ptr] != '\n') {
      content_buf[content_ptr++] = page_buf[page_ptr];
    }
    else {
      count_spilt++;
      content_buf[content_ptr] = '\0';
      switch (count_spilt) {
      case 1: orderkeyBuffer[count_reco] = atoi(content_buf); break;
      case 2: custkeyBuffer[count_reco] = atoi(content_buf); break;
      case 4: totalpriceBuffer[count_reco] = atof(content_buf); break;
      case 8: shippriorityBuffer[count_reco] = atoi(content_buf); break;
      case 9: count_reco++; count_spilt = -1; break;
      default:
        break;
      }
      content_ptr = 0;
    }
    page_ptr++;
  }
  fclose(fin);
  importcounter = count_reco;
  if (maxcount == 0) {
    return true;
  }
  else {
    return false;
  }
}

bool FileManager::getMiniCustomers(int &times, int* custkeyBuffer) {
  memset(custkeyBuffer, 0, sizeof(custkeyBuffer));
  FILE* fin = fopen("../customer.tbl", "rb");
  if (fin == NULL) {
    fin = fopen("customer.tbl", "rb");
  }
  if (fin == NULL) {
    std::cout << "FILE CANNOT READ: customer.tbl" << std::endl;
  }
  int count_reco = 0;
  int content_ptr = 0;
  char content_buf[128];

  while (count_reco < SIZE_PAGE) {
    // page_buf������
    if (page_ptr == maxcount) {
      fseek(fin, times++ * SIZE_PAGE * sizeof(char), SEEK_SET);
      maxcount = fread(page_buf, sizeof(char), SIZE_PAGE, fin);
      page_ptr = 0;
    }
    // �Ѿ�����Ϊpage_buf�����κ�����
    if (maxcount == 0)
      break;
    // �����ַ���ת����������Ӧ������
    if (page_buf[page_ptr] != '|' && page_buf[page_ptr] != '\n') {
      content_buf[content_ptr++] = page_buf[page_ptr];
    }
    else {
      count_spilt++;
      content_buf[content_ptr] = '\0';
      switch (count_spilt) {
      case 1: custkeyBuffer[count_reco] = atoi(content_buf); break;
      case 8: count_reco++; count_spilt = -1; break;
      default:
        break;
      }
      content_ptr = 0;
    }
    page_ptr++;
  }
  fclose(fin);
  importcountercustomer = count_reco;
  if (maxcount == 0) {
    return true;
  }
  else {
    return false;
  }
}

void FileManager::importOrders() {
  __sflag = WRITING;
  bool finflag = false;
  int pcounter = 0;
  FILE *fout1, *fout2, *fout3, *fout4;
  // ����ļ��Ѿ����ڣ�������ļ�
  FILE *fdel = fopen("task1_orders_orderkey.db", "w");
  fclose(fdel);
  fdel = fopen("task1_orders_custkey.db", "w");
  fclose(fdel);
  fdel = fopen("task1_orders_totalprice.db", "w");
  fclose(fdel);
  fdel = fopen("task1_orders_shippriority.db", "w");
  fclose(fdel);
  fout1 = fopen("task1_orders_orderkey.db", "ab");
  fout2 = fopen("task1_orders_custkey.db", "ab");
  fout3 = fopen("task1_orders_totalprice.db", "ab");
  fout4 = fopen("task1_orders_shippriority.db", "ab");
  do {
    // �Ȳ��ϵض����ļ���������
    finflag = getMiniOrders(
      pcounter,
      Buffer_orderkey,
      Buffer_custkey,
      Buffer_totalprice,
      Buffer_shippriority
      );
    // Ȼ�󽫻���������д���ĸ��ļ���
    fwrite(Buffer_orderkey, sizeof(int), importcounter, fout1);
    fwrite(Buffer_custkey, sizeof(int), importcounter, fout2);
    fwrite(Buffer_totalprice, sizeof(double), importcounter, fout3);
    fwrite(Buffer_shippriority, sizeof(int), importcounter, fout4);
  } while (finflag != true);
  fclose(fout1);
  fclose(fout2);
  fclose(fout3);
  fclose(fout4);
  __sflag = FNOP;
}

void FileManager::importCustomers() {
  __sflag = WRITING;
  bool finflag = false;
  int pcounter = 0;
  FILE *fout1;
  FILE *fdel = fopen("task3_customer_custkey.db", "w");
  fclose(fdel);
  fout1 = fopen("task3_customer_custkey.db", "ab");
  do {
    finflag = getMiniCustomers(
      pcounter,
      Buffer_orderkey
      );
    fwrite(Buffer_orderkey, sizeof(int), importcountercustomer, fout1);
  } while (finflag != true);
  fclose(fout1);
  __sflag = FNOP;
}

int* FileManager::getOrdersBuffer(int _times, int &_maxcount) {
  FILE* fin = fopen("task1_orders_orderkey.db", "rb");
  if (fin == NULL) return NULL;
  fseek(fin, (long)(_times * 65536 * sizeof(int)), SEEK_SET);
  if (fin == 0) {
    return NULL;
  }
  _maxcount = fread(externSortOrdersBufferPool, sizeof(int), 65536, fin);
  fclose(fin);
  return externSortOrdersBufferPool;
}

void FileManager::getEXOrdersBuffer(int _times, int &_maxcount) {
  FILE* fin = fopen("task1_orders_orderkey.db", "rb");
  fseek(fin, (long)(_times * 64 * SIZE_PAGE * sizeof(int)), SEEK_SET);
  if (fin == 0) return;
  _maxcount = fread(externSortOrdersBufferPool, sizeof(int), 64 * SIZE_PAGE, fin);
  fclose(fin);
}

void FileManager::getEXCustkeyBuffer(int _times, int &_maxcount) {
  FILE* fin = fopen("task1_orders_custkey.db", "rb");
  fseek(fin, (long)(_times * 64 * SIZE_PAGE * sizeof(int)), SEEK_SET);
  if (fin == 0) return;
  _maxcount = fread(externSortCustkeyBufferPool, sizeof(int), 64 * SIZE_PAGE, fin);
  fclose(fin);
}

void FileManager::externSort() {
  int maxcount = 1024;
  int pc = 0;
  // �γ�����
  while (maxcount > 0) {
    // ÿ�ζ���128ҳ
    getEXOrdersBuffer(pc, maxcount);
    getEXCustkeyBuffer(pc, maxcount);
    if (maxcount == 0) break;
    // Ȼ�����custkey����������
    innerSort(externSortCustkeyBufferPool, externSortOrdersBufferPool, maxcount);
    // ��������Ϻ�д��ʱ�ļ�
    char strBuffer[10];
    std::string outFileName = "temp_exSortTempCustkey";
    sprintf(strBuffer, "%d", pagecounter);
    outFileName += strBuffer;
    FILE* fout = fopen(outFileName.c_str(), "wb");
    fwrite(externSortCustkeyBufferPool, sizeof(int), maxcount, fout);
    fclose(fout);
    outFileName = "temp_exSortTempOrders";
    outFileName += strBuffer;
    fout = fopen(outFileName.c_str(), "wb");
    fwrite(externSortOrdersBufferPool, sizeof(int), maxcount, fout);
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
        memset(mergeBuffer[i], 0, mergeBufferCapacity * sizeof(int));
        // �����ļ���
        char fstrBuffer[10];
        inFileName = "temp_exSortTempCustkey";
        sprintf(fstrBuffer, "%d", i);
        inFileName += fstrBuffer;
        // �������ļ�ʣ�ಿ�ַ��뻺����
        FILE* fin = fopen(inFileName.c_str(), "rb");
        fseek(fin, (long)(mergeTimes[i] * mergeBufferCapacity * sizeof(int)), SEEK_SET);
        freadflag = fread(mergeBuffer[i], sizeof(int), mergeBufferCapacity, fin);
        fclose(fin);
        // �ٴι����ļ���������Ƕ�orderkey���е�
        inFileName = "temp_exSortTempOrders";
        sprintf(fstrBuffer, "%d", i);
        inFileName += fstrBuffer;
        // �ٴζ��ļ����Ż�����
        fin = fopen(inFileName.c_str(), "rb");
        fseek(fin, (long)(mergeTimes[i] * mergeBufferCapacity * sizeof(int)), SEEK_SET);
        freadflag = fread(mergeBuffer[pagecounter + i], sizeof(int), mergeBufferCapacity, fin);
        fclose(fin);
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
        writeCustkeyOutputBufferToFile(outputBuffer, outputPointer);
        writeOrdersOutputBufferToFile(syncBuffer, outputPointer);
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
      writeCustkeyOutputBufferToFile(outputBuffer, outputPointer);
      writeOrdersOutputBufferToFile(syncBuffer, outputPointer);
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
  // ѹ��
  compressCustkey();
}

void FileManager::compressCustkey() {
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
  FILE* fin = fopen("task2_custkey_sorted.db", "rb");
  while (true) {
    memset(compressBuffer, 0, sizeof(int)* COMPRESS_SIZE);
    maxcount = fread(compressBuffer, sizeof(int), COMPRESS_SIZE, fin);
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
          writeCompressedCustkeyToFile(outputBuffer, COMPRESS_SIZE);
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
        writeCompressedCustkeyToFile(outputBuffer, COMPRESS_SIZE);
        outputPointer = 0;
      }
      // ����������һ�ֵ��ǻ��������������Ǿ�д��ȥ
      else if (maxcount == 0 && outputPointer > 0) {
        writeCompressedCustkeyToFile(outputBuffer, outputPointer);
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
        writeCompressedCustkeyToFile(outputBuffer, outputPointer);
        outputPointer = 0;
      }
      break;
    }
    // �ض�λ�ļ�ָ��
    fseek(fin, (long)(COMPRESS_SIZE * (++ftimes) * sizeof(int)), SEEK_SET);
  }
}

void FileManager::join() {
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
  FILE* joinFileOrderkey = fopen("task2_orderkey_sorted.db", "rb");
  FILE* joinFileCustkey = fopen("task2_custkey_compressed.db", "rb");
  FILE* joinFileCustomer = fopen("task3_customer_custkey.db", "rb");
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
            writeJoinedItemToFile(joinOutputOrderkeyBuffer, joinOutputCustkeyBuffer, joinOutputPointer);
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
      writeJoinedItemToFile(joinOutputOrderkeyBuffer, joinOutputCustkeyBuffer, joinOutputPointer);
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
      writeJoinedItemToFile(joinOutputOrderkeyBuffer, joinOutputCustkeyBuffer, joinOutputPointer);
      joinOutputPointer = 0;
    }
    else if ((custkeyMaxcount == 0 || customerMaxcount == 0) && (joinOutputPointer == 0)) {
      break;
    }

  }
  // ȷ�ϰ�ȫ���˳�
  if (joinOutputPointer > 0) {
    writeJoinedItemToFile(joinOutputOrderkeyBuffer, joinOutputCustkeyBuffer, joinOutputPointer);
    joinOutputPointer = 0;
  }
  // �����ļ�
  fclose(joinFileOrderkey);
  fclose(joinFileCustkey);
  fclose(joinFileCustomer);
}

int FileManager::count() {
  FILE* fin = fopen("task2_custkey_compressed.db", "rb");
  fseek(fin, 0L, SEEK_SET);
  int ctr = 0;
  int atr = 0;
  int buf[1024];
  int it = 0;
  do {
    memset(buf, 0, sizeof(int)* 1024);
    ctr = fread(buf, sizeof(int), 1024, fin);
    for (int j = 0; j < ctr; j++) {
      if (j % 2 == 1) {
        atr += buf[j];
      }
    }
    fseek(fin, (long)(sizeof(int)* 1024 * ++it), SEEK_SET);
  } while (ctr != 0);
  return atr;
}

void FileManager::writeJoinedItemToFile(int* _orderBuffer, int* _custBuffer, int _incounter) {
  //FILE* joinOutOrderkey = fopen("task3_joined_orderkey.db", "ab");
  //FILE* joinOutCustkey = fopen("task3_joined_custkey.db", "ab");
  //fwrite(_orderBuffer, sizeof(int), _incounter, joinOutOrderkey);
  //fwrite(_custBuffer, sizeof(int), _incounter, joinOutCustkey);
  //fclose(joinOutOrderkey);
  //fclose(joinOutCustkey);
  for (int i = 0; i < _incounter; i++)
  {
    std::cout << _custBuffer[i] << " " << _orderBuffer[i] << std::endl;
  }
}

void FileManager::writeCompressedCustkeyToFile(int* _orgBuffer, int _incounter) {
  FILE* fout = fopen("task2_custkey_compressed.db", "ab");
  fwrite(_orgBuffer, sizeof(int), _incounter, fout);
  fclose(fout);
}

void FileManager::writeOrdersOutputBufferToFile(int* _orgBuffer, int _incounter) {
  FILE* fout = fopen("task2_orderkey_sorted.db", "ab");
  fwrite(_orgBuffer, sizeof(int), _incounter, fout);
  fclose(fout);
}

void FileManager::writeCustkeyOutputBufferToFile(int* _orgBuffer, int _incounter) {
  FILE* fout = fopen("task2_custkey_sorted.db", "ab");
  fwrite(_orgBuffer, sizeof(int), _incounter, fout);
  fclose(fout);
}

void FileManager::innerSort(int* _orgBuffer, int* _syncBuffer, int _maxcount) {
  quickSort(_orgBuffer, _syncBuffer, 0, _maxcount - 1);
}

void FileManager::quickSort(int _org[], int _sync[], int low, int high) {
  if (low < high) {
    int _key = partition(_org, _sync, low, high);
    quickSort(_org, _sync, low, _key - 1);
    quickSort(_org, _sync, _key + 1, high);
  }
}

// ����ȡ�зָ����������Ԫ�±ꡣ
// _sync[]����_org[]�仯��
// ��֤key����߶���keyС���ұ߶���key��
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

void FileManager::swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}

// ===================  SYSTEM FUNS  =================== //
FileManager* FileManager::getInstance() {
  if (__instance == NULL) {
    __instance = new FileManager();
  }
  return __instance;
}

FileManager::FileManager() {
  __sflag = FNOP;
  pos = 0;
  tpos = 0;
}

FileManager* FileManager::__instance = NULL;
