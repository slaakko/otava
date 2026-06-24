// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.codegen.goto_target_map_builder;

import otava.symbols.bound_tree_visitor;
import otava.symbols.exception;
import util.unicode;

namespace otava::codegen {

class GotoTargetMapBuilder : public otava::symbols::DefaultBoundTreeVisitor
{
public:
    enum class Stage
    {
        none, mapLabeledStatements, mapGotoStatements
    };
    GotoTargetMapBuilder(otava::symbols::Context* context_);
    inline void SetStage(Stage stage_) { stage = stage_; }
    void Visit(otava::symbols::BoundLabeledStatementNode& node) override;
    void Visit(otava::symbols::BoundGotoStatementNode& node) override;
private:
    otava::symbols::Context* context;
    Stage stage;
    std::map<std::string, otava::symbols::BoundLabeledStatementNode*> labeledStatementMap;
};

GotoTargetMapBuilder::GotoTargetMapBuilder(otava::symbols::Context* context_) :
    context(context_), stage(Stage::none), labeledStatementMap()
{
}

void GotoTargetMapBuilder::Visit(otava::symbols::BoundLabeledStatementNode& node)
{
    if (stage == Stage::mapLabeledStatements)
    {
        auto it = labeledStatementMap.find(node.Label());
        if (it != labeledStatementMap.end())
        {
            otava::symbols::ThrowException("goto target label '" + util::ToUtf8(node.Label()) + "' not unique", node.GetFullSpan(), context);
        }
        else
        {
            labeledStatementMap[node.Label()] = &node;
        }
    }
    node.Statement()->Accept(*this);
}

void GotoTargetMapBuilder::Visit(otava::symbols::BoundGotoStatementNode& node)
{
    if (stage == Stage::mapGotoStatements)
    {
        auto it = labeledStatementMap.find(node.Target());
        if (it == labeledStatementMap.end())
        {
            otava::symbols::ThrowException("goto target label '" + util::ToUtf8(node.Target()) + "' not found", node.GetFullSpan(), context);
        }
        else
        {
            node.SetLabeledStatement(it->second);
        }
    }
}

void BuildGotoTargetMap(otava::symbols::BoundCompoundStatementNode* functionBody, otava::symbols::Context* context)
{
    GotoTargetMapBuilder builder(context);
    builder.SetStage(GotoTargetMapBuilder::Stage::mapLabeledStatements);
    functionBody->Accept(builder);
    builder.SetStage(GotoTargetMapBuilder::Stage::mapGotoStatements);
    functionBody->Accept(builder);
}

} // namespace otava::codegen
