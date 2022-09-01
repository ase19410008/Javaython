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

	struct Statement::Impl {
		StatementType m_StmType;
		int m_LineNumber;
	};

	Statement::Statement(StatementType type) :
		StmExBase(ObjType::StmType),
		pImpl(new Impl)
	{
		pImpl->m_StmType = type;
		pImpl->m_LineNumber = 0;
	}

	/*Statement::~Statement() {
		delete pImpl;
	}*/

	StatementType Statement::getType() const {
		return pImpl->m_StmType;
	}

	struct StatementList::Impl {
		const Statement* m_Statement;
		StatementList* m_Next;
	};

	StatementList::StatementList(const Statement* stm) :
		StmExBase(ObjType::StmListType),
		pImpl(new Impl)
	{
		pImpl->m_Statement = stm;
		pImpl->m_Next = nullptr;
	}

	/*StatementList::~StatementList() {
		delete pImpl;
	}*/

	const Statement* StatementList::getStm() const {
		return pImpl->m_Statement;
	}

	StatementList* StatementList::getNext() const {
		return pImpl->m_Next;
	}

	void StatementList::setNext(StatementList* next) {
		pImpl->m_Next = next;
	}

	SmtRes StatementList::Excute() const {
		setRuntimeLineNumber();
		auto stml = this;
		while (stml) {
			auto stm = stml->getStm();
			if (stm) {
				auto res = stm->Excute();
			}
			else {
				break;
			}
			stml = stml->getNext();
		}
		return SmtRes();
	}

	SmtRes StatementList::ExcuteFromCase(Value val) const {
		setRuntimeLineNumber();
		auto stml = this;
		bool findCase = false;
		SmtRes stmRes;
		while (stml)
		{
			auto stm = stml->getStm();
			if (stm) {
				if (findCase) {
					stmRes = stm->Excute();
					stmRes.m_CaseExec = true;
					if (stmRes.m_Type == SmtResType::breakType || stmRes.m_Type == SmtResType::voidType) {
						return stmRes;
					}
				}
				else if (typeid(*stm) == typeid(CaseStm)) {
					auto caseStm = dynamic_cast<const CaseStm*>(stm);
					auto caseVal = caseStm->getExpExcute();
					if (val == caseVal.getTypedValue(val.getType())) {
						findCase = true;
						stmRes = stm->Excute();
						stmRes.m_CaseExec = true;
						if (stmRes.m_Type == SmtResType::breakType || stmRes.m_Type == SmtResType::voidType) {
							return stmRes;
						}
					}
				}
			}
			else {
				break;
			}
			stml = stml->getNext();
		}
		if (findCase) {
			stmRes.m_CaseExec = true;
		}
		return stmRes;
	}

	SmtRes StatementList::ExcuteFromDefault() const {
		setRuntimeLineNumber();
		auto stml = this;
		bool findDefault = false;
		while (stml)
		{
			auto stm = stml->getStm();
			if (stm) {
				if (findDefault) {
					auto res = stm->Excute();
					if (res.m_Type == SmtResType::breakType || res.m_Type == SmtResType::voidType) {
						return res;
					}
				}
				else if (typeid(*stm) == typeid(DefaultStm)) {
					findDefault = true;
					auto res = stm->Excute();
					if (res.m_Type == SmtResType::breakType || res.m_Type == SmtResType::voidType) {
						return res;
					}
				}
			}
			else {
				break;
			}
			stml = stml->getNext();
		}
		return SmtRes();
	}

	struct ExpressionStm::Impl {
		const Expression* m_Exp;
	};

	ExpressionStm::ExpressionStm(const Expression* exp)
		: Statement(StatementType::expressionStm),
		pImpl(new Impl) {
		pImpl->m_Exp = exp;
	}

	ExpressionStm::ExpressionStm()
		: Statement(StatementType::expressionStm),
		pImpl(nullptr) {
	}

	/*ExpressionStm::~ExpressionStm() {
		delete pImpl;
	}*/

	const Expression* ExpressionStm::getExp() const {
		return pImpl->m_Exp;
	}

	SmtRes ExpressionStm::Excute() const {
		setRuntimeLineNumber();
		if (pImpl) {
			getExp()->Excute();
		}
		return SmtRes();
	}

	struct IfStm::Impl {
		const Expression* m_Condition;
		const Statement* m_Statement;
	};

	IfStm::IfStm(const Expression* condition, const Statement* stm)
		: Statement(StatementType::ifStm),
		pImpl(new Impl) {
		pImpl->m_Condition = condition;
		pImpl->m_Statement = stm;
	}

	/*IfStm::~IfStm() {
		delete pImpl;
	}*/

	const Expression* IfStm::getCondition() const {
		return pImpl->m_Condition;
	}

	const Statement* IfStm::getStatement() const {
		return pImpl->m_Statement;
	}

	SmtRes IfStm::Excute() const {
		setRuntimeLineNumber();
		auto val = getCondition()->Excute();
		if (val.getBool()) {
			return getStatement()->Excute();
		}
		return SmtRes();
	}

	struct CaseStm::Impl {
		const Expression* m_Expression;
		const Statement* m_Statement;
	};

	CaseStm::CaseStm(const Expression* exp, const Statement* stm)
		: Statement(StatementType::ifStm),
		pImpl(new Impl) {
		pImpl->m_Expression = exp;
		pImpl->m_Statement = stm;
	}

	/*CaseStm::~CaseStm() {
		delete pImpl;
	}*/

	const Expression* CaseStm::getExp() const {
		return pImpl->m_Expression;
	}

	const Statement* CaseStm::getStatement() const {
		return pImpl->m_Statement;
	}

	Value CaseStm::getExpExcute() const {
		return getExp()->Excute();
	}

	SmtRes CaseStm::Excute() const {
		setRuntimeLineNumber();
		return getStatement()->Excute();
	}

	struct DefaultStm::Impl {
		const Statement* m_Statement;
	};

	DefaultStm::DefaultStm(const Statement* stm)
		: Statement(StatementType::ifStm),
		pImpl(new Impl) {
		pImpl->m_Statement = stm;
	}

	/*DefaultStm::~DefaultStm() {
		delete pImpl;
	}*/

	const Statement* DefaultStm::getStatement() const {
		return pImpl->m_Statement;
	}

	SmtRes DefaultStm::Excute() const {
		setRuntimeLineNumber();
		return getStatement()->Excute();
	}

	struct ForStm::Impl {
		const Statement* m_InitStatement;
		const Expression* m_Condition;
		const Expression* m_Post;
		const Statement* m_Statement;
	};

	ForStm::ForStm(const Statement* initstm, const Expression* condition, const Expression* post, const Statement* stm)
		: Statement(StatementType::forStm),
		pImpl(new Impl) {
		pImpl->m_InitStatement = initstm;
		pImpl->m_Condition = condition;
		pImpl->m_Post = post;
		pImpl->m_Statement = stm;
	}

	/*ForStm::~ForStm() {
		delete pImpl;
	}*/

	const Statement* ForStm::getInitStatement() const {
		return pImpl->m_InitStatement;
	}

	const Expression* ForStm::getCondition()const {
		return pImpl->m_Condition;
	}

	const Expression* ForStm::getPost()const {
		return pImpl->m_Post;
	}

	const Statement* ForStm::getStatement() const {
		return pImpl->m_Statement;
	}

	SmtRes ForStm::Excute() const {
		setRuntimeLineNumber();
		Interpreter::getInp()->pushBreak();
		Interpreter::getInp()->pushLoop();
		for (getInitStatement()->Excute();
			getCondition()->Excute().getBool();
			getPost()->Excute()) {
			getCondition()->Excute().getBool();
			getPost()->Excute();
			auto res = getStatement()->Excute();
			if (res.m_Type == SmtResType::continueType) {
				continue;
			}
			else if (res.m_Type == SmtResType::breakType) {
				break;
			}
			else if (res.m_Type == SmtResType::returnType) {
				Interpreter::getInp()->popLoop();
				Interpreter::getInp()->popBreak();
				return res;
			}
		}
		Interpreter::getInp()->popLoop();
		Interpreter::getInp()->popBreak();
		return SmtRes();
	}

	struct BlockStm::Impl {
		StatementList* m_StatementList;
	};

	BlockStm::BlockStm(StatementList* stml)
		: Statement(StatementType::ifStm),
		pImpl(new Impl) {
		pImpl->m_StatementList = stml;
	}

	BlockStm::BlockStm()
		: Statement(StatementType::blockStm),
		pImpl(new Impl)
	{
		pImpl->m_StatementList = nullptr;
	}

	const StatementList* BlockStm::getStatementList() const {
		return pImpl->m_StatementList;
	}

	SmtRes BlockStm::Excute() const {
		setRuntimeLineNumber();
		Interpreter::getInp()->pushRuntime();
		if (auto stml = getStatementList()) {
			auto res = stml->Excute();
			Interpreter::getInp()->popRuntime();
			return res;
		}
		Interpreter::getInp()->popRuntime();
		return SmtRes();
	}

	SmtRes BlockStm::ExcuteFromCase(Value val) const {
		setRuntimeLineNumber();
		Interpreter::getInp()->pushRuntime();
		if (auto stml = getStatementList()) {
			 auto res = stml->ExcuteFromCase(val);
			 Interpreter::getInp()->popRuntime();
			return res;
		}
		Interpreter::getInp()->popRuntime();
		return SmtRes();
	}

	SmtRes BlockStm::ExcuteFromDefault() const {
		setRuntimeLineNumber();
		Interpreter::getInp()->pushRuntime();
		if (auto stml = getStatementList()) {
			 auto res = stml->ExcuteFromDefault();
			 Interpreter::getInp()->popRuntime();
			return res;
		}
		Interpreter::getInp()->popRuntime();
		return SmtRes();
	}


	struct PrintStm::Impl {
		const Expression* m_Expression;
		bool m_Nflg;
	};

	PrintStm::PrintStm(const Expression* target, bool nflg)
		: Statement(StatementType::printStm),
		pImpl(new Impl)
	{
		pImpl->m_Expression = target;
		pImpl->m_Nflg = nflg;
	}

	/*PrintStm::~PrintStm() {
		delete pImpl;
	}*/

	const Expression* PrintStm::getExpression() const {
		return pImpl->m_Expression;
	}

	bool PrintStm::getNflg() const {
		return pImpl->m_Nflg;
	}

	SmtRes PrintStm::Excute() const {
		setRuntimeLineNumber();
		auto val = pImpl->m_Expression->Excute();
		switch (val.getType())
		{
		case ValueType::boolVal:
			cout << val.getBool();
			break;
		case ValueType::intVal:
			cout << val.getInt();
			break;
		case ValueType::doubleVal:
			cout << val.getDouble();
			break;
		case ValueType::stringVal:
			cout << val.getString();
			break;
		case ValueType::voidVal:
			cout << val.getString();
		}
		if (getNflg()) {
			cout << endl;
		}
		return SmtRes();
	}
}