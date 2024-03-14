#pragma once

#include "TreeNode.hh"

namespace mana
{
    class TreeVisitor {
    public:
        virtual bool visitSymbol(TreeNode* symbol) { return true; }

        virtual bool visitType(TreeNode* type) { return true; };
    
        virtual bool visitDeclaration(TreeNode* literal) { return true; }

        virtual bool visitAttribute(TreeNode* attribute) { return true; }

        virtual bool visitLiteral(TreeNode* literal) { return true; }

        virtual bool visitOperator(TreeNode* literal) { return true; }

        virtual bool visitStatement(TreeNode* statement) { return true; }
    };
} 