#include "DBTransaction.hpp"

CSTORE_NS_BEGIN

// ������
DBTransaction::DBTransaction(std::string queryCode)
  :DBObject("DBTransaction", this) {
  id = DBTransaction::encounter++;
  time_t ts = time(NULL);
  tm* __CSTRACETIMESTAMPPTR__ = localtime(&ts);
  char timeBuffer[10];
  sprintf(timeBuffer, "%02d:%02d:%02d", __CSTRACETIMESTAMPPTR__->tm_hour, __CSTRACETIMESTAMPPTR__->tm_min, __CSTRACETIMESTAMPPTR__->tm_sec);
  this->timestamp = std::string(timeBuffer);
  this->successFlag = false;
  this->finishstamp = "";
  this->code = queryCode;
  beginStampObj = std::clock();
}

// ��ȡ�������ʱ��
inline std::string DBTransaction::GetDuration() {
  char dBuffer[10];
  sprintf(dBuffer, "%.8lf", (double)(std::clock() - this->beginStampObj) / (double)CLOCKS_PER_SEC);
  return std::string(dBuffer);
}

// ��д�ַ���������
std::string DBTransaction::ToString() {
  CSCommonUtil::StringBuilder sb;
  sb.Append("ID:\t").Append(this->id).Append(NEWLINE);
  sb.Append("Begin:\t").Append(this->timestamp).Append(NEWLINE);
  sb.Append("Over:\t").Append(this->finishstamp).Append(NEWLINE);
  sb.Append("Cost:\t").Append(this->GetDuration()).Append(NEWLINE);
  sb.Append("Code:\t").Append((this->code.length() > 15 ? this->code.substr(0, 14) + "..." : this->code)).Append(NEWLINE);
  return sb.ToString();
}

// ��̬������
int DBTransaction::encounter = 0;

CSTORE_NS_END