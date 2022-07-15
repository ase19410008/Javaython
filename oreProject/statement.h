#pragma once

namespace ore {
	enum class StatementType {
		nullStm,
		expressionStm,
		blockStm,
		ifStm,
		printStm,
		stmTypeCount
	};

	enum class SmtResType {
		voidType,
		breakType,
		returnType,
	};

	struct SmtRes {
		SmtResType m_Type;
		Value m_RetValue;
		bool m_CaseExec;
		SmtRes() :
			m_Type(SmtResType::voidType),
			m_RetValue(),
			m_CaseExec(false){}
	};

	class StmExBase : public ObjBase {
	protected:
		StmExBase(ObjType type) :
			ObjBase(type) {}
	public:
		virtual ~StmExBase(){}
		virtual SmtRes Excute() const {
			return SmtRes();
		}
	};

	class Statement : public StmExBase {
	protected:
		Statement(StatementType type);
	public:
		virtual ~Statement() {}
		StatementType getType() const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class StatementList : public StmExBase {
	public:
		StatementList(const Statement* stm);
		virtual ~StatementList() {}
		const Statement* getStm() const;
		StatementList* getNext() const;
		void setNext(StatementList* next);
		virtual SmtRes Excute() const override;
		// StatementType getType() const;
		SmtRes ExcuteFromCase(Value val) const;
		SmtRes ExcuteFromDefault() const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class ExpressionStm : public Statement {
	public:
		ExpressionStm(const Expression* exp	);
		ExpressionStm();
		virtual ~ExpressionStm() {}
		const Expression* getExp() const;
		virtual SmtRes Excute() const override;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class IfStm : public Statement {
	public:
		IfStm(const Expression* condition, const Statement* stm);
		virtual ~IfStm() {}
		const Expression* getCondition() const;
		const Statement* getStatement() const;
		virtual SmtRes Excute() const override;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class CaseStm : public Statement {
	public:
		CaseStm(const Expression* exp, const Statement* stm);
		virtual ~CaseStm() {}
		const Expression* getExp() const;
		Value getExpExcute() const;
		const Statement* getStatement() const;
		virtual SmtRes Excute() const override;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class DefaultStm : public Statement {
	public:
		DefaultStm(const Statement* stm);
		virtual ~DefaultStm() {}
		const Statement* getStatement() const;
		virtual SmtRes Excute() const override;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class BlockStm : public Statement {
	public:
		BlockStm(StatementList* stml);
		BlockStm();
		virtual ~BlockStm() {}
		const StatementList* getStatementList() const;
		virtual SmtRes Excute() const override;
		SmtRes ExcuteFromCase(Value val) const;
		SmtRes ExcuteFromDefault() const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class PrintStm : public Statement {
	public:
		PrintStm(const Expression* target, bool nflg);
		virtual ~PrintStm() {}
		const Expression* getExpression() const;
		bool getNflg() const;
		virtual SmtRes Excute() const override;
	private:
		struct Impl;
		Impl* pImpl;
	};
}