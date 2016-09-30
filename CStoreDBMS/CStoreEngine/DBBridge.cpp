#include "DBBridge.h"

// IBridge���캯��
DBBridge::DBBridge()
  :DBObject("DBBridge", this) {
  sourceCodeTokenStream = NULL;
  currentSentence.clear();
  currentSentenceTokenStream.Reset();
  iDB.Reset();
  Pile.Reset();
  currentTree = NULL;
  istr sourceCode = "";
  istr sourcePath = "";
  iType = RunType::RUN_CONSOLE;
}

// IBridge��ʼ������
void DBBridge::Init(RunType gbState, const istr& query) {
  this->iType = gbState;
  this->sourceCode = query;
}

// IBridgeִ�н�����
void DBBridge::StartDash(bool isDebug) {
  // ����̨�������
  if (this->iType == RunType::RUN_CONSOLE) {
    istr commandBuffer;
    bool rflag = true;
    bool breakFlag = false;
    FOREVER {
      // ��ȡ����̨�����ԷֺŽ���
      if (isDebug) {
        PILEPRINT("SSQL# ");
      }
      else {
        PILEPRINT("SSQL> ");
      }
      char charBuilder;
      istr commandBuilder = "";
      do {
        charBuilder = getchar();
        if (charBuilder == '\n') {
          charBuilder = ' ';
          if (rflag) {
            PILEPRINT("    > ");
          }
          rflag = true;
        }
        if (breakFlag) {
          breakFlag = false;
        }
        else {
          commandBuilder += charBuilder;
        }
      } while (charBuilder != SENTENCETERMINATOR);
      breakFlag = true;
      rflag = false;
      // ����������ж�
      if (commandBuilder == "exit;") {
        PILEPRINTLN("Good Bye!");
        return;
      }
      else if (commandBuilder == "debug;") {
        isDebug = true;
        continue;
      }
      else if (commandBuilder == "no debug;") {
        isDebug = false;
        continue;
      }
      // ��λ�������ʷ�����
      sourceCode = commandBuilder;
      iLexiana.Reset();
      iLexiana.SetSourceCode(sourceCode);
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
      currentTree = iParser.Parse();
      if (isDebug) {
        PILEPRINTLN(">>> Parser Output:");
        if (currentTree != NULL) {
          PILEPRINTLN(currentTree->ToString());
        }
      }
      // �������
      if (currentTree != NULL) {
        Pile.SetParseTree(currentTree);
        currentProxy = Pile.Semanticer();
        // ����ִ��
        if (isDebug) {
          PILEPRINTLN(">>> Interpreter Output:");
        }
        iDB.Interpreter(currentProxy);
      }
      if (isDebug) {
        PILEPRINTLN("====================== Interpreted.");
      }
      PILEPRINT(NEWLINE);
    }
  }
  // �ļ��������
  else if (this->iType == RunType::RUN_INFILE || this->iType == RunType::RUN_COMMAND) {
    // �ʷ�����
    iLexiana.SetSourceCode(sourceCode);
    sourceCodeTokenStream = iLexiana.Analyze();
    if (isDebug) {
      PILEPRINTLN(">>> Lexical Analyzer Output:");
      PILEPRINTLN(*sourceCodeTokenStream);
    }
    int spointer = 0;
    FOREVER {
      currentSentence = sourceCodeTokenStream->NextSentence();
      if (!currentSentence.size()) {
        break;
      }
      currentSentenceTokenStream.Reset();
      for (int i = 0; i < (int)currentSentence.size(); i++) {
        currentSentenceTokenStream.Add(currentSentence[i]);
      }
      // �﷨����
      PILEPRINTLN(">> " + iLexiana.GetStrVec()->at(spointer++));
      iParser.SetTokenStream(&currentSentenceTokenStream, iLexiana.GetStrVec());
      currentTree = iParser.Parse();
      if (isDebug) {
        PILEPRINTLN(">>> Parser Output:");
        if (currentTree != NULL) {
          PILEPRINTLN(currentTree->ToString());
        }
      }
      // �������
      if (currentTree != NULL) {
        Pile.SetParseTree(currentTree);
        currentProxy = Pile.Semanticer();
        if (isDebug) {
          PILEPRINTLN(">>> Interpreter Output:");
        }
        // ����ִ��
        iDB.Interpreter(currentProxy);
      }
      if (isDebug) {
        PILEPRINTLN("====================== Interpreted.");
      }
      PILEPRINT(NEWLINE);
    }
    if (this->iType == RunType::RUN_INFILE) {
      // ִ����ϣ��ȴ��س��˳�
      PILEPRINTLN("\n# All interpretation missions accomplished!");
      PILEPRINTLN("# Press Enter to exit");
      char charBuilder;
      FOREVER {
        charBuilder = getchar();
        if (charBuilder == '\n') {
          PILEPRINTLN("# Good Bye!");
          break;
        }
      }
    }
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
