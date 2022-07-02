#pragma once

namespace ore {
	enum class StatementType {
		nullStm,
		expressionStm,
		printStm,
		stmTypeCount
	};

	enum class SmtResType {
		voidType,
	};

	struct SmtRes {
		SmtResType m_Type;
		Value m_RetValue;
		SmtRes() :
			m_Type(SmtResType::voidType),
			m_RetValue(){}
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