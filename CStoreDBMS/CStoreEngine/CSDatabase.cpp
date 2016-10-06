#include "CSDatabase.h"

CSTORE_NS_BEGIN

// CSDatabase���캯��
CSDatabase::CSDatabase() {
  this->tableMana = TableManager::GetInstance();
  this->fileMana = FileManager::GetInstance();
}

// CSDatabase�г����б�
void CSDatabase::ShowTable() {
  istr tableDescription = this->tableMana->ShowTable();
  TRACE("Table in database:");
  PILEPRINTLN(tableDescription);
}

// CSDatabase���
void CSDatabase::Reset() {
  this->tableMana->Clear();
  this->_param.clear();
}

// CSDatabase������
int CSDatabase::Size() {
  return this->tableMana->CountTable();
}

// CSDatabase�����ֵ�
int CSDatabase::Reference(istr paraname) {
  return this->_param[paraname];
}

// CSDatabase���Ƿ����
bool CSDatabase::Exist(istr name) {
  return this->tableMana->ExistTable(name);
}

// CSDatabaseִ��һ�����
bool CSDatabase::Interpreter(DBCProxy& proxy, DBTransaction& trans) {
  // ȡ�ò�����
  DashType opCode = proxy.opCode;
  bool res = false;
  // ��ղ����ֵ�
  _param.clear();
  switch (opCode)
  {
  case DashType::dash_load:
    res = this->Load(&trans, proxy.opTable);
    break;
  case DashType::dash_retrieve:
    res = this->Retrieve(&trans, proxy.opTable, static_cast<int>(proxy.aTag));
    break;
  case DashType::dash_create:
    res = this->Create(trans, proxy.opTable, proxy.Pi, proxy.PiType);
    break;
  case DashType::dash_delete:
    res = this->Delete(proxy.opTable, proxy.CondPi, proxy.condPtr, &proxy);
    break;
  case DashType::dash_insert:
    res = this->Insert(proxy.opTable, proxy.Pi, proxy.ProxyPi, proxy.errorBit);
    break;
  case DashType::dash_select:
    res = this->Select(proxy.opTable, proxy.Pi, proxy.isStar, proxy.CondPi, proxy.condPtr, &proxy);
    break;
  default:
    res = this->iException("No_Object_Exception", proxy.id);
    break;
  }
  // ���������������
  if (res == false) {
    PILEPRINT("# This command is ignored due to exception.");
  }
  return res;
}

// CSDatabase����
bool CSDatabase::Create(DBTransaction& trans, istr name, StrVec &pi, StrVec &pitype) {
  this->dbMutex.lock();
  if (this->tableMana->AddTable(name, name) == false) {
    return false;
  }
  DBTable* tobj = this->tableMana->GetTable(name);
  this->tableMana->GetTableLock(name)->LockTransaction = &trans;
  tobj->PiList = std::vector<std::string>(pi);
  tobj->PiTypeList = std::vector<std::string>(pitype);
  this->dbMutex.unlock();
  return true;
}

// CSDatabaseɾ��
bool CSDatabase::Delete(istr name, StrVec &condVec, SyntaxTreeNode* cond, DBCProxy* iproxy) {
  TRACE("NOT IMPLEMENTATION EXCEPTION");
  return true;
}

// CSDatabase����
bool CSDatabase::Insert(istr name, StrVec &pilist, IntVec &pivalue, bool &errorbit) {
  TRACE("NOT IMPLEMENTATION EXCEPTION");
  return true;
}

// CSDatabase��ѯ
bool CSDatabase::Select(istr name, StrVec &pi, bool star, StrVec &condVec, SyntaxTreeNode* cond, DBCProxy* iproxy) {
  TRACE("NOT IMPLEMENTATION EXCEPTION");
  return true;
}

// �������������
bool CSDatabase::Load(DBTransaction* trans, istr tname) {
  // ȡ�ñ����
  this->dbMutex.lock();
  DBTable* tobj = this->tableMana->GetTable(tname);
  if (tobj == NULL) {
    this->iException("table not exist: " + tname);
    return false;
  }
  this->dbMutex.unlock();
  // �ӻ�����
  this->tableMana->GetTableLock(tname)->LockWrite(trans);
  // �����ļ��������˿�
  bool flag = this->fileMana->LoadTable(*tobj);
  // �⻥����
  this->tableMana->GetTableLock(tname)->UnlockWrite();
  return flag;
}

// ͨ��������ȡ��¼
bool CSDatabase::Retrieve(DBTransaction* trans, istr tname, int tkey) {
  // ȡ�ñ����
  this->dbMutex.lock();
  DBTable* tobj = this->tableMana->GetTable(tname);
  if (tobj == NULL) {
    this->iException("table not exist: " + tname);
    return false;
  }
  this->dbMutex.unlock();
  // �ӹ�����
  this->tableMana->GetTableLock(tname)->LockRead(trans);
  // �����ļ��������ӿ�
  std::string outStr = "";
  bool flag = this->fileMana->Retrieve(*tobj, tkey, outStr);
  if (flag) {
    PILEPRINTLN(outStr);
  }
  else {
    PILEPRINTLN(CSCommonUtil::StringBuilder("Primary key value: ").Append(tkey).Append(" not exist").ToString());
  }
  // �⹲����
  this->tableMana->GetTableLock(tname)->UnlockRead();
  return true;
}

// ѹ����
bool CSDatabase::Compress(DBTransaction* trans, istr tname, istr pi) {
  // ȡ�ñ����
  this->dbMutex.lock();
  DBTable* tobj = this->tableMana->GetTable(tname);
  if (tobj == NULL) {
    this->iException("table not exist: " + tname);
    return false;
  }
  this->dbMutex.unlock();
  // �ӹ�����
  this->tableMana->GetTableLock(tname)->LockRead(trans);
  // �����ļ��������ӿ�
  std::vector<std::string> syncList;
  syncList.push_back(tobj->PiList[0]);
  this->fileMana->ExternSort(*tobj, tobj->PiList[1], syncList);
  PILEPRINTLN("External Sort and Compress col " + pi + " OK.");
  // �⹲����
  this->tableMana->GetTableLock(tname)->UnlockRead();
  return true;
}

// ��Ȼ���ӱ�
bool CSDatabase::Join(DBTransaction* trans, istr t1name, istr t2name) {
  // ȡ�ñ����
  this->dbMutex.lock();
  DBTable* tobj1 = this->tableMana->GetTable(t1name);
  DBTable* tobj2 = this->tableMana->GetTable(t2name);
  if (tobj1 == NULL) {
    this->iException("table not exist: " + t1name);
    return false;
  }
  if (tobj2 == NULL) {
    this->iException("table not exist: " + t2name);
    return false;
  }
  this->dbMutex.unlock();
  // �ӹ�����
  this->tableMana->GetTableLock(t1name)->LockRead(trans);
  this->tableMana->GetTableLock(t2name)->LockRead(trans);
  // �����ļ��������ӿ�
  this->fileMana->Join();
  // �⹲����
  this->tableMana->GetTableLock(t1name)->UnlockRead();
  this->tableMana->GetTableLock(t2name)->UnlockRead();
  return true;
}

// �����¼��Ŀ
bool CSDatabase::Count(DBTransaction* trans, istr tname) {
  // ȡ�ñ����
  this->dbMutex.lock();
  DBTable* tobj = this->tableMana->GetTable(tname);
  if (tobj == NULL) {
    this->iException("table not exist: " + tname);
    return false;
  }
  this->dbMutex.unlock();
  // �ӹ�����
  this->tableMana->GetTableLock(tname)->LockRead(trans);
  // �����ļ��������ӿ�
  this->fileMana->Count(*tobj, "custkey");
  // �⹲����
  this->tableMana->GetTableLock(tname)->UnlockRead();
  return true;
}


// CSDatabase�쳣����
bool CSDatabase::iException(istr _info, int _index) {
  TRACE("# Interpreter Exception Spotted.");
  if (_index != -1) {
    PILEPRINTLN("# When executing SSQL at command: "
      << _index);
  }
  PILEPRINTLN("# " + _info);
  return false;
}

// CSDatabase�����﷨����ֵ
bool CSDatabase::AST(SyntaxTreeNode* mynode, CSDatabase* myexec, DBCProxy* myproxy) {
  istr optype;
  switch (mynode->nodeSyntaxType)
  {
  case SyntaxType::case_sexpr:
    AST(mynode->children[0], myexec, myproxy); // ��ʽ
    AST(mynode->children[1], myexec, myproxy); // ����
    mynode->aTag = mynode->children[0]->aTag + mynode->children[1]->aTag;
    break;
  case SyntaxType::case_wexpr:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___wexpr__wmulti__wexpr_pi_45) {
      AST(mynode->children[0], myexec, myproxy); // ��ʽ
      AST(mynode->children[1], myexec, myproxy); // ����
      mynode->aTag = mynode->children[0]->aTag + mynode->children[1]->aTag;
    }
    else {
      mynode->aTag = 0;
    }
    break;
  case SyntaxType::case_sexpr_pi:
  case SyntaxType::case_wexpr_pi:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___sexpr_pi__splus__sexpr_pi_70
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wexpr_pi__wplus__wexpr_pi_72) {
      AST(mynode->children[0], myexec, myproxy); // ����
      AST(mynode->children[1], myexec, myproxy); // ����հ�
      mynode->aTag = mynode->children[0]->aTag + mynode->children[1]->aTag;
    }
    break;
  case SyntaxType::case_splus:
  case SyntaxType::case_wplus:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___splus__plus_smulti_14
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wplus__plus_wmulti_46) {
      AST(mynode->children[1], myexec, myproxy);
      mynode->aTag = mynode->children[1]->aTag; // �ӷ�
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___splus__minus_smulti_15
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wplus__minus_wmulti_47) {
      AST(mynode->children[1], myexec, myproxy);
      mynode->aTag = (-1) * mynode->children[1]->aTag; // ����
    }
    else {
      mynode->aTag = 0;
    }
    break;
  case SyntaxType::case_smulti:
  case SyntaxType::case_wmulti:
    AST(mynode->children[0], myexec, myproxy); // ����
    AST(mynode->children[1], myexec, myproxy); // ����հ�
    mynode->aTag = mynode->children[0]->aTag * mynode->children[1]->aTag;
    break;
  case SyntaxType::case_smultiOpt:
  case SyntaxType::case_wmultiOpt:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___smultiOpt__multi_sunit__smultiOpt_18
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wmultiOpt__multi_wunit__wmultiOpt_50) {
      AST(mynode->children[1], myexec, myproxy); // ����
      AST(mynode->children[2], myexec, myproxy); // ����հ�
      mynode->aTag = mynode->children[1]->aTag * mynode->children[2]->aTag; // �˷�
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___smultiOpt__div_sunit__smultiOpt_19
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wmultiOpt__div_wunit__wmultiOpt_51) {
      AST(mynode->children[1], myexec, myproxy); // ����
      AST(mynode->children[2], myexec, myproxy); // ����հ�
      if (mynode->children[1]->aTag * mynode->children[2]->aTag == 0) {
        mynode->aTag = 0;
        myproxy->errorBit = true; // �������
      }
      else {
        mynode->aTag = 1.0f / mynode->children[1]->aTag * mynode->children[2]->aTag; // ����
      }
    }
    else {
      mynode->aTag = 1.0f;
    }
    break;
  case SyntaxType::case_sunit:
  case SyntaxType::case_wunit:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___sunit__number_21
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wunit__number_53) {
      mynode->aTag = atoi(mynode->children[0]->nodeValue.c_str());
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___sunit__minus_sunit_22
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wunit__minus_wunit_55) {
      AST(mynode->children[1], myexec, myproxy);
      mynode->aTag = (-1) * mynode->children[1]->aTag;
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___sunit__plus_sunit_23
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wunit__plus_wunit_56) {
      AST(mynode->children[1], myexec, myproxy);
      mynode->aTag = mynode->children[1]->aTag;
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___sunit__brucket_sexpr_24
      || mynode->candidateFunction->GetType() == CFunctionType::deri___wunit__brucket_disjunct_57) {
      AST(mynode->children[1], myexec, myproxy);
      mynode->aTag = mynode->children[1]->aTag;
    }
    else if (mynode->candidateFunction->GetType() == CFunctionType::deri___wunit__iden_54) {
      mynode->aTag = myexec->Reference(mynode->children[0]->nodeValue); // ������ֵ�
    }
    break;
  case SyntaxType::case_disjunct:
    AST(mynode->children[0], myexec, myproxy); // ��ȡ��
    AST(mynode->children[1], myexec, myproxy); // ��ȡ�հ�
    mynode->aTag = mynode->children[0]->aTag + mynode->children[1]->aTag;
    return fabs(mynode->aTag) <= FZERO ? false : true;
    break;
  case SyntaxType::case_disjunct_pi:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___disjunct_pi__conjunct__disjunct_pi_36) {
      AST(mynode->children[1], myexec, myproxy); // ��ȡ��
      AST(mynode->children[2], myexec, myproxy); // ��ȡ�հ�
      mynode->aTag = mynode->children[1]->aTag + mynode->children[2]->aTag;
    }
    else {
      mynode->aTag = false; // ��ȡfalse��Ӱ����
    }
    break;
  case SyntaxType::case_conjunct:
    AST(mynode->children[0], myexec, myproxy); // ������
    AST(mynode->children[1], myexec, myproxy); // ��ȡ�հ�
    mynode->aTag = mynode->children[0]->aTag * mynode->children[1]->aTag;
    break;
  case SyntaxType::case_conjunct_pi:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___conjunct_pi__bool__conjunct_pi_39) {
      AST(mynode->children[1], myexec, myproxy); // ������
      AST(mynode->children[2], myexec, myproxy); // ��ȡ�հ�
      mynode->aTag = mynode->children[1]->aTag * mynode->children[2]->aTag;
    }
    else {
      mynode->aTag = true; // ��ȡtrue��Ӱ����
    }
    break;
  case SyntaxType::case_bool:
    if (mynode->candidateFunction->GetType() == CFunctionType::deri___bool__not_bool_42) {
      AST(mynode->children[1], myexec, myproxy); // ����
      mynode->aTag = mynode->children[1]->aTag != 0 ? false : true;
    }
    else {
      AST(mynode->children[0], myexec, myproxy); // ���ʽ
      mynode->aTag = mynode->children[0]->aTag;
    }
    break;
  case SyntaxType::case_comp:
    if (mynode->children[1]->candidateFunction->GetType() == CFunctionType::deri___rop__epsilon_80) {
      AST(mynode->children[0], myexec, myproxy); // ���
      mynode->aTag = mynode->children[0]->aTag;
    }
    else {
      optype = mynode->children[1]->nodeValue; // �����
      AST(mynode->children[0], myexec, myproxy); // ���
      AST(mynode->children[2], myexec, myproxy); // �ұ�
      mynode->aTag = false;
      if (optype == "<>") {
        mynode->aTag = mynode->children[0]->aTag != mynode->children[2]->aTag;
      }
      else if (optype == "==") {
        mynode->aTag = mynode->children[0]->aTag == mynode->children[2]->aTag;
      }
      else if (optype == ">") {
        mynode->aTag = mynode->children[0]->aTag > mynode->children[2]->aTag;
      }
      else if (optype == "<") {
        mynode->aTag = mynode->children[0]->aTag < mynode->children[2]->aTag;
      }
      else if (optype == ">=") {
        mynode->aTag = mynode->children[0]->aTag >= mynode->children[2]->aTag;
      }
      else if (optype == "<=") {
        mynode->aTag = mynode->children[0]->aTag <= mynode->children[2]->aTag;
      }
    }
    break;
  default:
    break;
  }
  return true;
}

CSTORE_NS_END