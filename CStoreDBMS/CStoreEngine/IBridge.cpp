#include "IBridge.h"

// IBridge���캯��
IBridge::IBridge() {
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

// IBridge��������
IBridge* IBridge::Invoke() {
  return iInstance == NULL ? iInstance = new IBridge() : iInstance;
}

// IBridge��ʼ������
void IBridge::Init(int _myArgc, char* _myArgv[]) {
  // û�и��Ӳ����ͽ���������ģʽ
  if (_myArgc == 1) {
    PILEPRINTLN(NEWLINE
      << "SSQL Interpreter v1.0" << NEWLINE
      << "Copyright Group 12 of CAS 2012 SYSU" << NEWLINE
      << "-----------------------------------------" << NEWLINE);
    this->sourcePath = _myArgv[0];
    this->iType = RunType::RUN_CONSOLE;
  }
  // �����Ƕ��ļ������
  else {
    this->sourcePath = _myArgv[1];
    this->iType = RunType::RUN_INFILE;
    this->sourceCode = readCode(this->sourcePath);
  }
}

// IBridgeִ�н�����
void IBridge::StartDash(bool isDebug) {
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
      else if (commandBuilder == "show table;") {
        iDB.ShowTable();
        continue;
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
  else if (this->iType == RunType::RUN_INFILE) {
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

// IBridge��ȡ�ļ�����
istr IBridge::readCode(istr path) {
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

// IBridgeΨһ�����Ķ���
IBridge* IBridge::iInstance = NULL;