/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "pagenode.h"

#include <QtCore/qglobal.h>
#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

class FunctionNode;
class QmlTypeNode;
class QmlPropertyNode;

class Aggregate : public PageNode
{
public:
    Node *findChildNode(const QString &name, Node::Genus genus, int findFlags = 0) const;
    Node *findNonfunctionChild(const QString &name, bool (Node::*)() const);
    void findChildren(const QString &name, NodeVector &nodes) const;
    FunctionNode *findFunctionChild(const QString &name, const Parameters &parameters);
    FunctionNode *findFunctionChild(const FunctionNode *clone);

    void normalizeOverloads();
    void markUndocumentedChildrenInternal();

    bool isAggregate() const override { return true; }
    const EnumNode *findEnumNodeForValue(const QString &enumValue) const;

    int count() const { return m_children.size(); }
    const NodeList &childNodes() const { return m_children; }
    const NodeList &nonfunctionList();
    NodeList::ConstIterator constBegin() const { return m_children.constBegin(); }
    NodeList::ConstIterator constEnd() const { return m_children.constEnd(); }

    void addIncludeFile(const QString &includeFile);
    void setIncludeFiles(const QStringList &includeFiles);
    const QStringList &includeFiles() const { return m_includeFiles; }

    QStringList primaryKeys();
    QmlPropertyNode *hasQmlProperty(const QString &) const;
    QmlPropertyNode *hasQmlProperty(const QString &, bool attached) const;
    virtual QmlTypeNode *qmlBaseNode() const { return nullptr; }
    void addChildByTitle(Node *child, const QString &title);
    void printChildren(const QString &title);
    void addChild(Node *child);
    void adoptChild(Node *child);
    void setOutputSubdirectory(const QString &t) override;

    FunctionMap &functionMap() { return m_functionMap; }
    void findAllFunctions(NodeMapMap &functionIndex);
    void findAllNamespaces(NodeMultiMap &namespaces);
    void findAllAttributions(NodeMultiMap &attributions);
    bool hasObsoleteMembers() const;
    void findAllObsoleteThings();
    void findAllClasses();
    void findAllSince();
    void resolveQmlInheritance();
    bool hasOverloads(const FunctionNode *fn) const;
    void appendToRelatedByProxy(const NodeList &t) { m_relatedByProxy.append(t); }
    NodeList &relatedByProxy() { return m_relatedByProxy; }
    QString typeWord(bool cap) const;

protected:
    Aggregate(NodeType type, Aggregate *parent, const QString &name)
        : PageNode(type, parent, name), m_functionCount(0)
    {
    }
    ~Aggregate() override;
    void removeFunctionNode(FunctionNode *fn);

private:
    friend class Node;
    void addFunction(FunctionNode *fn);
    void adoptFunction(FunctionNode *fn);
    static bool isSameSignature(const FunctionNode *f1, const FunctionNode *f2);

protected:
    NodeList m_children {};
    NodeList m_relatedByProxy {};
    int m_functionCount {};
    FunctionMap m_functionMap {};

private:
    QStringList m_includeFiles {};
    NodeList m_enumChildren {};
    NodeMultiMap m_nonfunctionMap {};
    NodeList m_nonfunctionList {};
};

QT_END_NAMESPACE

#endif // AGGREGATE_H
