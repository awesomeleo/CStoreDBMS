#include "DBBridge.h"

CSTORE_NS_BEGIN

// IBridge���캯��
DBBridge::DBBridge()
  :DBObject("DBBridge", this) {
  sourceCodeTokenStream = NULL;
  currentSentence.clear();
  currentSentenceTokenStream.Reset();
  Pile.Reset();
  currentTree = NULL;
  istr sourceCode = "";
  istr sourcePath = "";
}

// IBridgeִ�н�����
void DBBridge::StartTransaction(DBTransaction& trans, bool isDebug) {
  // ��λ�������ʷ�����
  this->sourceCode = trans.GetCode();
  iLexiana.Reset();
  iLexiana.SetSourceCode(this->sourceCode);
  sourceCodeTokenStream = iLexiana.Analyze();
  if (isDebug) {
    PILEPRINTLN(">>> Lexical Analyzer Output:");
    PILEPRINTLN(*sourceCodeTokenStream);
  }
  currentSentence = sourceCodeTokenStream->NextSentence();
  currentSentenceTokenStream.Reset();
  for (int i = 0; i < (int)currentSentence.size(); i++) {
    currentSentenceTokenStream.Add(currentSentence[i]);
  }
  // �﷨����
  iParser.SetTokenStream(&currentSentenceTokenStream, iLexiana.GetStrVec());
  iParser.iPTRnextSectence = 0;
  if (isDebug) {
    PILEPRINTLN(">>> Parser Output:");
  }
  currentTree = iParser.Parse();
  if (isDebug && currentTree != NULL) {
    PILEPRINTLN(currentTree->ToString());
  }
  // �������
  if (currentTree != NULL) {
    Pile.SetParseTree(currentTree);
    currentProxy = Pile.Semanticer();
    // ����ִ��
    if (isDebug) {
      PILEPRINTLN(">>> Interpreter Output:");
    }
    iDB.Interpreter(currentProxy, trans);
  }
}

// IBridge��ȡ�ļ�����
istr DBBridge::readCode(istr path) {
  ifile infile;
  istr code = "";
  // ���ļ�
  infile.open(path, std::ios::in);
  if (!infile) {
    // ��ʧ�ܣ�����NULL
    PILEPRINTLN("# Exception: Open file failed!");
    return "";
  }
  istr str;
  while (getline(infile, str)) {
    code = code + str + "\n";
  }
  infile.close();
  return code;
}

CSTORE_NS_END
