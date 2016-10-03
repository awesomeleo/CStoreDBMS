#ifndef ___CSTORE_DBCOMMONUTILS
#define ___CSTORE_DBCOMMONUTILS
#include <string>
#include <vector>
#include "DBObject.h"
#include "DBMarcos.h"

namespace CSCommonUtil {
  //�������ã� ��ȥ�ַ���ǰ��Ŀհ�
  //�����б�
  //       str Ҫ������ַ���
  //�� �� ֵ�� N/A
  template<typename _Ty = std::string>
  std::string CStrTrim(const std::string& str) {
    std::string::size_type pos = str.find_first_not_of(' ');
    if (pos == std::string::npos) {
      return str;
    }
    std::string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != std::string::npos) {
      return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
  }

  //�������ã� �滻�ַ����е�����
  //�����б�
  //       str Ҫ������ַ���
  //       src Ҫ�滻��ԭ���Ӵ�
  //      dest Ҫ�滻�ɵ�Ŀ���Ӵ�
  //�� �� ֵ�� �ָ�����Ӵ�����
  template<typename _Ty = std::string>
  std::string CStrReplace(const std::string& str, const std::string& src, const std::string& dest) {
    std::string ret;
    std::string::size_type pos_begin = 0;
    std::string::size_type pos = str.find(src);
    while (pos != std::string::npos) {
      ret.append(str.data() + pos_begin, pos - pos_begin);
      ret += dest;
      pos_begin = pos + 1;
      pos = str.find(src, pos_begin);
    }
    if (pos_begin < str.length()) {
      ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
  }

  //�������ã� ���ַ����ָ���Ӵ�����
  //�����б�
  //       str Ҫ������ַ���
  //       sep �ָ���
  //�� �� ֵ�� �Ӵ�����
  template<typename _Ty = std::string>
  std::vector<std::string> CStrSplit(const std::string& str, std::string sep = ",") {
    std::vector<std::string> retVec;
    if (str.empty()) {
      return retVec;
    }
    std::string tmp;
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type comma_pos = 0;
    while (pos_begin != std::string::npos) {
      comma_pos = str.find(sep, pos_begin);
      if (comma_pos != std::string::npos) {
        tmp = str.substr(pos_begin, comma_pos - pos_begin);
        pos_begin = comma_pos + sep.length();
      } else {
        tmp = str.substr(pos_begin);
        pos_begin = comma_pos;
      }
      if (!tmp.empty()) {
        retVec.push_back(tmp);
        tmp.clear();
      }
    }
    return retVec;
  }

  class StringBuilder : CStore::DBObject {
  public:
    //�������ã� ���캯��
    //�����б� N/A
    //�� �� ֵ�� N/A
    StringBuilder() : CStore::DBObject("StringBuilder", this) {
      this->_str = "";
    }

    //�������ã� ����ʼ�ַ����Ĺ��캯��
    //�����б�
    //       str ��ʼ���ַ���
    //�� �� ֵ�� N/A
    StringBuilder(const std::string& str) : CStore::DBObject("StringBuilder", this) {
      this->_str = str;
    }

    //�������ã� ����ʼ�ַ����Ĺ��캯��
    //�����б�
    //       str ��ʼ���ַ���
    //�� �� ֵ�� N/A
    StringBuilder(const char* str) : CStore::DBObject("StringBuilder", this) {
      this->_str = std::string(str);
    }

    //�������ã� ��ȡ��ǰ�ַ�������
    //�����б� N/A
    //�� �� ֵ�� �ַ�����������
    inline int Length() {
      return this->_str.length();
    }

    //�������ã� ����ַ���������
    //�����б� N/A
    //�� �� ֵ�� N/A
    inline void Clear() {
      this->_str.clear();
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(const std::string& appender) {
      this->_str.append(appender);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(char appender) {
      this->_str.push_back(appender);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(const char* appender) {
      this->_str.append(appender);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(int appender) {
      char numbuf[32];
      std::sprintf(numbuf, "%d", appender);
      this->_str.append(numbuf);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(double appender) {
      char numbuf[64];
      std::sprintf(numbuf, "%lf", appender);
      this->_str.append(numbuf);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append(float appender) {
      char numbuf[64];
      std::sprintf(numbuf, "%f", appender);
      this->_str.append(numbuf);
      return *this;
    }

    //�������ã� ׷������
    //�����б�
    //  appender Ҫ׷�ӵĶ���
    //�� �� ֵ�� N/A
    StringBuilder& Append (CStore::DBObject& appender) {
      this->_str.append(appender.ToString().c_str());
      return *this;
    }

    //�������ã� ��д�ַ���������
    //�����б� N/A
    //�� �� ֵ�� ���ַ�����������Ӧ���ַ���ʵ��
    virtual std::string ToString() {
      return this->_str;
    }

    //�������ã� ��д�Ա����������Ƿ���ȣ�ֱ�ӱȽ϶�Ӧ���ַ����Ƿ����
    //�����б�
    //    RefObj Ҫ�ȽϵĶ�������
    //�� �� ֵ�� �Ƿ����
    virtual bool Equals(StringBuilder& RefObj) {
      return this->_str == RefObj.ToString();
    }

  private:
    // �ַ�������
    std::string _str;

    // ��ֹ��������
    DISALLOW_COPY_AND_ASSIGN(StringBuilder);
  }; /* StringBuilder */

} /* CStoreUtil */

#endif /* ___CSTORE_DBCOMMONUTILS */