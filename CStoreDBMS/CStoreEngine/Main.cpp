#include "DBController.h"
using namespace CStore;

int main(int argc, char* argv[]) {
  DBController* core = DBController::Invoke();
  core->DBInit(argc, argv);
  core->DBStartDash();
  return 0;
}

/**
 * ��ǰ�߼���
 * Controller   - ǰ�����̨���������������ַ���  - �������string��
 * Connector    - ����ѯ�ַ�����װΪ���ݿ�����    - �������DBTransaction��
 * Bridge       - ���ñ����������ݿ����洦������  - �������string[]��
 * Lexer        - ����ѯ�������Ϊ������          - �������TokenStream��
 * Parser       - ������������Ϊ�﷨��            - �������SyntaxTreeNode��
 * Pile         - ���﷨������Ϊִ�е�Ԫ          - �������DBCProxy��
 * IDatabase    - ����ִ�е�ԪΪ���ݿ⶯��        - ���������FileManager�������ĵ��ã�
 * TableMananer - ��Ҫ�����ı�����������        - ����¼�����Ϣ��������������� 
 * FileManager  - ����ʵ�����ݿ��ļ�����          - ��ʵ���ļ�������
 **/