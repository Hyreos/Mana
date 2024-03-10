#include "Lexer.hh"

#include <iostream>
#include <format>
#include <functional>

using ErrorCallback_T = void(*)(const std::string& msg);

static ErrorCallback_T g_errorCallback;

#define MONA_CHECK_MAYBE_RETURN(X, MSG) if (!(X)) { if (g_errorCallback) g_errorCallback(std::format("({}:{}): {}", m_stats.lineIndex, m_stats.columnIndex,  MSG)); return {}; }

#define MONA_TRY_GET(X, PTR, MSG) PTR = X; if (!PTR) { if (g_errorCallback) g_errorCallback(MSG); return {}; }

namespace mona {
    template<typename T>
    using DeletedUnique_T = std::unique_ptr<T, std::function<void(void*)>>;

    template<typename T, typename...Args>
    DeletedUnique_T<T> MakeUniquePtr(Args&&... args) {
        auto* ptr = new T(std::forward<Args>(args)...);
        
        return DeletedUnique_T<T> {
            ptr,
            [ptr](void*) {
                delete ptr;
            }
        };
    }

    enum class ASTKind {
        kAttribute,
        kComponent,
        kComponentField,
        kTSymbol,
        kNumber
    };

    class TreeNode {
    public:
        TreeNode(ASTKind kind) 
            : m_kind{ kind } 
        {
        }

        virtual DeletedUnique_T<TreeNode> clone() = 0;

        template<typename T>
        T* cast() 
        {
            if (m_kind != T::kind) return nullptr;

            return static_cast<T*>(this);
        }

        void addAttribute(DeletedUnique_T<TreeNode> attr)
        {
            m_attributes.push_back(attr->clone());
        }

        std::vector<DeletedUnique_T<TreeNode>>& attributes()
        {
            return m_attributes;
        }
    private:
        ASTKind m_kind;

        std::vector<DeletedUnique_T<TreeNode>> m_attributes;
    };

    class Attribute : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kAttribute };

        Attribute(
            const std::string& name,
            DeletedUnique_T<TreeNode> value
        ) 
            : TreeNode(kind),
                m_name { name },
                m_value { std::move(value) } 
        {
        }

        DeletedUnique_T<TreeNode> clone() override
        {
            return MakeUniquePtr<Attribute>(m_name, m_value->clone());
        }
    private:
        std::string m_name;
        DeletedUnique_T<TreeNode> m_value;
    };

    class Component : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponent };

        Component(
            std::vector<DeletedUnique_T<TreeNode>> fields
        ) 
            : TreeNode(kind),
                m_fields{ std::move(fields) }
        {
        }

        DeletedUnique_T<TreeNode> clone() override
        {
            std::vector<DeletedUnique_T<TreeNode>> cfields;

            for (auto& f : m_fields) cfields.push_back(f->clone());

            return MakeUniquePtr<Component>(std::move(cfields));
        }
    private:
        std::vector<DeletedUnique_T<TreeNode>> m_fields;
    };

    class CField : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponentField };

        CField(
            DeletedUnique_T<TreeNode> type,
            const std::string& name
        ) 
            : TreeNode(kind),
                m_name{ name },
                m_type{ std::move(type) }
        {
        }

        DeletedUnique_T<TreeNode> clone() override
        {
        return MakeUniquePtr<CField>(m_type->clone(), m_name);
        }
    private:
        std::string m_name;
        DeletedUnique_T<TreeNode> m_type;
    };

    class TSymbol : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kTSymbol };

        TSymbol(const std::string& identifier) 
            : TreeNode(kind),
                m_identifier { identifier }
        {
        }

        DeletedUnique_T<TreeNode> clone() override
        {
        return MakeUniquePtr<TSymbol>(m_identifier);
        }
    private:
        std::string m_identifier;
    };

    class Number : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kNumber };

        Number(const std::string& identifier) 
            : TreeNode(kind),
                m_identifier { identifier }
        {
        }

        DeletedUnique_T<TreeNode> clone() override
        {
        return MakeUniquePtr<Number>(m_identifier);
        }
    private:
        std::string m_identifier;
    };

    class Parser {
    public:
        Parser(const std::string& code) 
            : m_lexer { code },
                m_tokenIdx { -1 },
                m_stats { 
                    .lineIndex = 1, 
                    .columnIndex = 0
                }
        {
            if (m_lexer.size() > 0)
                if (m_lexer[0].kind == mona::Kind::kWS || m_lexer[0].kind == mona::Kind::kLnBrk) {
                    consume();

                    m_tokenIdx--;
                }
        }

        void registerErrorCallback(ErrorCallback_T callback)
        {
            g_errorCallback = callback;
        }

        void parse() 
        {
            for (DeletedUnique_T<TreeNode> ptr = processToken(); ptr; ptr = processToken()) {}
        }
    private:
        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;

        template<typename T, typename... Args>
        static DeletedUnique_T<TreeNode> makeAST(Args&&... args)
        {
            return MakeUniquePtr<TreeNode>(
                static_cast<TreeNode*>(new T(std::forward<Args>(args)...)),
                [](TreeNode* ptr) {
                    delete static_cast<T*>(ptr);
                }
            );
        }

        DeletedUnique_T<TreeNode> processToken()
        {
            auto tk = consume();

            if (!tk) return nullptr;

            switch (tk->kind) {
                case mona::Kind::kAt: {
                    const mona::Token* attr_name;
                    
                    MONA_TRY_GET(consumeCheck(mona::Kind::kIdentifier), attr_name, "Missing identifier in attribute.");

                    MONA_CHECK_MAYBE_RETURN(
                        attr_name->view == "serialize", 
                        std::format("Invalid identifier '{}' in attribute.", attr_name->view)
                    );
                
                    MONA_CHECK_MAYBE_RETURN(consumeCheck(mona::Kind::kRightParenthesis), "Missing '(' in attribute."); // (
                    
                    auto attr_value = processToken();

                    MONA_CHECK_MAYBE_RETURN(consumeCheck(mona::Kind::kLeftParenthesis), "Missing ')' in attribute."); // )

                    auto attr = MakeUniquePtr<Attribute>(std::string(attr_name->view), std::move(attr_value));

                    DeletedUnique_T<TreeNode> nxt;
                    
                    MONA_TRY_GET(processToken(), nxt, "Failed to gerante ASTNode");

                    nxt->addAttribute(std::move(attr));

                    return std::move(nxt);
                } break;

                case mona::Kind::kNumber: {
                    return MakeUniquePtr<Number>(std::string(tk->view));
                } break;

                case mona::Kind::kIdentifier: {
                    if (tk->view == "component") {
                        std::vector<DeletedUnique_T<TreeNode>> fields;

                        const Token* identifier;
                        
                        MONA_TRY_GET(consumeCheck(mona::Kind::kIdentifier), identifier, "Missing identifier in component declaration.");
                        
                        MONA_CHECK_MAYBE_RETURN(consumeCheck(mona::Kind::kRightBracket), "Missing '{' in component declaration");

                        // Keep looping until next token is a left bracket
                        while (peek(1)->kind != mona::Kind::kLeftBracket) {
                            auto field_type = processToken();

                            // Peek a identifier
                            const Token* field_name;
                            
                            MONA_TRY_GET(consumeCheck(mona::Kind::kIdentifier), field_name, "Invalid token");

                            fields.push_back(MakeUniquePtr<CField>(std::move(field_type), std::string(field_name->view)));
                        }

                        MONA_CHECK_MAYBE_RETURN(
                            consumeCheck(mona::Kind::kLeftBracket), 
                            "Unexpected token '{}'! Expected \"{}\"", 
                            peek(0)->view, mona::GetKindName(mona::Kind::kLeftBracket)
                        );

                        return MakeUniquePtr<Component>(std::move(fields));
                    } else if (tk->view == "string") return MakeUniquePtr<TSymbol>("string");
                    else return MakeUniquePtr<TSymbol>(std::string(tk->view));
                } break;
            }

            return nullptr;
        }

        const Token* peekExpected(size_t off, mona::Kind kind, bool skip_ws = true, bool skip_lnbrks = true)
        {
            auto tk = peek(off, skip_ws, skip_lnbrks);

            if (tk)
                if (tk->kind != kind)
                    return nullptr;

            return tk;
        }

        bool canPeek(size_t off, bool skip_ws = true, bool skip_lnbrks = true) {
            if (off == 0) return &m_lexer[m_tokenIdx];

            auto tokenidx = m_tokenIdx + 1;

            while (off != 0 && tokenidx < m_lexer.size()) {
                if (skip_ws && m_lexer[tokenidx].kind == mona::Kind::kWS) {
                    tokenidx++;
                    continue;
                }

                if (skip_lnbrks && m_lexer[tokenidx].kind == mona::Kind::kLnBrk) {
                    tokenidx++;
                    continue;
                }

                off--;
                
                if (off > 0) tokenidx++;
            }

            if (off != 0) return false;

            return true;
        }

        const Token* peek(size_t off, bool skip_ws = true, bool skip_lnbrks = true) 
        {
            if (off == 0) return &m_lexer[m_tokenIdx];

            if (m_tokenIdx == m_lexer.size() - 1) return nullptr;

            auto tokenidx = m_tokenIdx + 1;

            for (; off != 0 && tokenidx < m_lexer.size(); tokenidx++) {
                if (skip_ws && m_lexer[tokenidx].kind == mona::Kind::kWS) 
                    continue;

                if (skip_lnbrks && m_lexer[tokenidx].kind == mona::Kind::kLnBrk)
                    continue;
            
                if (--off == 0) break;
            }

            if (off != 0) return nullptr;

            return &m_lexer[tokenidx];        
        }

        const Token* consumeCheck(mona::Kind kind, size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true) 
        {
            const Token* tk = consume(off, skip_ws, skip_lnbrks);

            if (tk->kind != kind) {           
                return nullptr;
            }

            return tk;   
        }

        const Token* consume(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true) 
        {
            if (off == 0) 
                return nullptr;

            if (m_tokenIdx == m_lexer.size() - 1) 
                return nullptr;

            m_tokenIdx++;

            while (off != 0 && m_tokenIdx < m_lexer.size()) {
                m_stats.columnIndex += m_lexer[m_tokenIdx].view.size();

                if (m_lexer[m_tokenIdx].kind == mona::Kind::kWS)
                    if (skip_ws) {
                        m_tokenIdx++;
                        continue;
                    }

                if (m_lexer[m_tokenIdx].kind == mona::Kind::kLnBrk) {
                    m_stats.columnIndex = 0;
                    m_stats.lineIndex++;

                    if (skip_lnbrks) {
                        m_tokenIdx++;

                        continue;
                    }
                }

                off--;
                
                if (off > 0) m_tokenIdx++;
            }

            if (off != 0) return nullptr;

            return &m_lexer[m_tokenIdx];   
        }

        int64_t m_tokenIdx;   

        mona::GrLexer m_lexer;
    };
}

void LogParserError(const std::string& msg) {
    std::cerr << msg << std::endl;

    std::exit(1);
}

int main(int argc, char* argv[]) {
    auto code = 
R"(    
component A {
    @serialize(1) string p1

    @serialize(2) string p2
}

component B {
    @serialize (1) string p1

    @serialize(2) string p2
}

)";

    auto parser = mona::Parser(code);

    parser.registerErrorCallback(LogParserError);

    parser.parse();

    return 0;
}