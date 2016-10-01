#include "ISyntaxParser.h"

// ǰ������
SyntaxTreeNode* Homura(SyntaxTreeNode*, void*, CFunctionType, SyntaxType, istr);

// ISyntaxParser��ȡtoken������
TokenStream* ISyntaxParser::GetTokenStream() {
  return istream;
}

// ISyntaxParser���ô�����token������
void ISyntaxParser::SetTokenStream(TokenStream* _myProxy, StrVec* _mySVect) {
  istream = _myProxy;
  this->SetSentencePtr(_mySVect);
}

// ISyntaxParser��ȡsentenceָ��
StrVec* ISyntaxParser::GetSectencePtr() {
  return this->isentence;
}

// ISyntaxParser����sentence����ָ��
void ISyntaxParser::SetSentencePtr(StrVec* _myProxy) {
  this->isentence = _myProxy;
}

// LL1SyntaxParserĬ�Ϲ�����
LL1SyntaxParser::LL1SyntaxParser()
  :DBObject("LL1SyntaxParser", this) {
  this->iPTRnextToken = this->iPTRnextSectence = 0;
  this->iMap = new LL1SyntaxParserMap(LL1PARSERMAPROW, LL1PARSERMAPCOL);
  this->InitMap();
}

// LL1SyntaxParser��token�����캯��
LL1SyntaxParser::LL1SyntaxParser(TokenStream *_myProxy, StrVec* _mySVect)
  :DBObject("LL1SyntaxParser", this) {
  this->SetTokenStream(_myProxy, _mySVect);
  this->iPTRnextToken = this->iPTRnextSectence = 0;
  this->iMap = new LL1SyntaxParserMap(LL1PARSERMAPROW, LL1PARSERMAPCOL);
  this->InitMap();
}

// LL1SyntaxParser��λ����
void LL1SyntaxParser::Reset() {
  // �������ڻ�
  this->iPTRnextToken = 0;
  //this->iPTRnextSectence = 0;
  while (!this->iParseStack.empty()) {
    this->iParseStack.pop();
  }
  // ������ʼ�ڵ�
  iParseStack.push(SyntaxType::tail_startEndLeave);
  iParseStack.push(SyntaxType::case_ssql_stmt);
  // ����token���ǿհ׵�ֱ�ӷ�һ������������
  if (this->GetTokenStream()->Length() == 0) {
    Token* ccToken = new Token();
    ccToken->indexOfCode = 0;
    ccToken->aColumn = 0;
    ccToken->aLine = 0;
    ccToken->length = 1;
    ccToken->detail = '#';
    ccToken->errorCode = 0;
    ccToken->aType = TokenType::token_startEnd;
    this->GetTokenStream()->Add(ccToken);
  }
  // ������׷��׷��һ�����������ǵ�token
  else {
    Token* lastToken = this->GetTokenStream()->_tokenContainer[this->GetTokenStream()->Length() - 1];
    Token* ccToken = new Token();
    ccToken->indexOfCode = lastToken->indexOfCode + lastToken->length + 1;
    ccToken->aColumn = lastToken->aColumn + lastToken->length + 1;
    ccToken->aLine = lastToken->aLine;
    ccToken->length = 1;
    ccToken->detail = '#';
    ccToken->errorCode = 0;
    ccToken->aType = TokenType::token_startEnd;
    this->GetTokenStream()->Add(ccToken);
  }
}

// LL1SyntaxParser����ָ��
void LL1SyntaxParser::Dash() {
  this->iPTRnextSectence++;
}

// LL1SyntaxParser����CSTORE���Ᵽ����
bool LL1SyntaxParser::CSTOREQL(Token* xtoken, SyntaxTreeNode*& curRoot) {
  curRoot->nodeType = CFunctionType::umi_cstore;
  switch (xtoken->aType) {
  case TokenType::token_load:
    curRoot->nodeSyntaxType = SyntaxType::cstore_load;
    // require identifier
    if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
      Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
      if (tableNameToken->aType == TokenType::token_iden) {
        curRoot->nodeValue = tableNameToken->detail;
      }
      else {
        PILEPRINTLN("Load command should follow with a file *.tbl");
        curRoot = NULL;
        return true;
      }
      this->iPTRnextToken++;
      curRoot->nodeName = "cstore_load";
      return true;
    }
    else {
      PILEPRINTLN("Load command should follow with a file *.tbl");
      curRoot = NULL;
      return true;
    }
    break;
  case TokenType::token_retrieve:
    curRoot->nodeSyntaxType = SyntaxType::cstore_retrieve;
    // require identifier
    if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
      Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
      if (tableNameToken->aType == TokenType::token_iden) {
        curRoot->nodeValue = tableNameToken->detail;
      }
      else {
        TRACE("Retrieve command should follow with a table name");
        curRoot = NULL;
        return true;
      }
      if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
        Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
        if (tableNameToken->aType == TokenType::number) {
          curRoot->nodeValue += "@" + tableNameToken->detail;
        }
        else {
          TRACE("Retrieve command should end with a primary key value");
          curRoot = NULL;
          return true;
        }
        this->iPTRnextToken++;
        curRoot->nodeName = "cstore_retrieve";
        return true;
      }
      else {
        TRACE("Retrieve command should end with a primary key value");
        curRoot = NULL;
        return true;
      }
    }
    else {
      TRACE("Retrieve command should follow with a table name");
      curRoot = NULL;
      return true;
    }
    break;
  case TokenType::token_compress:
    curRoot->nodeSyntaxType = SyntaxType::cstore_compress;
    // require identifier
    if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
      Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
      if (tableNameToken->aType == TokenType::token_iden) {
        curRoot->nodeValue = tableNameToken->detail;
      }
      else {
        TRACE("Compress command should follow with tablename");
        curRoot = NULL;
        return true;
      }
      this->iPTRnextToken++;
      curRoot->nodeName = "cstore_compress";
      return true;
    }
    else {
      TRACE("Compress command should follow with tablename");
      curRoot = NULL;
      return true;
    }
    return true;
  case TokenType::token_join:
    curRoot->nodeSyntaxType = SyntaxType::cstore_join;
    // require identifier
    if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
      Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
      if (tableNameToken->aType == TokenType::token_iden) {
        curRoot->nodeValue = tableNameToken->detail;
      }
      else {
        TRACE("Join command should follow with a table name");
        curRoot = NULL;
        return true;
      }
      if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
        Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
        if (tableNameToken->aType == TokenType::token_iden) {
          curRoot->nodeValue += "@" + tableNameToken->detail;
        }
        else {
          TRACE("Join command should end with a table name");
          curRoot = NULL;
          return true;
        }
        this->iPTRnextToken++;
        curRoot->nodeName = "cstore_join";
        return true;
      }
      else {
        TRACE("Join command should end with a table name");
        curRoot = NULL;
        return true;
      }
    }
    else {
      TRACE("Join command should follow with a table name");
      curRoot = NULL;
      return true;
    }
    break;
  case TokenType::token_count:
    curRoot->nodeSyntaxType = SyntaxType::cstore_count;
    // require identifier
    if (this->GetTokenStream()->_tokenContainer.size() > this->iPTRnextToken + 1) {
      Token* tableNameToken = this->GetTokenStream()->_tokenContainer[++this->iPTRnextToken];
      if (tableNameToken->aType == TokenType::token_iden) {
        curRoot->nodeValue = tableNameToken->detail;
      }
      else {
        TRACE("Count command should follow with a table name");
        curRoot = NULL;
        return true;
      }
      this->iPTRnextToken++;
      curRoot->nodeName = "cstore_count";
      return true;
    }
    else {
      TRACE("Count command should follow with a table name");
      curRoot = NULL;
      return true;
    }
    break;
  default:
    return false;
  }
  return true;
}

// LL1SyntaxParser������
SyntaxTreeNode* LL1SyntaxParser::Parse() {
  // ���ڻ�
  this->Reset();
  // ԭʼ�ڵ�
  SyntaxTreeNode* parsePointer = new SyntaxTreeNode();
  register SyntaxTreeNode* currentPtr = parsePointer;
  // ����CStore���Ᵽ����
  bool spottedCQL = this->CSTOREQL(this->GetTokenStream()->_tokenContainer[iPTRnextToken], parsePointer);
  // ����CStore�ؼ���
  if (spottedCQL == true) {
    // �﷨����ʱ
    if (parsePointer == NULL) {
      return NULL;
    }
    return parsePointer;
  }
  // ����ջȥ�ݹ��½�
  while (iPTRnextToken < this->GetTokenStream()->Length()) {
    // ��Ԥ�����ò���ʽ������
    SyntaxType nodeType = this->iParseStack.top();
    Token* iToken = this->GetTokenStream()->_tokenContainer[iPTRnextToken];
    TokenType tokenType = iToken->aType;
    CandidateFunction* func = this->iMap->GetCFunction(nodeType, tokenType, Homura);
    // �﷨����ʱ
    if (func->GetType() == CFunctionType::umi_errorEnd) {
      // ���ô�����
      this->iException();
      // ��Ծ��������
      this->Dash();
      return NULL;
    }
    // ������ڷ��ս�������������ĺ�ѡʽ
    if (currentPtr != NULL) {
      currentPtr->candidateFunction = func;
    }
    // ���ò���ʽ���½�
    if (func != NULL) {
      if (currentPtr != NULL) {
        currentPtr = currentPtr->candidateFunction->Call(currentPtr, this, nodeType, iToken->detail);
      }
      else {
        currentPtr = Homura(currentPtr, this, func->GetType(), nodeType, iToken->detail);
      }
    }
    // û�ж�Ӧ�ĺ�ѡʽʱ
    else {
      if (currentPtr != NULL) {
        currentPtr->errorCode = 1;
      }
      break;
    }
  }
  // ������Ϣ
  parsePointer->mappedLength = this->GetTokenStream()->Length();
  // ��Ծָ��
  this->Dash();
  return parsePointer;
}

// LL1SyntaxParser��һ�ڵ�
SyntaxTreeNode* LL1SyntaxParser::NextNode(SyntaxTreeNode* _res, LL1SyntaxParser* _parser) {
  // �Ѿ�û����Ҫ��չ�Ľڵ��˾ͷ��ؿ�
  if (_res == NULL) {
    return NULL;
  }
  // ����ȡ�丸�ڵ���ȡ�����ֵܽڵ�
  SyntaxTreeNode* parent = _res->parent;
  // ���޸�ĸ�ڵ���Ѿ����ݹ��˻ص����ϲ���
  if (parent == NULL) {
    return NULL;
  }
  int i = 0;
  // ����Ѱ���Լ��ڽ����е���λ
  for (; i < (int)parent->children.size(); i++) {
    if (parent->children[i] == _res) {
      break;
    }
  }
  // �����Լ���ѡ�����ýڵ���Ϊ��һ��չ���ڵ�
  if (i + 1 < (int)parent->children.size()) {
    return parent->children[i + 1];
  }
  // �Ѿ�û�б��Լ�ͬ����С�Ľڵ㣬�͵ݹ�ȥ�Ҹ�ĸ�����ֵܽ���
  else {
    if (i + 1 == parent->children.size()) {
      parent->mappedLength = _parser->iPTRnextToken - parent->mappedBegin;
    }
    return NextNode(parent, _parser);
  }
  return NULL;
}

// LL1SyntaxParser������
void LL1SyntaxParser::iException() {
  PILEPRINT("# Syntax Error: At ("
    << this->GetTokenStream()->_tokenContainer[iPTRnextToken]->aLine << ", "
    << this->GetTokenStream()->_tokenContainer[iPTRnextToken]->aColumn << ")"
    << ", which Token detail: " << this->GetTokenStream()->_tokenContainer[iPTRnextToken]->detail << NEWLINE
    << "# In sentence:  " << this->GetSectencePtr()->at(iPTRnextSectence) << NEWLINE << "#");
  for (int i = 0; i < this->GetTokenStream()->_tokenContainer[iPTRnextToken]->aColumn + 14; i++) {
    PILEPRINT(" ");
  }
  PILEPRINTLN("^" << NEWLINE
    << "# Cannot map any syntax type, this sentence will be ignored."
    << NEWLINE);
}

// LL1SyntaxParser��ʼ��Ԥ�������
void LL1SyntaxParser::InitMap() {
  this->InitLinkerVector();
  this->InitMapProperties();
  this->InitCellular();
}

// LL1SyntaxParserȡ������
SyntaxVector LL1SyntaxParser::GetVector(CFunctionType _type) const {
  return this->iDict[_type];
}

// LL1SyntaxParserȡջָ��
SyntaxStack* LL1SyntaxParser::GetStack() {
  return &(this->iParseStack);
}

// LL1SyntaxParser��ʼ����������
void LL1SyntaxParser::InitLinkerVector() {
  this->iDict.resize(192);
  // <ssql_stmt> -> <create_stmt>
  this->iDict[CFunctionType::deri___ssql_stmt__create_stmt_1].push_back(SyntaxType::case_create_stmt);
  // <ssql_stmt> -> <insert_stmt>
  this->iDict[CFunctionType::deri___ssql_stmt__insert_stmt_2].push_back(SyntaxType::case_insert_stmt);
  // <ssql_stmt> -> <delete_stmt>
  this->iDict[CFunctionType::deri___ssql_stmt__delete_stmt_3].push_back(SyntaxType::case_delete_stmt);
  // <ssql_stmt> -> <query_stmt>
  this->iDict[CFunctionType::deri___ssql_stmt__query_stmt_4].push_back(SyntaxType::case_query_stmt);
  // <create_stmt> -> "create" "table" id "(" <decl_list> ")" ";"
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_createLeave);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_tableLeave);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::case_decl_list);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_rightParentheses_Leave);
  this->iDict[CFunctionType::deri___create_stmt__decl_list_5].push_back(SyntaxType::tail_semicolon_Leave);
  // <decl_list> -> <decl> <decl_listpi>
  this->iDict[CFunctionType::deri___decl_list__decl__decl_listpi_6].push_back(SyntaxType::case_decl);
  this->iDict[CFunctionType::deri___decl_list__decl__decl_listpi_6].push_back(SyntaxType::case_decl_listpi);
  // <decl_listpi> -> "," <decl> <decl_listpi>
  this->iDict[CFunctionType::deri___decl_listpi__decl__decl_listpi_67].push_back(SyntaxType::tail_comma_Leave);
  this->iDict[CFunctionType::deri___decl_listpi__decl__decl_listpi_67].push_back(SyntaxType::case_decl);
  this->iDict[CFunctionType::deri___decl_listpi__decl__decl_listpi_67].push_back(SyntaxType::case_decl_listpi);
  // <decl_listpi> -> epsilon
  this->iDict[CFunctionType::deri___decl_listpi__epsilon_7].push_back(SyntaxType::epsilonLeave);
  // <decl> -> id "int" <default_spec>
  this->iDict[CFunctionType::deri___decl__default_spec_8].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___decl__default_spec_8].push_back(SyntaxType::tail_intLeave);
  this->iDict[CFunctionType::deri___decl__default_spec_8].push_back(SyntaxType::case_default_spec);
  // <decl> -> "primary" "key" "(" <column_list> ")"
  this->iDict[CFunctionType::deri___decl__column_list_9].push_back(SyntaxType::tail_primaryLeave);
  this->iDict[CFunctionType::deri___decl__column_list_9].push_back(SyntaxType::tail_keyLeave);
  this->iDict[CFunctionType::deri___decl__column_list_9].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___decl__column_list_9].push_back(SyntaxType::case_column_list);
  this->iDict[CFunctionType::deri___decl__column_list_9].push_back(SyntaxType::tail_rightParentheses_Leave);
  // <default_spec> -> "default" "=" sexpr
  this->iDict[CFunctionType::deri___default_spec__sexpr_10].push_back(SyntaxType::tail_defaultLeave);
  this->iDict[CFunctionType::deri___default_spec__sexpr_10].push_back(SyntaxType::tail_equality_Leave);
  this->iDict[CFunctionType::deri___default_spec__sexpr_10].push_back(SyntaxType::case_sexpr);
  // <default_spec> -> epsilon
  this->iDict[CFunctionType::deri___default_spec__epsilon_11].push_back(SyntaxType::epsilonLeave);
  // <sexpr> -> <smulti> <sexpr_pi>
  this->iDict[CFunctionType::deri___sexpr__smulti__sexpr_pi_12].push_back(SyntaxType::case_smulti);
  this->iDict[CFunctionType::deri___sexpr__smulti__sexpr_pi_12].push_back(SyntaxType::case_sexpr_pi);
  // <splus> -> "+" <smulti>
  this->iDict[CFunctionType::deri___splus__plus_smulti_14].push_back(SyntaxType::tail_plus_Leave);
  this->iDict[CFunctionType::deri___splus__plus_smulti_14].push_back(SyntaxType::case_smulti);
  // <splus> -> "-" <smulti>
  this->iDict[CFunctionType::deri___splus__minus_smulti_15].push_back(SyntaxType::tail_minus_Leave);
  this->iDict[CFunctionType::deri___splus__minus_smulti_15].push_back(SyntaxType::case_smulti);
  // <sexpr_pi> -> <splus> <sexpr_pi>
  this->iDict[CFunctionType::deri___sexpr_pi__splus__sexpr_pi_70].push_back(SyntaxType::case_splus);
  this->iDict[CFunctionType::deri___sexpr_pi__splus__sexpr_pi_70].push_back(SyntaxType::case_sexpr_pi);
  // <sexpr_pi> -> epsilon
  this->iDict[CFunctionType::deri___sexpr_pi__epsilon_71].push_back(SyntaxType::epsilonLeave);
  // <smulti> -> <sunit> <smultiOpt>
  this->iDict[CFunctionType::deri___smulti__sunit__smultiOpt_17].push_back(SyntaxType::case_sunit);
  this->iDict[CFunctionType::deri___smulti__sunit__smultiOpt_17].push_back(SyntaxType::case_smultiOpt);
  // <smultiOpt> -> "*" <sunit>
  this->iDict[CFunctionType::deri___smultiOpt__multi_sunit__smultiOpt_18].push_back(SyntaxType::tail_multiply_Leave);
  this->iDict[CFunctionType::deri___smultiOpt__multi_sunit__smultiOpt_18].push_back(SyntaxType::case_sunit);
  this->iDict[CFunctionType::deri___smultiOpt__multi_sunit__smultiOpt_18].push_back(SyntaxType::case_smultiOpt);
  // <smultiOpt> -> "/" <sunit>
  this->iDict[CFunctionType::deri___smultiOpt__div_sunit__smultiOpt_19].push_back(SyntaxType::tail_divide_Leave);
  this->iDict[CFunctionType::deri___smultiOpt__div_sunit__smultiOpt_19].push_back(SyntaxType::case_sunit);
  this->iDict[CFunctionType::deri___smultiOpt__div_sunit__smultiOpt_19].push_back(SyntaxType::case_smultiOpt);
  // <smultiOpt> -> epsilon
  this->iDict[CFunctionType::deri___smultiOpt__epsilon_20].push_back(SyntaxType::epsilonLeave);
  // <sunit> -> number
  this->iDict[CFunctionType::deri___sunit__number_21].push_back(SyntaxType::numberLeave);
  // <sunit> -> "-" <sunit>
  this->iDict[CFunctionType::deri___sunit__minus_sunit_22].push_back(SyntaxType::tail_minus_Leave);
  this->iDict[CFunctionType::deri___sunit__minus_sunit_22].push_back(SyntaxType::case_sunit);
  // <sunit> -> "+" <sunit>
  this->iDict[CFunctionType::deri___sunit__plus_sunit_23].push_back(SyntaxType::tail_plus_Leave);
  this->iDict[CFunctionType::deri___sunit__plus_sunit_23].push_back(SyntaxType::case_sunit);
  // <sunit> -> "(" <sexpr> ")"
  this->iDict[CFunctionType::deri___sunit__brucket_sexpr_24].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___sunit__brucket_sexpr_24].push_back(SyntaxType::case_sexpr);
  this->iDict[CFunctionType::deri___sunit__brucket_sexpr_24].push_back(SyntaxType::tail_rightParentheses_Leave);
  // <insert_stmt> -> "insert" "into" id "(" <column_list> ")" "values" "(" <value_list> ")" ";"
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_insertLeave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_intoLeave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::case_column_list);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_rightParentheses_Leave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_valuesLeave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::case_value_list);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_rightParentheses_Leave);
  this->iDict[CFunctionType::deri___insert_stmt__column_list__value_list_28].push_back(SyntaxType::tail_semicolon_Leave);
  // <value_list> -> <sexpr> <value_listpi>
  this->iDict[CFunctionType::deri___value_list__sexpr__value_listpi_29].push_back(SyntaxType::case_sexpr);
  this->iDict[CFunctionType::deri___value_list__sexpr__value_listpi_29].push_back(SyntaxType::case_value_listpi);
  // <value_listpi> -> "," <sexpr> <value_listpi>
  this->iDict[CFunctionType::deri___value_listpi__sexpr__value_listpi_30].push_back(SyntaxType::tail_comma_Leave);
  this->iDict[CFunctionType::deri___value_listpi__sexpr__value_listpi_30].push_back(SyntaxType::case_sexpr);
  this->iDict[CFunctionType::deri___value_listpi__sexpr__value_listpi_30].push_back(SyntaxType::case_value_listpi);
  // <value_listpi> -> epsilon
  this->iDict[CFunctionType::deri___value_listpi__epsilon_31].push_back(SyntaxType::epsilonLeave);
  // <delete_stmt> -> "delete" "from" id <where_clause> ";"
  this->iDict[CFunctionType::deri___delete_stmt__where_clause_32].push_back(SyntaxType::tail_deleteLeave);
  this->iDict[CFunctionType::deri___delete_stmt__where_clause_32].push_back(SyntaxType::tail_fromLeave);
  this->iDict[CFunctionType::deri___delete_stmt__where_clause_32].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___delete_stmt__where_clause_32].push_back(SyntaxType::case_where_clause);
  this->iDict[CFunctionType::deri___delete_stmt__where_clause_32].push_back(SyntaxType::tail_semicolon_Leave);
  // <where_clause> -> "where" <disjunct>
  this->iDict[CFunctionType::deri___where_clause__disjunct_33].push_back(SyntaxType::tail_whereLeave);
  this->iDict[CFunctionType::deri___where_clause__disjunct_33].push_back(SyntaxType::case_disjunct);
  // <where_clause> -> epsilon
  this->iDict[CFunctionType::deri___where_clause__epsilon_34].push_back(SyntaxType::epsilonLeave);
  // <disjunct> -> <conjunct> <disjunct_pi>
  this->iDict[CFunctionType::deri___disjunct__conjunct__disjunct_pi_35].push_back(SyntaxType::case_conjunct);
  this->iDict[CFunctionType::deri___disjunct__conjunct__disjunct_pi_35].push_back(SyntaxType::case_disjunct_pi);
  // <disjunct_pi> -> "||" <conjunct> <disjunct_pi>
  this->iDict[CFunctionType::deri___disjunct_pi__conjunct__disjunct_pi_36].push_back(SyntaxType::tail_or_Or_Leave);
  this->iDict[CFunctionType::deri___disjunct_pi__conjunct__disjunct_pi_36].push_back(SyntaxType::case_conjunct);
  this->iDict[CFunctionType::deri___disjunct_pi__conjunct__disjunct_pi_36].push_back(SyntaxType::case_disjunct_pi);
  // <disjunct_pi> -> epsilon
  this->iDict[CFunctionType::deri___disjunct_pi__epsilon_37].push_back(SyntaxType::epsilonLeave);
  // <conjunct> -> <bool> <conjunct_pi>
  this->iDict[CFunctionType::deri___conjunct__bool__conjunct_pi_38].push_back(SyntaxType::case_bool);
  this->iDict[CFunctionType::deri___conjunct__bool__conjunct_pi_38].push_back(SyntaxType::case_conjunct_pi);
  // <conjunct_pi> -> "&&" <bool> <conjunct_pi>
  this->iDict[CFunctionType::deri___conjunct_pi__bool__conjunct_pi_39].push_back(SyntaxType::tail_and_And_Leave);
  this->iDict[CFunctionType::deri___conjunct_pi__bool__conjunct_pi_39].push_back(SyntaxType::case_bool);
  this->iDict[CFunctionType::deri___conjunct_pi__bool__conjunct_pi_39].push_back(SyntaxType::case_conjunct_pi);
  // <conjunct_pi> -> epsilon
  this->iDict[CFunctionType::deri___conjunct_pi__epsilon_40].push_back(SyntaxType::epsilonLeave);
  // <bool> -> "!" <bool>
  this->iDict[CFunctionType::deri___bool__not_bool_42].push_back(SyntaxType::tail_not_Leave);
  this->iDict[CFunctionType::deri___bool__not_bool_42].push_back(SyntaxType::case_bool);
  // <bool> -> <comp>
  this->iDict[CFunctionType::deri___bool__comp_43].push_back(SyntaxType::case_comp);
  // <comp> -> <wexpr> <rop> <wexpr>
  this->iDict[CFunctionType::deri___comp__wexpr__rop__wexpr_44].push_back(SyntaxType::case_wexpr);
  this->iDict[CFunctionType::deri___comp__wexpr__rop__wexpr_44].push_back(SyntaxType::case_rop);
  this->iDict[CFunctionType::deri___comp__wexpr__rop__wexpr_44].push_back(SyntaxType::case_wexpr);
  // <rop> -> "<>"
  this->iDict[CFunctionType::deri___rop__lessgreater_58].push_back(SyntaxType::tail_lessThan_GreaterThan_Leave);
  // <rop> -> "=="
  this->iDict[CFunctionType::deri___rop__equalequal_59].push_back(SyntaxType::tail_equality_Equality_Leave);
  // <rop> -> ">"
  this->iDict[CFunctionType::deri___rop__greater_60].push_back(SyntaxType::tail_greaterThan_Leave);
  // <rop> -> "<"
  this->iDict[CFunctionType::deri___rop__less_61].push_back(SyntaxType::tail_lessThan_Leave);
  // <rop> -> ">="
  this->iDict[CFunctionType::deri___rop__greaterequal_62].push_back(SyntaxType::tail_greaterThan_Equality_Leave);
  // <rop> -> "<="
  this->iDict[CFunctionType::deri___rop__lessequal_63].push_back(SyntaxType::tail_lessThan_Equality_Leave);
  // <rop> -> epsilon
  this->iDict[CFunctionType::deri___rop__epsilon_80].push_back(SyntaxType::epsilonLeave);
  // <wexpr> -> <wmulti> <wexpr_pi>
  this->iDict[CFunctionType::deri___wexpr__wmulti__wexpr_pi_45].push_back(SyntaxType::case_wmulti);
  this->iDict[CFunctionType::deri___wexpr__wmulti__wexpr_pi_45].push_back(SyntaxType::case_wexpr_pi);
  // <wexpr> -> epsilon
  this->iDict[CFunctionType::deri___wexpr__epsilon_81].push_back(SyntaxType::epsilonLeave);
  // <wplus> -> "+" <wmulti>
  this->iDict[CFunctionType::deri___wplus__plus_wmulti_46].push_back(SyntaxType::tail_plus_Leave);
  this->iDict[CFunctionType::deri___wplus__plus_wmulti_46].push_back(SyntaxType::case_wmulti);
  // <wplus> -> "-" <wmulti>
  this->iDict[CFunctionType::deri___wplus__minus_wmulti_47].push_back(SyntaxType::tail_minus_Leave);
  this->iDict[CFunctionType::deri___wplus__minus_wmulti_47].push_back(SyntaxType::case_wmulti);
  // <wexpr_pi> -> <wplus> <wexpr_pi>
  this->iDict[CFunctionType::deri___wexpr_pi__wplus__wexpr_pi_72].push_back(SyntaxType::case_wplus);
  this->iDict[CFunctionType::deri___wexpr_pi__wplus__wexpr_pi_72].push_back(SyntaxType::case_wexpr_pi);
  // <wexpr_pi> -> epsilon
  this->iDict[CFunctionType::deri___wexpr_pi__epsilon_73].push_back(SyntaxType::epsilonLeave);
  // <wmulti> -> <wunit> <wmultiOpt>
  this->iDict[CFunctionType::deri___wmulti__wunit__wmultiOpt_49].push_back(SyntaxType::case_wunit);
  this->iDict[CFunctionType::deri___wmulti__wunit__wmultiOpt_49].push_back(SyntaxType::case_wmultiOpt);
  // <wmultiOpt> -> "*" <wunit>
  this->iDict[CFunctionType::deri___wmultiOpt__multi_wunit__wmultiOpt_50].push_back(SyntaxType::tail_multiply_Leave);
  this->iDict[CFunctionType::deri___wmultiOpt__multi_wunit__wmultiOpt_50].push_back(SyntaxType::case_wunit);
  this->iDict[CFunctionType::deri___wmultiOpt__multi_wunit__wmultiOpt_50].push_back(SyntaxType::case_wmultiOpt);
  // <wmultiOpt> -> "/" <wunit>
  this->iDict[CFunctionType::deri___wmultiOpt__div_wunit__wmultiOpt_51].push_back(SyntaxType::tail_divide_Leave);
  this->iDict[CFunctionType::deri___wmultiOpt__div_wunit__wmultiOpt_51].push_back(SyntaxType::case_wunit);
  this->iDict[CFunctionType::deri___wmultiOpt__div_wunit__wmultiOpt_51].push_back(SyntaxType::case_wmultiOpt);
  // <wmultiOpt> -> epsilon
  this->iDict[CFunctionType::deri___wmultiOpt__epsilon_52].push_back(SyntaxType::epsilonLeave);
  // <wunit> -> number
  this->iDict[CFunctionType::deri___wunit__number_53].push_back(SyntaxType::numberLeave);
  // <wunit> -> iden
  this->iDict[CFunctionType::deri___wunit__iden_54].push_back(SyntaxType::tail_idenLeave);
  // <wunit> -> "-" <wunit>
  this->iDict[CFunctionType::deri___wunit__minus_wunit_55].push_back(SyntaxType::tail_minus_Leave);
  this->iDict[CFunctionType::deri___wunit__minus_wunit_55].push_back(SyntaxType::case_wunit);
  // <wunit> -> "+" <wunit>
  this->iDict[CFunctionType::deri___wunit__plus_wunit_56].push_back(SyntaxType::tail_plus_Leave);
  this->iDict[CFunctionType::deri___wunit__plus_wunit_56].push_back(SyntaxType::case_wunit);
  // <wunit> -> "(" <disjunct> ")"
  this->iDict[CFunctionType::deri___wunit__brucket_disjunct_57].push_back(SyntaxType::tail_leftParentheses_Leave);
  this->iDict[CFunctionType::deri___wunit__brucket_disjunct_57].push_back(SyntaxType::case_disjunct);
  this->iDict[CFunctionType::deri___wunit__brucket_disjunct_57].push_back(SyntaxType::tail_rightParentheses_Leave);
  // <query_stmt> -> "select" <select_list> "from" id <where_clause> ";"
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::tail_selectLeave);
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::case_select_list);
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::tail_fromLeave);
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::case_where_clause);
  this->iDict[CFunctionType::deri___query_stmt__select_list__where_clause_64].push_back(SyntaxType::tail_semicolon_Leave);
  // <select_list> -> <column_list>
  this->iDict[CFunctionType::deri___select_list__column_list_65].push_back(SyntaxType::case_column_list);
  // <select_list> -> "*"
  this->iDict[CFunctionType::deri___select_list__star_66].push_back(SyntaxType::tail_multiply_Leave);
  // <column_list> -> id <column_pi>
  this->iDict[CFunctionType::deri___column_list__column_pi_25].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___column_list__column_pi_25].push_back(SyntaxType::case_column_pi);
  // <column_pi> -> "," id <column_pi>
  this->iDict[CFunctionType::deri___column_pi__comma_column_pi_26].push_back(SyntaxType::tail_comma_Leave);
  this->iDict[CFunctionType::deri___column_pi__comma_column_pi_26].push_back(SyntaxType::tail_idenLeave);
  this->iDict[CFunctionType::deri___column_pi__comma_column_pi_26].push_back(SyntaxType::case_column_pi);
  // <column_pi> -> epsilon
  this->iDict[CFunctionType::deri___column_pi__epsilon_27].push_back(SyntaxType::epsilonLeave);
}

// LL1SyntaxParser��ʼ������������
void LL1SyntaxParser::InitMapProperties() {
  // ���������ԣ����ս��
  iMap->SetRow(0, SyntaxType::case_ssql_stmt);
  iMap->SetRow(1, SyntaxType::case_create_stmt);
  iMap->SetRow(2, SyntaxType::case_decl_list);
  iMap->SetRow(3, SyntaxType::case_decl_listpi);
  iMap->SetRow(4, SyntaxType::case_decl);
  iMap->SetRow(5, SyntaxType::case_default_spec);
  iMap->SetRow(6, SyntaxType::case_sexpr);
  iMap->SetRow(7, SyntaxType::case_sexpr_pi);
  iMap->SetRow(8, SyntaxType::case_splus);
  iMap->SetRow(9, SyntaxType::case_smulti);
  iMap->SetRow(10, SyntaxType::case_smultiOpt);
  iMap->SetRow(11, SyntaxType::case_sunit);
  iMap->SetRow(12, SyntaxType::case_insert_stmt);
  iMap->SetRow(13, SyntaxType::case_value_list);
  iMap->SetRow(14, SyntaxType::case_value_listpi);
  iMap->SetRow(15, SyntaxType::case_delete_stmt);
  iMap->SetRow(16, SyntaxType::case_where_clause);
  iMap->SetRow(17, SyntaxType::case_disjunct);
  iMap->SetRow(18, SyntaxType::case_disjunct_pi);
  iMap->SetRow(19, SyntaxType::case_conjunct);
  iMap->SetRow(20, SyntaxType::case_conjunct_pi);
  iMap->SetRow(21, SyntaxType::case_bool);
  iMap->SetRow(22, SyntaxType::case_comp);
  iMap->SetRow(23, SyntaxType::case_rop);
  iMap->SetRow(24, SyntaxType::case_wexpr);
  iMap->SetRow(25, SyntaxType::case_wexpr_pi);
  iMap->SetRow(26, SyntaxType::case_wplus);
  iMap->SetRow(27, SyntaxType::case_wmulti);
  iMap->SetRow(28, SyntaxType::case_wmultiOpt);
  iMap->SetRow(29, SyntaxType::case_wunit);
  iMap->SetRow(30, SyntaxType::case_query_stmt);
  iMap->SetRow(31, SyntaxType::case_select_list);
  iMap->SetRow(32, SyntaxType::case_column_list);
  iMap->SetRow(33, SyntaxType::case_column_pi);
  // ���������ԣ��ս��
  iMap->SetRow(34, SyntaxType::tail_createLeave);
  iMap->SetRow(35, SyntaxType::tail_tableLeave);
  iMap->SetRow(36, SyntaxType::tail_idenLeave);
  iMap->SetRow(37, SyntaxType::tail_leftParentheses_Leave);
  iMap->SetRow(38, SyntaxType::tail_rightParentheses_Leave);
  iMap->SetRow(39, SyntaxType::tail_semicolon_Leave);
  iMap->SetRow(40, SyntaxType::tail_comma_Leave);
  iMap->SetRow(41, SyntaxType::epsilonLeave);
  iMap->SetRow(42, SyntaxType::tail_intLeave);
  iMap->SetRow(43, SyntaxType::tail_primaryLeave);
  iMap->SetRow(44, SyntaxType::tail_keyLeave);
  iMap->SetRow(45, SyntaxType::tail_defaultLeave);
  iMap->SetRow(46, SyntaxType::tail_equality_Leave);
  iMap->SetRow(47, SyntaxType::tail_plus_Leave);
  iMap->SetRow(48, SyntaxType::tail_minus_Leave);
  iMap->SetRow(49, SyntaxType::tail_multiply_Leave);
  iMap->SetRow(50, SyntaxType::tail_divide_Leave);
  iMap->SetRow(51, SyntaxType::numberLeave);
  iMap->SetRow(52, SyntaxType::tail_insertLeave);
  iMap->SetRow(53, SyntaxType::tail_intoLeave);
  iMap->SetRow(54, SyntaxType::tail_valuesLeave);
  iMap->SetRow(55, SyntaxType::tail_deleteLeave);
  iMap->SetRow(56, SyntaxType::tail_fromLeave);
  iMap->SetRow(57, SyntaxType::tail_whereLeave);
  iMap->SetRow(58, SyntaxType::tail_or_Or_Leave);
  iMap->SetRow(59, SyntaxType::tail_and_And_Leave);
  iMap->SetRow(60, SyntaxType::tail_not_Leave);
  iMap->SetRow(61, SyntaxType::tail_lessThan_GreaterThan_Leave);
  iMap->SetRow(62, SyntaxType::tail_equality_Equality_Leave);
  iMap->SetRow(63, SyntaxType::tail_greaterThan_Leave);
  iMap->SetRow(64, SyntaxType::tail_lessThan_Leave);
  iMap->SetRow(65, SyntaxType::tail_greaterThan_Equality_Leave);
  iMap->SetRow(66, SyntaxType::tail_lessThan_Equality_Leave);
  iMap->SetRow(67, SyntaxType::tail_selectLeave);
  iMap->SetRow(68, SyntaxType::tail_startEndLeave);
  // ���������ԣ���ǰ����һ��token
  iMap->SetCol(0, TokenType::token_create);
  iMap->SetCol(1, TokenType::token_table);
  iMap->SetCol(2, TokenType::token_iden);
  iMap->SetCol(3, TokenType::token_LeftParentheses_);
  iMap->SetCol(4, TokenType::token_RightParentheses_);
  iMap->SetCol(5, TokenType::token_Semicolon_);
  iMap->SetCol(6, TokenType::token_Comma_);
  iMap->SetCol(7, TokenType::epsilon);
  iMap->SetCol(8, TokenType::token_int);
  iMap->SetCol(9, TokenType::token_primary);
  iMap->SetCol(10, TokenType::token_key);
  iMap->SetCol(11, TokenType::token_default);
  iMap->SetCol(12, TokenType::token_Equality_);
  iMap->SetCol(13, TokenType::token_Plus_);
  iMap->SetCol(14, TokenType::token_Minus_);
  iMap->SetCol(15, TokenType::token_Multiply_);
  iMap->SetCol(16, TokenType::token_Divide_);
  iMap->SetCol(17, TokenType::number);
  iMap->SetCol(18, TokenType::token_insert);
  iMap->SetCol(19, TokenType::token_into);
  iMap->SetCol(20, TokenType::token_values);
  iMap->SetCol(21, TokenType::token_delete);
  iMap->SetCol(22, TokenType::token_from);
  iMap->SetCol(23, TokenType::token_where);
  iMap->SetCol(24, TokenType::token_Or_Or_);
  iMap->SetCol(25, TokenType::token_And_And_);
  iMap->SetCol(26, TokenType::token_Not_);
  iMap->SetCol(27, TokenType::token_LessThan_GreaterThan_);
  iMap->SetCol(28, TokenType::token_Equality_Equality_);
  iMap->SetCol(29, TokenType::token_GreaterThan_);
  iMap->SetCol(30, TokenType::token_LessThan_);
  iMap->SetCol(31, TokenType::token_GreaterThan_Equality_);
  iMap->SetCol(32, TokenType::token_LessThan_Equality_);
  iMap->SetCol(33, TokenType::token_select);
  iMap->SetCol(34, TokenType::token_startEnd);
}

// LL1SyntaxParser��ʼ���ķ�
void LL1SyntaxParser::InitCellular() {
  // iProco��ָ��ͨ��չ��ʽ����Homura
  iHandle iProco = Homura;
  // ���������£�û�п��Ƕ����εĴ���ָ�����˴���������ָ��NULL
  for (int i = 0; i < LL1PARSERMAPROW; i++) {
    for (int j = 0; j < LL1PARSERMAPCOL; j++) {
      this->iMap->SetCellular(i, j, new CandidateFunction(NULL, CFunctionType::umi_errorEnd));
    }
  }
  // �����У� <ssql_stmt> ::= "create"��Token
  // ������У� <ssql_stmt> ::= <create_stmt>
  this->iMap->SetCellular(SyntaxType::case_ssql_stmt, TokenType::token_create,
    new CandidateFunction(iProco, CFunctionType::deri___ssql_stmt__create_stmt_1));
  // �����У� <ssql_stmt> ::= "insert"��Token
  // ������У� <ssql_stmt> ::= <insert_stmt>
  this->iMap->SetCellular(SyntaxType::case_ssql_stmt, TokenType::token_insert,
    new CandidateFunction(iProco, CFunctionType::deri___ssql_stmt__insert_stmt_2));
  // �����У� <ssql_stmt> ::= "delete"��Token
  // ������У� <ssql_stmt> ::= <delete_stmt>
  this->iMap->SetCellular(SyntaxType::case_ssql_stmt, TokenType::token_delete,
    new CandidateFunction(iProco, CFunctionType::deri___ssql_stmt__delete_stmt_3));
  // �����У� <ssql_stmt> ::= "select"��Token
  // ������У� <ssql_stmt> ::= <query_stmt>
  this->iMap->SetCellular(SyntaxType::case_ssql_stmt, TokenType::token_select,
    new CandidateFunction(iProco, CFunctionType::deri___ssql_stmt__query_stmt_4));
  // �����У� <create_stmt> ::= "create"��Token
  // ������У� <create_stmt> ::= "create" "table" iden "(" <decl_list> ")" ";"
  this->iMap->SetCellular(SyntaxType::case_create_stmt, TokenType::token_create,
    new CandidateFunction(iProco, CFunctionType::deri___create_stmt__decl_list_5));
  // �����У� <decl_list> ::= iden��Token
  // ������У� <decl_list> ::= <decl> <decl_listpi>
  this->iMap->SetCellular(SyntaxType::case_decl_list, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___decl_list__decl__decl_listpi_6));
  // �����У� <decl_list> ::= "primary"��Token
  // ������У� <decl_list> ::= <decl> <decl_listpi>
  this->iMap->SetCellular(SyntaxType::case_decl_list, TokenType::token_primary,
    new CandidateFunction(iProco, CFunctionType::deri___decl_list__decl__decl_listpi_6));
  // �����У� <decl_listpi> ::= ")"��Token
  // ������У� <decl_listpi> ::= null
  this->iMap->SetCellular(SyntaxType::case_decl_listpi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___decl_listpi__epsilon_7));
  // �����У� <decl_listpi> ::= ","��Token
  // ������У� <decl_listpi> ::= "," <decl> <decl_listpi>
  this->iMap->SetCellular(SyntaxType::case_decl_listpi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___decl_listpi__decl__decl_listpi_67));
  // �����У� <decl> ::= iden��Token
  // ������У� <decl> ::= iden "int" <default_spec>
  this->iMap->SetCellular(SyntaxType::case_decl, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___decl__default_spec_8));
  // �����У� <decl> ::= "primary"��Token
  // ������У� <decl> ::= "primary" "key" "(" <column_list> ")"
  this->iMap->SetCellular(SyntaxType::case_decl, TokenType::token_primary,
    new CandidateFunction(iProco, CFunctionType::deri___decl__column_list_9));
  // �����У� <default_spec> ::= ")"��Token
  // ������У� <default_spec> ::= null
  this->iMap->SetCellular(SyntaxType::case_default_spec, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___default_spec__epsilon_11));
  // �����У� <default_spec> ::= ","��Token
  // ������У� <default_spec> ::= null
  this->iMap->SetCellular(SyntaxType::case_default_spec, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___default_spec__epsilon_11));
  // �����У� <default_spec> ::= "default"��Token
  // ������У� <default_spec> ::= "default" "=" <sexpr>
  this->iMap->SetCellular(SyntaxType::case_default_spec, TokenType::token_default,
    new CandidateFunction(iProco, CFunctionType::deri___default_spec__sexpr_10));
  // �����У� <sexpr> ::= "("��Token
  // ������У� <sexpr> ::= <smulti> <splus>
  this->iMap->SetCellular(SyntaxType::case_sexpr, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr__smulti__sexpr_pi_12));
  // �����У� <sexpr> ::= "+"��Token
  // ������У� <sexpr> ::= <smulti> <splus>
  this->iMap->SetCellular(SyntaxType::case_sexpr, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr__smulti__sexpr_pi_12));
  // �����У� <sexpr> ::= "-"��Token
  // ������У� <sexpr> ::= <smulti> <splus>
  this->iMap->SetCellular(SyntaxType::case_sexpr, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr__smulti__sexpr_pi_12));
  // �����У� <sexpr> ::= number��Token
  // ������У� <sexpr> ::= <smulti> <splus>
  this->iMap->SetCellular(SyntaxType::case_sexpr, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr__smulti__sexpr_pi_12));
  // �����У� <sexpr_pi> ::= ")"��Token
  // ������У� <sexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_sexpr_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr_pi__epsilon_71));
  // �����У� <sexpr_pi> ::= ","��Token
  // ������У� <sexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_sexpr_pi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr_pi__epsilon_71));
  // �����У� <sexpr_pi> ::= "+"��Token
  // ������У� <sexpr_pi> ::= <splus> <sexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_sexpr_pi, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr_pi__splus__sexpr_pi_70));
  // �����У� <sexpr_pi> ::= "-"��Token
  // ������У� <sexpr_pi> ::= <splus> <sexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_sexpr_pi, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___sexpr_pi__splus__sexpr_pi_70));
  // �����У� <splus> ::= "+"��Token
  // ������У� <splus> ::= "+" <smulti>
  this->iMap->SetCellular(SyntaxType::case_splus, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___splus__plus_smulti_14));
  // �����У� <splus> ::= "-"��Token
  // ������У� <splus> ::= "-" <smulti>
  this->iMap->SetCellular(SyntaxType::case_splus, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___splus__minus_smulti_15));
  // �����У� <smulti> ::= "("��Token
  // ������У� <smulti> ::= <sunit> <smultiOpt>
  this->iMap->SetCellular(SyntaxType::case_smulti, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___smulti__sunit__smultiOpt_17));
  // �����У� <smulti> ::= "+"��Token
  // ������У� <smulti> ::= <sunit> <smultiOpt>
  this->iMap->SetCellular(SyntaxType::case_smulti, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___smulti__sunit__smultiOpt_17));
  // �����У� <smulti> ::= "-"��Token
  // ������У� <smulti> ::= <sunit> <smultiOpt>
  this->iMap->SetCellular(SyntaxType::case_smulti, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___smulti__sunit__smultiOpt_17));
  // �����У� <smulti> ::= number��Token
  // ������У� <smulti> ::= <sunit> <smultiOpt>
  this->iMap->SetCellular(SyntaxType::case_smulti, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___smulti__sunit__smultiOpt_17));
  // �����У� <smultiOpt> ::= ")"��Token
  // ������У� <smultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__epsilon_20));
  // �����У� <smultiOpt> ::= ","��Token
  // ������У� <smultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__epsilon_20));
  // �����У� <smultiOpt> ::= "+"��Token
  // ������У� <smultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__epsilon_20));
  // �����У� <smultiOpt> ::= "-"��Token
  // ������У� <smultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__epsilon_20));
  // �����У� <smultiOpt> ::= "*"��Token
  // ������У� <smultiOpt> ::= "*" <sunit>
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_Multiply_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__multi_sunit__smultiOpt_18));
  // �����У� <smultiOpt> ::= "/"��Token
  // ������У� <smultiOpt> ::= "/" <sunit>
  this->iMap->SetCellular(SyntaxType::case_smultiOpt, TokenType::token_Divide_,
    new CandidateFunction(iProco, CFunctionType::deri___smultiOpt__div_sunit__smultiOpt_19));
  // �����У� <sunit> ::= "("��Token
  // ������У� <sunit> ::= "(" <sexpr> ")"
  this->iMap->SetCellular(SyntaxType::case_sunit, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___sunit__brucket_sexpr_24));
  // �����У� <sunit> ::= "+"��Token
  // ������У� <sunit> ::= "+" <sunit>
  this->iMap->SetCellular(SyntaxType::case_sunit, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___sunit__plus_sunit_23));
  // �����У� <sunit> ::= "-"��Token
  // ������У� <sunit> ::= "-" <sunit>
  this->iMap->SetCellular(SyntaxType::case_sunit, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___sunit__minus_sunit_22));
  // �����У� <sunit> ::= number��Token
  // ������У� <sunit> ::= number
  this->iMap->SetCellular(SyntaxType::case_sunit, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___sunit__number_21));
  // �����У� <insert_stmt> ::= "insert"��Token
  // ������У� <insert_stmt> ::= "insert" "into" iden "(" <column_list> ")" "values" "(" <value_list> ")" ";"
  this->iMap->SetCellular(SyntaxType::case_insert_stmt, TokenType::token_insert,
    new CandidateFunction(iProco, CFunctionType::deri___insert_stmt__column_list__value_list_28));
  // �����У� <value_list> ::= iden��Token
  // ������У� <value_list> ::= <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_list, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___value_list__sexpr__value_listpi_29));
  // �����У� <value_list> ::= "("��Token
  // ������У� <value_list> ::= <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_list, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___value_list__sexpr__value_listpi_29));
  // �����У� <value_list> ::= "+"��Token
  // ������У� <value_list> ::= <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_list, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___value_list__sexpr__value_listpi_29));
  // �����У� <value_list> ::= "-"��Token
  // ������У� <value_list> ::= <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_list, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___value_list__sexpr__value_listpi_29));
  // �����У� <value_list> ::= number��Token
  // ������У� <value_list> ::= <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_list, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___value_list__sexpr__value_listpi_29));
  // �����У� <value_listpi> ::= ")"��Token
  // ������У� <value_listpi> ::= null
  this->iMap->SetCellular(SyntaxType::case_value_listpi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___value_listpi__epsilon_31));
  // �����У� <value_listpi> ::= ","��Token
  // ������У� <value_listpi> ::= "," <wexpr> <value_listpi>
  this->iMap->SetCellular(SyntaxType::case_value_listpi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___value_listpi__sexpr__value_listpi_30));
  // �����У� <delete_stmt> ::= "delete"��Token
  // ������У� <delete_stmt> ::= "delete" "from" iden <where_clause> ";"
  this->iMap->SetCellular(SyntaxType::case_delete_stmt, TokenType::token_delete,
    new CandidateFunction(iProco, CFunctionType::deri___delete_stmt__where_clause_32));
  // �����У� <where_clause> ::= ";"��Token
  // ������У� <where_clause> ::= null
  this->iMap->SetCellular(SyntaxType::case_where_clause, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___where_clause__epsilon_34));
  // �����У� <where_clause> ::= "where"��Token
  // ������У� <where_clause> ::= "where" <disjunct>
  this->iMap->SetCellular(SyntaxType::case_where_clause, TokenType::token_where,
    new CandidateFunction(iProco, CFunctionType::deri___where_clause__disjunct_33));
  // �����У� <disjunct> ::= iden��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct> ::= "("��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct> ::= "+"��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct> ::= "-"��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct> ::= number��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct> ::= "!"��Token
  // ������У� <disjunct> ::= <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct, TokenType::token_Not_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct__conjunct__disjunct_pi_35));
  // �����У� <disjunct_pi> ::= ")"��Token
  // ������У� <disjunct_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_disjunct_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct_pi__epsilon_37));
  // �����У� <disjunct_pi> ::= ";"��Token
  // ������У� <disjunct_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_disjunct_pi, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct_pi__epsilon_37));
  // �����У� <disjunct_pi> ::= "||"��Token
  // ������У� <disjunct_pi> ::= "||" <conjunct> <disjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_disjunct_pi, TokenType::token_Or_Or_,
    new CandidateFunction(iProco, CFunctionType::deri___disjunct_pi__conjunct__disjunct_pi_36));
  // �����У� <conjunct> ::= iden��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct> ::= "("��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct> ::= "+"��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct> ::= "-"��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct> ::= number��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct> ::= "!"��Token
  // ������У� <conjunct> ::= <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct, TokenType::token_Not_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct__bool__conjunct_pi_38));
  // �����У� <conjunct_pi> ::= ")"��Token
  // ������У� <conjunct_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_conjunct_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct_pi__epsilon_40));
  // �����У� <conjunct_pi> ::= ";"��Token
  // ������У� <conjunct_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_conjunct_pi, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct_pi__epsilon_40));
  // �����У� <conjunct_pi> ::= "||"��Token
  // ������У� <conjunct_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_conjunct_pi, TokenType::token_Or_Or_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct_pi__epsilon_40));
  // �����У� <conjunct_pi> ::= "&&"��Token
  // ������У� <conjunct_pi> ::= "&&" <bool> <conjunct_pi>
  this->iMap->SetCellular(SyntaxType::case_conjunct_pi, TokenType::token_And_And_,
    new CandidateFunction(iProco, CFunctionType::deri___conjunct_pi__bool__conjunct_pi_39));
  // �����У� <bool> ::= iden��Token
  // ������У� <bool> ::= <comp>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___bool__comp_43));
  // �����У� <bool> ::= "("��Token
  // ������У� <bool> ::= <comp>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___bool__comp_43));
  // �����У� <bool> ::= "+"��Token
  // ������У� <bool> ::= <comp>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___bool__comp_43));
  // �����У� <bool> ::= "-"��Token
  // ������У� <bool> ::= <comp>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___bool__comp_43));
  // �����У� <bool> ::= number��Token
  // ������У� <bool> ::= <comp>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___bool__comp_43));
  // �����У� <bool> ::= "!"��Token
  // ������У� <bool> ::= "!" <bool>
  this->iMap->SetCellular(SyntaxType::case_bool, TokenType::token_Not_,
    new CandidateFunction(iProco, CFunctionType::deri___bool__not_bool_42));
  // �����У� <comp> ::= iden��Token
  // ������У� <comp> ::= <wexpr> <rop> <wexpr>
  this->iMap->SetCellular(SyntaxType::case_comp, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___comp__wexpr__rop__wexpr_44));
  // �����У� <comp> ::= "("��Token
  // ������У� <comp> ::= <wexpr> <rop> <wexpr>
  this->iMap->SetCellular(SyntaxType::case_comp, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___comp__wexpr__rop__wexpr_44));
  // �����У� <comp> ::= "+"��Token
  // ������У� <comp> ::= <wexpr> <rop> <wexpr>
  this->iMap->SetCellular(SyntaxType::case_comp, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___comp__wexpr__rop__wexpr_44));
  // �����У� <comp> ::= "-"��Token
  // ������У� <comp> ::= <wexpr> <rop> <wexpr>
  this->iMap->SetCellular(SyntaxType::case_comp, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___comp__wexpr__rop__wexpr_44));
  // �����У� <comp> ::= number��Token
  // ������У� <comp> ::= <wexpr> <rop> <wexpr>
  this->iMap->SetCellular(SyntaxType::case_comp, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___comp__wexpr__rop__wexpr_44));
  // �����У� <rop> ::= "<>"��Token
  // ������У� <rop> ::= "<>"
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_LessThan_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__lessgreater_58));
  // �����У� <rop> ::= "=="��Token
  // ������У� <rop> ::= "=="
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_Equality_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__equalequal_59));
  // �����У� <rop> ::= ">"��Token
  // ������У� <rop> ::= ">"
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__greater_60));
  // �����У� <rop> ::= "<"��Token
  // ������У� <rop> ::= "<"
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_LessThan_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__less_61));
  // �����У� <rop> ::= ">="��Token
  // ������У� <rop> ::= ">="
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_GreaterThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__greaterequal_62));
  // �����У� <rop> ::= "<="��Token
  // ������У� <rop> ::= "<="
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_LessThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__lessequal_63));
  // �����У� <rop> ::= ";"��Token
  // ������У� <rop> ::= epsilon
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__epsilon_80));
  // �����У� <rop> ::= "("��Token
  // ������У� <rop> ::= epsilon
  this->iMap->SetCellular(SyntaxType::case_rop, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___rop__epsilon_80));
  // �����У� <wexpr> ::= iden��Token
  // ������У� <wexpr> ::= <wmulti> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__wmulti__wexpr_pi_45));
  // �����У� <wexpr> ::= ";"��Token
  // ������У� <wexpr> ::= epsilon
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__epsilon_81));
  // �����У� <wexpr> ::= ")"��Token
  // ������У� <wexpr> ::= epsilon
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__epsilon_81));
  // �����У� <wexpr> ::= "("��Token
  // ������У� <wexpr> ::= <wmulti> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__wmulti__wexpr_pi_45));
  // �����У� <wexpr> ::= "+"��Token
  // ������У� <wexpr> ::= <wmulti> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__wmulti__wexpr_pi_45));
  // �����У� <wexpr> ::= "-"��Token
  // ������У� <wexpr> ::= <wmulti> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__wmulti__wexpr_pi_45));
  // �����У� <wexpr> ::= number��Token
  // ������У� <wexpr> ::= <wmulti> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr__wmulti__wexpr_pi_45));
  // �����У� <wexpr_pi> ::= ")"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= ";"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= ","��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "+"��Token
  // ������У� <wexpr_pi> ::= <wplus> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__wplus__wexpr_pi_72));
  // �����У� <wexpr_pi> ::= "-"��Token
  // ������У� <wexpr_pi> ::= <wplus> <wexpr_pi>
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__wplus__wexpr_pi_72));
  // �����У� <wexpr_pi> ::= "||"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Or_Or_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "&&"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_And_And_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "<>"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_LessThan_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "=="��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Equality_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= ">"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "<"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_LessThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= ">="��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_GreaterThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= "<="��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_LessThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wexpr_pi> ::= ")"��Token
  // ������У� <wexpr_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wplus> ::= ";"��Token
  // ������У� <wplus> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wplus> ::= ","��Token
  // ������У� <wplus> ::= null
  this->iMap->SetCellular(SyntaxType::case_wexpr_pi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___wexpr_pi__epsilon_73));
  // �����У� <wplus> ::= "+"��Token
  // ������У� <wplus> ::= "+" <wmulti>
  this->iMap->SetCellular(SyntaxType::case_wplus, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wplus__plus_wmulti_46));
  // �����У� <wplus> ::= "-"��Token
  // ������У� <wplus> ::= "-" <wmulti>
  this->iMap->SetCellular(SyntaxType::case_wplus, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wplus__minus_wmulti_47));
  // �����У� <wmulti> ::= iden��Token
  // ������У� <wmulti> ::= <wunit> <wmultiOpt>
  this->iMap->SetCellular(SyntaxType::case_wmulti, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___wmulti__wunit__wmultiOpt_49));
  // �����У� <wmulti> ::= "("��Token
  // ������У� <wmulti> ::= <wunit> <wmultiOpt>
  this->iMap->SetCellular(SyntaxType::case_wmulti, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wmulti__wunit__wmultiOpt_49));
  // �����У� <wmulti> ::= "+"��Token
  // ������У� <wmulti> ::= <wunit> <wmultiOpt>
  this->iMap->SetCellular(SyntaxType::case_wmulti, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wmulti__wunit__wmultiOpt_49));
  // �����У� <wmulti> ::= "-"��Token
  // ������У� <wmulti> ::= <wunit> <wmultiOpt>
  this->iMap->SetCellular(SyntaxType::case_wmulti, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wmulti__wunit__wmultiOpt_49));
  // �����У� <wmulti> ::= number��Token
  // ������У� <wmulti> ::= <wunit> <wmultiOpt>
  this->iMap->SetCellular(SyntaxType::case_wmulti, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___wmulti__wunit__wmultiOpt_49));
  // �����У� <wmultiOpt> ::= ")"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= ";"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= ","��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "+"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "-"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "*"��Token
  // ������У� <wmultiOpt> ::= "*" <wunit>
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Multiply_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__multi_wunit__wmultiOpt_50));
  // �����У� <wmultiOpt> ::= "/"��Token
  // ������У� <wmultiOpt> ::= "/" <wunit>
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Divide_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__div_wunit__wmultiOpt_51));
  // �����У� <wmultiOpt> ::= "||"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Or_Or_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "&&"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_And_And_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "<>"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_LessThan_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "=="��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_Equality_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= ">"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "<"��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_LessThan_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= ">="��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_GreaterThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wmultiOpt> ::= "<="��Token
  // ������У� <wmultiOpt> ::= null
  this->iMap->SetCellular(SyntaxType::case_wmultiOpt, TokenType::token_LessThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::deri___wmultiOpt__epsilon_52));
  // �����У� <wunit> ::= iden��Token
  // ������У� <wunit> ::= iden
  this->iMap->SetCellular(SyntaxType::case_wunit, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___wunit__iden_54));
  // �����У� <wunit> ::= "("��Token
  // ������У� <wunit> ::= "(" <disjunct> ")"
  this->iMap->SetCellular(SyntaxType::case_wunit, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___wunit__brucket_disjunct_57));
  // �����У� <wunit> ::= "+"��Token
  // ������У� <wunit> ::= "+" <wunit>
  this->iMap->SetCellular(SyntaxType::case_wunit, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::deri___wunit__plus_wunit_56));
  // �����У� <wunit> ::= "-"��Token
  // ������У� <wunit> ::= "-" <wunit>
  this->iMap->SetCellular(SyntaxType::case_wunit, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::deri___wunit__minus_wunit_55));
  // �����У� <wunit> ::= number��Token
  // ������У� <wunit> ::= number
  this->iMap->SetCellular(SyntaxType::case_wunit, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::deri___wunit__number_53));
  // �����У� <query_stmt> ::= "select"��Token
  // ������У� <query_stmt> ::= "select" <select_list> "from" iden <where_clause> ";"
  this->iMap->SetCellular(SyntaxType::case_query_stmt, TokenType::token_select,
    new CandidateFunction(iProco, CFunctionType::deri___query_stmt__select_list__where_clause_64));
  // �����У� <select_list> ::= iden��Token
  // ������У� <select_list> ::= <column_list>
  this->iMap->SetCellular(SyntaxType::case_select_list, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___select_list__column_list_65));
  // �����У� <select_list> ::= "*"��Token
  // ������У� <select_list> ::= "*"
  this->iMap->SetCellular(SyntaxType::case_select_list, TokenType::token_Multiply_,
    new CandidateFunction(iProco, CFunctionType::deri___select_list__star_66));
  // �����У� <column_list> ::= iden��Token
  // ������У� <column_list> ::= iden <column_pi>
  this->iMap->SetCellular(SyntaxType::case_column_list, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::deri___column_list__column_pi_25));
  // �����У� <column_pi> ::= ")"��Token
  // ������У� <column_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_column_pi, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::deri___column_pi__epsilon_27));
  // �����У� <column_pi> ::= ","��Token
  // ������У� <column_pi> ::= "," iden <column_pi>
  this->iMap->SetCellular(SyntaxType::case_column_pi, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::deri___column_pi__comma_column_pi_26));
  // �����У� <column_pi> ::= "from"��Token
  // ������У� <column_pi> ::= null
  this->iMap->SetCellular(SyntaxType::case_column_pi, TokenType::token_from,
    new CandidateFunction(iProco, CFunctionType::deri___column_pi__epsilon_27));
  // Ҷ���У� "create" ��Token
  this->iMap->SetCellular(SyntaxType::tail_createLeave, TokenType::token_create,
    new CandidateFunction(iProco, CFunctionType::umi_create));
  // Ҷ���У� "table" ��Token
  this->iMap->SetCellular(SyntaxType::tail_tableLeave, TokenType::token_table,
    new CandidateFunction(iProco, CFunctionType::umi_table));
  // Ҷ���У� iden ��Token
  this->iMap->SetCellular(SyntaxType::tail_idenLeave, TokenType::token_iden,
    new CandidateFunction(iProco, CFunctionType::umi_iden));
  // Ҷ���У� "(" ��Token
  this->iMap->SetCellular(SyntaxType::tail_leftParentheses_Leave, TokenType::token_LeftParentheses_,
    new CandidateFunction(iProco, CFunctionType::umi_leftParentheses_));
  // Ҷ���У� ")" ��Token
  this->iMap->SetCellular(SyntaxType::tail_rightParentheses_Leave, TokenType::token_RightParentheses_,
    new CandidateFunction(iProco, CFunctionType::umi_rightParentheses_));
  // Ҷ���У� ";" ��Token
  this->iMap->SetCellular(SyntaxType::tail_semicolon_Leave, TokenType::token_Semicolon_,
    new CandidateFunction(iProco, CFunctionType::umi_semicolon_));
  // Ҷ���У� "," ��Token
  this->iMap->SetCellular(SyntaxType::tail_comma_Leave, TokenType::token_Comma_,
    new CandidateFunction(iProco, CFunctionType::umi_comma_));
  // Ҷ���У� null ��Token
  this->iMap->SetCellular(SyntaxType::epsilonLeave, TokenType::epsilon,
    new CandidateFunction(iProco, CFunctionType::umi_epsilon));
  // Ҷ���У� "int" ��Token
  this->iMap->SetCellular(SyntaxType::tail_intLeave, TokenType::token_int,
    new CandidateFunction(iProco, CFunctionType::umi_int));
  // Ҷ���У� "primary" ��Token
  this->iMap->SetCellular(SyntaxType::tail_primaryLeave, TokenType::token_primary,
    new CandidateFunction(iProco, CFunctionType::umi_primary));
  // Ҷ���У� "key" ��Token
  this->iMap->SetCellular(SyntaxType::tail_keyLeave, TokenType::token_key,
    new CandidateFunction(iProco, CFunctionType::umi_key));
  // Ҷ���У� "default" ��Token
  this->iMap->SetCellular(SyntaxType::tail_defaultLeave, TokenType::token_default,
    new CandidateFunction(iProco, CFunctionType::umi_default));
  // Ҷ���У� "=" ��Token
  this->iMap->SetCellular(SyntaxType::tail_equality_Leave, TokenType::token_Equality_,
    new CandidateFunction(iProco, CFunctionType::umi_equality_));
  // Ҷ���У� "+" ��Token
  this->iMap->SetCellular(SyntaxType::tail_plus_Leave, TokenType::token_Plus_,
    new CandidateFunction(iProco, CFunctionType::umi_plus_));
  // Ҷ���У� "-" ��Token
  this->iMap->SetCellular(SyntaxType::tail_minus_Leave, TokenType::token_Minus_,
    new CandidateFunction(iProco, CFunctionType::umi_minus_));
  // Ҷ���У� "*" ��Token
  this->iMap->SetCellular(SyntaxType::tail_multiply_Leave, TokenType::token_Multiply_,
    new CandidateFunction(iProco, CFunctionType::umi_multiply_));
  // Ҷ���У� "/" ��Token
  this->iMap->SetCellular(SyntaxType::tail_divide_Leave, TokenType::token_Divide_,
    new CandidateFunction(iProco, CFunctionType::umi_divide_));
  // Ҷ���У� number ��Token
  this->iMap->SetCellular(SyntaxType::numberLeave, TokenType::number,
    new CandidateFunction(iProco, CFunctionType::umi_number));
  // Ҷ���У� "insert" ��Token
  this->iMap->SetCellular(SyntaxType::tail_insertLeave, TokenType::token_insert,
    new CandidateFunction(iProco, CFunctionType::umi_insert));
  // Ҷ���У� "into" ��Token
  this->iMap->SetCellular(SyntaxType::tail_intoLeave, TokenType::token_into,
    new CandidateFunction(iProco, CFunctionType::umi_into));
  // Ҷ���У� "values" ��Token
  this->iMap->SetCellular(SyntaxType::tail_valuesLeave, TokenType::token_values,
    new CandidateFunction(iProco, CFunctionType::umi_values));
  // Ҷ���У� "delete" ��Token
  this->iMap->SetCellular(SyntaxType::tail_deleteLeave, TokenType::token_delete,
    new CandidateFunction(iProco, CFunctionType::umi_delete));
  // Ҷ���У� "from" ��Token
  this->iMap->SetCellular(SyntaxType::tail_fromLeave, TokenType::token_from,
    new CandidateFunction(iProco, CFunctionType::umi_from));
  // Ҷ���У� "where" ��Token
  this->iMap->SetCellular(SyntaxType::tail_whereLeave, TokenType::token_where,
    new CandidateFunction(iProco, CFunctionType::umi_where));
  // Ҷ���У� "||" ��Token
  this->iMap->SetCellular(SyntaxType::tail_or_Or_Leave, TokenType::token_Or_Or_,
    new CandidateFunction(iProco, CFunctionType::umi_or_Or_));
  // Ҷ���У� "&&" ��Token
  this->iMap->SetCellular(SyntaxType::tail_and_And_Leave, TokenType::token_And_And_,
    new CandidateFunction(iProco, CFunctionType::umi_and_And_));
  // Ҷ���У� "!" ��Token
  this->iMap->SetCellular(SyntaxType::tail_not_Leave, TokenType::token_Not_,
    new CandidateFunction(iProco, CFunctionType::umi_not_));
  // Ҷ���У� "<>" ��Token
  this->iMap->SetCellular(SyntaxType::tail_lessThan_GreaterThan_Leave, TokenType::token_LessThan_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::umi_lessThan_GreaterThan_));
  // Ҷ���У� "==" ��Token
  this->iMap->SetCellular(SyntaxType::tail_equality_Equality_Leave, TokenType::token_Equality_Equality_,
    new CandidateFunction(iProco, CFunctionType::umi_equality_Equality_));
  // Ҷ���У� ">" ��Token
  this->iMap->SetCellular(SyntaxType::tail_greaterThan_Leave, TokenType::token_GreaterThan_,
    new CandidateFunction(iProco, CFunctionType::umi_greaterThan_));
  // Ҷ���У� "<" ��Token
  this->iMap->SetCellular(SyntaxType::tail_lessThan_Leave, TokenType::token_LessThan_,
    new CandidateFunction(iProco, CFunctionType::umi_lessThan_));
  // Ҷ���У� ">=" ��Token
  this->iMap->SetCellular(SyntaxType::tail_greaterThan_Equality_Leave, TokenType::token_GreaterThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::umi_greaterThan_Equality_));
  // Ҷ���У� "<=" ��Token
  this->iMap->SetCellular(SyntaxType::tail_lessThan_Equality_Leave, TokenType::token_LessThan_Equality_,
    new CandidateFunction(iProco, CFunctionType::umi_lessThan_Equality_));
  // Ҷ���У� "select" ��Token
  this->iMap->SetCellular(SyntaxType::tail_selectLeave, TokenType::token_select,
    new CandidateFunction(iProco, CFunctionType::umi_select));
  // Ҷ���У� # ��Token
  this->iMap->SetCellular(SyntaxType::tail_startEndLeave, TokenType::token_startEnd,
    new CandidateFunction(iProco, CFunctionType::umi_startEnd));
}

//�������ã� ���ĺ���Homura����ͨ�ò���ʽ������
//�����б�
//   _myNode ����ʽ�ڵ�
// _myParser ƥ����ָ��
//   _myType ��ѡʽ����
// _mySyntax �ڵ��﷨����
//  _myValue ���е����ı�
//�� �� ֵ�� SyntaxTreeNode* ��һ��չ���ڵ��ָ��
SyntaxTreeNode* Homura(SyntaxTreeNode* _myNode, void* _myParser, CFunctionType _myType, SyntaxType _mySyntax, istr _myValue) {
  // ȡ��ƥ���������½ڵ���Ϣ
  LL1SyntaxParser* iParser = (LL1SyntaxParser*)_myParser;
  if (_myNode != NULL) {
    _myNode->nodeType = _myType;
    _myNode->nodeValue = _myValue;
    _myNode->nodeSyntaxType = _mySyntax;
    _myNode->nodeName = SyntaxOutputTable[_mySyntax];
    _myNode->mappedList = iParser->GetTokenStream();
    _myNode->mappedBegin = iParser->iPTRnextToken;
  }
  // ȡƥ��ջ�ͺ�ѡ����
  SyntaxVector iSvec = iParser->GetVector(_myType);
  SyntaxStack* iStack = iParser->GetStack();
  // ��߳�ջ
  iStack->pop();
  // �������һ�����ս��
  if (_myType < CFunctionType::DERI_UMI_BOUNDARY) {
    // ��������ѹƥ��ջ
    for (SyntaxVector::reverse_iterator iter = iSvec.rbegin();
      iter != iSvec.rend(); iter++) {
      iStack->push(*iter);
    }
    // �������ҽ��ӽڵ�
    bool flag = false;
    SyntaxTreeNode* successor = NULL;
    for (SyntaxVector::const_iterator iter = iSvec.begin();
      iter != iSvec.end(); iter++) {
      SyntaxTreeNode* newNode = new SyntaxTreeNode();
      newNode->mappedList = iParser->GetTokenStream();
      newNode->mappedBegin = iParser->iPTRnextToken;
      newNode->parent = _myNode;
      _myNode->children.push_back(newNode);
      if (flag == false) {
        successor = newNode;
        flag = true;
      }
    }
    // ���ص�һ������ʽ
    return successor;
  }
  // �����һ���ս��
  else {
    // ���½ڵ���Ϣ
    if (_myNode != NULL && _myType != CFunctionType::umi_epsilon) {
      _myNode->mappedLength = iParser->GetTokenStream()->_tokenContainer[iParser->iPTRnextToken]->detail.size();
    }
    else if (_myType == CFunctionType::umi_epsilon) {
      _myNode->mappedLength = 0;
    }
    // ����tokenָ��
    if (_myType != CFunctionType::umi_epsilon) {
      iParser->iPTRnextToken++;
    }
    // �������ĺ�̽ڵ�
    return iParser->NextNode(_myNode, iParser);
  }
}
