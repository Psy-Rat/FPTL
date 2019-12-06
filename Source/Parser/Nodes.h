﻿#pragma once

#include "AST.h"
#include "Ident.h"

namespace FPTL
{
	namespace Parser
	{
		class Support;

		//-------------------------------------------------------------------------------------
		// Выражение.

		class ExpressionNode : public ASTNode
		{
		public:

			ExpressionNode(ASTNodeType aType, ASTNode * aLeft, ASTNode * aRight, ASTNode * aMiddle);
			~ExpressionNode();

			ASTNode * getLeft() const { return mChilds[mLeft]; }
			ASTNode * getRight() const { return mChilds[mRight]; }
			ASTNode * getMiddle() const { return mChilds[mMiddle]; }

			void accept(NodeVisitor * aVisitor) override;

			//ASTNode * copy() const override;

			enum
			{
				mLeft,
				mRight,
				mMiddle
			};

		private:
			//ASTNode * mLeft;
			//ASTNode * mRight;
			//ASTNode * mMiddle;
		};

		//-------------------------------------------------------------------------------------
		// Описание константы.

		class ConstantNode : public ASTNode
		{
		public:

			ConstantNode(const ASTNodeType aType, const Ident &aConstant) : 
			ASTNode(aType), mIdent(aConstant) {}

			Ident getConstant() const { return mIdent; }

			bool isNatural() const;
			void accept(NodeVisitor * aVisitor) override;
			//ASTNode * copy() const override;

		private:
			Ident          mIdent;
		};

		//-------------------------------------------------------------------------------------
		// Списковая синтаксическая структура.
		// При обработке АСД обходить только строго в обратном порядке!
		class ListNode : public ASTNode
		{
		public:

			explicit ListNode(const ASTNodeType aType) : ASTNode(aType) {}
			~ListNode() override;

			ListNode* addElement(ASTNode * aElem)
			{
				if (aElem) mChilds.push_back(aElem);
				//if (aElem) mChilds.insert(mChilds.begin(), aElem);
				return this;
			}

			size_t getNumElements() const
			{
				return mChilds.size();
			}

			void accept(NodeVisitor * aVisitor) override;
			//ListNode * copy() const override;
		};

		//-------------------------------------------------------------------------------------
		// Определение (типового уравнения, функциональной переменной, типового или функционального параметра, переменной).

		class DefinitionNode : public ASTNode
		{
		public:
			DefinitionNode(ASTNodeType aType, const Ident &aName, ASTNode * aDefinition);
			~DefinitionNode();

			Ident         getDefinitionName() const { return mDefinitionName; }
			ASTNode *     getDefinition() const { return mChilds[mDefinition]; }

			void accept(NodeVisitor * aVisitor) override;

			//ASTNode * copy() const override;

			enum
			{
				mDefinition
			};

		private:
			Ident mDefinitionName;
			//ASTNode * mDefinition;
		};

		//-------------------------------------------------------------------------------------
		// Ссылка на именованный объект (тип, конструктор, имя функции или функциональной переменной, имя встроенной функции).

		class NameRefNode : public ASTNode
		{
		public:

			NameRefNode(const Ident &aTypeName, ASTNodeType aNodeType, ListNode * aParams);
			~NameRefNode();

			void setTarget(ASTNode * aTarget) {
				delete mChilds[mTarget];
				mChilds[mTarget] = aTarget;
			}
			void accept(NodeVisitor * aVisitor) override;

			Ident                    getName() const { return mTypeName; }
			ListNode *               getParameters() const { return static_cast<ListNode*>(mChilds[mParameters]); }

			// Возвращает узел, на который ссылается данное имя.
			ASTNode *                getTarget() const { return mChilds[mTarget]; }
			//ASTNode *                copy() const override;
			int                      numParameters() const override
			{
				return mChilds[mParameters] ? static_cast<int>(static_cast<ListNode*>(mChilds[mParameters])->mChilds.size()) : 0;
			}

			enum
			{
				mParameters,
				mTarget
			};

		private:
			
			Ident mTypeName;
			//ListNode * mParameters;
			//ASTNode *  mTarget;
		};

		//-------------------------------------------------------------------------------------
		// Конструктор. FIXME: этот класс не нужен в новой нотации описания конструкторов.

		class ConstructorNode : public ASTNode
		{
		public:

			ConstructorNode(const Ident &aName, ListNode * aCtorParameters, const Ident &aCtorResultTypeName);
			virtual ~ConstructorNode();

			void accept(NodeVisitor * aVisitor) override;

			Ident                 getCtorName() const { return mName; }
			Ident                 getCtorResultTypeName() const { return mCtorResultTypeName; }
			ListNode *            getCtorParameters() const { return static_cast<ListNode*>(mChilds[mCtorParameters]); }

			//ASTNode *             copy() const override;

			enum
			{
				mCtorParameters
			};

		private:

			Ident mName;
			//ListNode * mCtorParameters;
			Ident mCtorResultTypeName;
		};

		//-------------------------------------------------------------------------------------
		// Блок описания данных.

		class DataNode : public ASTNode
		{
		public:

			DataNode(const Ident &aTypeName, ListNode * aTypeDefs, ListNode * aTypeParams, ListNode * aConstructors);
			~DataNode();

			void accept(NodeVisitor * aVisitor) override;

			ListNode *      getConstructors() const { return static_cast<ListNode*>(mChilds[mConstructors]); }
			Ident           getDataName() const { return mTypeName; }
			ListNode *      getTypeDefs() const { return static_cast<ListNode*>(mChilds[mTypeDefinitions]); }
			ListNode *      getTypeParams() const { return static_cast<ListNode*>(mChilds[mTypeParameters]); }

			//ASTNode *       copy() const override;
			int numParameters() const override
			{
				return mChilds[mTypeParameters] ? static_cast<int>(static_cast<ListNode*>(mChilds[mTypeParameters])->mChilds.size()) : 0;
			}

			enum
			{
				mConstructors,
				mTypeDefinitions,
				mTypeParameters
			};

		private:

			Ident mTypeName;
			//ListNode * mConstructors;
			//ListNode * mTypeDefinitions;
			//ListNode * mTypeParameters;
		};

		//-------------------------------------------------------------------------------------
		// Описание функции или схемы.

		class FunctionNode : public ASTNode
		{
		public:

			FunctionNode(const Ident &aFuncName, ListNode * aDefinitions, ListNode * aFormalParams);
			~FunctionNode();

			void accept(NodeVisitor * aVisitor) override;

			Ident             getFuncName() const { return mFuncName; }
			ListNode *        getFormalParameters() const { return static_cast<ListNode*>(mChilds[mFormalParameters]); }
			ListNode *        getDefinitions() const { return static_cast<ListNode*>(mChilds[mDefinitions]); }
			DefinitionNode *  getDefinition(const Ident &aName) const;

			//FunctionNode *    copy() const override;
			int numParameters() const override
			{
				return mChilds[mFormalParameters] ? static_cast<int>(static_cast<ListNode*>(mChilds[mFormalParameters])->mChilds.size()) : 0;
			}

			std::vector<FunctionNode *>    getFunctionNodes() const;

			enum
			{
				mDefinitions,
				mFormalParameters
			};

		private:
			Ident mFuncName;
			//ListNode * mDefinitions;
			//ListNode * mFormalParameters;
		};

		//-------------------------------------------------------------------------------------
		// Описание входных данных схемы.

		class ApplicationBlock : public ASTNode
		{
		public:

			ApplicationBlock(NameRefNode * aRunSchemeName, ASTNode * aSchemeParameters, ListNode * aDataVarDefs);
			~ApplicationBlock();

			void accept(NodeVisitor * aVisitor) override;

			NameRefNode * getRunningSchemeName() const { return static_cast<NameRefNode*>(mChilds[mRunSchemeName]); }
			ASTNode *     getSchemeParameters() const { return mChilds[mSchemeParameters]; }
			ListNode *    getDataVarDefinitions() const { return static_cast<ListNode*>(mChilds[mDataVarDefinitions]); }

			//ApplicationBlock *  copy() const override;

			enum
			{
				mRunSchemeName,
				mSchemeParameters,
				mDataVarDefinitions
			};

		private:
			//NameRefNode * mRunSchemeName;
			//ASTNode * mSchemeParameters;
			//ListNode * mDataVarDefinitions;
		};

		//-------------------------------------------------------------------------------------
		// Описание функциональной программы.

		class FunctionalProgram : public ASTNode
		{
		public:
			FunctionalProgram(ASTNode * aDataDefinitions, FunctionNode * aScheme, ApplicationBlock * aApplication);
			~FunctionalProgram();

			void accept(NodeVisitor * aVisitor) override;

			ListNode*             getDataDefinitions() const { return static_cast<ListNode*>(mChilds[mDataDefinitions]); }
			FunctionNode*         getScheme() const { return static_cast<FunctionNode*>(mChilds[mScheme]); }
			ApplicationBlock*     getApplication() const { return static_cast<ApplicationBlock*>(mChilds[mApplication]); }

			//ASTNode * copy() const override;

			enum
			{
				mDataDefinitions,
				mScheme,
				mApplication
			};

		private:

			//ListNode *         mDataDefinitions;
			//FunctionNode *     mScheme;
			//ApplicationBlock * mApplication;
		};
	}
}
