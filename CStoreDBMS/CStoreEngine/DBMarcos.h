#ifndef ___CSTORE_DBMARCOS
#define ___CSTORE_DBMARCOS

// ��ֹ���������쿽�����캯���͸�ֵ����
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);                \
  TypeName& operator=(const TypeName&)

// Ϊ����ṩͳһ�ӿ�
#define PILEPRINT(strpar) std::cout << strpar

// Ϊ��������ṩͳһ�ӿ�
#define PILEPRINTLN(strpar) std::cout << strpar << std::endl

// ����ѭ���ӿ�
#define FOREVER while (true)

// ����������ֵ
#define FZERO 1e-15

// Ϊ�����ṩͳһ����
#define NEWLINE "\n"

// Ϊ��ѯ����ṩͳһ�Ľ�������
#define SENTENCETERMINATOR ';'

// ������Parser���ķ�����ʼ�к�
#define COLSTARTNUM 1

// ������Parser���ķ�����ʼ�к�
#define ROWSTARTNUM 1

// ������Parser���ķ���������
#define LL1PARSERMAPROW 69

// ������Parser���ķ���������
#define LL1PARSERMAPCOL 35

#endif /* ___CSTORE_DBMARCOS */