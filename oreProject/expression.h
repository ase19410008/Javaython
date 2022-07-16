#pragma once

namespace ore {

    enum class ExpressionType {
        noExp = 0,
        boolLiteralExp = 0,
        intLiteralExp,
        doubleLiteralExp,
        stringLiteralExp,
        variableExp,
        assignExp,
        addAssExp,
        subAssExp,
        mulAssExp,
        divAssExp,
        modAssExp,
        addExp,
        subExp,
        mulExp,
        divExp,
        modExp,
        eqExp,
        printExp,
        expTypeCount,
        expoExp
    };

    class Expression : public ObjBase {
    protected:
        Expression(ExpressionType type);
    public:
        virtual ~Expression();
        ExpressionType getType()const;
        virtual Value Excute() const {
            return Value();
        }
    private:
        struct Impl;
        Impl* pImpl;
    };

    class BoolLiteralExp : public Expression {
    public:
        BoolLiteralExp(bool b = false);
        virtual ~BoolLiteralExp();
        bool getBoolValue()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class IntLiteralExp : public Expression {
    public:
        IntLiteralExp(int i = 0);
        virtual ~IntLiteralExp();
        int getIntValue()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class DoubleLiteralExp : public Expression {
    public:
        DoubleLiteralExp(double d = 0.0);
        virtual ~DoubleLiteralExp();
        double getDoubleValue()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class StringLiteralExp : public Expression {
    public:
        StringLiteralExp(const char* str);
        virtual ~StringLiteralExp();
        const char* getStringLiteral()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class VariableExp : public Expression {
    public:
        VariableExp(const char* ident);
        virtual ~VariableExp();
        const char* getIdentifier()const;
        Value& getVariableValue()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class AssignExp : public Expression {
    public:
        AssignExp(const Expression* variable, const Expression* operand);
        virtual ~AssignExp();
        const Expression* getVariable()const;
        const Expression* getOperand()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class ToAssExp : public Expression {
    public:
        ToAssExp(const Expression* variable, const Expression* operand, ExpressionType tp);
        virtual ~ToAssExp();
        const Expression* getVariable()const;
        const Expression* getOperand()const;
        ExpressionType getExpType()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class BinaryExp : public Expression {
    public:
        BinaryExp(ExpressionType type, const Expression* left, const Expression* right);
        virtual ~BinaryExp();
        const Expression* getLeft()const;
        const Expression* getRight()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };

    class RelationalExp : public Expression {
    public:
        RelationalExp(ExpressionType type, const Expression* left, const Expression* right);
        virtual ~RelationalExp();
        const Expression* getLeft()const;
        const Expression* getRight()const;
        virtual Value Excute() const  override;
    private:
        struct Impl;
        Impl* pImpl;
    };
}