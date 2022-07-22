#include <cstdlib>
#include <math.h>
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

    struct Expression::Impl {
        ExpressionType m_ExpType;
        Impl() {
            m_ExpType = ExpressionType::noExp;
        }
    };

    Expression::Expression(ExpressionType type)
        :ObjBase(ObjType::ExpType),
        pImpl(new Impl)
    {
        pImpl->m_ExpType = type;
    }

    Expression::~Expression() {
        delete pImpl;
    }

    ExpressionType Expression::getType()const {
        return pImpl->m_ExpType;
    }

    struct BoolLiteralExp::Impl {
        int m_BoolValue;
    };

    BoolLiteralExp::BoolLiteralExp(bool b)
        :Expression(ExpressionType::boolLiteralExp),
        pImpl(new Impl)
    {
        pImpl->m_BoolValue = b;
    }

    BoolLiteralExp::~BoolLiteralExp() {
        delete pImpl;
    }


    bool BoolLiteralExp::getBoolValue()const {
        return pImpl->m_BoolValue;
    }

    Value BoolLiteralExp::Excute() const {
        setRuntimeLineNumber();
        return Value(pImpl->m_BoolValue);
    }

    struct IntLiteralExp::Impl {
        int m_IntValue;
    };

    IntLiteralExp::IntLiteralExp(int i)
        :Expression(ExpressionType::intLiteralExp),
        pImpl(new Impl)
    {
        pImpl->m_IntValue = i;
    }

    IntLiteralExp::~IntLiteralExp() {
        delete pImpl;
    }


    int IntLiteralExp::getIntValue()const {
        return pImpl->m_IntValue;
    }

    Value IntLiteralExp::Excute() const {
        setRuntimeLineNumber();
        return Value(pImpl->m_IntValue);
    }

    struct DoubleLiteralExp::Impl {
        double m_DoubleValue;
    };

    DoubleLiteralExp::DoubleLiteralExp(double d)
        :Expression(ExpressionType::doubleLiteralExp),
        pImpl(new Impl)
    {
        pImpl->m_DoubleValue = d;
    }

    DoubleLiteralExp::~DoubleLiteralExp() {
        delete pImpl;
    }


    double DoubleLiteralExp::getDoubleValue()const {
        return pImpl->m_DoubleValue;
    }

    Value DoubleLiteralExp::Excute() const {
        setRuntimeLineNumber();
        return Value(pImpl->m_DoubleValue);
    }

    struct StringLiteralExp::Impl {
        string m_StringLiteral;
    };

    StringLiteralExp::StringLiteralExp(const char* str)
        :Expression(ExpressionType::stringLiteralExp),
        pImpl(new Impl)
    {
        pImpl->m_StringLiteral = str;
    }

    StringLiteralExp::~StringLiteralExp() {
        delete pImpl;
    }


    const char* StringLiteralExp::getStringLiteral() const {
        return pImpl->m_StringLiteral.c_str();
    }

    Value StringLiteralExp::Excute() const {
        setRuntimeLineNumber();
        return Value(pImpl->m_StringLiteral.c_str());
    }

    struct VariableExp::Impl {
        const char* m_VariableName;
    };

    VariableExp::VariableExp(const char* ident)
        :Expression(ExpressionType::variableExp),
        pImpl(new Impl)
    {
        pImpl->m_VariableName = Interpreter::getInp()->createFixedString(ident);
    }

    VariableExp::~VariableExp() {
        delete pImpl;
    }

    const char* VariableExp::getIdentifier()const {
        return pImpl->m_VariableName;
    }

    Value& VariableExp::getVariableValue() const {
        return Interpreter::getInp()->getGlobalValiableValue(pImpl->m_VariableName);
    }

    Value VariableExp::Excute() const {
        setRuntimeLineNumber();
        Value ret = getVariableValue();
        return ret;
    }

    struct AssignExp::Impl {
        const Expression* m_Variable;
        const Expression* m_Operand;
    };

    AssignExp::AssignExp(const Expression* variable, const Expression* operand)
        :Expression(ExpressionType::assignExp),
        pImpl(new Impl)
    {
        pImpl->m_Variable = variable;
        pImpl->m_Operand = operand;
    }

    AssignExp::~AssignExp() {
        delete pImpl;
    }

    const Expression* AssignExp::getVariable() const {
        return pImpl->m_Variable;
    }

    const Expression* AssignExp::getOperand() const {
        return pImpl->m_Operand;
    }

    Value AssignExp::Excute() const {
        setRuntimeLineNumber();
        auto valptr = dynamic_cast<const VariableExp*>(pImpl->m_Variable);
        if (valptr) {
            Value& v = valptr->getVariableValue();
            v = pImpl->m_Operand->Excute();
            return v;
        }
        return Value();
    }

    struct ToAssExp::Impl {
        const Expression* m_Variable;
        const Expression* m_Operand;
        ExpressionType m_ExpType;
    };

    ToAssExp::ToAssExp(const Expression* variable, const Expression* operand, ExpressionType tp)
        :Expression(ExpressionType::assignExp),
        pImpl(new Impl)
    {
        pImpl->m_Variable = variable;
        pImpl->m_Operand = operand;
        pImpl->m_ExpType = tp;
    }

    ToAssExp::~ToAssExp() {
        delete pImpl;
    }

    const Expression* ToAssExp::getVariable() const {
        return pImpl->m_Variable;
    }

    const Expression* ToAssExp::getOperand() const {
        return pImpl->m_Operand;
    }

    ExpressionType ToAssExp::getExpType() const {
        return pImpl->m_ExpType;
    }

    Value ToAssExp::Excute() const {
        setRuntimeLineNumber();
        auto valptr = dynamic_cast<const VariableExp*>(pImpl->m_Variable);
        if (valptr) {
            Value& v = valptr->getVariableValue();
            switch (pImpl->m_ExpType)
            {
            case ExpressionType::addAssExp:
                v += pImpl->m_Operand->Excute();
                break;
            case ExpressionType::subAssExp:
                v -= pImpl->m_Operand->Excute();
                break;
            case ExpressionType::ampAssExp:
                v &= pImpl->m_Operand->Excute();
                break;
            case ExpressionType::mulAssExp:
                v *= pImpl->m_Operand->Excute();
                break;
            case ExpressionType::divAssExp:
                v /= pImpl->m_Operand->Excute();
                break;
            case ExpressionType::modAssExp:
                v %= pImpl->m_Operand->Excute();
                break;
            }
            return v;
        }
        return Value();
    }

    struct BinaryExp::Impl {
        const Expression* m_Left;
        const Expression* m_Right;
    };

    BinaryExp::BinaryExp(ExpressionType type, const Expression* left, const Expression* right)
        :Expression(type),
        pImpl(new Impl)
    {
        pImpl->m_Left = left;
        pImpl->m_Right = right;
    }

    BinaryExp::~BinaryExp() {
        delete pImpl;
    }

    const Expression* BinaryExp::getLeft() const {
        return pImpl->m_Left;
    }

    const Expression* BinaryExp::getRight() const {
        return pImpl->m_Right;
    }

    Value BinaryExp::Excute() const {
        setRuntimeLineNumber();
        switch (getType()) {
            {
            case ExpressionType::addExp:
                return (pImpl->m_Left->Excute() + pImpl->m_Right->Excute());
                break;
            case ExpressionType::subExp:
                return (pImpl->m_Left->Excute() - pImpl->m_Right->Excute());
                break;
            case ExpressionType::ampExp:
                return (pImpl->m_Left->Excute() & pImpl->m_Right->Excute());
                break;
            case ExpressionType::mulExp:
                return (pImpl->m_Left->Excute() * pImpl->m_Right->Excute());
                break;
            case ExpressionType::divExp:
                return (pImpl->m_Left->Excute() / pImpl->m_Right->Excute());
            case ExpressionType::modExp:
                return (pImpl->m_Left->Excute() % pImpl->m_Right->Excute());
            case ExpressionType::expoExp:
                Value v(pImpl->m_Left->Excute());
                for (int i = 1; i < pImpl->m_Right->Excute().getInt(); i++) {
                    v *= pImpl->m_Left->Excute();
                }
                return v;
                break;
            }
        Interpreter::getInp()->runtimeExit(2004);
        return Value();
        }
    }

    struct RelationalExp::Impl {
        const Expression* m_Left;
        const Expression* m_Right;
    };

    RelationalExp::RelationalExp(ExpressionType type, const Expression* left, const Expression* right)
        :Expression(type),
        pImpl(new Impl)
    {
        pImpl->m_Left = left;
        pImpl->m_Right = right;
    }

    RelationalExp::~RelationalExp() {
        delete pImpl;
    }

    const Expression* RelationalExp::getLeft() const {
        return pImpl->m_Left;
    }

    const Expression* RelationalExp::getRight() const {
        return pImpl->m_Right;
    }

    Value RelationalExp::Excute() const {
        setRuntimeLineNumber();
        switch (getType()) {
            case ExpressionType::eqExp:
                return Value(pImpl->m_Left->Excute() == pImpl->m_Right->Excute());
                break;
            case ExpressionType::neExp:
                return Value(pImpl->m_Left->Excute() != pImpl->m_Right->Excute());
                break;
            case ExpressionType::ltExp:
                return Value(pImpl->m_Left->Excute() < pImpl->m_Right->Excute());
                break;
            case ExpressionType::gtExp:
                return Value(pImpl->m_Left->Excute() > pImpl->m_Right->Excute());
                break;
            case ExpressionType::leExp:
                return Value(pImpl->m_Left->Excute() <= pImpl->m_Right->Excute());
                break;
            case ExpressionType::geExp:
                return Value(pImpl->m_Left->Excute() >= pImpl->m_Right->Excute());
                break;
            }
            Interpreter::getInp()->runtimeExit(2010);
            return Value();
    }
}