#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include "util.h"
#include "proc.h"
using namespace std;

namespace ore {

	struct Root::Impl {
		const StatementList* m_StatementList;
	};

	Root::Root(const StatementList* stml) :
		StmExBase(ObjType::RootType),
		pImpl(new Impl)
	{
		pImpl->m_StatementList = stml;
	}

	Root::~Root() {
		delete pImpl;
	}

	const StatementList* Root::getStmeList() const {
		return pImpl->m_StatementList;
	}

	SmtRes Root::Excute() const {
		setRuntimeLineNumber();
		if (pImpl->m_StatementList) {
			auto pos = pImpl->m_StatementList;
			do
			{
				auto stm = pos->getStm();
				stm->Excute();
				pos = pos->getNext();
			} while (pos);
		}
		else {
			Interpreter::getInp()->runtimeExit(2001);
		}
		return SmtRes();
	}

	struct RuntimeObject : public enable_shared_from_this<RuntimeObject> {
		bool m_IsFunc;
		map<string, Value> m_VariableMap;
		weak_ptr<RuntimeObject> m_Parent;
		shared_ptr<RuntimeObject> m_Child;
		RuntimeObject(shared_ptr<RuntimeObject> parent, bool IsFunc = false) :
			m_Parent(parent), m_Child(nullptr), m_IsFunc(IsFunc)
		{}
		~RuntimeObject() {}
	};

	struct Interpreter::Impl {
		set<string> m_FixedStringPool;
		vector<ObjBase*> m_ObjectPool;
		map<string, string> m_ConfigMap;
		string m_LiteralTemp;
		Value m_TempVal;
		Root* m_Root;
		shared_ptr<RuntimeObject> m_RootRuntime;
		map<int, string> m_ErrMap;
		bool m_IsWorningOut;
		int m_RuntimeLineNumber;
	};

	Interpreter* theInp;
	struct PrintDouble {
		bool ret;
		double val;
	};

	vector<PrintDouble> PrintDoubleVec;

	Interpreter::Interpreter() :
		ObjBase(ObjType::InterpreterType),
		pImpl(new Impl)
	{
		pImpl->m_Root = nullptr;
		pImpl->m_ErrMap =
		{
			{ 0001, ""},
			{ 0002, ""},
			{ 1001, ""},
			{ 1002, ""},
			{ 1003, ""},
			{ 1004, ""},
			{ 1005, ""},
			{ 2001, ""},
			{ 2002, ""},
			{ 2003, ""},
			{ 2004, ""},
			{ 2005, ""},
			{ 2006, ""},
			{ 2007, ""},
			{ 2008, ""},
			{ 2009, ""},
			{ 2010, ""},
			{ 2011, ""},
			{ 2012, ""},
			{ 2013, ""},
			{ 2014, ""},
			{ 2015, ""},
			{ 2016, ""},
			{ 2017, ""},
			{ 2018, ""},
			{ 2019, ""},
			{ 2020, ""},
			{ 2021, ""},
			{ 2022, ""},
		};

		pImpl->m_IsWorningOut = true;
		pImpl->m_RuntimeLineNumber = 1;
		pImpl->m_TempVal = Value();
		pImpl->m_ConfigMap["calc_epsilon"] = ".000001";
	}

	Interpreter::~Interpreter() {
		/*if (theInp) {
			delete theInp;
		}*/
		delete pImpl;
	}

	int Interpreter::Compile(FILE* fp) {
		extern int yyparse(void);
		extern FILE* yyin;
		yyin = fp;
		if (yyparse()) {
			return 1;
		}
		return 0;
	}

	void Interpreter::Exec() const {
		if (pImpl->m_Root) {
			pImpl->m_RootRuntime = make_shared<RuntimeObject>(nullptr);
			pImpl->m_Root->Excute();
		}
		else {
			Interpreter::getInp()->runtimeExit(2001);
		}
		//for (auto &v : PrintDoubleVec) {
		//	if (v.ret) {
		//		cout << v.val << endl;
		//	}
		//	else {
		//		cout << v.val;
		//	}
		//}
	}

	void Interpreter::Delete() {
		// PrintDoubleVec.clear();
		for (auto v : pImpl->m_ObjectPool) {
			delete v;
		}
		pImpl->m_ObjectPool.clear();
		pImpl->m_FixedStringPool.clear();
	}

	void Interpreter::setConfig(const char* key, const Value val) {
		string keystr = key;
		string valstr = val.getString();
		if (keystr == "") {
			runtimeExit(2012);
		}
		pImpl->m_ConfigMap[keystr] = valstr;
	}

	Value Interpreter::getConfig(const char* key) {
		string keystr = key;
		if (keystr == "") {
			runtimeExit(2012);
		}
		return Value(pImpl->m_ConfigMap[keystr].c_str());
	}

	void Interpreter::pushBackObject(ObjBase* pobj) {
		pImpl->m_ObjectPool.push_back(pobj);
	}

	const char* Interpreter::createFixedString(const char* ext) {
		auto str = clampToken(ext);
		auto it = pImpl->m_FixedStringPool.insert(str);
		return it.first->c_str();
	}

	Expression* Interpreter::createVariableExp(const char* ident) {
		string str = clampToken(ident);
		auto ptr = new VariableExp(str.c_str());
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	Expression* Interpreter::createIntLiteralExp(const char* ext) {
		string str = clampToken(ext);
		auto ptr = new IntLiteralExp(stod(str));
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	Expression* Interpreter::createDoubleLiteralExp(const char* ext) {
		string str = clampToken(ext);
		auto ptr = new DoubleLiteralExp(stod(str));
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	void Interpreter::startStringLiteral() {
		pImpl->m_LiteralTemp = "";
	}

	void Interpreter::addStringLiteral(char ext) {
		pImpl->m_LiteralTemp += ext;
	}

	Expression* Interpreter::createStringLiteralExp() {
		auto ptr = new StringLiteralExp(pImpl->m_LiteralTemp.c_str());
		pImpl->m_LiteralTemp = "";
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	Expression* Interpreter::createBinaryExp(const Expression* left, const Expression* right, ExpressionType t) {
		auto ptr = new BinaryExp(t, left, right);
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	Expression* Interpreter::createAssExp(const Expression* ident, const Expression* right) {
		auto ptr = new AssignExp(ident, right);
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	Expression* Interpreter::createToAssExp(const Expression* ident, const Expression* right, ExpressionType t) {
		auto ptr = new ToAssExp(ident, right, t);
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	StatementList* Interpreter::createStatementList(const Statement* stm) {
		auto ptr = new StatementList(stm);
		pImpl->m_ObjectPool.push_back(ptr);
		return ptr;
	}

	StatementList* Interpreter::createStatementList(StatementList* stml, const Statement* stm) {
		StatementList *pos;
		if (stml == nullptr)
			return createStatementList(stm);
		for (pos = stml; pos->getNext(); pos = pos->getNext())
			;
		pos->setNext(createStatementList(stm));
		return stml;
	}

	Root* Interpreter::createRoot(const StatementList* stml) {
		auto ptr = new Root(stml);
		pImpl->m_ObjectPool.push_back(ptr);
		pImpl->m_Root = ptr;
		return ptr;
	}

	Value& Interpreter::getGlobalValiableValue(const char* key) {
		auto it = pImpl->m_RootRuntime->m_VariableMap.find(key);
		if (it != pImpl->m_RootRuntime->m_VariableMap.end()) {
			return it->second;
		}
		else {
			pImpl->m_RootRuntime->m_VariableMap[key] = Value("");
			return pImpl->m_RootRuntime->m_VariableMap[key];
		}
	}

	void Interpreter::syntaxKeepExit(int mess_id, const char* detail) {
		cerr << "\n"
			<< pImpl->m_ErrMap[1003]
			<< detail
			<< ": "
			<< pImpl->m_ErrMap[mess_id]
			<< endl;
		exit(1);
	}

	void Interpreter::syntaxExit(int mess_id, int linnum, const char* detail, bool bison) {
		string detailstr = detail;
		if (bison) {
			if (detailstr != "") {
				cerr << "\n"
					<< pImpl->m_ErrMap[mess_id]
					<< linnum
					<< " : "
					<< detailstr
					<< ""
					<< pImpl->m_ErrMap[0001]
					<< pImpl->m_ErrMap[0002]
					<< endl;
			}
			else {
				cerr << "\n"
					<< pImpl->m_ErrMap[mess_id]
					<< linnum
					<< " "
					<< pImpl->m_ErrMap[0001]
					<< endl;
			}
		}
		else {
			if (detailstr != "") {
				cerr << "\n"
					<< pImpl->m_ErrMap[mess_id]
					<< linnum
					<< " : "
					<< detailstr
					<< " "
					<< pImpl->m_ErrMap[0001]
					<< pImpl->m_ErrMap[0002]
					<< endl;
			}
			else {
				cerr << "\n"
					<< pImpl->m_ErrMap[mess_id]
					<< linnum
					<< " "
					<< pImpl->m_ErrMap[0001]
					<< endl;
			}
		}
		exit(1);
	}

	void Interpreter::setRuntimeLineNumber(int num) {
		pImpl->m_RuntimeLineNumber = num;
	}

	int Interpreter::getRuntimeLineNumber() const {
		return pImpl->m_RuntimeLineNumber;
	}

	void Interpreter::runtimeExit(int mess_id, const char* target) {
		if (target) {
			cerr << "\n"
				<< getRuntimeLineNumber()
				<< " : "
				<< target
				<< " "
				<< pImpl->m_ErrMap[mess_id]
				<< endl;
		}
		else {
			cerr << "\n"
				<< getRuntimeLineNumber()
				<< " : "
				<< pImpl->m_ErrMap[mess_id]
				<< endl;
		}
	}

	void Interpreter::runtimeWorning(int mess_id) {
		if (isWorningOut()) {
			cerr << "\n"
				<< getRuntimeLineNumber()
				<< " : "
				<< pImpl->m_ErrMap[mess_id]
				<< endl;
		}
	}

	bool Interpreter::isWorningOut() const {
		return pImpl->m_IsWorningOut;
	}

	void Interpreter::setWorningOut(bool b) {
		pImpl->m_IsWorningOut = b;
	}

	Interpreter* Interpreter::getInp() {
		static Interpreter* instance = nullptr;
		/*if (!theInp) {
			theInp = new Interpreter;
		}*/
		if (!instance) {
			instance = new Interpreter;
		}
		return instance;
	}

	/*void Interpreter::createPrintStatement(double val, bool retFlg) {
		PrintDouble tmp;
		tmp.ret = retFlg;
		tmp.val = val;
		PrintDoubleVec.push_back(tmp);
	}*/
}