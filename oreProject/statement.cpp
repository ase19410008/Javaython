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


	//--------------------------------------------------------------------------------------
    ///  if•¶ƒNƒ‰ƒX
    //--------------------------------------------------------------------------------------
    struct IfStm::Impl {
        const Expression* m_Condition;
        const Statement* m_Statement;
    };

    IfStm::IfStm(const Expression* condition, const Statement* stm)
        : Statement(StatementType::IfStm),
        pImpl(new Impl)
    {
        pImpl->m_Condition = condition;
        pImpl->m_Statement = stm;
    }

    IfStm::~IfStm() {
        delete pImpl;

    }

    const Expression* IfStm::getCondition()const {
        return pImpl->m_Condition;
    }

    const Statement* IfStm::getStatement()const {
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



}