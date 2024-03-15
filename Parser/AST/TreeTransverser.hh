#pragma once

#include "TreeNode.hh"

#include "Type.hh"
#include "TSymbol.hh"
#include "Declaration.hh"
#include "BinaryOp.hh"
#include "Expr.hh"
#include "Qual.hh"
#include "Literal.hh"
#include "Attribute.hh"
#include "Unary.hh"

namespace mana::ast
{
    class TreeVisitor {
    public:
        virtual bool visit(TSymbol* symbol) { return true; }

        virtual bool visit(Type* type) { return true; };
    
        virtual bool visit(Declaration* literal) { return true; }

        virtual bool visit(Attribute* attribute) { return true; }

        virtual bool visit(Literal* literal) { return true; }

        virtual bool visit(BinaryOp* op) { return true; }

        virtual bool visit(Expr* expression) { return true; }

        virtual bool visit(Qualifier* expression) { return true; }

        virtual bool visit(Unary* unary) { return true; }
    };
} 