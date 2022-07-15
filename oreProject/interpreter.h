#pragma once

namespace ore {

	class Root : public StmExBase {
	public:
		Root(const StatementList* stml);
		virtual ~Root();
		virtual SmtRes Excute() const override;
		const StatementList* getStmeList() const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class Interpreter : public ObjBase {
		Interpreter();
	public:
		virtual ~Interpreter();
		int Compile(FILE* fp);
		void Exec() const;
		void Delete();

		void setConfig(const char* key, const Value val);
		Value getConfig(const char* key);
		void pushBackObject(ObjBase* pobj);
		const char* createFixedString(const char* ext);

		Expression* createVariableExp(const char* ident);
		Expression* createBoolLiteralExp(bool b);
		Expression* createIntLiteralExp(const char* ext);
		Expression* createDoubleLiteralExp(const char* ext);
		void startStringLiteral();
		void addStringLiteral(char ext);
		Expression* createStringLiteralExp();
		Expression* createBinaryExp(const Expression* left, const Expression* right, ExpressionType t);
		Expression* createAssExp(const Expression* ident, const Expression* right);
		Expression* createToAssExp(const Expression* ident, const Expression* right, ExpressionType t);

		template<typename T, typename... Ts>
		Statement* createStatement(Ts&&... params) {
			auto ptr = new T(params...);
			pushBackObject(ptr);
			return ptr;
		}

		StatementList* createStatementList(const Statement* stm);
		StatementList* createStatementList(StatementList* stml, const Statement* stm);

		Root* createRoot(const StatementList* stml);

		static Interpreter* getInp();

		Value& getGlobalValiableValue(const char* key);

		void pushRuntime(bool IsFunc = false);
		void popRuntime();

		void syntaxKeepExit(int mess_id, const char* detail);
		void syntaxExit(int mess_id, int linnum, const char* detail, bool bison = true);

		void setRuntimeLineNumber(int num);
		int getRuntimeLineNumber() const;
		void runtimeExit(int mess_id, const char* target = nullptr);
		void runtimeWorning(int mess_id);
		bool isWorningOut() const;
		void setWorningOut(bool b);
	private:
		struct Impl;
		Impl* pImpl;
	};
}