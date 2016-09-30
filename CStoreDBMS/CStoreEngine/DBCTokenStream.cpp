#include "DBCTokenStream.h"

// TokenStream������
TokenStream::TokenStream()
  :DBObject("TokenStream", this) {
  this->Reset();
}

// TokenStream����������
TokenStream::TokenStream(const TokenStream &_other)
  : DBObject("TokenStream", this) {
  this->_iPointer = _other._iPointer;
  for (int i = 0; i < _other.Length(); i++) {
    this->Add(_other._tokenContainer[i]);
  }
}

// TokenStream׷�Ӻ���
void TokenStream::Add(Token* _org) {
  this->_tokenContainer.push_back(_org);
}

// TokenStream���Ⱥ���
int TokenStream::Length() const {
  return _tokenContainer.size();
}

// TokenStream��λ����
void TokenStream::Reset() {
  _iPointer = 0;
  _tokenContainer.clear();
}

// TokenStream�������
ios& operator<<(ios &os, const TokenStream &ts) {
  for (int i = 0; i < ts.Length(); i++) {
    os << "Token Series No. " << i << NEWLINE
      << " Type: " << TokenOutputTable[ts._tokenContainer[i]->aType] << NEWLINE
      << " At Row: " << ts._tokenContainer[i]->aLine
      << "  Col: " << ts._tokenContainer[i]->aColumn << NEWLINE
      << " Detail: " << ts._tokenContainer[i]->detail << NEWLINE
      << " ErrorBit: " << ts._tokenContainer[i]->errorCode
      << " (" << (ts._tokenContainer[i]->errorCode ? "Error" : "OK") << ")" << NEWLINE << NEWLINE;
  }
  return os;
}

// TokenStream������ı�
istr TokenStream::ToString() {
  istr builder = "";
  for (TokenList::iterator iter = _tokenContainer.begin();
    iter != _tokenContainer.end(); iter++) {
    builder += (*iter)->aType != TokenType::token_startEnd ? (*iter)->detail + " " : "";
  }
  return builder += NEWLINE;
}

// TokenStream��һToken
Token* TokenStream::Next() {
  if (_iPointer > 0 && _iPointer < (int)this->Length()) {
    return _tokenContainer[_iPointer++];
  }
  else {
    return NULL;
  }
}

// TokenStream��һ����
TokenList TokenStream::NextSentence() {
  TokenList sentenceBuilder;
  _sPointer = _iPointer;
  // ѭ��ֱ����һ����
  while (true) {
    if (_sPointer >= (int)_tokenContainer.size()) {
      break;
    }
    if (_tokenContainer[_sPointer]->aType == TokenType::token_Semicolon_ || _sPointer == 0) {
      break;
    }
    _sPointer++;
  }
  if (_sPointer != 0) {
    _sPointer = _iPointer;
  }
  // ���Խ��ͽ���
  if (_sPointer >= (int)_tokenContainer.size()) {
    return sentenceBuilder;
  }
  // ����ȡ����
  while (_sPointer >= 0 && _sPointer < (int)this->Length()) {
    sentenceBuilder.push_back(_tokenContainer[_sPointer]);
    if (_tokenContainer[_sPointer++]->aType == TokenType::token_Semicolon_) {
      break;
    }
  }
  _iPointer = _sPointer;
  return sentenceBuilder;
}